/**
 * condition variable 用来同步化线程之间的数据流逻辑依赖关系
 * future的限制：从某线程传递到另一线程只能一次
 * 针对目标条件而轮询通常不是好的解决办法（mutex_lock_test.cc有一例子）：
 * 1. 等待中的线程浪费宝贵的cpu时间重复检查flag
 * 2. 锁住mutex时，负责设置flag的线程会被阻塞
 * 3. 很难找出适当的sleep周期
 *
 * 较好的办法是使用condition variable, 借由它一个线程可以唤醒一个或多个其他等待中的线程
 * 原则上，condition variable运作方式如下：
 * 1. 声明一个mutex 和condvar
 * #include <mutex>
 * #include <condition_variable>
 *
 * std::mutex readyMutex;
 * std::condition_variable readyCondVar;
 *
 * 2. 那个激发“条件终于满足”的线程必须调用
 * readyCondVar.notify_one()
 * 或
 * readyCondVar.notify_all()
 *
 * 3. 那个“等待条件被满足”的线程必须调用：
 * std::unique_lock<std::mutex> ulock(readyMutex);
 * readyCondVar.wait(ulock);
 *
 * 需要注意以下2点：
 * 1. lock_guard是不够的，因为等待线程会锁定和解除mutex
 * 2. condition variable 有可能假醒，即wait动作有可能在未被notified时返回
 * 因此等待线程wakeup后仍然需要验证条件是否已经真正达成
 *
 * 细说condition variable：
 * 1. 所有的通知都被自动同步化，因此并发调用notify_one和notify_all，不会带来麻烦
 * 2.所有等待某个condvar的线程都必须使用相同的mutex
 * wait()家族的某个成员被调用时，mutex必须被unique_lock锁定，否则会发生未定义行为
 * wait函数会执行以下三个atomic步骤来暂时解锁mutex：（lock, check state, unlock, wait）
 * 1. unlock mutex， 然后进入等待状态（必须是atomic，否则unlock 和wait之间的notify会遗失）
 * 2. 解除因等待而造成的阻塞
 * 3. 再次lock mutex
 *
 * 所以wait的判断式总是在mutex在lock的状态下执行
 *
 */

#include <gtest/gtest.h>

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

bool readyFlag;
std::mutex readyMutex;
std::condition_variable readyCondVar;

void thread1() {
  cout << "<return>" << endl;
  std::cin.get();

  {
    std::lock_guard<std::mutex> lg(readyMutex);
    readyFlag = true;
  }
  readyCondVar.notify_all();
}

void thread2() {
  {
    std::unique_lock<std::mutex> ul(readyMutex);
    readyCondVar.wait(ul, [] { return readyFlag; });
  }
  cout << "thread2 done" << endl;
}

// thread2 中的wait执行验证函数处理假醒的代码
void thread3() {
  {
    std::unique_lock<std::mutex> ul(readyMutex);
    while (!readyFlag) {
      readyCondVar.wait(ul);
    }
  }
  cout << "thread3 done" << endl;
}

// 使用示例
TEST(CondVarTest, CondVar1) {
  auto f1 = std::async(std::launch::async, thread1);
  auto f2 = std::async(std::launch::async, thread2);
  auto f3 = std::async(std::launch::async, thread3);
}

//实现多线程queue
std::queue<int> q;
std::mutex qMutex;
std::condition_variable qCondVar;

void provider(int val) {
  for (int i = 0; i < 6; i++) {
    {
      std::lock_guard<std::mutex> lg(qMutex);
      q.push(val + i);
    }
    qCondVar.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(val));
  }
}

void consumer(int id) {
  while (true) {
    int val;
    {
      std::unique_lock<std::mutex> ul(qMutex);
      qCondVar.wait(ul, [] { return !q.empty(); });
      val = q.front();
      q.pop();
    }
    cout << "consumer " << id << ": " << val << endl;
  }
}

TEST(CondVarTest, CondVar2) {
  auto p1 = std::async(std::launch::async, provider, 100);
  auto p2 = std::async(std::launch::async, provider, 300);
  auto p3 = std::async(std::launch::async, provider, 500);

  auto c1 = std::async(std::launch::async, consumer, 1);
  auto c2 = std::async(std::launch::async, consumer, 2);
}