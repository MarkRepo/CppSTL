/**
 * thread 低层接口与async的区别：
 * 1. thread没有发射策略（launch policy），如果无法启动新的线程，抛出std::system_error异常
 * 2. 没有接口可获取线程结果，唯一可获得的是线程id
 * 3.
 * 如果发生异常，但未被捕捉于线程内，程序会立刻终止并调用std::terminate(),若想将异常传播至线程外的某个context，必须使用exception_ptr
 * 4.
 * 必须声明是否等待线程结束（使用join()），还是将它自母体分离（使用detach），使他运行于后台而不受任何控制，如果
 * 在thread obj寿命结束前不这么做，或如果它发生了一次move_assignment, 程序会终止并调用std::terminate
 * 5. 如果线程运行于后台而main结束了，所有的线程会鲁莽而硬性的终止。
 *
 * detach 线程问题：
 * 1. 强烈建议by value方式传递变量和obj
 * 2. 当程序退出时(exit), detach 线程可能仍在运行，意味着他有可能访问已被销毁或正在析构的global
 * 或static obj，这会导致不可预期行为。
 * 如果detach thread 使用了global或static obj，应该做以下事情之一：
 *  2.1. 确保这些global或static obj在detach thread结束之前不会被析构。
 * 一种做法是使用condition variable，它让detach thread发信号来说它已经结束，
 * 离开main或exit之前必须先设置妥当这些condition variable，然后发信号说可析构。
 *  2.2 理想中你应该调用notify_all_at_thread_exit()来设定condition variable
 * 以确保thread local variable被销毁
 *  2.3 调用quick_exit()以不调用global或static obj的析构函数的方式结束程序
 *
 * 3. std::cout, std::cin, std::cerr 标准说在程序运行期间，这些不会销毁
 */

#include <gtest/gtest.h>

#include <chrono>
#include <exception>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

void doSomething(int num, char c) {
  try {
    std::default_random_engine dre(42 * c);
    std::uniform_int_distribution<int> id(10, 1000);
    for (int i = 0; i < num; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
      std::cout.put(c).flush();
    }
  } catch (const std::exception& e) {
    cout << "thread-Exception, thread: " << std::this_thread::get_id() << ", message: " << e.what()
         << endl;
  } catch (...) {
    cout << "thread-Exception, thread: " << std::this_thread::get_id() << endl;
  }
}

// first example
TEST(ThreadTest, thread1) {
  try {
    // 创建线程有可能抛出异常
    thread t1(doSomething, 5, '.');
    cout << "- start fg thread: " << t1.get_id() << endl;
    for (int i = 0; i < 5; i++) {
      thread t(doSomething, 10, 'a' + i);
      cout << "- detach started bg thread: " << t.get_id() << endl;
      t.detach();
    }
    cin.get();
    cout << "- join fg thread: " << t1.get_id() << endl;
    t1.join();
  } catch (const exception& e) {
    cerr << "Exception: " << e.what() << endl;
  }
}

// 唯一允许对thread id进行的操作是“比较”，
// 以及调用output操作符输出到某个stream，除此之外，不应该对id有任何进一步假设。
// 有相应的hash函数，因此可作为unordered容器的key
TEST(ThreadTest, thread_id) {
  thread t1(doSomething, 5, '.');
  thread t2(doSomething, 5, '+');
  thread t3(doSomething, 5, '*');
  cout << "t3       ID: " << t3.get_id() << endl;
  cout << "main     ID: " << std::this_thread::get_id() << endl;
  cout << "nothread ID: " << std::thread::id() << endl;
  // ID 支持 ==, !=, < , >, >=, <=, << 操作， 可直接看定义
  if (t3.get_id() == std::this_thread::get_id()) {
    cout << "no possible" << endl;
  }
  t1.join();
  t2.join();
  t3.join();
  cout << "possible concurrency num: " << std::thread::hardware_concurrency() << endl;
}