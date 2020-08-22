/**
 * forward_list:
 * 1. 只提供前向迭代器，没有reverse_iterator
 * 2. 不提供size()
 * 3. 没有指向最末元素的锚点，因此没有back(), push_back(), pop_back()
 * 4.
 * 对于所有“从forward_list的某特定位置安插或删除“的成员函数，forward_list提供带_after后缀的特殊版本,
 * 因为单链表无法回头
 * 5. 提供before_begin(), cbefore_begin(),产出第一元素之前的一个虚拟元素位置
 * 6. 除此之外，与list基本相同
 */

#include <gtest/gtest.h>

#include <forward_list>
#include <string>

#include "Stl/Container/common.h"

using namespace std;

// 除Stl/Container/Common:op_test 以外和Sequence操作之外，forwardlist操作
template <typename T>
void FowardListTestOp(const T& t, const std::initializer_list<T>& initlist) {
  // 1. Create
  forward_list<T> f(10);
  forward_list<T> f1(10, t);
  // 2. 不提供size(), 使用distance 计算距离, 线性复杂度
  cout << "f1.size(): " << std::distance(f1.begin(), f1.end()) << endl;
  // 3. front
  cout << "first elem: " << f1.front() << endl;
  // 4. 迭代器, begin, end, cbegin, cend, before_begin, cbefore_begin
  // 5. insert, emplace, erase
  // insert_after(pos, initlist)
  f1.insert_after(f1.before_begin(), {t, t, t});
  PrintElem(f1, "f1 after insert_after");
  // push_front, pop_front
  f1.push_front(t);
  f1.pop_front();
  // insert_after(pos,v)
  f1.insert_after(f1.before_begin(), t);
  // insert_after(pos, n, v)
  f1.insert_after(f1.before_begin(), 3, t);
  // insert_after(pos, beg, end)
  f1.insert_after(f1.before_begin(), initlist.begin(), initlist.end());
  // emplace_after(pos, args...);
  f1.emplace_after(f1.before_begin(), t);
  // emplace_front(args...);
  f1.emplace_front(t);
  // erase_after(pos)
  f1.erase_after(f1.before_begin());
  // erase_after(beg, end)
  f1.erase_after(f1.begin(), f1.begin());
  // remove(val)
  f1.remove(t);
  // remove_if(op)
  f1.remove_if([](const T& t) { return true; });
  // clear
  f1.clear();
  // resize
  f1.resize(10);
  f1.resize(15, t);
  PrintElem(f1, "after all op");
}

TEST(fwlTest, op) {
  FowardListTestOp<int>(9, {1, 2, 3, 4, 5, 6});
  FowardListTestOp<string>("hello", {"this", "is", "forwardlist", "test"});
}

TEST(fwlTest, find) {
  forward_list<int> fw{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto posBefore = fw.before_begin();
  for (auto pos = fw.begin(); pos != fw.end(); ++pos, ++posBefore) {
    if (*pos % 2 == 0) {
      cout << "find elem: " << *pos << endl;
      break;  // element find
    }
  }
  fw.insert_after(posBefore, 42);
  PrintElem(fw, "after insert_after(pos, 42)");
  posBefore = fw.before_begin();
  for (; next(posBefore) != fw.end(); ++posBefore) {
    if (*posBefore % 3 == 0) break;
  }
  fw.insert_after(posBefore, 43);
  PrintElem(fw, "after insert_after(pos, 43)");
}

TEST(fwlTest, splice_after) {
  forward_list<int> fw1{1, 2, 3, 4, 5};
  forward_list<int> fw2{97, 98, 99};
  auto pos1 = fw1.before_begin();
  for (; next(pos1) != fw1.end(); ++pos1) {
    if (*(next(pos1)) == 3) break;
  }
  auto pos2 = fw2.before_begin();
  for (; next(pos2) != fw2.end(); ++pos2) {
    if (*(next(pos2)) == 99) break;
  }
  // l1.splice_after(pos, l2, pos2);
  // l1.splice_after(pos, l2);
  // l1.splice_after(pos, l2, l2beg, l2end);
  fw1.splice_after(pos1, fw2, pos2);
  fw1.splice_after(pos1, fw2, fw2.before_begin());
  PrintElem(fw1, "after splice_after");
  PrintElem(fw2, "after splice_after");

  // unique, unique(op), sort(), sort(op), l1.merge(l2), l1.merge(l2, op)， reverse（）
  // 与list相同，不再重复
}