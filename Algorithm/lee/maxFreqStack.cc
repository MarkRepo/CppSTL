#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class FreqStack {
 private:
  int maxFreq;                      // 当前最大freq
  map<int, int> keyToFreq;          // 元素对应的freq
  map<int, stack<int>> freqToKeys;  // 同一freq的元素在同一个stack

 public:
  FreqStack() { maxFreq = 0; }
  void push(int key);
  int pop();
};

void FreqStack::push(int key) {
  // 更新keyToFreq
  auto freq = keyToFreq[key] + 1;
  keyToFreq[key] = freq;
  // 跟新maxFreq
  maxFreq = max(freq, maxFreq);
  // 插入元素
  freqToKeys[freq].push(key);
}

int FreqStack::pop() {
  if (maxFreq == 0) return -1;
  // 弹出最大freq栈栈顶元素
  auto k = freqToKeys[maxFreq].top();
  freqToKeys[maxFreq].pop();
  // 更新该元素对应的freq
  keyToFreq[k] -= 1;
  // 更新maxFreq
  if (freqToKeys[maxFreq].empty()) maxFreq--;
  return k;
}

TEST(Algo, maxFreqStack) {
  FreqStack fs;
  fs.push(2);
  fs.push(7);
  fs.push(2);
  fs.push(7);
  fs.push(2);
  fs.push(4);
  int i = 0;
  while (i < 6) {
    cout << fs.pop() << endl;
    i++;
  }
}