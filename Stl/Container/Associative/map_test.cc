/**
 * map,multimap 的能力与set，multiset 一样
 */

#include <gtest/gtest.h>

#include <map>
#include <tuple>
#include <utility>

#include "Stl/Container/common.h"
using namespace std;

//除Stl/Container/Common: op_test 以外的map 操作, 与set类似

template <typename T>
void MapTestOp(const pair<const T, T>& t, std::initializer_list<std::pair<const T, T>> initlist) {
  // 1. Create
  auto comp = [](const T& t1, const T& t2) { return t1 < t2; };
  // map(op)
  map<T, T, decltype(comp)> m(comp);
  m.insert(initlist);
  PrintElem(m, "m after insert initlist");
  // map(beg, end, op)
  map<T, T, decltype(comp)> m1(initlist.begin(), initlist.end(), comp);
  PrintElem(m1, "m1 after init with initlist");
  // 2. key_comp, value_comp
  auto key_comp = m1.key_comp();
  m1.value_comp();
  ASSERT_EQ(key_comp, comp);
  // 3. count, find, lower_bound, upper_bound, equal_range, 针对key，与set相同
  // 4. insert, emplace, erase 均与set相同，除了value是pair
  // insert
  m1.insert(t);
  m1.insert(m1.begin(), t);
  m1.insert(initlist.begin(), initlist.end());
  m1.insert(initlist);
  m1.insert(std::make_pair(t.first, t.second));
  m1.insert(typename std::map<T, T>::value_type(t));
  m1.insert(typename decltype(m1)::value_type(t));
  m1.insert({t.first, t.second});
  // emplace(key_args..., value_args...) 必须传递两列实参，一列为key， 一列为value
  m1.emplace(std::piecewise_construct, std::make_tuple(t.first), std::make_tuple(t.second));
  m1.emplace_hint(
      m1.begin(), std::piecewise_construct, std::make_tuple(t.first), std::make_tuple(t.second));
  // 正确删除元素，c11 erase 会返回后继元素；（注意迭代器失效问题）
  for (auto pos = m1.begin(); pos != m1.end();) {
    if (*pos == t) {
      pos = m1.erase(pos);
    } else {
      ++pos;
    }
  }
  // before c11
  for (auto pos = m1.begin(); pos != m1.end();) {
    if (*pos == t) {
      m1.erase(pos++);
    } else {
      ++pos;
    }
  }
  m1.erase(t.first);
  m1.erase(m1.begin(), m1.end());
  m1.clear();
  // access
  m1[t.first] = t.second;  // only for nonconst map
  m1.at(t.first);
}

TEST(MapTest, op) {
  std::initializer_list<std::pair<const int, int>> ii{{1, 2}, {3, 4}, {5, 6}};
  MapTestOp<int>({9, 9}, ii);
  // MapTestOp<string>({"hello", "world"}, {{"this", "is"}, {"map", "test"}});
}