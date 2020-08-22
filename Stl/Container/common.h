#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>

template <typename Os, typename T1, typename T2>
Os& operator<<(Os& out, const std::pair<const T1, T2>& p) {
  out << "{" << p.first << " : " << p.second << "}, ";
  return out;
}

template <typename Cont>
void PrintElem(const Cont& C, const std::string& s) {
  std::cout << "========= " << s << " ==========" << std::endl;
  for (const auto& v : C) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
}

template <typename Cont>
void PrintElem2(const Cont& C, const std::string& s) {
  std::cout << "========= " << s << " ==========" << std::endl;
  std::copy(C.cbegin(), C.cend(), std::ostream_iterator<typename Cont::value_type>(std::cout, " "));
  std::cout << std::endl;
}
