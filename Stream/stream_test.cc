/**
 *  操控器：
 * 1. endl，输出'\n', 并刷新缓冲区
 * 2. ends，输出'\0'
 * 3. flush, 刷新output缓冲区
 * 4. ws，读入并忽略空白字符
 *
 */

#include <gtest/gtest.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

TEST(StreamTest, stream1) {
  double x, y;
  cout << "enter two double x and y, ouput their multiply result: " << endl;
  cout << "first operand: ";
  if (!(cin >> x)) {
    cerr << "reading double value error " << endl;
    return;
  }
  cout << "second operand: ";
  if (!(cin >> y)) {
    cerr << "reading double value error " << endl;
    return;
  }
  cout << x << " multiply " << y << " result in " << x * y << endl;
}

TEST(StreamTest, SpecialIO) {
  int x = 1;
  cout << "please enter a number: ";
  cin >> x;
  // 1. Numeric Type
  // 如果输入不是以数字起头，x会被设置为0，且failbit被设置
  // 如果没有输入，或如果failbit已被设置，x的值保持不变
  cout << "enter not number, x is : " << x << endl;  // 0
  if (cin.rdstate() & ios::failbit) {
    cout << "failbit was set" << endl;
  }

  // 2. Bool 默认以数字进行IO，true为1， false为0， 如果不是0或1，会设置failbit
  // 可用格式化选项boolalpha控制为true、false值
  bool b;
  cout << "please enter a boolean: ";
  cin >> b;
  cout << "enter bool value b: " << b << endl;
  if (cin.rdstate() & ios::failbit) {
    cout << "failbit was set" << endl;
  }
  cout << "please enter true or false: ";
  cin >> boolalpha >> b;
  cout << "enter bool value b: " << boolalpha << b << endl;
  if (cin.rdstate() & ios::failbit) {
    cout << "failbit was set" << endl;
  }

  // 3. void*
  char* cstring = "hello";
  cout << "cstring: " << cstring << " locate in address: " << static_cast<void*>(cstring) << endl;

  // 4. date
  auto now = std::chrono::system_clock::now();
  time_t t = chrono::system_clock::to_time_t(now);
  tm* nowTM = localtime(&t);
  cout << put_time(nowTM, "date: %x\ntime: %X\n") << endl;
  tm date;
  cout << "new date: ";
  cin >> get_time(&date, "%x");
  if (!cin) {
    cerr << "invalid format read" << endl;
  }
  cout << put_time(&date, "date: %x\n") << endl;
}