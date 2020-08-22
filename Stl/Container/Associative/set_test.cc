/**
 * set: strict weak ordering:
 * 1. 非对称：
 * if x < y == true; then y < x == false
 * if op(x, y) == true; then op(y,x) == false
 * 2. 可传递
 * if x < y && y < z; then  x < z
 * if op(x,y) && op (y,z); then op(x, z)
 * 3. 非自反
 * x < x == false
 * op(x,x) == false
 * 4. 等效传递
 * if a == b && b == c; then a == c
 * if !(a<b) && !(b<a) && !(b<c) && !(c<b); then !(a<c) && !(c<a);
 * if !op(a,b) && !op(b,a) && !op(b,c) && !op(c,b); then !op(a,c) && !op(c,a);
 */

/**
 * set, multiset 能力：
 * 1.
 * 不能直接改变元素值，因为会打乱原本正确的顺序，必须先删除旧的，再插入新的；不能用于remove算法，因为key为常量值，不允许覆盖
 * 2. 不提供操作函数直接访问元素
 * 3. 使用迭代器间接访问：从迭代器的角度看，元素值是常量
 * 4. 双向迭代器
 * 5. 赋值会将排序准则实例一起复制，容器类型相同才可以赋值。
 */

#include <gtest/gtest.h>

#include <set>
#include <string>

#include "Stl/Container/common.h"

using namespace std;

//除Stl/Container/Common:op_test 之外的操作
// set/multi_set 操作基本一样，有一些insert,emplace 返回类型不一样，源于multi_set 允许重复元素。
template <typename T>
void SetTestOp(const T& t, const std::initializer_list<T>& initlist) {
  // 1. Create
  auto comp = [](const T& t1, const T& t2) { return t1 < t2; };
  // set(op)
  set<T, decltype(comp)> s(comp);
  s.insert(initlist);
  PrintElem(s, "s after insert initlist");
  // set(beg, end, op)
  set<T, decltype(comp)> s1(initlist.begin(), initlist.end(), comp);
  PrintElem(s1, "s1 after init with initlist");
  // 2. key_comp, value_comp
  auto key_comp = s1.key_comp();
  auto value_comp = s1.value_comp();
  ASSERT_EQ(key_comp, comp);
  ASSERT_EQ(value_comp, comp);
  // 3. 查找操作函数
  // count
  cout << "s1 has " << s1.count(t) << " " << t << endl;
  // find
  if (s1.find(t) == s1.end()) {
    cout << "s1 not find " << t << endl;
  }
  // lower_bound;
  cout << "lower_bound(" << *(initlist.begin()) << "): " << *s1.lower_bound(*initlist.begin())
       << endl;
  // upper_bound;
  cout << "upper_bound(" << *(initlist.begin()) << "): " << *s1.upper_bound(*initlist.begin())
       << endl;
  // equal_range;
  cout << "equal_range(" << *(initlist.begin()) << "): " << *s1.equal_range(*initlist.begin()).first
       << "  " << *s1.equal_range(*initlist.begin()).second << endl;
  // 4. insert and remove； c11 保证multiset 的insert、 emplace、
  // erase成员函数会保存等值元素间的相对顺序，插入的元素会被放在“既有的等值元素” 的末尾
  // insert(val)， return 新元素位置
  s1.insert(t);
  // insert(pos, val)， return 新元素位置
  s1.insert(s1.end(), t);
  // insert(beg, end)， 无返回值
  s1.insert(initlist.begin(), initlist.end());
  // insert(initlist)， 无返回值
  s1.insert(initlist);
  // emplace(args...)， return 新元素位置
  s1.emplace(t);
  // emplace(pos, args...)， return 新元素位置
  s1.emplace_hint(s1.begin(), t);
  // erase(val)， return 移除元素个数
  s1.erase(t);
  // erase(pos)， 无返回值
  s1.erase(s1.begin());
  // erase(beg, end)， 无返回值
  s1.erase(s1.begin(), s1.end());
  // clear
  s1.clear();
}

TEST(SetTest, op) {
  SetTestOp<int>(9, {1, 2, 3, 4, 5, 6});
  SetTestOp<string>("hello", {"this", "is", "set", "test", "thjk"});
}