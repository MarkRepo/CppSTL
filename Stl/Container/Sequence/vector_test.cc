#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "Stl/Container/common.h"

using namespace std;

// capacity(), reserve(n), shrink_to_fit()
template <typename T>
void TestVectorCap(const T& t) {
  vector<T> v;
  v.reserve(10);
  vector<T> v2(10);
  v2.reserve(20);
  PrintElem(v, "vector reserve inited?");
  PrintElem(v2, "vector v(n) is inited?");
  // 不能使用reserve 缩减容量
  std::cout << "before reserve, capacity: " << v.capacity() << endl;
  v.reserve(5);
  ASSERT_EQ(v.capacity(), 10);
  // shrink_to_fit: 不具强制力的要求
  v.shrink_to_fit();
  cout << "after shrink_to_fit, v.capacity is: " << v.capacity() << endl;
  v2.shrink_to_fit();
  cout << "after shrink_to_fit, v2.capacity is: " << v2.capacity() << endl;
  // before c11, 缩减容量窍门
  v2.reserve(20);
  vector<T>(v2).swap(v2);
  cout << "after swap, v2.capacity: " << v2.capacity() << endl;
}

// 除Stl/Container/Common:op_test 外的vector 操作
template <typename T>
void TestVectorOp(const T& t, const std::initializer_list<T>& initlist = {}) {
  cout << endl << endl << "<<<<<<< enter TestVectorOp initlist: ";
  for (const auto& i : initlist) {
    cout << i << " ";
  }
  cout << ">>>>>>>>" << endl;
  // 1. constructor
  vector<T> v(10);
  vector<T> v2(10, t);
  // 2. assign, 与构造函数一一对应, assign(n, v); assign(beg, end); assign(initlist)
  cout << "before assign, v.size(): " << v.size() << ", v.capacity: " << v.capacity() << endl;
  v.assign(5, t);
  cout << "after assign, v.size(): " << v.size() << ", v.capacity: " << v.capacity() << endl;
  PrintElem(v, "after v.assign(5, t)");
  v.assign(initlist.begin(), initlist.end());
  cout << "after assign, v.size(): " << v.size() << ", v.capacity: " << v.capacity() << endl;
  PrintElem(v, "after v.assign(begin, end)");
  v.assign(initlist);
  cout << "after assign, v.size(): " << v.size() << ", v.capacity: " << v.capacity() << endl;
  PrintElem(v, "after v.assign(initlist)");
  // 3. access, return reference, at() would check range, throw out_of_range exception when invalid
  auto first = *initlist.begin();
  auto last = *(initlist.end() - 1);
  ASSERT_EQ(v[0], first);
  ASSERT_EQ(v.at(0), first);
  ASSERT_EQ(v.front(), first);
  ASSERT_EQ(v.back(), last);
  // 4. iterator, 一般用pointer 实现，但依赖于具体实现
  // begin, end, rbegin, rend
  // cbegin, cend, crbegin, crend
  // 5. inserting or removing
  if (!v.empty()) {
    v.pop_back();
    PrintElem(v, "after pop_back");
  }

  // erase(pos) 移除与某值相等的第一个元素
  auto pos = find(v.begin(), v.end(), t);
  if (pos != v.end()) {
    v.erase(pos);
    PrintElem(v, "after erase pos");
  }
  // erase(beg, end): 将值为t的所有元素删除，remove算法返回逻辑尾部
  v.erase(remove(v.begin(), v.end(), t), v.end());
  PrintElem(v, "after erase t");
  v.clear();
  // push_back, pop_back
  for (int i = 0; i < 5; ++i) {
    v.push_back(t);
  }
  v.pop_back();
  PrintElem(v, "after push_back 5 t and pop_back");
  v.clear();
  // insert(pos, v)
  v.insert(v.begin(), *initlist.begin());
  PrintElem(v, "after insert initlist[0] before begin");
  v.clear();
  // insert(pos, n, v)
  v.insert(v.begin(), 5, *initlist.begin());
  PrintElem(v, "after insert initlist 5 elem before begin");
  v.clear();
  // insert(pos, beg, end)
  v.insert(v.begin(), initlist.begin(), initlist.end());
  PrintElem(v, "after insert initlist beg to end  elems before begin");
  v.clear();
  // insert(pos, initlist)
  v.insert(v.begin(), initlist);
  PrintElem(v, "after insert initlist before begin");
  v.clear();
  // emplace, emplace_back
  v.emplace(v.begin(), t);
  v.emplace_back(t);
  PrintElem(v, "after emplace t to beg and end");
  // resize
  v.resize(5);
  v.resize(10, t);
  PrintElem(v, "after resize to 10 with t");
  // data()
  cout << "v.data(): " << *v.data() << endl;
}

TEST(VectorTest, capacity) {
  TestVectorCap<int>(5);
  TestVectorCap<std::string>("abcde");
}

TEST(VectorTest, Op) {
  TestVectorOp<int>(9, {1, 2, 3, 4, 5, 6});
  TestVectorOp<string>("hello", {"this", "is", "init", "list"});
}

TEST(VectorTest, VectorBoolTest) {
  vector<bool> v{1, 1, 1, 0, 0, 0};
  v.push_back(true);
  v.push_back(false);
  PrintElem(v, "vector<bool>");
  v[0].flip();
  v[1] = true;
  v[4] = v[5];
  v.flip();
  PrintElem(v, "vector<bool>");
}

/**
 * 异常处理：
 * 1. at是唯一被c++ standard认可可以抛出异常的函数
 * 2. 只有一般标准异常（如bad_alloc）或用户自定义操作函数的异常，才有可能抛出异常
 * 3. 如果vector调用的函数（元素类型的函数，或用户自定义操作函数）抛出异常，c++有如下保证：
 * 4. push_back 安插元素时发生异常，函数将不产生任何效用
 * 5. 如果元素的copy/move操作不抛出异常，那么insert，emplace, emplace_back,
 * push_back,要么成功，要么不产生效用； erase不会抛出异常， pop_back  不会抛出任何异常
 * 6. 以上保证的前提是：析构函数不抛出异常
 */