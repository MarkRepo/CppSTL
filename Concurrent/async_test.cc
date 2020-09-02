#include <gtest/gtest.h>

#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

int doSomething(char c) {
  std::default_random_engine dre(c);
  std::uniform_int_distribution<int> id(10, 1000);
  for (int i = 0; i < 5; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
    cout.put(c).flush();
  }

  return c;
}

int func1() { return doSomething('+'); }

int func2() { return doSomething('*'); }

// future 不可拷贝，可移动
TEST(AsyncTest, Async1) {
  // 尝试异步执行func1
  std::future<int> result1(std::async(func1));
  int result2 = func2();
  // 如果func1没有执行，get()或wait()会启动执行
  int result = result1.get() + result2;
  cout << "result1 + result2 is : " << result << endl;
}

TEST(AsyncTest, Launch) {
  // 如果不能异步执行，抛出std::system_error异常
  // 如果result1生命结束，会等待func1结束；但使用get会更加清晰
  // 如果不将std::async的结果赋值出去，调用者会在此处等待func1函数结束
  std::future<int> result1(std::async(std::launch::async, func1));
  //延缓func2执行，直到调用get()
  std::future<int> result2(std::async(std::launch::deferred, func2));
  result2.get();
  result1.get();
}

class MyException : public std::exception {
  const char* what() const noexcept override { return "MyException"; }
};

int task1() {
  throw MyException();
  return 0;
}

TEST(AsyncTest, ExceptionHandle) {
  auto result(std::async(std::launch::async, task1));
  try {
    result.get();
  } catch (std::exception& e) {
    cout << "exception: " << e.what() << endl;
  }
}

TEST(AsyncTest, Wait) {
  // wait强制启动线程，等待终止，可调用多次；get只能调用一次，因为get调用后future<>对象处于非法状态
  auto f(std::async(func1));
  f.wait();
  // wait_for, wait_until不会强制启动线程， 返回以下三个状态之一
  // 1. std::future_status::defered 如果async延缓执行，且没有调用wait或get，立刻返回
  // 2. std::future_status::timeout 尚未结束，且逾期
  // 3. std::future_status::ready 操作已完成
  f.wait_for(std::chrono::milliseconds(3));
  f.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1));
}

int quickComputation() {
  // 模拟快速的粗略计算
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  return 0;
}

int accurateComputation() {
  // 模拟耗时的精确计算
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return 1;
}

std::future<int> f;

int bestResultInTime() {
  auto tp = std::chrono::system_clock::now() + std::chrono::seconds(2);
  f = std::async(std::launch::async, accurateComputation);
  int guess = quickComputation();
  std::future_status status = f.wait_until(tp);
  if (status == std::future_status::ready) {
    return f.get();
  } else {
    return guess;
  }
}

int bestResultWithPoll() {
  f = std::async(accurateComputation);
  // 首先判断async有没有启动线程执行任务，如果使用launch选项，可不做判断
  if (f.wait_for(std::chrono::seconds(0)) != std::future_status::deferred) {
    while (f.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
      // cout << "wait for accurateComputation finish!" << endl;
      std::this_thread::yield();  // 切换线程，防止cpu被当前线程占用，导致其他线程无法执行;或者sleep
    }
    try {
      return f.get();
    } catch (const std::exception& e) {
      cout << "bestResultWithPoll Get Exception: " << e.what() << endl;
    }
  }
  return 0;
}

TEST(AsyncTest, Poll) {
  cout << "bestResultInTime: " << bestResultInTime() << endl;      // 0
  cout << "bestResultWithPoll: " << bestResultWithPoll() << endl;  // 1
}

class X {
 public:
  void echo(std::string s) { cout << "echo " << s << endl; }
};

TEST(AsyncTest, param) {
  char c = '@';
  // 尽量以by value方式传递参数
  auto f(std::async(doSomething, c));
  // 如果传递引用，需要自己控制变量的生命周期和并发处理
  auto f1(std::async(doSomething, std::ref(c)));  // risky
  c = '#';
  auto f2(std::async([&] { doSomething(c); }));  // risky
  X x;
  auto ff(std::async(&X::echo, x, "mark"));
}

int queryNumber() {
  cout << "enter a number: ";
  int num;
  cin >> num;
  if (!cin) throw std::runtime_error("no number read");
  return num;
}

// 这里如果使用const std::shared_future<int>& f; 需要考虑shared_future本身的data
// race和生命周期问题，而不是其指向的shared data
void doSomething2(char c, std::shared_future<int> f) {
  try {
    int num = f.get();
    for (int i = 0; i < num; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      cout.put(c).flush();
    }
  } catch (const std::exception& e) {
    cout << "Exception in thread: " << std::this_thread::get_id() << ", message: " << e.what()
         << endl;
  }
}

// shared_future, 可多次使用get获取相同结果，或抛出相同异常；支持copy语义
TEST(AsyncTest, SharedFutureTest) {
  try {
    // std::shared_future<int> f = std::async(queryNumber);
    auto f = std::async(queryNumber).share();
    auto f1 = std::async(doSomething2, '.', f);
    auto f2 = std::async(doSomething2, '+', f);
    auto f3 = std::async(doSomething2, '*', f);
    f1.get();
    f2.get();
    f3.get();
  } catch (const std::exception& e) {
    cout << "exception: " << e.what() << endl;
  }
}