#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

using namespace std;

TEST(RandomTest, example1) {
  std::default_random_engine dre;
  // [beg, end]
  std::uniform_int_distribution<int> di(10, 20);
  for (int i = 0; i < 20; i++) {
    cout << di(dre) << " ";
  }
  cout << std::endl;
  // [beg, end)
  std::uniform_real_distribution<double> dr(10, 20);
  for (int i = 0; i < 8; i++) {
    cout << dr(dre) << " " ;
  }
  cout << std::endl;

  //shuffle
  std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::shuffle(v.begin(), v.end(), dre);
  for (auto e : v) {
    cout << e << " ";
  }
  cout << endl;

  // dre init state is same
  std::default_random_engine dre1;
  std::default_random_engine dre2;
  cout << di(dre1) << std::endl;
  cout << di(dre2) << std::endl;
  
}