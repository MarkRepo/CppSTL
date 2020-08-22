/**
 * unordered_set/map
 * can't
 * 1. no operator <,<=,>,>=, has == , !=
 * 2. no lower_bound, upper_bound
 * 3. just forward iterator
 * 4. const key
 * can:
 * 1. 指定bucket 最小数量
 * 2. hash function
 * 3. equal function
 * 4. 最大负载系数，超过则rehash
 * 5. 强制rehash
 *
 * rehashing 只可能发生在以下调用后：
 * 1. insert()
 * 2. rehash()
 * 3. reserve()
 * 4. clear()
 *
 * 这是为了保证erase() 不会造成reference, pointer, iterator的失效
 *
 * 带有等价key的元素会被相邻排列，即使是rehash后
 */

#include <gtest/gtest.h>

#include <unordered_map>

#include "Stl/Container/common.h"

using namespace std;

template <typename T>
class CustomHash {
 public:
  std::size_t operator()(const T& t) const {
    static long long c = 0;
    c++;
    return c % 100;
  }
};

template <typename T>
class CustomCmp {
 public:
  bool operator()(const T& t1, const T& t2) const { return t1 == t2; }
};

// 除通用操作外
template <typename T>
void UnordMapTestOp(const std::pair<T, T>& t,
                    const std::initializer_list<std::pair<const T, T>>& initlist) {
  // 1. Create
  // Unord(bnum); bucket num
  unordered_map<T, T> um(10);
  // Unord(bnum, hf); hash function
  unordered_map<T, T, CustomHash<T>> umh(10, CustomHash<T>());
  // Unord(bnum , hf, Cmp);
  unordered_map<T, T, CustomHash<T>, CustomCmp<T>> umhc(10, CustomHash<T>(), CustomCmp<T>());
  // Unord(beg, end)
  unordered_map<T, T>(initlist.begin(), initlist.end());
  // Unord(beg, end, bnum)
  unordered_map<T, T>(initlist.begin(), initlist.end(), 10);
  // Unord(beg, end, bnum, hf)
  unordered_map<T, T, CustomHash<T>> umih(initlist.begin(), initlist.end(), 10, CustomHash<T>());
  // Unord(beg, end, bnum, hf, cmp)
  unordered_map<T, T, CustomHash<T>, CustomCmp<T>> umihc(
      initlist.begin(), initlist.end(), 10, CustomHash<T>(), CustomCmp<T>());
  // max_load_factor(float); 0.7 ~0.8 是一个速度和内存之间较好的折中
  cout << "um max_load_factor" << um.max_load_factor() << endl;
  um.max_load_factor(0.8);
  // hash_function
  um.hash_function();
  um.key_eq();
  um.bucket_count();
  um.max_bucket_count();
  um.load_factor();
  um.max_load_factor();
  um.max_load_factor(0.8);
  um.rehash(20);
  cout << "after rehash(20), bucket cout " << um.bucket_count() << endl;
  um.reserve(100);
  PrintElem(um, "unordered_map um");
}

TEST(uSetTest, op) { UnordMapTestOp<int>({9, 9}, {{1, 2}, {2, 3}, {3, 4}}); };