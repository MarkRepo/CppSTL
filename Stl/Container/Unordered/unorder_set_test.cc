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
 *
 * 异常处理，适用于所有unordered 容器：
 * 1. clear不抛出异常
 * 2. 如果hash函数和等价准则不抛出异常，那么：
 *  2.1 单元素安插，erase， swap， rehash 要么成功，要么无效
 */

#include <gtest/gtest.h>

#include <unordered_set>

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
void UnordSetTestOp(const T& t, const std::initializer_list<T>& initlist) {
  // 1. Create
  // Unord(bnum); bucket num
  unordered_set<T> um(10);
  // Unord(bnum, hf); hash function
  unordered_set<T, CustomHash<T>> umh(10, CustomHash<T>());
  // Unord(bnum , hf, Cmp);
  unordered_set<T, CustomHash<T>, CustomCmp<T>> umhc(10, CustomHash<T>(), CustomCmp<T>());
  // Unord(beg, end)
  unordered_set<T>(initlist.begin(), initlist.end());
  // Unord(beg, end, bnum)
  unordered_set<T>(initlist.begin(), initlist.end(), 10);
  // Unord(beg, end, bnum, hf)
  unordered_set<T, CustomHash<T>> umih(initlist.begin(), initlist.end(), 10, CustomHash<T>());
  // Unord(beg, end, bnum, hf, cmp)
  unordered_set<T, CustomHash<T>, CustomCmp<T>> umihc(
      initlist.begin(), initlist.end(), 10, CustomHash<T>(), CustomCmp<T>());
  // max_load_factor(float); 0.7 ~0.8 是一个速度和内存之间较好的折中
  cout << "um max_load_factor" << um.max_load_factor() << endl;
  um.max_load_factor(0.8);
  // hash_function
  um.hash_function();
  // equal_function
  um.key_eq();
  um.bucket_count();
  um.max_bucket_count();
  // 负载系数：已使用buckey/all bucket
  um.load_factor();
  // 返回最大负载系数
  um.max_load_factor();
  // 设置最大负载系数，最大为1
  um.max_load_factor(0.8);
  // rehash(bnum); 不考虑最大负载系数
  um.rehash(20);
  cout << "after rehash(20), bucket count " << um.bucket_count() << endl;
  // reserve(bnum) : 考虑最大负载系数
  um.reserve(100);
  // count, find, equal_range
  um.count(t);
  um.find(t);
  um.equal_range(t);
  cout << "after count find equal_range" << endl;
  // insert emplace erase
  // insert(val)
  um.insert(t);
  // insert(hint, val)
  um.insert(um.begin(), t);
  // insert(beg,end)
  um.insert(initlist.begin(), initlist.end());
  // insert(initlist)
  um.insert(initlist);
  cout << "after insert test" << endl;
  // emplace(args...)
  um.emplace(t);
  // emplace(hint, args...)
  um.emplace_hint(um.begin(), t);
  cout << "after emplace" << endl;
  // erase(val)
  um.erase(t);
  // erase(pos)
  um.erase(um.begin());
  // erase(beg, end)
  um.erase(um.begin(), um.end());
  cout << "after erase" << endl;
  // clear()
  um.clear();
  // bucket 接口
  um.insert(t);
  // bucket_count  当前bucket个数
  cout << "current bucket count" << um.bucket_count() << endl;
  // bucket(val) 返回val被找到的哪个bucket
  cout << t << " is in " << um.bucket(t) << "bucket" << endl;
  // bucket_size(buckidx)， 返回第buckidx个bucket包含的元素个数
  cout << "bucket_size( " << um.bucket(t) << "): " << um.bucket_size(um.bucket(t)) << endl;
  // begin(buckidx): 返回forward_iterator，指向第buckidx个bucket中第一个元素
  um.begin(um.bucket(t));
  // end(buckidx)
  um.end(um.bucket(t));
  // cbegin(buckidx)
  um.cbegin(um.bucket(t));
  // cend(buckidx)
  um.cend(um.bucket(t));
}

TEST(uSetTest, op) {
  UnordSetTestOp<int>(9, {1, 2, 3, 4, 5, 6});
  UnordSetTestOp<std::string>("hello", {"this", "is", "set", "test"});
}