#include <gtest/gtest.h>

#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

using namespace std;

/**
 * stringstream class    成员函数
 * str() 将缓冲区内容当做一个string 返回
 * str(string) 将string设为缓冲区内容
 */

TEST(ssTest, stringstream1) {
  ostringstream os;
  os << "dec: " << 15 << hex << "hex: " << 15 << endl;
  cout << os.str() << endl;
  bitset<15> b(5789);
  os << "float: " << 4.67 << " bitset: " << b << endl;
  os.seekp(0);
  os << "oct: " << 15;
  cout << os.str() << endl;
  int x = 0;
  double f = 0.1;
  string s = "3.7";
  istringstream is(s);
  is >> x >> f;
  cout << "last s: " << is.str() << ", get int and float form string , x: " << x << " f: " << f
       << endl;

  string ss("value: ");
  ostringstream oss(ss, ios::out | ios::ate);
  oss << 77 << " " << hex << 77 << endl;
  cout << oss.str() << endl;
  cout << ss << endl;
}

// after c11, string stream 提供了move语义
// 1. 可使用临时ostringstream对象
// 2. ostringstream 对象可作为函数参数或返回值

tuple<string, string, string> parsename(string name) {
  string s1, s2, s3;
  istringstream(name) >> s1 >> s2 >> s3;
  if (s3.empty()) {
    return tuple<string, string, string>(move(s1), "", move(s2));
  } else {
    return tuple<string, string, string>(move(s1), move(s2), move(s3));
  }
}

TEST(ssTest, rvalue) {
  auto t = parsename("Nicolai M. Justies");
  cout << "firstname: " << get<0>(t) << ", secondname: " << get<1>(t) << ", lastname: " << get<2>(t)
       << endl;
  auto t1 = parsename("mark fqwu");
  cout << "firstname: " << get<0>(t1) << ", secondname: " << get<1>(t1)
       << ", lastname: " << get<2>(t1) << endl;
}