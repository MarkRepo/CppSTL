## 线程同步化与concurrency问题

### 当心并发

1. 多个线程并发处理相同数据而又不曾同步化，唯一安全的情况是： 所有线程只读取数据。
相同数据是指相同的内存区，c++11后保证每个变量都拥有自己的内存区，唯一例外是bitfield

### concurrent data access 为什么造成问题

1. 语言只定义what，不定义how
2. 明确语言给了什么保证

### 什么情况下可能出错

1. Unsynchronized data access 未同步化的数据访问： 并行运行的两个线程同时读和写一笔数据，不知道哪一个先来
2. Half written data（写至半途的数据） ： 一个线程读取另一个线程正在改动的数据
3. Reordered statement（重新安排的语句）：语句和操作有可能被重新安排了顺序，也许对每一个单线程正确，但是对于多线程的组合却破坏了预期的行为。
C++只要求编译所得的代码在单一线程内的可观测行为正确。

## 解决问题需要的性质

1. Atomicty(不可切割性)
2. Order（次序）

c++ 提供的方法：
1. future和promise， 一定在形成成果后设定shared state
2. mutex 和lock， 只提供atomicity
3. condition variable
4. atomic date type
5. atomic data type的低层接口 18.7.4 节


