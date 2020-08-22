// from boost

#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Stl/Container/common.h"

//一个良好的hash函数
template <typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
  seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
inline void hash_val(std::size_t& seed, const T& val) {
  return hash_combine(seed, val);
}

template <typename T, typename... Types>
inline void hash_val(std::size_t& seed, const T& val, const Types&... args) {
  hash_combine(seed, val);
  hash_val(seed, args...);
}

// auxilirary generic function to create a hash value out of heterogenous list of arguments
template <typename... Types>
inline std::size_t hash_val(const Types&... args) {
  std::size_t seed = 0;
  hash_val(seed, args...);
  return seed;
}

struct Customer {
  std::string first_name;
  std::string last_name;
  int age;
  double f;
};

class CustomerHash {
 public:
  std::size_t operator()(const Customer& c) const {
    return hash_val(c.first_name, c.last_name, c.age, c.f);
  }
};

class CustomerCmp {
 public:
  bool operator()(const Customer& c1, const Customer& c2) const {
    return c1.first_name == c2.first_name && c1.last_name == c2.last_name && c1.age == c2.age &&
           c1.f == c2.f;
  }
};

std::ostream& operator<<(std::ostream& os, const Customer& c) {
  os << "{ first_name: " << c.first_name << ", last_name: " << c.last_name << "}" << std::endl;
  return os;
}

template <typename T>
void PrintHashTableState(const T& cont, const std::string& desc) {
  // basic layout data
  std::cout << "========================== " << desc
            << " ==============================" << std::endl;
  std::cout << "size:               " << cont.size() << std::endl;
  std::cout << "bucket_count:       " << cont.bucket_count() << std::endl;
  std::cout << "load factor:        " << cont.load_factor() << std::endl;
  std::cout << "max load factor:    " << cont.max_load_factor() << std::endl;

  // iterator category
  if (typeid(typename std::iterator_traits<typename T::iterator>::iterator_category) ==
      typeid(std::bidirectional_iterator_tag)) {
    std::cout << "chaining-type double-linked" << std::endl;
  } else {
    std::cout << "chaing-type single-linked" << std::endl;
  }

  // elements per bucket
  std::cout << "data: " << std::endl;
  for (auto idx = 0; idx < cont.bucket_count(); idx++) {
    std::cout << "b[" << std::setw(2) << idx << "]: ";
    for (auto pos = cont.begin(idx); pos != cont.end(idx); ++pos) {
      std::cout << *pos << " ";
    }
    std::cout << std::endl;
  }
}

TEST(HashTest, test1) {
  std::unordered_set<Customer, CustomerHash, CustomerCmp> cum{100};
  cum.max_load_factor(0.8);  // 测试是否rehash了
  std::cout << "before insert, bucket_count: " << cum.bucket_count()
            << ", load_factor: " << cum.load_factor() << std::endl;
  for (int i = 0; i < 81; i++) {
    Customer c{std::string("Luo") + std::to_string(i),
               std::string("yunlong") + std::to_string(i),
               55 + i,
               1.8 + i};
    cum.insert(c);
  }
  PrintElem(cum, "cum");
  std::cout << "after insert, bucket_count: " << cum.bucket_count()
            << ", load_factor: " << cum.load_factor() << std::endl;
  std::unordered_set<int> intset = {1, 2, 3, 5, 7, 11, 13, 17, 19};
  PrintHashTableState(intset, "intset before insert");
  intset.insert({-7, 17, 33, 4});
  PrintHashTableState(intset, "intset after insert");
  std::unordered_map<int, int> intmap = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
  PrintHashTableState(intmap, "intmap before insert");
  intmap.insert({{7, 7}, {8, 8}, {9, 9}, {10, 10}, {11, 11}});
  PrintHashTableState(intmap, "intmap after insert");
  intmap.max_load_factor(0.4);
  PrintHashTableState(intmap, "intmap.max_load_factor(0.2)");
  intmap.insert({15, 15});
  PrintHashTableState(intmap, "intmap after insert");
}