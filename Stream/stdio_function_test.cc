/**
 * 标准io函数：
 * 1. 他们用于读写无格式数据，不会跳过开头的空白字符
 * 2. 处理异常方式不同：如果发生异常，无论源自哪个函数调用，或由于哪个state flag被设置，
 *    badbit flag都会被设置， 如果异常屏蔽(exception mask)中包含badbit，就重抛该异常。
 *
 * 异常处理：
 * 若出现错误（错误状态标志不是 goodbit ）并且设置了 exceptions() 为对该状态抛出则为 failure 。
 * 若内部操作抛出异常，则捕获它并设置 badbit 。若对 badbit 设置了 exceptions() ，则重抛该异常。
 *
 * 所有版本都设置 gcount() 的值为释出的字符数。
 */

#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;

TEST(stdioTest, input) {
  /**
   * 1. int_type istream::get()
   * 读取一个字符，若可用则返回它。否则，返回 Traits::eof() 并设置 failbit 和 eofbit
   */
  // cin.get 只会启动一次用户输入，多次get从这次输入中获取
  char c = cin.get();
  if (c == char_traits<char>::eof())
    cout << "get eof" << endl;
  else
    cout << "get c: " << c << endl;

  /**
   * 2. basic_istream& get( char_type& ch );
   * 读取一个字符，而若可用则存储于 ch。否则，保留 ch 不修改并设置 failbit 和 eofbit
   * 。注意此函数不在类型 signed char 和 unsigned char 重载，不同于有格式字符输入 operator>> 。
   */
  char a = 'b';
  cin.clear();
  cin.get(a);
  cout << "get(char&) a: " << a << endl;

  /**
   * basic_istream& get( char_type* s, std::streamsize count, char_type delim );
   * 读取字符并存储它们到首元素为 s
   * 所指向的字符数组的相继位置。释出并存储字符，直至出现任何下列条件：
   * 1. 已存储 count - 1 个字符
   * 2. 输入序列中出现文件尾条件（调用 setstate(eofbit) ）
   * 3. 下个可用输入字符 c 等于 delim ，以 Traits::eq(c, delim) 确定。
   *    不释出此字符（不同于basic_istream::getline() ）。
   *
   * 若未释出字符，则调用 setstate(failbit) 。任何情况下，若 count>0，
   * 则存储空字符（ CharT() ）于数组的下个相继位置。
   */
  std::istringstream iss("hello,world");
  char str[5];
  iss.get(str, sizeof(str), '\n');
  cout << "get str: " << str << ", gcount: " << iss.gcount() << endl;

  /**
   * basic_istream& get( char_type* s, std::streamsize count );
   * 同 get(s, count, widen('\n')) ，即读取至多 count-1 个字符
   * 并存储它们到 s所指向的字符串中，直至找到 '\n'
   */
  char str2[6];
  iss.get(str2, sizeof(str2));
  cout << "get str2: " << str2 << ", gcount: " << iss.gcount() << endl;

  /**
   * basic_istream& get( basic_streambuf& strbuf, char_type delim );
   * 读取字符并插入它们到给定的 basic_streambuf 对象所控制的输出序列。
   * 释出并插入字符到 strbuf 中，直至出现任何下列条件：
   * 1. 输入序列中出现文件尾条件
   * 2. 插入输出序列失败（该情况下不释出无法插入的字符）
   * 3. 下个可用输入字符 c 等于 delim ，以 Traits::eq(c, delim) 确定。不释出此字符。
   * 4. 出现异常（该情况下捕获而不重抛异常）。
   *
   * 若未释出字符，则调用 setstate(failbit) 。
   */
  iss.get(*cout.rdbuf(), '\n');
  cout << "\nafter the last get, gcount: " << iss.gcount() << endl;

  /**
   * basic_istream& get( basic_streambuf& strbuf );
   * 同 get(strbuf, widen('\n')) ，即读取可用字符并插入它们到给定的 basic_streambuf 对象，直至找到
   * '\n'
   */

  /**
   * basic_istream& getline( char_type* s, std::streamsize count ); (1)
   * basic_istream& getline( char_type* s, std::streamsize count, char_type delim );(2)
   * 从流释出字符，直至行尾或指定的分隔符 delim 。第一版本等价于 getline(s, count, widen('\n')) 。
   * 表现为无格式输入函数 (UnformattedInputFunction) 。构造并检查 sentry 对象后，从 *this
   * 释出字符并存储它们于首元素为s所指向的数组的相继位置，直至出现任何下列条件（按出示顺序测试）：
   * 1. 输入序列中出现文件尾条件（该情况下执行 setstate(eofbit) ）
   * 2. 下个可用字符 c 是以 Traits::eq(c, delim) 确定的分隔符。
   *    释出该分隔符（不同于 basic_istream::get() ）并计入 gcount() ，但不存储它。
   * 3. 已经释出 count-1 个字符（该情况下执行 setstate(failbit) ）。
   *
   * 若函数未释出字符（即 count < 1 ），则执行 setstate(failbit) 。
   * 任何情况下，若 count>0 ，则它存储空字符 CharT() 到数组的下个相继位置，并更新 gcount() 。
   */
  std::istringstream input("abc|def|gh");
  std::vector<std::array<char, 4>> v;

  // 注意：下列循环在从  getline() 返回的流上的
  // std::ios_base::operator bool() 返回 false 时终止
  for (std::array<char, 4> a; input.getline(&a[0], 4, '|');) {
    v.push_back(a);
  }
  for (auto& a : v) {
    std::cout << &a[0] << '\n';
  }

  /**
   * basic_istream& read( char_type* s, std::streamsize count );
   * 从流释出字符。
   * 表现为无格式输入函数 (UnformattedInputFunction) 。构造并检查 sentry 对象后，
   * 释出字符并存储它们到首元素为s所指向的字符数组的相继位置。释出并存储字符，
   * 直至出现任何下列条件：
   * 1. 释出并存储了 count 个字符
   * 2. 输入序列上的文件尾条件（该情况下调用 setstate(failbit|eofbit) ）。
   * 成功释出的字符数能用 gcount() 查询。
   */
  // read() 常用于二进制 I/O
  std::string bin = {'\x12', '\x12', '\x12', '\x12'};
  std::istringstream raw(bin);
  std::uint32_t n;
  if (raw.read(reinterpret_cast<char*>(&n), sizeof n))
    std::cout << std::hex << std::showbase << n << '\n';

  // 为下个片段准备文件
  std::ofstream("test.txt", std::ios::binary) << "abcd1\nabcd2\nabcd3";

  // 读取整个文件到 string
  if (std::ifstream is{"test.txt", std::ios::binary | std::ios::ate}) {
    auto size = is.tellg();
    std::string str(size, '\0');  // 构造 string 为流大小
    is.seekg(0);
    if (is.read(&str[0], size)) std::cout << str << '\n';
  }

  /**
   * std::streamsize readsome( char_type* s, std::streamsize count );
   * 从输入流释出至多 count 个立即可用的字符。存储释出的字符于 s 所指向的字符数组。
   * 表现为无格式输入函数 (UnformattedInputFunction) 。构造并检查 sentry 对象后，
   * 1. 若 rdbuf()->in_avail() == -1 ，则调用 setstate(eofbit) 而不释出字符。
   * 2. 若 rdbuf()->in_avail() == 0 ，则不释出字符。
   * 3. 若 rdbuf()->in_avail() > 0 ，则释出 std::min(rdbuf()->in_avail(), count) 个字符，
   * 并存储它们到首元素为 s 所指向的字符数组的相继位置。
   */
  char cc[10] = {};
  std::istringstream input2("This is sample text.");  // std::stringbuf 令个缓冲可用于无阻塞读取
  input2.readsome(cc, 5);                             // 读取 'This ' 并存储于 c[0] .. c[4]
  input2.readsome(cc, 9);  // 读取 'is sample' 并存储于 c[0] .. c[8]
  std::cout << cc << endl;

  /**
   * basic_istream& ignore( std::streamsize count = 1, int_type delim = Traits::eof() );
   * 从输入流释出并舍弃字符，直至并包含 delim 。
   * ignore 表现为无格式输入函数 (UnformattedInputFunction) 。构造并检查 sentry 对象后，
   * 它从流释出并舍弃字符，直至出现任一下列条件：
   * 1. 已释出 count 个字符。
   *    在 count 等于 std::numeric_limits<std::streamsize>::max() 的特殊情况下禁用此测试。
   * 2. 输入序列中出现文件尾条件，该情况下函数调用 setstate(eofbit) 。
   * 3. 输入序列中下个可用字符 c 为 delim ，以 Traits::eq_int_type(Traits::to_int_type(c), delim)
   *    确定。 释出并舍弃分隔符。若为Traits::eof()则禁用此测试。
   */
  std::istringstream input3(
      "1\n"
      "some non-numeric input\n"
      "2\n");
  for (;;) {
    int n;
    input3 >> n;

    if (input3.eof() || input3.bad()) {
      break;
    } else if (input3.fail()) {
      input3.clear();                                                    // 反设置 failbit
      input3.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 跳过坏输入
    } else {
      std::cout << std::dec << std::noshowbase << n << '\n';
    }
  }

  /**
   * int_type peek();
   * 表现为无格式输入函数 (UnformattedInputFunction) 。构造并测试 sentry 对象后，
   * 从输入流读取下个字符而不释出它。
   * 若 good() == true ，则返回获得自 rdbuf()->sgetc() 的下个字符。
   * 否则，返回 Traits::eof()
   */
  std::istringstream s1("Hello, world.");
  char c1 = s1.peek();
  char c2 = s1.get();
  std::cout << "Peeked: " << c1 << " got: " << c2 << '\n';

  /**
   * std::basic_istream<CharT,Traits>::unget
   * 令最近释出的字符再次可用
   * 首先清除 eofbit 。(C++11 起)
   * 若设置了任何 ios_base::iostate 标志，则函数设置 failbit 并返回。
   * 否则，调用rdbuf()->sungetc()。
   * 若rdbuf()->sungetc()返回Traits::eof() ，则调用setstate(badbit)。
   * 任何情况下，设置gcount() 计数器为零。
   */
  std::istringstream ss1("Hello, world.");
  char cc1 = ss1.get();
  if (ss1.unget()) {
    char cc2 = ss1.get();
    std::cout << "Got: " << cc1 << " got again: " << cc2 << '\n';
  }

  /**
   * basic_istream& putback( char_type ch );
   * 回放字符 ch 到输入流，从使得下个释出的字符将为 ch 。
   * 首先清除 eofbit ，
   * 1. 若 rdbuf() 非空，则调用 rdbuf()->sputbackc(ch) ，
   * 2. 若 ch 不等于最近输出的字符，则它调用 rdbuf()->pbackfail(ch) 。
   * 3. 若rdbuf()为空或若rdbuf->sputbackc(ch)返回Traits::eof(),则调用setstate(badbit) 。
   *
   * 任何情况下，设置 gcount() 计数器为零。
   */
  std::stringstream s11("Hello, world");  // IO 流
  s11.get();
  if (s11.putback('Y'))  // 修改缓冲区
    std::cout << s11.rdbuf() << '\n';
  else
    std::cout << "putback failed\n";

  std::istringstream s22("Hello, world");  // 仅输入流
  s22.get();
  if (s22.putback('Y'))  // cannot modify input-only buffer
    std::cout << s22.rdbuf() << '\n';
  else
    std::cout << "putback failed\n";

  s22.clear();
  if (s22.putback('H'))  // 非修改回放
    std::cout << s22.rdbuf() << '\n';
  else
    std::cout << "putback failed\n";
}

void f() {
  std::cout << "Output from thread...";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "...thread calls flush()\n";
  std::cout.flush();
}

TEST(stdioTest, output) {
  /**
   * basic_ostream& write( const char_type* s, std::streamsize count );
   * 表现为无格式输出函数 (UnformattedOutputFunction) 。构造并检查 sentry 对象后，
   * 输出来自首元素为 s
   * 所指向的字符数组中的相继位置的字符。插入字符到输出序列，直至出现下列条件之一：
   * 1. 准确插入了 count 个字符
   * 2. 插入到输出序列失败（该情况下调用 setstate(badbit) ）
   */
  int n = 0x41424344;
  std::cout.write(reinterpret_cast<char*>(&n), sizeof n) << '\n';
  char c[] = "This is sample text.";
  std::cout.write(c, 4).write("!\n", 2);

  /**
   * basic_ostream& put( char_type ch );
   * 表现为无格式输出函数 (UnformattedOutputFunction) 。构造并检查 sentry 对象后，
   * 写字符 ch 到输出流。若输出因任何原因失败，则设置 badbit 。
   */
  std::cout.put('a');  // 正常用法
  std::cout.put('\n');
  std::ofstream s("/does/not/exist/");
  s.clear();  // 假装流是好的
  std::cout << "Unformatted output: ";
  s.put('c');  // 这将设置 badbit ，但非 failbit
  std::cout << " fail=" << bool(s.rdstate() & s.failbit);
  std::cout << " bad=" << s.bad() << '\n';
  s.clear();
  std::cout << "Formatted output:   ";
  s << 'c';  // 这将设置 badbit 和 failbit
  std::cout << " fail=" << bool(s.rdstate() & s.failbit);
  std::cout << " bad=" << s.bad() << '\n';

  /**
   * basic_ostream& flush();
   * 写入未提交更改到底层输出序列。
   * 若 rdbuf() 为空指针，则不做任何事。
   * 否则，表现为无格式输出函数 (UnformattedOutputFunction) (C++11 起)。
   * 构造并检查 sentry 对象后，调用 rdbuf()->pubsync() 。
   * 若该调用返回 -1 ，则调用 setstate(badbit)
   */
  std::thread t1(f);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::clog << "Output from main\n";
  t1.join();
}