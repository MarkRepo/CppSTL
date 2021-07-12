#include <gtest/gtest.h>

#include <deque>
#include <iostream>

using namespace std;

// 必须使用一种数据结构支持在头部和尾部进行插入和删除，很明显 deque 和 list 符合要求。
class MonotonicQueue {
 private:
  deque<int> d;

 public:
  MonotonicQueue() {}
  void push(int n);
  void pop(int n);
  int max();
};

void MonotonicQueue::push(int n) {
  // 要把前面比自己小的元素都删掉
  // 如果每个元素被加入时都执行了该操作，最终单调队列中的元素大小就会保持一个单调递减的顺序
  while (!d.empty() && d.back() < n) {
    d.pop_back();
  }
  // 将n加入尾部
  d.push_back(n);
}

void MonotonicQueue::pop(int n) {
  // 想删除的队头元素n可能已经被「压扁」了，已经不存在了，所以这时候就不用删除了
  if (n == d.front()) {
    d.pop_front();
  }
}

int MonotonicQueue::max() { return d.front(); }

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
  MonotonicQueue w;
  vector<int> res;
  for (int i = 0; i < nums.size(); i++) {
    // 先填满窗口的前k-1个元素
    if (i < k - 1) {
      w.push(nums[i]);
    } else {
      w.push(nums[i]);
      res.push_back(w.max());
      w.pop(nums[i - k + 1]);
    }
  }

  return res;
}

TEST(Algo, monotonic_queue) {
  vector<int> nums{1, 3, -1, -3, 5, 3, 6, 7};
  auto res = maxSlidingWindow(nums, 3);
  for (auto v : res) {
    cout << v << " ";
  }
  cout << endl;
}