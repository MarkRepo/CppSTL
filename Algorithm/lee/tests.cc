#include <gtest/gtest.h>

#include <map>

using namespace std;

struct Node {
  Node *prev, *next;
  int k, v;

  Node(int key, int val) : k(key), v(val) {
    prev = NULL;
    next = NULL;
  }
};

class DoubleList {
 public:
  DoubleList();
  void AddLast(int key, int val);
  void Remove(Node* x);
  void RemoveFirst();
  int Size() { return size; }

 private:
  Node *head, *tail;
  int size;
};

DoubleList::DoubleList() {
  tail = new Node(0, 0);
  head = new Node(0, 0);
  head->next = tail;
  tail->prev = head;
}

void DoubleList::AddLast(int key, int val) {
  Node* x = new Node(key, val);
  x->prev = tail->prev;
  x->next = tail;
  x->prev->next = x;
  tail->prev = x;
  size++;
}

void DoubleList::Remove(Node* x) {
  x->prev->next = x->next;
  x->next->prev = x->prev;
  size--;
}

Node* DoubleList::RemoveFirst() {
  if (size == 0) return;
  Node* d = head->next;
  Remove(d);
  return d;
}

class LRU {
 private:
  map<int, Node*> m;
  DoubleList cache;
  int cap;

 public:
  LRU(int cap) : cap(cap) {}
  int Get(int key);
  void Put(int Key, int val);

 private:
  void makeRecently(int key);
}

void LRU::makeRecently(int key) {
  Node* x = m[key];
  cache.Remove(x);
  cache.AddLast(x->k, x->v);
}

int LRU::Get(int key) {
  if (m.find(key) == m.end()) {
    return -1;  /// 没找到
  }

  makeRecently(key);
  return m[key]->v;
}

void LRU::Put(int key, int val) {
  if (m.find(key) != m.end()) {
    makeRecently(key);
    m[key] = val;
    return;
  }

  if (cap == cache.Size()) {
    auto d = cache.RemoveFirst();
    // 删除m中的key
    m.erase(d->k);
  }

  // 可以封装成函数
  cache.AddLast(key, val);
  m[key] = val;
}
