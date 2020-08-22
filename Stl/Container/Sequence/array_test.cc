#include <gtest/gtest.h>

#include <array>
#include <iostream>
#include <utility>

#include "Stl/Container/common.h"

using namespace std;

//聚合体概念：
//聚合体是一个array 或 class， 不带用户提供的构造函数，也没有private 或protected 的non-static
//数据成员， 也没有base-class, 没有virtual函数
// std::array 是个聚合体

class MyType {
 public:
  MyType() { cout << "MyType is initialized" << endl; }
};

ostream& operator<<(ostream& out, const MyType& m) {
  out << "MyType elem" << endl;
  return out;
}

TEST(ArrayTest, Initialize) {
  std::array<int, 5> ai;
  PrintElem(ai, "array<int, 5> not initialized");
  std::array<MyType, 5> am;
  PrintElem(am, "array<MyType, 2> Initialze");
  std::array<int, 5> ai2 = {};
  PrintElem(ai2, "array<int, 5> ai2 = {}; is initialized to 0");
  std::array<int, 5> ai3 = {42};  //聚合体初始化语法
  PrintElem(ai3, "array<int, 5> ai3 = {42}, all is initialized");
  // array 没有为initializer_list 提供构造函数或赋值操作符
  std::array<int, 5> ai5{1, 2, 3, 4, 5};     // 与下面等价
  std::array<int, 5> ai6 = {1, 2, 3, 4, 5};  // 与下面等价
  std::array<int, 5> ai4({1, 2, 3, 4, 5});  // initlist元素个数超过array size 会导致编译问题,
                                            // 这里使用的仍然是聚合体语法，不是构造函数语法
  PrintElem(ai4, "array<int, t> ai4({1,2,3,4,5}) initialzie");
  PrintElem(ai5, "array<int, t> ai5{1,2,3,4,5} initialzie");
  PrintElem(ai6, "array<int, t> ai6 = {1,2,3,4,5} initialzie");
}

TEST(ArrayTest, op_test) {
  // swap test, 线性复杂度
  std::array<int, 3> ai1{1, 2, 3}, ai2{2, 3, 4};
  auto iter1 = ai1.begin();
  ai1.swap(ai2);
  PrintElem(ai1, "ai1 after swap should be 2 3 4");
  std::cout << "after swap *iter1 should be 2 : " << *iter1 << std::endl;
  // move test
  ai1 = std::move(ai2);
  PrintElem(ai2, "ai2 after moved");
  PrintElem(ai1, "ai1 after moved");
  // size
  std::cout << "ai1 size : " << ai1.size() << std::endl;
  std::array<int, 0> am;
  std::cout << "am data(): " << am.data() << std::endl;
  std::cout << "am begin(): " << am.begin() << std::endl;
}

// 除了 Stl/Container/Common:op_test 里面的通用操作以外，array的操作:
template <typename Elem, std::size_t N>
void TestArrayOp(const Elem& e) {
  // 1. fill
  std::array<Elem, N> a;
  a.fill(e);
  PrintElem(a, "a after fill e");
  // access, return reference, at() would check, throw out_of_range exception when invaid
  ASSERT_EQ(a[0], e);
  ASSERT_EQ(a.at(0), e);
  ASSERT_EQ(a.front(), e);
  ASSERT_EQ(a.back(), e);
  // tuple interface
  ASSERT_EQ((std::tuple_size<std::array<Elem, N>>::value), N);
  typename std::tuple_element<1, std::array<Elem, N>>::type e1 = e;
  std::cout << "use type to define e1: " << e1 << endl;
  ASSERT_EQ(std::get<1>(a), e);
  // iterator, 一般用pointer 实现，但依赖于具体实现
  // begin, end, rbegin, rend
  // cbegin, cend, crbegin, crend
}

TEST(ArrayTest, operatrion) {
  TestArrayOp<std::string, 5>("abcde");
  TestArrayOp<char, 10>('a');
  TestArrayOp<int, 8>(10);
  TestArrayOp<float, 9>(8.0);
}