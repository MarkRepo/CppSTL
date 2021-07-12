/**
 * file flag
 * 1. in 打开，用于读取（ifstream默认模式）
 * 2. out 打开，用于涂写（ofstream默认模式）
 * 3. app 涂写时始终添加于尾端
 * 4. ate 开启文件后令读/写位置移动至文件尾端
 * 5. trunc 将先前的文件内容移除
 * 6. binary 不要替换特殊字符
 *
 * flags 组合，于c的fopen对比(binary 相当于在c模式后加一个b，
 * ate表示打开文件后立即跳到文件尾端)，其他组合没有意义：
 * ios_base flag    含义                            c模式
 * 1. in           读取（来源文件必须存在）             "r"
 * 2. out          清空而后涂写（有必要才创建）          "w"
 * 3. out|trunc    清空而后涂写（有必要才创建）          "w"
 * 4. out|app      追加（有必要才创建）                 "a"
 * 5. app          追加（有必要才创建）                 "a"
 * 6. in|out       读写，最初位置在起点，文件必须存在     "r+"
 * 7. in|out|trunc 先清空，再读写（有必要才创建）        "w+"
 * 8. in|app       在尾端更新（有必要才创建）            "a+"
 * 9. in|out|app   在尾端更新（有必要才创建）            "a+"
 */

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
/**
 *  几个成员函数，通常只在file stream未初始化时有用
 * 1. open(name)
 * 2. open(name,flags)
 * 3. close()
 * 4. is_open()
 */

TEST(FileFlagsTest, memberFunc) {
  ifstream file;
  vector<string> filenames{"xyz.tmp", "fstream.tmp"};
  for (auto& filename : filenames) {
    file.open(filename);
    if (file.is_open()) {
      char c;
      while (file.get(c)) {
        cout.put(c);
      }
    }
    // 处理文件过后，必须调用clear清除被设置于文件尾端的state flag，这是必要的，
    // 因为file 这个stream对象由多个文件共享，open()并不会清除任何的state flag。
    // 因此，如果某个stream未处于良好状态，关闭重新打开之后还是必须调用clear以取得一个良好状态
    file.clear();
    file.close();
  }
}