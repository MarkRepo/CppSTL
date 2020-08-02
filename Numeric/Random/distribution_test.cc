#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>

using namespace std;

template <typename Distr, typename Eng>
void distr(Distr d, Eng e, const std::string& name) {
  // print min max and four example values
  cout << name << ":" << endl;
  cout << "- min: " << d.min() << endl;
  cout << "- max: " << d.max() << endl;
  cout << "- values: " << d(e) << " " << d(e) << " " << d(e) << " " << d(e) << endl;

  // count the generated values(convert to intergal values)
  map<long long, int> counter;
  for (int i = 0; i < 2000000; ++i) {
    counter[d(e)]++;
  }

  // print the result distribution
  cout << "==========" << endl;
  for (auto& v : counter) {
    cout << setw(3) << v.first << ": " << v.second << endl;
  }
  cout << "==========" << endl;
  cout << endl << endl;
}

TEST(DistributionTest, exammple) {
  std::knuth_b e;
  std::uniform_real_distribution<> ud(0, 10);
  distr(ud, e, "uniform_real_distribution");

  std::normal_distribution<> nd;
  distr(nd, e, "normal_distribution");

  std::exponential_distribution<> ed;
  distr(ed, e, "exponential_distribution");

  std::gamma_distribution<> gd;
  distr(gd, e, "gamma_distribution");
}

TEST(DistributionTest, MemberTest) {
  //param(); d(args)
  std::uniform_int_distribution<> ud(10, 20);
  cout << "a: " << ud.a() << " or: " << ud.param().a() << endl;
  cout << "b: " << ud.b() << " or: " << ud.param().b() << endl;

  std::knuth_b e;
  //d(e, pt); d::param_type
  std::uniform_int_distribution<>::param_type pt(100, 200);
  cout << "use pt to generate value:  " << ud(e, pt) << endl;

  std::uniform_int_distribution<>::param_type pt2(200, 300);
  //d(pt)
  ud.param(pt2);
  cout << "get 3 values: " << ud(e) << " " << ud(e) << " " << ud(e) << endl;
  // stream >> d; stream << d;
  std::stringstream dState;
  dState << ud;
  cout << "after Save: " << ud(e) << endl;
  dState >> ud;
  cout << "after Restore: " << ud(e) << endl;

  std::uniform_int_distribution<> ud2(10, 20);
  std::uniform_int_distribution<> ud3(10, 20);
  // operator ==
  cout << (ud3 == ud2 ? "ud2 == ud3" : "ud3 != ud2") << endl;

  stringstream eState;
  ud2(e);
  eState << e;
  //d::result_type
  std::uniform_int_distribution<>::result_type res2 = ud2(e);
  eState >> e;
  std::uniform_int_distribution<>::result_type res3 = ud2(e);
  cout << "res2: " << res2 << ", res3: " << res3 << endl;
}