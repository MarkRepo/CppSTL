/**
 * 非更易型算法概览：
 *
 * 1. for_each
 * 2. count
 * 3. count_if
 * 4. min_element
 * 5. max_element
 * 6. minmax_element
 * 7. find
 * 8. find_if
 * 9. find_if_not
 * 10. search_n
 * 11. search
 * 12. find_end
 * 13. find_first_of
 * 14. adjacent_find
 * 15. equal
 * 16. is_permutation
 * 17. mismatch
 * 18. lexicographical_compare
 * 19. is_sorted
 * 20. is_sorted_until
 * 21. is_partitioned
 * 22. partition_point
 * 23. is_heap
 * 24. is_heap_until
 * 25. all_of
 * 26. any_of
 * 27. none_of
 */

// op不应在算法调用过程中改变内部状态
// op不应改变传入的参数
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>

#include "Algorithm/algo_helper.h"

using namespace std;
using namespace std::placeholders;

/**
 * UnaryProc for_each(beg, end, UnaryProc op);
 * 对每一个元素执行Op（内部状态已改变）;返回op的一个拷贝， since c11, return op is moved；
 * op(elem)的任何返回值都会被忽略
 */

class MeanValue {
 public:
  MeanValue() : num(0), sum(0) {}
  void operator()(int e) {
    ++num;
    sum += e;
  }
  operator double() { return static_cast<double>(sum) / static_cast<double>(num); }

 private:
  long num;
  long sum;
};

TEST(AlgoTest, for_each_test) {
  vector<int> vi = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  cout << "for_each vi: ";
  for_each(vi.begin(), vi.end(), [](int e) { cout << e << ' '; });
  cout << endl;
  for_each(vi.begin(), vi.end(), [](int& elem) { elem += 10; });
  PRINT_ELEMENTS(vi, "after += 10, vi:");
  for_each(vi.begin(), vi.end(), [=](int& elem) { elem += *vi.begin(); });
  PRINT_ELEMENTS(vi, "after += vi.begin(), vi:");
  double val = for_each(vi.begin(), vi.end(), MeanValue());
  cout << "MeanValue: " << val << endl;
}

// 元素计数: count, count_if
TEST(AlgoTest, count) {
  vector<int> coll;
  INSERT_ELEMENTS(coll, 1, 9);
  PRINT_ELEMENTS(coll, "coll: ");

  int num_5 = count(coll.begin(), coll.end(), 5);
  int num_modulus = count_if(coll.begin(), coll.end(), [](int elem) { return elem % 2 == 0; });
  int num_bigger = count_if(coll.begin(), coll.end(), [](int elem) { return elem > 4; });
  cout << num_5 << "\n" << num_modulus << "\n" << num_bigger << "\n";
}

// minmax: min_element, max_element, minmax_element
// 1.如果存在多个最小、最大值，min_element, max_element返回找到的第一个元素，
// minmax_element返回第一个最小和最后一个最大值
bool AbsLess(int e1, int e2) { return abs(e1) < abs(e2); }
TEST(AlgoTest, MinMax) {
  deque<int> coll;
  INSERT_ELEMENTS(coll, 2, 6);
  INSERT_ELEMENTS(coll, -3, 6);
  PRINT_ELEMENTS(coll, "coll:");
  // operator<
  cout << "min: " << *min_element(coll.begin(), coll.end());
  cout << "max: " << *max_element(coll.begin(), coll.end());
  auto mm = minmax_element(coll.begin(), coll.end());
  cout << "min_max: " << *(mm.first) << ": " << *(mm.second)
       << "; distance: " << distance(mm.first, mm.second) << endl;
  // with absless
  cout << "abs min:" << *min_element(coll.begin(), coll.end(), AbsLess);
  cout << "abs max:" << *max_element(coll.begin(), coll.end(), AbsLess);
  auto abs_mm = minmax_element(coll.begin(), coll.end(), AbsLess);
  cout << "abs min_max" << *(abs_mm.first) << ": " << *(abs_mm.second)
       << "; distance: " << distance(abs_mm.first, abs_mm.second) << endl;
}

//查找
TEST(AlgoTest, Search) {
  // 查找第一个匹配元素： find, find_if, find_if_not
  list<int> coll;
  INSERT_ELEMENTS(coll, 1, 9);
  INSERT_ELEMENTS(coll, 1, 9);
  PRINT_ELEMENTS(coll, "coll: ");

  auto pos1 = std::find(coll.begin(), coll.end(), 4);
  list<int>::iterator pos2;
  if (pos1 != coll.end()) {
    pos2 = std::find(++pos1, coll.end(), 4);
  }
  if (pos1 != coll.end() && pos2 != coll.end())
    std::copy(--pos1, ++pos2, std::ostream_iterator<int>(std::cout, " "));
  cout << endl;

  vector<int> v;
  INSERT_ELEMENTS(v, 1, 9);
  PRINT_ELEMENTS(v, "vector<int> v: ");
  auto pos = std::find_if(v.begin(), v.end(), std::bind(std::greater<int>(), _1, 3));
  cout << "the " << distance(v.begin(), pos) + 1 << "th element is first greater than 3" << endl;
  pos = std::find_if(v.begin(), v.end(), [](int elem) { return elem % 3 == 0; });
  cout << "the " << distance(v.begin(), pos) + 1 << "th element is first modules 3" << endl;
  pos = std::find_if_not(v.begin(), v.end(), std::bind(std::less<int>(), _1, 5));
  cout << "the " << distance(v.begin(), pos) + 1 << "th element is first not less than 5" << endl;

  //查找前n个连续匹配值：
  // search_n(beg, end, count, val), search_n(beg, end, count, val,/binary_pred)
  // 1. 返回连续count个元素值都为val的第一个元素位置， 没有找到返回end
  // 2. 返回连续count个元素造成pred(elem, val) == true的第一个元素位置，没有找到返回end
  deque<int> d{1, 2, 7, 7, 6, 3, 9, 5, 7, 7, 7, 3, 6};
  auto dpos = search_n(d.begin(), d.end(), 3, 7);
  cout << "3个7: " << distance(d.begin(), dpos) + 1 << endl;
  dpos = search_n(d.begin(), d.end(), 4, 0, [](int elem, int value) { return elem % 2 != 0; });
  cout << "连续4个奇数： " << distance(d.begin(), dpos) + 1 << endl;

  //查找第一个子区间
  // 1. search(beg, end, sbeg, send) 相等判断
  // 2. search(beg, end, sbeg, send, binary_pred) binary_pred(elem, selem) == true
  // 3. 返回子区间第一个元素位置， 如果没有找到，都返回end
  deque<int> ds{1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7};
  list<int> ls{3, 4, 5, 6};
  auto dpos2 = search(ds.begin(), ds.end(), ls.begin(), ls.end());
  while (dpos2 != ds.end()) {
    cout << "distance: " << distance(ds.begin(), dpos2) + 1 << endl;
    dpos2 = search(++dpos2, ds.end(), ls.begin(), ls.end());
  }
  auto checkEven = [](int elem, bool even) {
    if (even)
      return elem % 2 == 0;
    else
      return elem % 2 != 0;
  };
  vector<int> vs{1, 2, 3, 4, 5, 6, 7, 8, 9};
  bool checkEvenArgs[3] = {true, false, true};  // 查找 偶数、奇数、偶数子序列
  auto vpos = search(vs.begin(), vs.end(), checkEvenArgs, checkEvenArgs + 3, checkEven);
  while (vpos != vs.end()) {
    cout << "distance: " << distance(vs.begin(), vpos) + 1 << endl;
    vpos = search(++vpos, vs.end(), checkEvenArgs, checkEvenArgs + 3, checkEven);
  }

  // 查找最后一个子区间
  // find_end(beg, end, sbeg, send)
  // find_end(beg, end, sbeg, send, binary_pred)
  // 返回子区间第一个元素位置，如果没有找到，返回end
  auto end = ds.end();
  auto dpos3 = find_end(ds.begin(), end, ls.begin(), ls.end());
  while (dpos3 != end) {
    cout << "distance: " << distance(ds.begin(), dpos3) + 1 << endl;
    end = dpos3;
    dpos3 = find_end(ds.begin(), end, ls.begin(), ls.end());
  }

  auto vend = vs.end();
  auto vpos2 = find_end(vs.begin(), vs.end(), checkEvenArgs, checkEvenArgs + 3, checkEven);
  auto dis = distance(checkEvenArgs, checkEvenArgs + 3);
  while (vpos2 != vend) {
    cout << "distance: " << distance(vs.begin(), vpos2) + 1 << endl;
    vend = vpos2 + dis - 1;
    vpos2 = find_end(vs.begin(), vend, checkEvenArgs, checkEvenArgs + 3, checkEven);
  }

  //查找某些元素的第一次出现地点
  // 1. find_first_of(beg, end, sbeg, send) : 第一个同时出现在[beg, end),[sbeg, send)区间的元素
  // 2. find_first_of(beg, end, sbeg, send, binary_pred)： 第一个满足以下条件的元素：
  // 他和区间[sbeg, send)内的任一个元素执行 op(elem, s_elem) 等于 true
  auto vpos3 = find_first_of(vs.begin(), vs.end(), ls.begin(), ls.end());
  if (vpos3 != vs.end()) cout << "vs first of ls: " << *vpos3 << endl;
  auto vpos4 = find_first_of(vs.rbegin(), vs.rend(), ls.begin(), ls.end());  // reverse_iterator
  if (vpos4 != vs.rend()) cout << "vs last of ls: " << *vpos4 << endl;
  vpos3 = find_first_of(
      vs.begin(), vs.end(), ls.begin(), ls.end(), [](int elem, int selem) { return elem > selem; });
  if (vpos3 != vs.end()) cout << "vs first of ls with op: " << *vpos3 << endl;

  // 查找两个连续相等的元素，返回第一个元素位置，没找到返回end
  // 1. adjacent_find(beg, end)
  // 2. adjacent_end(beg, end, op)
  auto doubled = [](int e1, int e2) { return e1 * 2 == e2; };
  vector<int> voll{1, 3, 2, 4, 5, 5, 0};
  auto apos = adjacent_find(voll.begin(), voll.end());
  cout << "连续相等元素： " << *apos << endl;
  apos = adjacent_find(voll.begin(), voll.end(), doubled);
  cout << "满足doubled: " << *apos << endl;
}

//区间比较
TEST(AlgoTest, qujian_test) {
  // 比较两区间元素是否相等
  // bool equal(beg, end, cmpBeg); operator==
  // bool equal(beg, end, cmpBeg, binary_op); op(e1, e2) == true
  auto bothEvenOrOdd = [](int e1, int e2) { return e1 % 2 == e2 % 2; };
  vector<int> coll1;
  list<int> coll2;
  INSERT_ELEMENTS(coll1, 1, 7);
  INSERT_ELEMENTS(coll2, 3, 9);
  if (equal(coll1.begin(), coll1.end(), coll2.begin()))
    cout << "coll1 == coll 2" << endl;
  else
    cout << "coll1 != coll2" << endl;
  if (equal(coll1.begin(), coll1.end(), coll2.begin(), bothEvenOrOdd))
    cout << "coll1 == coll2 with bothEvenOrOdd" << endl;
  else
    cout << "coll1 != coll2 with bothEvenOrOdd" << endl;

  // 测试不定序之相等性, 即是否为同一个排列组合，即顺序无所谓的情况下两区间的元素是否相等
  // bool is_permutation(beg, end, cmpBeg); operator==
  // boll is_permutation(beg, end, cmpBeg, binary_op) ; op(e1, e2) == true
  coll1 = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  coll2 = {1, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  deque<int> coll3 = {11, 12, 13, 19, 18, 17, 16, 15, 14, 11};

  if (is_permutation(coll1.begin(), coll1.end(), coll2.begin()))
    cout << "coll1 permutation with coll2" << endl;
  else
    cout << "coll1 is not permutation with coll2" << endl;
  if (is_permutation(coll1.begin(), coll1.end(), coll3.begin(), bothEvenOrOdd))
    cout << "coll1 permutaion with coll3, use bothEvenOrOdd" << endl;
  else
    cout << "coll1 not permutatin with coll3, use bothEvenOrOdd" << endl;

  // 查找第一处不同， 返回第一处不同的对应元素
  // 如果没有找到，返回end和对应的第二元素位置
  // pair<Iter1, Iter2> mismatch(beg, end, cmpBeg);
  // pair<Iter1, Iter2> mismatch(beg, end, cmpBeg, op)
  coll1 = {1, 2, 3, 4, 5, 6};
  coll2 = {1, 2, 4, 8, 16, 3};
  auto pair = mismatch(coll1.begin(), coll1.end(), coll2.begin());
  if (pair.first == coll1.end()) {
    cout << "no mismatch" << endl;
  } else {
    cout << "first mismatch " << *pair.first << " and " << *pair.second << endl;
  }
  pair = mismatch(coll1.begin(), coll1.end(), coll2.begin(), std::less_equal<int>());
  if (pair.first == coll1.end()) {
    cout << "no mismatch with std::less<int>" << endl;
  } else {
    cout << "first mismatch with std::less<int> " << *pair.first << " and " << *pair.second << endl;
  }

  // 字典序比较，检验小于
  // 字典序排序：两序列中的元素一一比较，知道出现以下情况：
  // 1. 如果两元素不相等，则两个元素的比较结果就是两个序列的比较结果
  // 2. 如果序列元素数量不同，则元素少的序列小于另一列， 如果较少的是第一个序列，返回true
  // 3. 如果两序列都没有更多的元素比较，则相等， 返回false
  //
  // bool lexicographical_compare(beg1, end1, beg2, end2);
  // boll lexicographical_compare(beg1, end1, beg2, end2, op);
  auto printCollection = [](const list<int>& l) { PRINT_ELEMENTS(l); };
  auto lessForCollection = [](const list<int>& l1, const list<int>& l2) {
    return lexicographical_compare(l1.begin(), l1.end(), l2.begin(), l2.end());
  };
  list<int> c1, c2, c3, c4;
  INSERT_ELEMENTS(c1, 1, 5);
  c4 = c3 = c2 = c1;
  c1.push_back(7);
  c3.push_back(2);
  c3.push_back(0);
  c4.push_back(2);

  vector<list<int>> cc;
  cc.insert(cc.begin(), {c1, c2, c3, c4, c3, c1, c4, c2});
  for_each(cc.begin(), cc.end(), printCollection);
  sort(cc.begin(), cc.end(), lessForCollection);
  cout << endl << endl;
  for_each(cc.begin(), cc.end(), printCollection);

  // Predicate, 用以检验区间是否符合某个条件
  // 1. 检验是否排序
  // bool is_sorted(beg, end)
  // boll is_sorted(beg, end, op)
  // ForwardIterator is_sorted_until(beg, end);
  // FOrwardIterator is_sorted_until(beg, end, op); 返回第一个破坏排序的元素，如果不存在返回end
  coll1 = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  if (is_sorted(coll1.begin(), coll1.end())) {
    cout << "is_sorted" << endl;
  } else {
    cout << "not sorted" << endl;
  }

  auto compareName = [](const std::pair<int, string>& e1, const std::pair<int, string>& e2) {
    return e1.second < e2.second;
  };
  map<int, string> moll;
  moll = {{1, "Bill"}, {2, "Jim"}, {3, "Nico"}, {4, "Liu"}, {5, "Ai"}};
  if (is_sorted(moll.begin(), moll.end(), compareName)) {
    cout << "name is sorted" << endl;
  } else {
    cout << "name is not sorted" << endl;
  }
  auto pos = is_sorted_until(moll.begin(), moll.end(), compareName);
  if (pos != moll.end()) {
    cout << "first unsorted name " << pos->second << endl;
  }

  /**
   * 检验是否被分割：
   * bool is_partitioned(beg, end, op)； //所有满足op的元素是否在不满足元素之前
   * ForwardIterator partition_point(beg, end, op); //返回被分割区间的一个不满足op的元素
   */
  auto Odd = [](int e) { return e % 2 == 1; };
  vector<int> vii = {5, 3, 9, 1, 3, 4, 8, 6, 2};
  // vii = {1,3,5,7,9}; // vii_pos == vii.end();
  if (is_partitioned(vii.begin(), vii.end(), Odd)) {
    cout << "vii is partitioned by Odd" << endl;
    auto vii_pos = partition_point(vii.begin(), vii.end(), Odd);
    if (vii_pos != vii.end())
      cout << "first Even number is " << *vii_pos << endl;
    else
      cout << "viipos == vii.end()" << endl;
  } else {
    cout << "vii is not partitioned" << endl;
  }

  /**
   * 检验是否形成heap（最大值元素在第一位）
   * bool is_heap(beg, end)
   * boll is_heap(beg, end, op)
   * RandomAccessIterator is_heap_until(beg, end) // 返回第一个破坏排序使无法成为heap的
   * 元素位置（该元素比第一个元素大）
   * RandomAccessIterator is_heap_until(beg, end, op)
   */
  vector<int> heap = {9, 8, 7, 7, 7, 5, 4, 2, 1};
  vector<int> heap2 = {5, 3, 2, 1, 4, 7, 9, 8, 6};
  cout << boolalpha << "heap is heap: " << is_heap(heap.begin(), heap.end()) << endl;
  cout << boolalpha << "heap2 is heap: " << is_heap(heap2.begin(), heap2.end()) << endl;
  auto heap_pos = is_heap_until(heap2.begin(), heap2.end());
  if (heap_pos != heap2.end()) {
    cout << "first non-heap element: " << *heap_pos << endl;
  }

  /**
   * 检验All， Any， None； 所有，至少一个，没有
   * bool all_of(beg, end, op); // 如果区间为空， 返回true
   * bool any_of(beg, end, op); // 如果区间为空， 返回false
   * bool none_of(beg, end, op); // 如果区间为空，返回true
   */
  vii = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  cout << "all Odd: " << all_of(vii.begin(), vii.end(), Odd) << endl;
  cout << "any Odd: " << any_of(vii.begin(), vii.end(), Odd) << endl;
  cout << "none Odd: " << none_of(vii.begin(), vii.end(), Odd) << endl;
}