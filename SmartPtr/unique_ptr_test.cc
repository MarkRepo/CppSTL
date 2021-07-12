#include <gtest/gtest.h>

#include <memory>

class Base {
 public:
  virtual void Handle() = 0;
  virtual ~Base() = default;
};

class Derived : public Base {
 public:
  void Handle() override {}
  ~Derived() {}
};

TEST(UniqueTest, dynamic) {
  auto pDrv = std::unique_ptr<Base>{new Derived()};
  pDrv->Handle();
}