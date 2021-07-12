#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace std;

TEST(StringTest, get_tmp_file_name) {
  string argv[] = {"prog.data", "mydir", "hello.", "oops.tmp", "end.data"};
  string extname, tmpname, suffix("tmp");
  for (auto& filename : argv) {
    auto pos = filename.find('.');
    // pos应该使用string::size_type 类型，而不是int或者unsigned
    if (pos == string::npos) {
      tmpname = filename + "." + suffix;
    } else {
      tmpname = filename;
      // substr
      // 1. 索引值必须合法；（查找单一字符或某个位置的所有查找函数，索引值可为任意值）
      // 2. 字符个数可为任意值
      if (filename.substr(pos + 1) == suffix) {
        tmpname.replace(pos + 1, string::npos, "xxx");
      } else {
        tmpname.replace(pos + 1, string::npos, suffix);
      }
    }
    cout << filename << " => " << tmpname << endl;
  }
}

TEST(StringTest, reverseWord) {
  string delims{" \t,.;"};
  string line;
  while (getline(cin, line)) {
    string::size_type begIdx, endIdx;
    begIdx = line.find_first_not_of(delims);
    while (begIdx != string::npos) {
      endIdx = line.find_first_of(delims, begIdx);
      if (endIdx == string::npos) {
        endIdx = line.length();
      }
      // begIdx 转化为整数再比较
      for (int i = endIdx - 1; i >= static_cast<int>(begIdx); --i) {
        cout << line[i];  // length() 是一个合法的索引值，默认返回'\0'
      }
      cout << " ";
      begIdx = line.find_first_not_of(delims, endIdx);
    }
    cout << endl;
  }
}

TEST(rngTest, rng) {
  std::mt19937_64 rng(std::random_device{}());
  for (int i = 0; i < 10; i++) {
    uint64_t low = rng();
    uint64_t high = rng();
    std::ostringstream os;
    os << std::setw(16) << std::setfill('0') << std::hex << high;
    os << std::setw(16) << std::setfill('0') << std::hex << low;
    std::cout << os.str() << endl;
  }
}

TEST(ostreamTest, ostream) {
  std::ostringstream os;
  os << "hello ";
  os << "world";
  cout << os.str() << endl;
}