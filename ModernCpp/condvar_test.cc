#include <gtest/gtest.h>

#include <condition_variable>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using std::thread;

class ThreadRAII {
 public:
  enum class DtorAction { join, detach };
  // 1. 只接受右值型别的std::thread，移入ThreadRAII
  // 2. 形参顺序的设计（thread在前，action在后）更符合调用者的直觉
  ThreadRAII(thread&& t, DtorAction a) : _a(a), _t(std::move(t)) {}
  ~ThreadRAII() {
    // 1. 校验thread以确保可联结。这是必要的，因为针对一个不可联结的线程调用join或detach会产生未定义行为
    // 2. 这里不需要考虑竞险。因为thread只能通过成员函数变成不可联结。但是析构函数调用时，不应该有其他线程调用该对象的成员函数
    if (_t.joinable()) {
      if (_a == DtorAction::join) {
        _t.join();
      } else {
        _t.detach();
      }
    }
  }
  // 析构函数的显示声明抑制了编译器生成移动函数，这里显示声明
  ThreadRAII(ThreadRAII&&) = default;
  ThreadRAII& operator=(ThreadRAII&&) = default;

  // 提供get可以避免让ThreadRAII去重复std::thread的所有接口，意味着ThreadRAII适用于直接使用thread的语境
  thread& get() { return _t; }

  // 一个成员变量的初始化有可能会依赖另一个成员变量，又因为std::thread型别对象初始化之后可能会马上用来运行函数，
  // 所以把他们声明在类的最后是个好习惯，这保证了底层线程执行时可以安全的访问其他成员变量。
 private:
  DtorAction _a;
  thread _t;
};

std::promise<void> p;

void react() {std::cout << "react" << std::endl;}
void detect() {
  auto sf = p.get_future().share();
  std::vector<ThreadRAII> vt;
  for(int i = 0; i < 10; i++) {
    vt.push_back( 
      ThreadRAII(std::thread([]{
        p.get_future().wait();
        react();
      }), ThreadRAII::DtorAction::join));
  }
  //...   // 如果这里发生异常，detect函数将失去响应
  p.set_value();
  
}
