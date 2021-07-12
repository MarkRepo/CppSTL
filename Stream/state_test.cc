/**
 * stream 维护着一个state，标识io是否成功，并指出不成功的原因
 * state: std::ios_base::state 或者 std::ios::state，如std::ios::failbit
 * 1. goodbit 一切都好，没有任何其他bit被设置(所有bit被清零)
 * 2. eofbit 遇到end-of-file
 * 3. failbit 错误，某个io动作未成功
 * 4. badbit 毁灭性错误，造成不确定的状态
 *
 * eofbit 和 failbit经常一起出现，读取最后一个字符时，eofbit并未被设置，但再一次读取字符时，
 * 会导致eofbit 和 failbit被同时设置
 *
 * 处理state的成员函数
 * 1. good() 若stream正常无误，返回true（goodbit被设置）
 * 2. eof() 若遭遇end-of-file ， 返回true（eofbit被设置）
 * 3. fail() 若发生错误，返回true （failbit或badbit被设置）
 * 4. bad（） 若发生毁灭性错误，返回true （badbit被设置，如果没有stream buffer，
 *    即 rdbuf() == 0，badbit始终会被设置）
 * 5. rdstate() 返回当前已设置的所有flag
 * 6. clear() 清除所有flag
 * 7. clear(state) 清除所有flag后设置 state flag
 * 8. setstate(state) 加设state flag
 *
 * 1. stream 有可能被配置为”当某些flag被clear()或setstate()设置时便抛出异常“
 * 2. c语言可以在”格式错误“发生后仍然读取字符。
 * c++不同：如果设置了failbit，除非明白予以清除，否则无法进行下一个操作
 *
 * stream 转 bool 操作
 * 1. operator bool() 是否未出错
 * 2. operator ! ()   是否已出错
 *
 * stream 兼容异常，默认不抛出异常:
 * 1. exceptions(flags) 设定”会触发异常“ 的flag
 * 2. exception()  返回”触发异常”的flag， 如果返回goodbit，表示没有任何异常被抛出
 * 异常的抛出时机是在“程序调用clear()或setstate()”设置某些flag之际，
 * 如果某个标志已被设置但未被清除，也会抛出异常,如：
 *
 * strm.exceptions(ios::failbit)  // 如果failbit已被设置，这个调用将抛出异常
 * strm.setstate(ios::failbit) // 抛出异常， 即使failbit已被设置
 */

#include <gtest/gtest.h>

#include <iostream>

using namespace std;

TEST(StateTest, State) {
  if (cin.rdstate() & std::ios::failbit) {
    cout << "failbit was set" << endl;
    // clear only failbit
    cin.clear(cin.rdstate() & ~ios::failbit);
  }
}

TEST(StateTest, exception) {
  // throw exception for all errors
  cin.exceptions(ios::failbit | ios::eofbit | ios::badbit);
  // do not generate exception
  cin.exceptions(ios::goodbit);
}

namespace MyLib {

double readAndProcessSum(std::istream& strm) {
  double value, sum;
  // save current state of exception flags
  ios::iostate oldExceptions = strm.exceptions();
  // let failbit and bad bit throw exception
  // NOTE: failbit is also set at end-of-file
  strm.exceptions(ios::failbit | ios::badbit);
  try {
    sum = 0;
    cout << "enter double value1: ";
    while (strm >> value) {
      sum += value;
    }
  } catch (...) {
    // if exception not caused by end-of-file
    // - restore old exceptions flags
    // - rethrow exception
    // 如果不是eof引发的异常，则往外传播异常
    if (!strm.eof()) {
      cout << "before restore 1" << endl;
      strm.exceptions(oldExceptions);
      cout << "after restore 1" << endl;
      throw;
    }
  }
  // 恢复原来的异常 flags时，如果stream已设置相应的flag，那么此时也会抛出异常。
  // 因此如果程序在进入readAndProcessSum函数之前就设置为会因为eofbit、failbit或badbit而抛出异常，
  // 那么这些异常也会被传播给调用者, 也适用于restore 1 处。
  cout << "before restore 2" << endl;
  strm.exceptions(oldExceptions);
  cout << "after restore 2 " << endl;
  return sum;
}

// 第二种方式，只在检测到错误时才抛出异常
double readAndProcessSum2(std::istream& strm) {
  double value, sum;
  sum = 0;
  cout << "enter double value2: ";
  while (strm >> value) {
    sum += value;
  }

  if (!strm.eof()) {
    throw std::ios::failure("input error in readAndProcessSum()");
  }
  return sum;
}

}  // namespace MyLib

TEST(StateTest, exception2) {
  double sum;
  try {
    // cin.exceptions(ios::eofbit | ios::failbit);
    // sum = MyLib::readAndProcessSum(cin);
    sum = MyLib::readAndProcessSum2(cin);
  } catch (const ios::failure& error) {
    cerr << "I/O Exception: " << error.what() << endl;
    return;
  } catch (const exception& error) {
    cerr << "Standard Excepiton: " << error.what() << endl;
    return;
  } catch (...) {
    cerr << "unknown exception" << endl;
    return;
  }
  cout << "sum: " << sum << endl;
}
