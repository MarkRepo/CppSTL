#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void writeCharSetToFile(const std::string& filename);
void outputFile(const std::string& filename);
void fastOutputFile(const std::string& filename);

TEST(FileAccTest, file1) {
  writeCharSetToFile("charset.out");
  fastOutputFile("charset.out");
}

void writeCharSetToFile(const std::string& filename) {
  std::ofstream file(filename);
  if (!file) {
    cerr << "can't open output file " << filename << endl;
    return;
  }
  for (int i = 32; i < 256; i++) {
    file << "value: " << setw(3) << i << "  "
         << "char:  " << static_cast<char>(i) << endl;
  }
}

void outputFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    cerr << "can't open file " << filename << endl;
    return;
  }
  char c;
  while (file.get(c)) {
    cout.put(c);
  }
}

void fastOutputFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    cerr << "can't open file " << filename << endl;
    return;
  }
  cout << file.rdbuf();
}

// C11 后，file stream 提供了rvalue语义，提供了move构造函数和move assignment， swap函数
TEST(FileAccTest, rvalue) {
  // 1. 可使用临时的filestream对象
  std::string s("hello");
  std::ofstream("fstream.tmp") << s << endl;
  std::ofstream("fstream.tmp", ios::app) << "world" << endl;
  cout << std::ifstream("fstream.tmp").rdbuf() << endl;
}

// 2. 函数可返回filestream
std::ofstream openFile(const std::string& filename) {
  std::ofstream file(filename);
  return file;
}

TEST(FileAccTest, openFile) {
  auto file = openFile("xyz.tmp");
  file << "hello, world" << endl;
}

/**
 * 1. tellg()
 * 2. seekg(pos)
 * 3. seekg(off, dir)
 * 4. tellp();
 * 5. seekp(pos);
 * 6. seekp(off, dir)
 *
 * dir:
 * 1. beg
 * 2. cur
 * 3. end
 *
 * 关于文件位置，pos_type无论如何不是一个整数值，考虑windows下换行符是两个字符的特殊情况，以及wchar
 * 关于off_type ，他是个整数值
 */

TEST(FileAccTest, RandomAccess_tellp) {
  // 1. tellp
  cout << "tellp test: \n";
  std::ostringstream s;
  std::cout << s.tellp() << '\n';
  s << 'h';
  std::cout << s.tellp() << '\n';
  s << "ello, world ";
  std::cout << s.tellp() << '\n';
  s << 3.14 << '\n';
  std::cout << s.tellp() << '\n' << s.str();
}

TEST(FileAccTest, RandomAccess_tellg) {
  std::string str = "Hello, world";
  std::istringstream in(str);
  std::string word;
  in >> word;
  std::cout << "After reading the word \"" << word << "\" tellg() returns " << in.tellg() << '\n';
}

TEST(FileAccTest, RandomAccess_seekg) {
  std::string str = "Hello, world";
  std::istringstream in(str);
  std::string word1, word2;

  in >> word1;
  in.seekg(0);  // 回溯
  in >> word2;

  std::cout << "word1 = " << word1 << '\n' << "word2 = " << word2 << '\n';
}

TEST(FileAccTest, RandomAccess_seekp) {
  std::ostringstream os("hello, world");
  os.seekp(7);
  os << 'W';
  os.seekp(0, std::ios_base::end);
  os << '!';
  os.seekp(0);
  os << 'H';
  std::cout << os.str() << '\n';
}

void printFileTwice(const std::string& filename) {
  std::ifstream file(filename);
  // 1. 直接操作stream缓冲区，并不会改变stream状态。如果是通过stream的接口函数（如getline）
  // 打印file内容,必须先调用clear清除file的状态，在它能够被任何方式处理之前（包括改变读取位置），
  // 因为这些函数到达文件尾端时会设置eofbit,failbit,
  // 2. 如果位置定位到文件开头之前，或者尾端之后，会导致不明确的行为。
  std::cout << file.rdbuf();
  file.seekg(0);
  std::cout << file.rdbuf();
}

TEST(FileAccTest, printFileTwice) { printFileTwice("xyz.tmp"); }