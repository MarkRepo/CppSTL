#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <list>
using namespace std;

struct interval {
  int start;
  int end;
};

int intervalSchedule(vector<interval> intvs) {
  if (intvs.size() == 0) return 0;
  // 按 end 升序排序
  sort(intvs.begin(), intvs.end(), [](interval i1, interval i2) { return i1.end <= i2.end; });
  // 至少有一个区间不相交
  int count = 1;
  // 排序后，第一个就是end最小的区间
  int x_end = intvs[0].end;
  for (auto i : intvs) {
    if (i.start >= x_end) {
      count++;
      x_end = i.end;
    }
  }
  return count;
}

TEST(Algo, intervalSchedule) {
  vector<interval> intvs{
      interval{2, 4}, interval{1, 3}, interval{3, 6}, interval{6, 9}, interval{5, 8}};
  cout << intervalSchedule(intvs) << endl;
}

TEST(Algo, random) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(1, 6);  // [1, 6];
  vector<int> count(6, 0);
  cout << count.size() << endl;
  for (int i = 0; i < 100; i++) {
    auto g = dis(gen);
    cout << g << " " << i << endl;
    count[g - 1]++;
  }
  cout << count.size() << endl;
  for (int i = 0; i < count.size(); i++) {
    cout << count[i] << endl;
  }
  for (auto c : count) {
    cout << c << " ";
  }
  cout << endl;
}