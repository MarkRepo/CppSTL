#include <gtest/gtest.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

const double PI = 3.1415926535;
TEST(FormatTest, setf) {
  /**
   * fmtflags setf( fmtflags flags );                  (1)
   * fmtflags setf( fmtflags flags, fmtflags mask );   (2)
   * void     unsetf(fmtflags flags);                  (3)
   *
   * 设置格式化标志以指定设置。
   * 1) 设置 flags 所标识的格式化标志。等效地进行下列操作：
   * fl = fl | flags ，其中 fl 定义内部格式化标志的状态。
   * 2) 清除 mask 下的格式化标志，并设置被清除的标志为 flags 所指定者。
   * 等效地进行下列操作：
   * fl = (fl & ~mask) | (flags & mask) ，其中 fl 定义格式化标志的内部状态。
   * 3) 清除flags
   *
   * 返回值
   * 调用函数前的格式化标志
   */
  const int WIDTH = 15;

  std::cout.setf(std::ios::right);  // 等价： cout << right;
  std::cout.setf(ios::hex, ios::basefield);
  std::cout << std::setw(WIDTH / 2) << "radius" << std::setw(WIDTH) << "circumference" << '\n';
  std::cout.setf(std::ios::fixed);
  for (double radius = 1; radius <= 6; radius += 0.5) {
    std::cout << std::setprecision(1) << std::setw(WIDTH / 2) << radius << std::setprecision(2)
              << std::setw(WIDTH) << (2 * PI * radius) << '\n';
  }
  std::cout.unsetf(ios::hex | ios::right);
}

TEST(FormatTest, flags) {
  /**
   * fmtflags flags() const;             (1)
   * fmtflags flags( fmtflags flags );   (2)
   * 管理格式化标志。
   * 1) 返回当前格式化设置。
   * 2) 以给定者替换当前设置。
   *
   * 返回值
   * 调用函数前的格式化标志
   */
  ios::fmtflags oldFlags = cout.flags();
  cout.setf(ios::showpos | ios::showbase | ios::uppercase);
  cout.setf(ios::internal, ios::adjustfield);
  int x = 42;
  cout << std::hex << x << endl;
  cout.flags(oldFlags);

  // copyfmt 将stream的所有格式信息赋值给另一个stream
  std::ofstream out;

  out.copyfmt(std::cout);                         // 复制 rdstate 和 rdbuf 外的所有内容
  out.clear(std::cout.rdstate());                 // 复制 rdstate
  out.basic_ios<char>::rdbuf(std::cout.rdbuf());  // 共享缓冲

  out << "Hello, world\n";
}

TEST(FormatTest, setiosflags) {
  /**
   * 1. setiosflags(flags) 等价于调用相应stream的setf(flags)
   * 2. resetiosflags(mask)等价于调用相应stream的setf(0, mask)
   */

  /**
   * 1. width() 返回当前的最小栏位宽度，默认为0，只应用于下一次格式化输出
   * 2. width(val) 设置val为最小栏位宽度，返回先前的
   * 3. fill() 返回当前填充字符，默认为空格space
   * 4. fill(c) 定义当前的填充字符为c，返回先前的填充字符
   *
   * 读入类型为char*的字符序列时，可以运用栏位宽度来定义读取的最大字符数
   */
  char buf[11];
  // at most 10 charactor
  cin >> setw(sizeof(buf)) >> buf;
  cout << "read to buf: " << buf << endl;
  std::string s;
  cin >> s;
  cout << "read to string s: " << s << endl;
}

TEST(FormatTest, locale) {
  /**
   * 1. imbue(loc) 设置locale对象, 每个stream可以关联一个locale对象，默认情况下，
   *  stream构造时会赋值全聚德locale对象。
   * 2. getloc（）返回当前的locale对象
   * 3. widen(c) 把char类型的字符c转换为stream字符集内的字符
   * 4. narrow(c,def) 把stream字符集内的字符c转化为一个char字符，如无对应的char字符，则返回def
   */
}