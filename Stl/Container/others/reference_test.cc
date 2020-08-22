#include <gtest/gtest.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

// 容器通常实现的是value语义，如果需要reference语义，有两种方式：
// 1. 使用smartpointer
// 2. 使用reference_wrapper

using namespace std;

class Item {
 private:
  string name_;
  float price_;

 public:
  Item(const string& name, float price) : name_(name), price_(price) {}
  string GetName() const { return name_; }
  void SetName(const string& name) { name_ = name; }
  float GetPrice() const { return price_; }
  void SetPrice(float price) { price_ = price; }
};

template <typename T>
void PrintItems(const string& msg, const T& t) {
  std::cout << msg << std::endl;
  for (const auto& elem : t) {
    cout << "name: " << elem->GetName() << ", price: " << elem->GetPrice() << endl;
  }
}

using ItemPtr = shared_ptr<Item>;

TEST(ReferenceTest, smart_pointer_test) {
  set<ItemPtr> allItems;
  deque<ItemPtr> bestsellers;
  bestsellers = {ItemPtr(new Item("name1", 1.0)),
                 ItemPtr(new Item("name2", 2.0)),
                 ItemPtr(new Item("name3", 3.0))};
  allItems = {ItemPtr{new Item{"name4", 4.0}}, ItemPtr{new Item{"name5", 5.0}}};
  allItems.insert(bestsellers.begin(), bestsellers.end());
  PrintItems("bestsellers", bestsellers);
  PrintItems("allitems", allItems);
  cout << endl;

  // double bestselles's price
  for_each(bestsellers.begin(), bestsellers.end(), [](shared_ptr<Item>& elem) {
    elem->SetPrice(elem->GetPrice() * 2);
  });

  // find会比较指针，所以这里使用自定义pred
  bestsellers[1] = *(find_if(bestsellers.begin(), bestsellers.end(), [](shared_ptr<Item>& elem) {
    return elem->GetName() == "name3";
  }));

  bestsellers[0]->SetPrice(44.77);
  PrintItems("bestsellers", bestsellers);
  PrintItems("allitems", allItems);
}

//使用reference_wrapper的前提是“元素的生命周期比容器长”
TEST(ReferenceTest, reference_wrapper_test) {
  std::vector<std::reference_wrapper<Item>> books;
  Item f{"name", 1.1};
  books.push_back(f);
  for (const auto& book : books) {
    std::cout << book.get().GetName() << ":" << book.get().GetPrice() << std::endl;
  }
  f.SetPrice(9.9);
  std::cout << books[0].get().GetPrice() << std::endl;
  // print books usint type of element(no get() necessary)
  for (const Item& book : books) {
    std::cout << book.GetName() << ":" << book.GetPrice() << std::endl;
  }
}