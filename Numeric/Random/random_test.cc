#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

TEST(RandomTest, example1) {
  std::default_random_engine dre;
  // [beg, end]
  std::uniform_int_distribution<int> di(10, 20);
  for (int i = 0; i < 20; i++) {
    cout << di(dre) << " ";
  }
  cout << std::endl;
  // [beg, end)
  std::uniform_real_distribution<double> dr(10, 20);
  for (int i = 0; i < 8; i++) {
    cout << dr(dre) << " ";
  }
  cout << std::endl;

  // shuffle
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::shuffle(v.begin(), v.end(), dre);
  for (auto e : v) {
    cout << e << " ";
  }
  cout << endl;

  // dre init state is same
  std::default_random_engine dre1;
  std::default_random_engine dre2;
  cout << di(dre1) << std::endl;
  cout << di(dre2) << std::endl;
}

TEST(RandomTest, EngineTest) {
  auto printNumbers = [](std::default_random_engine& dre) {
    for (int i = 0; i < 6; i++) {
      cout << dre() << " ";
    }
    cout << endl;
  };

  std::default_random_engine dre;
  printNumbers(dre);

  std::default_random_engine dre2;
  printNumbers(dre2);

  std::default_random_engine dre3(42);
  printNumbers(dre3);

  std::stringstream engineState;
  // save engine state to stringstream
  engineState << dre;
  printNumbers(dre);
  // restore enginesate
  engineState >> dre;
  printNumbers(dre);
  // restore engine state and set read position to begin
  engineState.clear();
  engineState.seekg(0);
  engineState >> dre;
  // skip 2 numbers
  dre.discard(2);
  printNumbers(dre);
}

/**
 * std 提供了16个engine：
 * 1. default_random_engine
 *
 * Basic Engine:
 * 2. Class std::linear_congruential_engine
 * 3. Class std::mersenne_twister_engine
 * 4. Class std::subtract_with_carry_engine
 *
 * Engine adapters, which can be initialized by a (basic) engine:
 * 5. Class std::discard_block_engine, which adapts an engine by discarding a given number of
 * generated values each time
 * 6. Class std::independent_bits_engine, which adapts an engine to produce random values with a
 * specified number of bits
 * 7. Class std::shuffle_order_engine, which adapts an engine by permutation of the
 * order of their generated values
 *
 * Adapters with predefined parameters:
 * 8. std::minstd_rand0
 * 9. std::minstd_rand
 * 10. std::mt19937
 * 11. std::mt19937_64
 * 12. std::ranlux24_base
 * 13. std::ranlux48_base
 * 14. std::ranlux24
 * 15. std::ranlux48
 * 16. std::knuth_b
 */
TEST(RandomTest, EngineMemberTest) {
  auto printNumbers = [](std::mt19937_64& mt) {
    for (int i = 0; i < 6; i++) {
      cout << mt() << " ";
    }
    cout << endl;
  };

  // default ctor
  std::mt19937_64 mt64e;
  printNumbers(mt64e);

  // init state are same
  std::mt19937_64 mt64e2;
  printNumbers(mt64e2);

  // test 2 engine wether in same state
  if (mt64e == mt64e2) {
    cout << "two mt64 engine in same state" << endl;
  }

  // copy ctor
  std::mt19937_64 mt64e3(mt64e2);
  printNumbers(mt64e3);

  // use seed to set enggine state
  std::mt19937_64 mt64e4(42);
  printNumbers(mt64e4);

  // set engine to init state
  mt64e4.seed();
  printNumbers(mt64e4);
  // set engine state
  mt64e4.seed(42);
  printNumbers(mt64e4);

  // skip 2 numbers
  mt64e4.discard(2);
  printNumbers(mt64e4);

  // use stringstream to save and restore state
  std::stringstream eState;
  eState << mt64e4;
  printNumbers(mt64e4);
  eState >> mt64e4;
  printNumbers(mt64e4);
}