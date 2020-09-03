/**
 * reference wrapper
 * 声明于<functional> 的std::reference_wrapper 主要用来"喂" reference 给function template
 * 后者原本以by value方式传递参数。对于一个给定类型T，这个class提供ref()隐式转化为T&,
 * 提供cref()隐式转化为const T&. 这往往允许function template操作reference而不需要另写特化版本
 *
 * 使用场景：
 * 1. make_pair 用此特性构造 pair<> of reference
 * 2. make_tuple 用此特性构造 tuple<> of reference
 * 3. Binder   用此特性能够绑定reference
 * 4. Thread   可以传递reference参数
 * 5. 可用作STL容器的元素类型，存储元素的reference
 *
 * function type wrapper
 * std::function<>
 */

#include <gtest/gtest.h>

#include <functional>
#include <iostream>

using namespace std;

void f(int a, int b) { cout << "call f" << endl; }

template <typename T>
void foo(T t) {
  cout << "call foo" << endl;
}

TEST(RerferWrapper, func_template_test) {
  int x = 0;
  foo(x);             // T 为 int
  foo(std::ref(x));   // T 为 int&
  foo(std::cref(x));  // T 为 const int&
  std::function<void(int, int)> fw = f;
  fw(1, 2);
  std::vector<std::function<void(int, int)>> vfw;
  vfw.push_back(f);
  vfw[0](1, 0);
}