/**
 * atomic高层接口所提供的操作使用默认保证，不论内存次序如何。这份默认保证提供了顺序一致性，
 * 意思是在线程之中atomic操作保证一定“像代码出现的次序“那样的发生。因此重排语句不会出现。
 *
 * 六种内存序：参考（https://zhuanlan.zhihu.com/p/55901945）
 * 1. 无承诺的保证
 * std::memory_order_relax就是一种无承诺的保证。使用这种模式与没有使用这种模式达到的效果是一样的。
 *
 * 2. 写顺序保证
 * std::memory_order_release
 * 在本行代码之前，如果有任何写内存的操作，都是不能放到本行语句之后的。简单地说，就是“写不后”
 *
 * 3. 读顺序的保证
 * std::memory_order_acquire 表示的是，后续的读操作都不能放到这条指令之前。简单地可以写成“读不前”。
 *
 * 至此可以明白，一个线程想要呈现给别人看的顺序，不仅需要自己做好相应的memory_order，
 * 连看的一边的人，也需要做好相应的顺序约定。
 *
 * 4. 读顺序的消弱
 * std::memory_order_consume。这个内存序的含义是说，所有后续对本原子类型的操作，
 * 必须在本操作完成之后才可以执行。简单点就是 “不得前”。但是这个操作只能用来对读者进行优化
 *
 * 注意：std::memory_order_acquire与std::memory_order_consume的区别在于：
 * std::memory_order_acquire是要求后面所有的读都不得提前。
 * std::memory_order_consume是要求后面依赖于本次消费操作的读不能乱序。
 * 一个是针对所有的读，容易形成误伤。而consume只是要求依赖于consume这条语句的读写不得乱序。
 *
 * 5. 读写的加强
 * 使用std::memory_order_acq_rel， 即表示“写不后”，“读不前”同时生效
 *
 * 6. 最强的约束
 * std::memory_order_seq_cst表示最强约束；
 * 即所有这条指令前面的语句不能放到后面，所有这条语句后面的语句不能放到前面来执行。
 *
 */

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

// 总是应该将atomic object 初始化，default构造函数并不完全初始化（lock未被初始化）
// 如果只使用default构造函数，接下来唯一允许的操作是atomic_init:
// std::atomic<bool> readyFlag;
// std::atomic_init(&readyFlag, false);
std::atomic<bool> readyFlag(false);

void thread1() {
  // dosomething thread2 nees preparation
  std::cout << "<return>" << std::endl;
  std::cin.get();
  // atomic类型出了load store以外，可以继续使用有用而寻常的操作
  readyFlag.store(true);
}

void thread2() {
  while (!readyFlag.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "waiting!" << std::endl;
  }
  // do whatever shall happen after thread1 has prepared things
  std::cout << "thread1 prepared" << std::endl;
}

TEST(AtomicTest, atomic1) {
  auto f1 = std::async(std::launch::async, thread1);
  auto f2 = std::async(std::launch::async, thread2);
}

long data;
std::atomic<bool> readyFlag2(false);
void provider() {
  std::cout << "<return>" << std::endl;
  std::cin.get();

  data = 42;
  // store() 会对影响所及的内存区执行一个所谓的release操作，确保此前的所有内存操作，不论是否为atomic
  // 在store发挥效用之前都变成“可被其他线程看见”
  readyFlag2.store(true);
}

void consumer() {
  // load() 会对影响所及的内存区执行一个所谓的acquire操作，确保随后所有内存操作，不论是否为atomic
  // 在load之后都变成“可被其他线程看见”
  while (!readyFlag2.load()) {
    std::cout.put('.').flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  // 对data的设置发生在store之前，对data的处理发生在load之后
  // 因此对data的处理保证发生在data提供之后
  // 这中保证是由于所有atomic操作默认使用一个特别的内存次序：memory_order_seq_cst,
  // 他代表sequtial consistent memory order(顺序一致的内存次序)，
  // 低层的atomic操作能放宽这一次序保证。
  std::cout << "get value: " << data << std::endl;
}

TEST(AtomicTest, atomic2) {
  auto p = std::async(std::launch::async, provider);
  auto c = std::async(std::launch::async, consumer);
}

// atomic 高层操作
// 1. 一般而言，这些操作获得的是copy而不是reference
// 2. 接受相关类型值的那个构造函数并不是atomic
// 3. 除了构造函数，其他函数都被重载为volatile 和 non-volatile 版本
// 4. atomic 可用于一般类型，对bool, 整型和pointer有特化版本
TEST(AtomicTest, op) {
  // 1. 构造函数，非atomic
  // 1~8 适用于所有支持的类型
  int a = 10, d = 20;
  atomic<bool> b(false);
  atomic<int> i(10);
  atomic<int*> pi(nullptr);

  atomic<bool> b2;
  atomic<int> i2;
  atomic<int*> pi2;
  atomic_init(&b2, false);
  atomic_init(&i2, 10);
  atomic_init(&pi2, &a);

  // 2. is_lock_free 如果内部不使用lock，返回true
  cout << "atomic<bool> is lock free: " << boolalpha << b.is_lock_free() << endl;
  cout << "atomic<int> is lock free: " << boolalpha << i.is_lock_free() << endl;
  cout << "atomic<int*> is lock free: " << boolalpha << pi.is_lock_free() << endl;

  // 3. a.store()
  b2.store(true);
  i2.store(100);
  pi2.store(&d);

  // 4. a.load()返回数值a的拷贝
  cout << "now b2 is: " << b2.load() << endl;
  cout << "now i2 is: " << i2.load() << endl;
  cout << "now pi2 is: " << pi2.load() << endl;

  // 5. a.exchange(val) 赋值val并返回旧值a的拷贝
  cout << "old b2 is: " << b2.exchange(false) << endl;
  cout << "old i2 is: " << i2.exchange(200) << endl;
  cout << "old pi2 is: " << pi2.exchange(&a) << endl;

  // 6. compare_exchange_strong(expect, desire)
  /**
   * 伪代码
   * bool compare_exchange_strong(T& expected, T desired) {
   *  if(this.load() == expected){
   *    this.store(desired);
   *    return true;
   *  } else {
   *    expected = this.load();
   *    return false;
   *  }
   * }
   *
   * compare_exchange_weak 有可能假失败，即期望值出现，但仍返回false。
   * weak有时比strong更高效
   */
  bool exp_b = false;
  cout << "new value b2 is: " << b2.compare_exchange_strong(exp_b, true) << ", exp_b: " << boolalpha
       << exp_b << endl;
  cout << "new value b2 is: " << b2.compare_exchange_weak(exp_b, true) << endl;

  // 7. a = val； 赋值并返回val的拷贝
  i2 = 100;

  // 8. a.operator atomic() 返回a的拷贝
  // 9. a.fetch_add(val); a.fetch_sub(100) 不可切割之 a += val, a -= val; 返回新值拷贝
  // 9,10,11 适用于整型和指针型
  i2.fetch_add(100);
  i2.fetch_sub(100);

  // 10. a+=val, a-=val; 等同于a.fetch_add, a.fetch_sub
  i2 += 100;
  i2 -= 100;

  // 11. ++a,a++, --a,a-- 等同于a.fetch_add(1), a.fetch_sub(1);返回a的拷贝
  i2++;
  ++i2;
  i2--;
  --i2;

  // a &= val, a |= val, a ^= val 等同于 a.fetch_and(val), a.fetch_or(val), a.fetch_xor(val)
  // 不可切割操作，返回新值得拷贝；只适用于整型
  i2 &= 100;
  i2 |= 100;
  i2 ^= 100;
  i2.fetch_and(100);
  i2.fetch_or(100);
  i2.fetch_xor(100);
}
