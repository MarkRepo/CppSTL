// 用来传递结果和异常的一般性机制： std::promise
// promise 和 future是配对的，二者都能暂时持有一个shared state（用来表现一个结果或者异常），
// 但future允许你取回数据（get）, promise 需要你提供数据（set_...()）

#include <gtest/gtest.h>

#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

using namespace std;

// promise 不能既存储值， 又存储异常， 否则导致std::future_error
void doSomething(std::promise<string>& p) {
  try {
    // read charactor and throw exception if 'x'
    cout << "read char('x' for exception): ";
    char c = std::cin.get();
    if (c == 'x') {
      throw std::runtime_error(std::string("char ") + c + " read");
    }
    std::string s = std::string("char ") + c + " processed";
    // 设置操作是线程安全的
    p.set_value(std::move(s));
  } catch (...) {
    // current_exception 将当前异常以exception_ptr类型生成出来，如无异常，生成nullptr
    p.set_exception(std::current_exception());
  }
}

void doSomething2(std::promise<string>& p) {
  try {
    cout << "read char('x' for exception): ";
    char c = std::cin.get();
    if (c == 'x') {
      throw std::runtime_error(std::string("char ") + c + " read");
    }
    std::string s = std::string("char ") + c + " processed";
    // 令shared state在线程结束时ready， 以确保线程的local obj及其他资源在结果处理前被清除
    p.set_value_at_thread_exit(s);
  } catch (...) {
    p.set_exception_at_thread_exit(std::current_exception());
  }
}

TEST(PromiseTest, promise) {
  try {
    std::promise<string> p;
    // 确保by reference 传递， copying 不能用于promise
    std::thread t(doSomething, std::ref(p));
    t.detach();
    // 共享相同的 shared state的future，用于取出结果，或重新抛出异常
    //（针对内部存储的exception_ptr调用rethrow_exception()）
    // get_future只能调用一次，第二次抛出std::future_error
    std::future<string> f(p.get_future());
    // get阻塞直到promise ready，并不意味这线程t结束
    std::cout << "result: " << f.get() << endl;
  } catch (const std::exception& e) {
    cout << "exception: " << e.what() << endl;
  } catch (...) {
    cout << "excepton" << endl;
  }
}