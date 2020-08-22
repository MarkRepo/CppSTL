#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Stl/Container/common.h"

using namespace std;

// 容器的共通能力
// 1. 所有容器提供的都是 'value 语义' 而非 'reference 语义'
// 2. 元素在容器内有其特定顺序
// 3. 一般而言，各项操作并非绝对安全

template <typename Cont>
void TestContCommonOp(std::initializer_list<typename Cont::value_type> initlist) {
  // 1. default construct
  Cont c;
  // 2. copy construct
  Cont c1(c);
  Cont c2 = c1;
  // 3. move construct, except array<>
  Cont c3(std::move(c1));
  Cont c4 = std::move(c2);
  // 4. use [beg, end) to init Cont, except array<>
  Cont c5(initlist.begin(), initlist.end());
  // 4.1 move iterator
  std::vector<typename Cont::value_type> v2(std::make_move_iterator(c5.begin()),
                                            std::make_move_iterator(c5.end()));
  PrintElem(v2, "vector<int> v2");
  // 4.2 C-style array
  int carray[] = {2, 3, 17, 33, 45, 77};
  std::set<int> s(std::begin(carray), std::end(carray));  // after c11
  PrintElem(s, "set<int> init from C-style array");
  std::set<int> s1(carray, carray + sizeof(carray) / sizeof(carray[0]));  // before c11
  PrintElem(s, "set<int> init from C-style array before c11");
  // 4.3 from standard input
  std::deque<int> d{std::istream_iterator<int>(std::cin), std::istream_iterator<int>()};
  PrintElem(d, "deque<int> init from istream_itertor");
  // 5. initlist initialize, except array<>
  Cont c6(initlist);
  Cont c7 = initlist; // array 是聚合体，可以使用这种语法初始化元素
  // 6. empty(), size(), max_size()
  cout << "c7 is empty : " << boolalpha << c7.empty() << endl;
  cout << "c7 size: " << c7.size() << endl;         // except forward_list<>
  cout << "c7 max_size " << c7.max_size() << endl;  //最大可能数量
  // 7. 比较操作符 ==, !=, <, <=, >, >=
  cout << "c6 == c7: " << boolalpha << (c6 == c7) << endl;
  cout << "c6 != c7: " << boolalpha << (c6 != c7) << endl;
  // <,<=,>,>= ; except unordered container
  cout << "c6 < c7: " << boolalpha << (c6 < c7) << endl;
  cout << "c6 > c7: " << boolalpha << (c6 > c7) << endl;
  cout << "c6 <= c7: " << boolalpha << (c6 <= c7) << endl;
  cout << "c6 >= c7: " << boolalpha << (c6 >= c7) << endl;
  // 8. 赋值
  c7 = c6;
  c7 = std::move(c6);  // except array<>
  c7 = initlist;       // except array<>
  // 9. swap 保证常量复杂度, swap array 拥有线性复杂度，因为没有pointer可置换
  c7.swap(c5);
  swap(c7, c5);
  // 10 begin(), end(), cbegin(), cend()
  cout << "use begin() end() to iterate Cont: " << endl;
  for (auto iter = c7.begin(); iter != c7.end(); ++iter) {
    cout << *iter << " ";
  }
  cout << endl;
  cout << "use cbegin() cend() to iterate Cont: " << endl;
  for (auto iter = c7.cbegin(); iter != c7.cend(); ++iter) {
    cout << *iter << " ";
  }
  cout << endl;
  // 11. 移除所有元素，令容器为空，不适用于array<>
  c7.clear();
}

template <typename Cont>
void TestContCommonType(std::initializer_list<typename Cont::value_type> initlist) {
  Cont c = initlist;
  typename Cont::size_type s = c.size();
  typename Cont::difference_type d = c.end() - c.begin();  // not for list
  typename Cont::value_type v = *(c.begin());
  typename Cont::reference r = v;
  typename Cont::const_reference cr = v;
  typename Cont::iterator i = c.begin();
  typename Cont::const_iterator ci = c.cbegin();
  typename Cont::pointer p = &c[0];
  typename Cont::const_pointer cp = &c[0];
  cout << "c.size(): " << s << endl;
  cout << "begin to end difference: " << d << endl;
  cout << "begin value: " << v << endl;
  cout << "begin value reference, const_reference:  " << r << " " << cr << endl;
  cout << "iterator, const iterator to begin value: " << *i << " " << *ci << endl;
  cout << "pointer, const_pointer to begin value: " << *p << " " << *cp << endl;
  cout << endl;
}

TEST(CommonTest, op_test) {
  TestContCommonOp<std::vector<int>>({1, 2, 3, 4, 5, 6, 7, 8});
  TestContCommonOp<std::list<std::string>>({"a", "b", "c"});
}

TEST(CommonTest, type_test) {
  TestContCommonType<std::vector<int>>({1, 2, 3, 4, 5, 6, 7, 8});
  TestContCommonType<std::deque<int>>({1, 2, 3, 4, 5, 6});
  // TestContCommonType<std::list<std::string>>({"a", "b", "c"});
}