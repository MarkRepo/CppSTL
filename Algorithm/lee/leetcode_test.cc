#include <gtest/gtest.h>

#include <iostream>
#include <vector>

using namespace std;

TEST(LeetCodeTest, last) {
  int n = 70886, m = 116922;
  vector<int> vec;
  for (int i = 0; i < n; ++i) {
    vec.emplace_back(i);
  }
  int count = 1, step = 0, index = 0;
  while (count < n) {
    while (1) {
      if (vec[index] != -1) {
        cout << "++step, index: " << index << ", " << vec[index] << endl;
        ++step;
        if (step == m) break;
      }
      index = (index + 1) % n;
    }
    cout << "set to -1: " << index << ", " << vec[index] << endl;
    vec[index] = -1;
    index = (index + 1) % n;
    step = 0;
    count++;
  }
  step = 0;
  while (step < n) {
    if (vec[step] != -1) cout << vec[step] << endl;
    step++;
  }
  cout << 0 << endl;
}