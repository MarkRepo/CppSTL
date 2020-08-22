#include <gtest/gtest.h>

#include <iostream>
#include <iterator>
#include <vector>

TEST(CArrayTest, cTest) {
  int vals[] = {1, 2, 3, 4, 5, 6, 7};
  std::vector<int> v(std::begin(vals), std::end(vals));
  std::copy(std::begin(vals), std::end(vals), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}