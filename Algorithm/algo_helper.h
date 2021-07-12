#ifndef ALGO_HELPER_H_
#define ALGO_HELPER_H_

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
inline void INSERT_ELEMENTS(T& coll, int first, int last) {
  for (auto i = first; i <= last; ++i) {
    coll.insert(coll.end(), i);
  }
}

template <typename T>
inline void PRINT_ELEMENTS(const T& coll, const std::string& optstr = "") {
  std::cout << optstr;
  for (auto& elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}

template <typename T>
inline void PRINT_MAPPED_ELEMENTS(const T& coll, const std::string& optstr = "") {
  std::cout << optstr;
  for (auto& elem : coll) {
    std::cout << '[' << elem.first << ',' << elem.second << "] ";
  }
  std::cout << std::endl;
}

#endif