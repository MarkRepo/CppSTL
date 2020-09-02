// 用于可以控制task何时运行的情况，不像async一样尝试立刻启动

#include <gtest/gtest.h>

#include <future>
#include <thread>

using namespace std;

int multiply(int x, int y) { return x * y; }

TEST(PackagedTaskTest, packaged1) {
  std::packaged_task<int(int, int)> task(multiply);
  std::future<int> f(task.get_future());
  // 通常但非一定启动于分离线程中
  task(5, 6);
  cout << "result: " << f.get() << endl;
}