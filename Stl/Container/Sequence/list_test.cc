/**
 * list能力
 * 1. 不提供随机访问
 * 2. 任何位置插入和删除都很快
 * 3. 安插和删除不会导致其他其他元素的pointer, reference, iterator失效
 * 4. 异常处理方式：要么成功，要么什么都不发生
 *
 * 操作与array,vector,deque的差异：
 * 1. 不提供[], at()
 * 2. 不提供容量，空间重新分配等操作函数
 * 3. 提供不少特殊成员函数，比stl算法速度更快
 * 4. 其他操作与其他Sequence容器一样，参见vector_test.cc
 */

#include <gtest/gtest.h>

#include <iostream>
#include <list>
#include <string>

#include "Stl/Container/common.h"

using namespace std;

// 除Stl/Container/Common:op_test 和Sequence容器操作 以外的list操作

template <typename T>
void ListTestOp(const T& t, const std::initializer_list<T>& initlist) {
  // 1. Create
  list<T> l(10);
  list<T> l2(10, t);
  // 2. access
  cout << "first elem: " << l2.front() << ", last elem: " << l2.back() << endl;
  // 3. insert and remove
  l2.remove(t);  // remove all elem with value t
  PrintElem(l2, "after remove t");
  l2.assign(initlist);
  l2.remove_if([](const T& t) { return true; });  // remove_if(op); remove when op(elem) return
                                                  // true;
  PrintElem(l2, "after remove_if()");
  // 4. 特殊更易型操作
  // unique; 移除相邻相同元素
  list<T> l3({t, t});
  l3.unique();
  PrintElem(l3, "l3 after unique");
  l3.push_front(t);
  // unique(op)， 使用op判断式
  l3.unique([](const T& t1, const T& t2) { return true; });
  PrintElem(l3, "l3 after unique(op)");
  // c.splice(pos, c2), 将c2中的所有元素移到c中pos之前
  l2.clear();
  l2.assign(initlist);
  PrintElem(l2, "l2 before splice");
  l2.splice(l2.end(), l3);
  PrintElem(l2, "l2 after splice");
  // c.splice(pos, c2, c2pos), 将c2中c2pos位置的元素移动到c中pos之前
  l3.push_back(t);
  l2.splice(l2.end(), l3, l3.begin());
  PrintElem(l2, "l2 after splice");
  // c.splice(pos, c2, c2beg, c2end)， 将c2中[c2beg, c2end)所有元素移动到c中pos之前
  l3.assign(3, t);
  l2.splice(l2.end(), l3, l3.begin(), l3.end());
  PrintElem(l2, "l2 after splice");
  // sort
  l2.sort();
  PrintElem(l2, "l2 after sort");
  l2.assign(initlist);
  auto op = [](const T& t1, const T& t2) { return -1; };
  // sort(op)
  l2.sort(op);
  PrintElem(l2, "l2 after sort(op)");
  // merge
  l2.assign(initlist);
  l2.sort();
  l3.assign(initlist);
  l3.sort();
  l2.merge(l3);
  PrintElem(l2, "l2 after merge l3");
  // merge(op)
  l2.assign(initlist);
  l3.assign(initlist);
  l2.sort(op);
  l3.sort(op);
  l2.merge(l3, op);
  PrintElem(l2, "l2 after merge l3 with op");
  // reverse
  l2.reverse();
  PrintElem(l2, "l2 after reverse");
}

TEST(ListTest, op) {
  ListTestOp<int>(9, {1, 2, 3, 4, 5, 6});
  ListTestOp<string>("hello", {"this", "is", "list", "test"});
}