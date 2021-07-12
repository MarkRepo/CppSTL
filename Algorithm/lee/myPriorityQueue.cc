#include <gtest/gtest.h>

#include <vector>
using namespace std;

// 实现最大堆
template <typename T>
class MyPQ {
 public:
  void push(T t) {
    v.emplace_back(t);
    swim(v.size() - 1);
  }

  T top() { return v[0]; }

  void pop() {
    swap(0, v.size() - 1);
    v.pop_back();
    sink(0);
  }

 private:
  vector<T> v;

  void swim(int i) {
    while (i > 0 && less(parent(i), i)) {
      swap(parent(i), i);
      i = parent(i);
    }
  }

  void sink(int i) {
    int n = v.size();
    while (left(i) < n) {
      int max = left(i);
      if (right(i) < n && less(max, right(i))) max = right(i);
      if (less(max, i)) break;
      swap(max, i);
      i = max;
    }
  }

  int parent(int i) { return (i - 1) / 2; }
  int left(int i) { return 2 * i + 1; }
  int right(int i) { return 2 * i + 2; };

  void swap(int i, int j) {
    auto temp = v[i];
    v[i] = v[j];
    v[j] = temp;
  }

  bool less(int i, int j) { return v[i] < v[j]; }
};

TEST(Algo, pq) {
  MyPQ<int> pq;
  pq.push(1);
  pq.push(3);
  pq.push(7);
  pq.push(5);
  pq.push(4);
  cout << pq.top() << endl;
  pq.pop();
  cout << pq.top() << endl;
}