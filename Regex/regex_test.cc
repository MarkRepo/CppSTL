#include <gtest/gtest.h>

#include <iostream>
#include <regex>
#include <string>

using namespace std;

TEST(RegexTest, regex_match_search) {
  auto out = [](bool b) { cout << (b ? "found" : "not found") << endl; };
  const string data = "<tag>value</tag>";
  const string notfound_data = "<tag>value</notfound>";
  const string s = "<.*>.*</.*>";              // ‘.’ 意指处newline 即\n 之外任意字符
  const string group_s = "<(.*)>.*</\\1>";     // using group
  const string grep_s = "<\\(.*\\)>.*</\\1>";  // using grep syntax

  // find XML/HTML-tagged value with default syntax
  regex reg_s(s);
  out(regex_match(data, reg_s));  // found

  // tags before and after value must match
  regex reg_group_s(group_s);
  out(regex_match(data, reg_group_s));           // found
  out(regex_match(notfound_data, reg_group_s));  // not found

  // using grep syntax
  regex reg_grep_s(grep_s, regex_constants::grep);
  out(regex_match(data, reg_grep_s));  // found

  // using C-String as regular expresion(need explicit cast to regex)
  out(regex_match(data, regex("<(.*)>.*</\\1>")));  // found

  const string data2 = "XML tag: " + data;
  out(regex_match(data2, regex(group_s)));  // not found
  const string prefix_group_s = ".*" + group_s;
  out(regex_match(data2, regex(prefix_group_s)));  // found

  out(regex_search(data2, regex(group_s)));         //  found
  out(regex_search(data2, regex(prefix_group_s)));  // found
}

TEST(RegexTest, subexpression) {
  string data = "XML tag: <tag-name>the value </tag-name> end";
  cout << "data: " << data << endl << endl;

  /**
   * smatch  ---> string
   * cmatch ---> c-string
   * wsmatch ---> wstring
   * wcmatch ----> wide c-string
   */
  smatch m;  // for details of the match
  bool found = regex_search(data, m, regex("<(.*)>(.*)</(\\1)>"));

  cout << "m.empty(): " << boolalpha << m.empty() << endl;
  cout << "m.size(): " << m.size() << endl;

  if (found) {
    cout << "m.str():          " << m.str() << endl;
    cout << "m.length():       " << m.length() << endl;
    cout << "m.position():     " << m.position() << endl;
    cout << "m.prefix().str(): " << m.prefix().str() << endl;
    cout << "m.suffix().str(): " << m.suffix().str() << endl;
    cout << endl;

    // iterating all matchs (using the match index)
    for (int i = 0; i < m.size(); i++) {
      cout << "m[" << i << "].str(): " << m[i].str() << endl;
      cout << "m.str(" << i << "): " << m.str(i) << endl;
      cout << "m.position(" << i << "): " << m.position(i) << endl;
    }
    cout << endl;

    // iterating all matchs (using iterators)
    cout << "matches: " << endl;
    for (auto pos = m.begin(); pos != m.end(); ++pos) {
      cout << " " << *pos << " ";
      cout << "(length: " << pos->length() << ")" << endl;
    }
  }
}

TEST(RegexTest, search_all_matches) {
  // iterator over all matches
  auto search_all_matches = [](const string& data, const regex& reg) {
    auto pos = data.cbegin();
    auto end = data.cend();
    smatch m;
    cout << "====================" << endl;
    for (; regex_search(pos, end, m, reg); pos = m.suffix().first) {
      cout << "match: " << m.str() << endl;
      cout << "tag:   " << m.str(1) << endl;
      cout << "value: " << m.str(2) << endl;
    }
    cout << endl << endl;
  };

  const string data =
      "<person>\n"
      " <first>Nico</first>\n"
      " <last>Josuttis</last>\n"
      "</person>\n";

  regex reg("<(.*)>(.*)</(\\1)>");
  search_all_matches(data, reg);
  // 贪婪匹配
  const string data2 =
      "<person>"
      " <first>Nico</first>"
      " <last>Josuttis</last>"
      "</person>";
  search_all_matches(data2, reg);

  regex reg2("<(.*)>([^>]*)</(\\1)>");
  search_all_matches(data2, reg2);
}