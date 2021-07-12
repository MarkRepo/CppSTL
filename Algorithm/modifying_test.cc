/**
 * 更易型算法不能用于associative 或者unordered 容器，
 * 更易型算法概览
 */

#include <gtest/gtest.h>

#include "Algorithm/algo_helper.h"
using namespace std;

TEST(AlgoTest, copytest) {
  // 复制元素, 返回目标区间最后一个被复制元素的下一位置，即第一个未被覆盖的元素位置
  /**
   * OutputIterator copy(srcBeg, srcEnd, dstBeg) // dstBeg不能在[srcBeg, srcEnd) 中
   * OutputIterator copy_if(srcBeg, srcEnd, dstBeg, op)// 区间不可重叠
   * OuptutIterator copy_n(srcBeg, num, dstBeg)
   * BidirectionalIterator2 copy_backward(srcBeg, srcEnd, dstEnd)// dstEnd不可在[srcBeg, srcEnd)
   * copy正向遍历，copy_backward反向遍历，当区间有重叠时，两者有点区别，应注意使用不同的版本：
   * 1. 若把一个区间复制到前端，应使用copy，此时dstBeg在srcBeg前面
   * 2. 若把一个区间复制到后端，应使用copy_backward, 此时dstEnd 在srcEnd之后
   */
  vector<string> coll1{"hello", "this", "is", "a", "example"};
  list<string> coll2;
  copy(coll1.begin(), coll1.end(), back_inserter(coll2));
  copy(coll2.begin(), coll2.end(), ostream_iterator<string>(cout, " "));
  cout << endl;
  copy(coll1.crbegin(), coll1.crend(), coll2.begin());
  copy(coll2.begin(), coll2.end(), ostream_iterator<string>(cout, " "));
  cout << endl;

  vector<char> source(10, '.');
  source.insert(source.end(), {'a', 'b', 'c', 'd', 'e', 'f'});
  source.insert(source.end(), 10, '.');
  PRINT_ELEMENTS(source, "source:  ");
  vector<char> c1(source);
  copy(c1.begin() + 10, c1.begin() + 16, c1.begin() + 7);
  PRINT_ELEMENTS(c1, "c1:      ");
  vector<char> c2(source);
  copy_backward(c2.begin() + 10, c2.begin() + 16, c2.begin() + 19);
  PRINT_ELEMENTS(c2, "c2:      ");

  copy(istream_iterator<string>(cin),          // begining of source
       istream_iterator<string>(),             // end of source
       ostream_iterator<string>(cout, "\n"));  // destination
}

TEST(AlgoTest, movetest) {
  // 与copy, copy_backward相同，如果元素类型未提供move语义，则被copy
  vector<string> coll1{"hello", "this", "is", "a", "example"};
  list<string> coll2;
  copy(coll1.cbegin(), coll1.cend(), back_inserter(coll2));
  move(coll2.cbegin(), coll2.cend(), ostream_iterator<string>(cout, " "));
  cout << endl;
  move(coll1.crbegin(), coll1.crend(), coll2.begin());
  move(coll2.cbegin(), coll2.cend(), ostream_iterator<string>(cout, " "));
  cout << endl;
}
