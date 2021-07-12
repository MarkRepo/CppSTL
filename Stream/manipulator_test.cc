#include <gtest/gtest.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

TEST(ManipulatorTest, opTest) {
  // endl, 写出一个换行符，并清空输出
  // ends，写出一个表示结束的null字符
  // flush，清空输出
  // ws， 读入并且忽略空格
  std::cout << "endl" << std::endl;
  std::cout << std::ends;
  std::cout.flush();
  char a;
  std::istringstream sstrm("  h  e  llo, world");
  sstrm >> ws >> a;
  cout << "get a form sstrm: " << a << endl;
  // skipws 执行operator >> 时，跳过前导空格
  // noskipws 执行operator>> 时，不跳过前导空格
  sstrm >> skipws >> a;
  cout << "get a from sstrm: " << a << endl;
  sstrm >> noskipws >> a;
  cout << "get a from sstrm:" << a << a << "endl" << endl;
  // unitbuf, 每次写操作后清空输出缓冲区
  // nounitbuf， 每次写操作后不清空输出缓冲区
  cout << unitbuf << "unitubf\n";
  cout << nounitbuf << "nounitbuf\n";
  // setiosflags(flags) 将flags设置为格式标志
  // resetiosflags(m) 清除屏蔽m所示分组中的所有flag
  // setw(val) 设定下一次输入和输出val时的栏宽
  // setfill(c) 设定填充字符为c
  // left 数值靠左
  // right 数值靠右
  std::cout << std::resetiosflags(std::ios_base::dec)
            << std::setiosflags(std::ios_base::hex | std::ios_base::uppercase |
                                std::ios_base::showbase)
            << 42 << '\n';
  std::cout << std::resetiosflags(std::ios::hex) << std::setw(6) << std::setfill('0') << 42 << endl;
  std::istringstream is("hello, world");
  char arr[10];
  is >> std::setw(6) >> arr;
  std::cout << "Input from \"" << is.str() << "\" with setw(6) gave \"" << arr << "\"\n";
  std::cout << left << setw(6) << setfill('?') << 42 << endl;
  std::cout << right << setw(6) << 42 << endl;
  // internal 正负号靠左，数值靠右
  std::cout << internal << setw(6) << -42 << endl;
  // boolalpha 文本显示bool值
  // noboolalpha 数值显示bool值
  std::cout << "boolalpha: " << boolalpha << true << endl;
  std::cout << "noboolalpha: " << noboolalpha << true << endl;
  // showpos 对正值强制显示正号
  // noshowpos 对正值强制不显示正好
  std::cout << showpos << 42 << endl;
  std::cout << noshowpos << +42 << endl;
  // uppercase 对于数值强制使用大写字母
  // nouppercas对于数值强制使用小写字母e
  // oct, dec, hex, 以八，十，十六进制读写整数值
  // showbase 显示数值的基数
  // noshowbase 不显示数值的基数
  std::cout << uppercase << hex << noshowbase << 42 << endl;
  std::cout << nouppercase << hex << showbase << 42 << endl;
  // showpoint 总是为浮点数写出小数点
  // noshowpoint 不要为浮点数写出小数点
  // setpricision(val) 使用浮点数精度将val改设新值
  std::cout << "showpoint: " << showpoint << 42.0 << endl;
  std::cout << noshowpoint << 42.0 << endl;
  std::cout << "setprecision: " << showpoint << setprecision(8) << 42.0 << endl;
  // fixed 对浮点数使用十进制数表示法
  // scientific 对浮点数使用科学记号表示法
  // hexfloat 对浮点数使用十六进制科学表示法
  // defaultfloat 使用常规浮点数表示法
  std::cout << "fixed: " << fixed << 42.22 << endl;
  std::cout << "scientific: " << scientific << 42.22 << endl;
  std::cout << "hexfloat: " << hexfloat << 42.22 << endl;
  std::cout << "defaultfloat: " << defaultfloat << 42.22 << endl;

  // put_time(val, fmt) 根据格式fmt写出date/time值
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);
  std::cout << std::put_time(&tm, "%c %Z") << '\n';

  // get_time(val, fmt) 根据格式fmt读入time/date 值
  std::tm t1 = {};
  std::istringstream ss("2011-Februar-18 23:12:34");
  ss.imbue(std::locale("de_DE.utf-8"));
  ss >> std::get_time(&t1, "%Y-%b-%d %H:%M:%S");
  if (ss.fail()) {
    std::cout << "Parse failed\n";
  } else {
    std::cout << std::put_time(&t1, "%c") << '\n';
  }

  // put_money(val) 使用本地货币符号写出货币值
  // get_money(val) 使用本地货币符号读入货币值
  // put_money(val, intl) 根据intl的货币符号写出货币值
  // get_money(val, intl) 根据intl的货币符号读入货币值
}

//  操控器的实现模拟，
namespace mystd {
ostream& operator<<(ostream& strm, ostream (*op)(ostream&)) {
  (*op)(strm);
  return strm;
}

ostream& endl(std::ostream& strm) {
  strm.put('\n');
  strm.flush();
  return strm;
}
}  // namespace mystd

// 自定义操控器
template <typename CharT, typename traits>
inline std::basic_istream<CharT, traits>& ignoreLine(std::basic_istream<CharT, traits>& strm) {
  strm.ignore(std::numeric_limits<std::streamsize>::max(), strm.widen('\n'));
  return strm;
}

TEST(ManipulatorTest, ignoreLine) {
  std::istringstream is("hello,world\nyellow,world\n");
  std::string ss;
  is >> ignoreLine >> ss;
  std::cout << "after ignoreLine ss: " << ss << endl;
}

// 带参数的操控器
class ignoreMultiLine {
 private:
  int num;

 public:
  explicit ignoreMultiLine(int n = 1) : num{n} {}

  template <typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& strm,
                                                       const ignoreMultiLine& ign) {
    for (int i = 0; i < ign.num; i++) {
      strm.ignore(std::numeric_limits<std::streamsize>::max(), strm.widen('\n'));
    }
    return strm;
  }
};

TEST(ManipulatorTest, ignoreMultiLine) {
  std::istringstream is("aaaaaa\nbbbbbbb\ncccccccc\nddddddd\n");
  std::string ss;
  is >> ignoreMultiLine(2) >> ss;
  std::cout << "after ignore 2 line, ss: " << ss << endl;
}