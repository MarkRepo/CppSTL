#include <gtest/gtest.h>

#include <iostream>
#include <string>

using namespace std;

class Point2d {
 public:
  Point2d(int x = 0, int y = 0) : _x(x), _y(y) {}
  // virtual int z() const { return 0; }

 public:
  int _x, _y;
};

class Vertex : public virtual Point2d {
 public:
  Vertex(int v = 0) : _v(v) {}
  Vertex(int x, int y, int v) : Point2d(x, y), _v(v) {}
  virtual void print() {}

 public:
  int _v;
};

class Point3d : public virtual Point2d {
 public:
  Point3d(int x = 0, int y = 0, int z = 0) : Point2d(x, y), _z(z) {}
  int z() { return _z; }

 public:
  int _z;
};

class Vertex3d : public Point3d, public Vertex {
 public:
  Vertex3d(int m = 0) : _m(m) {}
  Vertex3d(int x, int y, int z, int v, int m)
      : Point3d(x, y, z), Vertex(x, y, v), Point2d(x, y), _m(m) {}

 public:
  int _m;
};

// 单个vptr且放置在开始位置
template <class T>
void print_layout_with_single_vptr_start(T t) {
  cout << "-------- type " << typeid(T).name() << " layout --------" << endl;
  union {
    T t;
    struct {
      long* vptr;
      int i[1];
    } layout;
  } ut{t};
  cout << "sizeof(" << typeid(T).name() << ") = " << sizeof(T) << ", sizeof(ut) = " << sizeof(ut)
       << endl;  // should be equal
  cout << "[0  ~  7] " << typeid(T).name() << " vptr: " << ut.layout.vptr << endl;

  int i = 0;
  for (int* p = ut.layout.i; reinterpret_cast<char*>(p) < reinterpret_cast<char*>(&ut) + sizeof(ut);
       p++, i++) {
    char str[256];
    snprintf(str,
             256,
             "[%02d ~ %02d] int[%d]: %08x",
             8 + sizeof(int) * i,
             8 + sizeof(int) * (i + 1) - 1,
             i,
             *p);
    cout << str << endl;
  }
  cout << endl;
}

// 验证多重派生对象的布局
TEST(ModelTest, Multiple_inheritance) {
  Point2d p2d{1, 2};
  Point3d p3d{1, 2, 3};
  Vertex v{1, 2, 4};
  Vertex3d v3d{1, 2, 3, 4, 5};
  print_layout_with_single_vptr_start(p2d);
  print_layout_with_single_vptr_start(p3d);
  print_layout_with_single_vptr_start(v);
  print_layout_with_single_vptr_start(v3d);
  cout << v3d._x << ", " << v3d._y << ", " << v3d._z << ", " << v3d._v << ", " << v3d._m << endl;

  int Point2d::*p1 = 0;
  int Point2d::*p2 = &Point2d::_x;
  if (p1 == p2) {
    cout << "p1 p2 contains the same value" << endl;
  } else {
    cout << "not same" << endl;
  }
}
