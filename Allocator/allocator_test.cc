#include <gtest/gtest.h>

#include <cstddef>
#include <iostream>

using namespace std;

class MyType {
 public:
  MyType() { cout << "ctor for MyType" << endl; }
  MyType(const MyType& val) { cout << "copy ctor for MyType" << endl; }
  ~MyType() { cout << "dtor for MyType" << endl; }
};

//自定义allocator必须提供以下内容
template <typename T>
class MyAlloc {
 public:
  // 1. value_type类型定义
  typedef T value_type;
  // 2. 构造函数
  MyAlloc() noexcept {}
  // 3. template 构造函数，用来在类型有所改变时复制内部状态
  template <typename U>
  MyAlloc(const MyAlloc<U>&) noexcept {}
  // 4. 成员函数allocate，用来提供新的内存
  T* allocate(std::size_t num) {
    cout << "MyAlloc allocate " << num << "elems" << endl;
    return static_cast<T*>(::operator new(num * sizeof(T)));
  }
  // 5. 成员函数deallocate，用来释放不再需要的内存
  void deallocate(T* p, std::size_t num) { ::operator delete(p); }

  void construct(T* p, const T& val) { new (p) T(val); }
  void destroy(T* p) { p->~T(); }
};

// 6. ==
template <typename T1, typename T2>
bool operator==(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept {
  return true;
}

// 7. !=
template <typename T1, typename T2>
bool operator!=(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept {
  return false;
}

// 程序库开发者的角度使用Allocator
namespace MyStd {

template <typename T, typename Allocator>
class Vector {
 private:
  Allocator alloc;
  T* elems;
  size_t numElems;
  size_t sizeElems;

 public:
  explicit Vector(const Allocator& = Allocator());
  explicit Vector(size_t num, const T& val = T(), const Allocator& a = Allocator()) : alloc(a) {
    sizeElems = numElems = num;
    elems = alloc.allocate(num);

    for (size_t i = 0; i < sizeElems; i++) {
      alloc.construct(&elems[i], val);
    }
    // uninitialized_fill_n(elems, num, val);
  }
  template <typename InputIterator>
  Vector(InputIterator beg, InputIterator end, const Allocator& = Allocator());
  Vector(const Vector<T, Allocator>& v);

  void reserve(size_t size) {
    // reserve() never shrinks the memory
    if (size <= sizeElems) {
      return;
    }
    // allocate new memory for size elements
    T* newmem = alloc.allocate(size);
    // copy old elements into new memory
    cout << "before copy" << endl;
    uninitialized_copy(elems, elems + numElems, newmem);
    cout << "after copy" << endl;
    // destroy old elements
    for (size_t i = 0; i < numElems; ++i) {
      alloc.destroy(&elems[i]);
    }
    // deallocate old memory
    alloc.deallocate(elems, sizeElems);
    // so, now we have our elements in the new memory
    sizeElems = size;
    elems = newmem;
  }
};

}  // namespace MyStd

TEST(AllocTest, MyAlloc) {
  MyType a;
  size_t num = 10;
  MyStd::Vector<MyType, MyAlloc<MyType>> myVec(num, a);
  myVec.reserve(15);
}

TEST(AllocTest, raw_storage_iterator) {
  // The first template argument (T*, here) has to be an output iterator for the type of the
  // //elements. The second template argument (T, here) has to be the type of the elements.
  vector<int> x{1, 2, 3, 4, 5, 6};
  int* elems = static_cast<int*>(::operator new(6 * sizeof(int)));
  copy(x.begin(),
       x.end(),                                  // source
       raw_storage_iterator<int*, int>(elems));  // destination
  for (size_t i = 0; i < 6; i++) {
    cout << *(elems + i) << endl;
  }
}

TEST(AllocTest, temporary_buffer) {
  // allocate memory for num elements of type MyType
  const int num = 10;
  pair<MyType*, std::ptrdiff_t> p = get_temporary_buffer<MyType>(num);
  if (p.second == 0) {
    // could not allocate any memory for elements
    cout << "alloc elems error\n";
  } else if (p.second < num) {
    // could not allocate enough memory for num elements // however, don’t forget to deallocate it
    cout << "alloc " << p.second << " elements" << endl;
  } else {
    cout << "alloc " << num << " elements success!\n";
  }
  // do your processing
  // free temporarily allocated memory, if any
  if (p.first != 0) {
    return_temporary_buffer(p.first);
  }
}