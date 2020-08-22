/**
 * deque 的能力
 * deque通常实现为一组区块,与vector相比，能力差异如下：
 * 1. 两端都能安插能移除元素
 * 2. 访问元素时deque内部会多一个间接过程
 * 3. 迭代器需要在不同区块间跳转，所以必须是个smartpointer，不能是寻常pointer
 * 4. 不支持对内存容量和重分配的时机控制，重分配时不必复制所有元素
 * 5. 会释放不再使用的内存区块
 * 6. 除了头尾两端，任何地方插入或移除元素都将导致指向deque的pointer，reference，iterator失效
 */

#include <gtest/gtest.h>

#include <deque>
#include <iostream>
#include <string>

#include "Stl/Container/common.h"

using namespace std;

// 除Stl/Container/Common:op_test 中操作
template <typename T>
void DequeTestOp(const T& t, const std::initializer_list<T>& initlist) {
  // 1.Create
  deque<T> d(10);
  deque<T> d2(10, t);
  // 2. deque不提供容量操作：capacity(), reserve()
  // 3. push_front(), pop_front()
  d2.push_front(t);
  d2.pop_front();
  d2.emplace_front(t);

  PrintElem(d2, "deque<T>");
  // 4. 其他操作与vector相同，间vector_test.cc
  // 5. deque内部用来存放“指向独立区块”
  // 的所有pointer的空间，通常不会被缩小，使用shrink_to_fit有可能缩小
  d2.shrink_to_fit();
}

TEST(DequeTest, deque) {
  DequeTestOp<int>(9, {1, 2, 3, 4, 5, 6});
  DequeTestOp<string>("hello", {"this", "is", "test"});
}