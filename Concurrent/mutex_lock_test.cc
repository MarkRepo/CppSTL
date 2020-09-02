#include <gtest/gtest.h>

#include <future>
#include <iostream>
#include <mutex>
#include <string>

using namespace std;

std::mutex printMutex;

void Print(const std::string& s) {
  std::lock_guard<std::mutex> lock(printMutex);
  for (char c : s) {
    cout.put(c);
  }
  cout << endl;
}

/**
 * 参考cpprefence
 * mutex m;
 * m.lock() 有可能抛出异常 std::system_error
 */
TEST(MutexTest, mutex1) {
  auto f1 = std::async(std::launch::async, Print, "this is the first thread");
  auto f2 = std::async(std::launch::async, Print, "this is the second thread");
  Print("this is the main thread");
}

class DataBaseAccess {
 public:
  void CreateTable() {
    std::lock_guard<std::recursive_mutex> lock(dbMutex);
    cout << "CreateTabel lock mutext" << endl;
  }
  void InsertData() {
    std::lock_guard<std::recursive_mutex> lock(dbMutex);
    cout << "InsertDdata lock mutext" << endl;
  }

  void CreateTableAndInsertData() {
    std::lock_guard<std::recursive_mutex> lock(dbMutex);
    CreateTable();
    InsertData();
    cout << "create and insert data finish" << endl;
  }

 private:
  std::recursive_mutex dbMutex;
};

// recursive_mutex, 允许同一时间多次被同一线程获取lock
TEST(MutexTest, recursive_mutex_test) {
  DataBaseAccess dba;
  dba.CreateTableAndInsertData();
}

void doSomeOtherStuff() { cout << "doSomeOtherStuff" << endl; }

void couldNotGetLock() { cout << "cout not get lock" << endl; }

std::mutex m;
// timed_mutex 允许使用时间段或时间点指定多长时间内尝试获取lock
std::timed_mutex tm;
std::recursive_timed_mutex rtm;
TEST(MutextTest, try_lock_test) {
  // try_lock有可能假性失败，也就是即使没有其他人占有，也有可能失败
  while (m.try_lock() == false) {
    doSomeOtherStuff();
  }
  // 已lock的mutex，使用lock_guard需要adopt_lock
  std::lock_guard<std::mutex> lock(m, std::adopt_lock);

  // try_lock_for, try_lock_until
  if (tm.try_lock_for(std::chrono::milliseconds(1000))) {
    std::lock_guard<std::timed_mutex> lock_tm(tm, std::adopt_lock);
  } else {
    couldNotGetLock();
  }

  // try_lock_until, try_lock_for
  if (rtm.try_lock_until(std::chrono::system_clock::now() + std::chrono::seconds(1))) {
    std::lock_guard<std::recursive_timed_mutex> lock_rtm(rtm, std::adopt_lock);
  } else {
    couldNotGetLock();
  }
}

std::mutex m1, m2;

TEST(MutexTest, multi_lock_test) {
  {
    // std::lock会锁住所有mutex，而且阻塞直到所有mutex都被锁定，或者抛出异常，如果是后者，已被成功锁定的mutex都会被解锁。std::lock提供了deadlock回避机制，多个所得锁定次序并不明确
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
  }

  {
    // std::try_lock,
    // 尝试取得多个锁，若非所有锁都可用也不至于阻塞。取得所有锁返回-1，否则返回第一个失败lock的index（从0开始），
    // 如果这样的话，所有成功的lock会被unlock。std::try_lock不提供deadlock回避机制，
    // 保证按出现在参数列表中顺序尝试lock
    int idx = std::try_lock(m1, m2);
    if (idx < 0) {
      std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
      std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
    } else {
      cout << "could not lock mutex: " << idx + 1 << endl;
    }
  }
}

std::mutex m3;
std::timed_mutex tm3;
std::mutex m4;
TEST(MutexTest, unique_lock_test) {
  // unique_lock
  // 允许明确写出“何时”以及“如何”锁定以及解锁mutex。因此其可能（也可能不）拥有一个被锁住的mutex；
  // 而lock_guard在其生命期间一定拥有。对于unique_lock,
  // 可调用owns_lock()、bool()判断是否mutex是否被锁住 unique lock
  // 析构时，如果mutex被锁，析构函数自动调用unlock, 否则啥也不做

  // 1. try_to_lock, 企图锁定，但不阻塞
  { std::unique_lock<std::mutex> lock(m3, std::try_to_lock); }
  // 2. 传递时间段或时间点，表示希望在一个明确的时间周期内锁定
  { std::unique_lock<std::timed_mutex> lock(tm3, std::chrono::seconds(1)); }
  // 3. defer_lock, 初始化lock obj，但是不打算锁住mutex
  {
    std::unique_lock<std::mutex> lock(m3, std::defer_lock);
    lock.lock();  // or (timed) try_lock
  }
  {
    std::unique_lock<std::mutex> lock1(m3, std::defer_lock);
    std::unique_lock<std::mutex> lock2(m4, std::defer_lock);
    std::lock(m3, m4);
  }
  // unique lock 提供release() 释放mutex， 或是将mutex拥有权转移给另一个lock
}

// 结合lock_guard和unique_lock轮询readyFlag
// readyflag 不加volatile的原因是，对readyflag的读取都在cretical
// section内（某个lock的设立和解除之前），这样的读写动作不会被优化，会实效的发生
// 通常更好更好的做法是使用condition variable
bool readyFlag = false;
std::mutex readyFlagMutex;

void thread1() {
  std::lock_guard<std::mutex> lg(readyFlagMutex);
  readyFlag = true;
}

void thread2() {
  {
    std::unique_lock<std::mutex> ul(readyFlagMutex);
    while (!readyFlag) {
      ul.unlock();
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      ul.lock();
    }
  }
}

//多线程环境的缓式初始化
// 原则上可以使用同一个call_flag调用不同的函数
// 被call_once调用函数抛出的异常都会被再次抛出，此情况下第一次调用视为不成功
class X {
 private:
  mutable std::once_flag initDataFlag;
  mutable int data_;
  void initData() const { data_ = 10; }

 public:
  int getData() const {
    std::call_once(initDataFlag, &X::initData, this);
    return data_;
  }
};

TEST(MutexTest, CallOnce) {
  X x;
  cout << "getData: " << x.getData() << endl;
}