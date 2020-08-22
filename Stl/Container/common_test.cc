#include "Stl/Container/common.h"

#include <gtest/gtest.h>

#include <map>
#include <string>

using namespace std;

TEST(CommonTest, op) {
  std::pair<const int, int> p{1, 2};
  cout << p << endl;
  std::pair<const string, string> ps{"hello", "world"};
  cout << ps << endl;
  std::map<int, int> m{{1, 2}, {3, 4}};
  PrintElem(m, "map<int, int>");
  // 下面用法类型有问题。
  // PrintElem2(m, "map<int, int>");
}