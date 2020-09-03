/**
 * type_trait的目的：
 * type_trait
 * 是一种用来处理type属性的方法，他是个template，可在编译期根据一或多个实参产出一个type或value
 */

#include <gtest/gtest.h>

#include <array>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 1. 根据类型是否为指针有不同的行为
template <typename T>
void foo_impl(const T& val, std::true_type) {
  std::cout << "foo() call for pointer type" << std::endl;
}

template <typename T>
void foo_impl(const T& val, std::false_type) {
  std::cout << "foo() call for value type" << std::endl;
}

template <typename T>
void foo(const T& val) {
  foo_impl(val, std::is_pointer<T>());
}

// std::common_type的实现，利用了?: 操作符的规则，该操作符必须根据两个操作数的类型产生结果类型
template <typename T1, typename T2>
struct common_type {
  typedef decltype(true ? std::declval<T1>() : std::declval<T2>()) type;
};

// 2. 处理共通类型
template <typename T1, typename T2>
typename common_type<T1, T2>::type min(const T1& x, const T2& y) {
  return x < y ? x : y;
}

struct person {
  std::string first_name;
  std::string last_name;
  void setname(std::string first_name);
};

struct Score {
  Score(double a, double b, double c) noexcept {
    this->a = a;
    this->b = b;
    this->c = c;
  }
  double a;
  double b;
  double c;
};

enum class Color { red, black };
void TestTypePropertyTrait() {
  // 1. is_void<T>
  ASSERT_EQ(std::is_void<void>(), true);
  ASSERT_EQ(std::is_void<int>(), false);

  // 2. is_integral<T> 整数类型, 包括bool， char, char16_t, char32_t, wchar_t
  ASSERT_EQ(std::is_integral<bool>(), true);
  ASSERT_EQ(std::is_integral<char>(), true);
  ASSERT_EQ(std::is_integral<int>(), true);
  ASSERT_EQ(std::is_integral<double>(), false);

  // 3. std::is_floating_point<T> 浮点数类型（float， double， long double）
  ASSERT_EQ(std::is_floating_point<float>(), true);
  ASSERT_EQ(std::is_floating_point<int>(), false);

  // 4. std::is_arithmetic<T> 整数或浮点数类型
  ASSERT_EQ(std::is_arithmetic<int>(), true);
  ASSERT_EQ(std::is_arithmetic<std::string>(), false);

  // 5. std::is_signed<T> 带正负号的算术类型
  // 6. std::is_unsigned<T> 不带正负号的算术类型
  ASSERT_EQ(std::is_signed<int>(), true);
  ASSERT_EQ(std::is_unsigned<unsigned int>(), true);

  // 7. is_const<T> 带const限定符
  ASSERT_EQ(std::is_const<const int>(), true);
  ASSERT_EQ(std::is_const<int>(), false);

  // 8. is_volatile<T> 带有volatile限定符
  ASSERT_EQ(std::is_volatile<volatile int>(), true);
  ASSERT_EQ(std::is_volatile<int>(), false);

  // 9. is_array<T> 寻常array类型（不是std::array）
  ASSERT_EQ(std::is_array<int[]>(), true);
  ASSERT_EQ((std::is_array<std::array<int, 5>>()), false);

  // 10. is_enum<T> 枚举类型
  ASSERT_EQ(std::is_enum<int>(), false);
  ASSERT_EQ(std::is_enum<Color>(), true);

  // 11. is_union<T> 联合类型
  union int_pointer {
    int a;
    int* p;
  };
  ASSERT_EQ(std::is_union<int>(), false);
  ASSERT_EQ(std::is_union<int_pointer>(), true);

  // 12. is_class<T> Class/Struct 类型，但不是union类型
  ASSERT_EQ(std::is_class<int_pointer>(), false);
  ASSERT_EQ(std::is_class<person>(), true);

  // 13. is_function<T> 函数类型
  auto lamda = []() { return 0; };
  ASSERT_EQ(std::is_function<decltype(lamda)>(), false);  // lambda 不是函数类型
  ASSERT_EQ(std::is_function<int()>(), true);

  // 14. is_reference<T>   lvalue or rvalue reference
  // 15. is_lvalue_reference<T> lvalue reference
  // 15. is_rvalue_reference<T> rvalue reference
  ASSERT_EQ(std::is_reference<int&>(), true);
  ASSERT_EQ(std::is_reference<int&&>(), true);
  ASSERT_EQ(std::is_lvalue_reference<int&>(), true);
  ASSERT_EQ(std::is_rvalue_reference<int&&>(), true);

  // 16. is_pointer<T> pointer类型，包括function pointer，但不包括pointer to nonstatic member
  ASSERT_EQ(std::is_pointer<int*>(), true);
  ASSERT_EQ(std::is_pointer<int (*)(int, int)>(), true);
  ASSERT_EQ(std::is_pointer<std::string person::*>(), false);

  // 17. is_member_pointer<T> pointer, 指向一个nonstatic成员
  // 18. is_member_object_pointer<T> pointer, 指向一个nonstatic数据成员
  // 19. is_member_function_pointer<T> pointer, 指向一个nonstatic 成员函数
  ASSERT_EQ(std::is_member_pointer<std::string person::*>(), true);
  ASSERT_EQ(std::is_member_pointer<void (person::*)(std::string)>(), true);
  ASSERT_EQ(std::is_member_object_pointer<std::string person::*>(), true);
  ASSERT_EQ(std::is_member_function_pointer<void (person::*)(std::string)>(), true);

  // 20. is_fundamental<T> void, 整型（包括bool，char），浮点数，std::nullptr_t
  ASSERT_EQ(std::is_fundamental<void>(), true);
  ASSERT_EQ(std::is_fundamental<std::nullptr_t>(), true);
  ASSERT_EQ(std::is_fundamental<float>(), true);

  // 21. is_scalar<T> 整数型（bool，char），浮点数，枚举，pointer， member pointer， nullptr_t
  ASSERT_EQ(std::is_scalar<bool>(), true);
  ASSERT_EQ(std::is_scalar<Color>(), true);
  ASSERT_EQ(std::is_scalar<std::string person::*>(), true);

  // 22. is_object<T> 任何类型，除了void， function， reference
  ASSERT_EQ(std::is_object<void>(), false);
  ASSERT_EQ(std::is_object<int()>(), false);
  ASSERT_EQ(std::is_object<int&>(), false);
  ASSERT_EQ(std::is_object<person>(), true);
  ASSERT_EQ(std::is_object<int>(), true);

  // 23. is_compound<T> Array，Enum, union, class, function, reference, pointer
  ASSERT_EQ((std::is_compound<std::array<int, 5>>()), true);
  ASSERT_EQ(std::is_compound<int[]>(), true);
  ASSERT_EQ(std::is_compound<Color>(), true);
  ASSERT_EQ(std::is_compound<int_pointer>(), true);

  // 24. is_trivial<T> Scalar,Trivial class或那些类型构成的array
  ASSERT_EQ(std::is_trivial<std::string person::*>(), true);
  ASSERT_EQ(std::is_trivial<int[]>(), true);

  // 25. is_trivial_copyable<T>, Scalar, trivial copyable class 或那些类型构成的array
  ASSERT_EQ(std::is_trivially_copyable<std::string person::*>(), true);
  ASSERT_EQ(std::is_trivially_copyable<std::string>(), false);

  // 26. is_standard_layout<T>, Scalar, standard layout class 或哪些类型构成的array
  ASSERT_EQ(std::is_standard_layout<std::string>(), true);
  ASSERT_EQ((std::is_standard_layout<std::pair<int, double>>()), true);

  // 27. is_pod<T>, plain old data type，memcpy可用来复制对象的类型
  ASSERT_EQ((std::is_pod<std::vector<int>>()), false);
  ASSERT_EQ(std::is_pod<char[]>(), true);

  // 28. is_literal_type<T>, Scalar, reference, class 或那些类型构成的array
  ASSERT_EQ(std::is_literal_type<int&>(), true);
  ASSERT_EQ(std::is_literal_type<person>(), false);
  ASSERT_EQ(std::is_literal_type<std::string>(), false);
  ASSERT_EQ(std::is_literal_type<int_pointer>(), true);
}

class Student : public person {
 public:
  Student() = default;
  Student(std::string first_name, std::string last_name) : person{first_name, last_name} {}
};
class Empty {
 public:
  void echo(std::string s) { std::cout << s << std::endl; }
};

class ITeacher {
 public:
  virtual void set() = 0;
};

void TestClassTypePropertyTrait() {
  // 1. is_empty<T> 不带任何成员，vitual 成员函数，或virtual base class
  ASSERT_EQ(std::is_empty<Student>(), false);
  ASSERT_EQ(std::is_empty<Empty>(), true);

  // 2. is_polymorphic<T>, 带有一个（derived）virtual 成员函数
  // 3. is_abstract<T>, 至少有一个pure virtual function
  ASSERT_EQ(std::is_polymorphic<ITeacher>(), true);
  ASSERT_EQ(std::is_abstract<ITeacher>(), true);

  // 4. has_virtual_destructor<T>, class 带有virtual析构函数
  // 5. is_default_constructible<T>, 能够完成defalut construction
  // 6. is_copy_constructible<T>, 能够完成copy construction
  // 7. is_move_constructible<T>, 能够完成move construction
  // 8. is_copy_assignable<T>, 能够完成copy assignment
  // 9. is_move_assignable<T>, 能够完成move assignment
  // 10. is_destructible<T> ,带有可被调用的析构函数(不能是deleted，protected， private)
  ASSERT_EQ(std::has_virtual_destructor<ITeacher>(), false);
  ASSERT_EQ(std::is_default_constructible<ITeacher>(), false);
  ASSERT_EQ(std::is_default_constructible<Student>(), true);
  ASSERT_EQ(std::is_copy_constructible<Student>(), true);
  ASSERT_EQ(std::is_copy_constructible<ITeacher>(), false);
  ASSERT_EQ(std::is_move_constructible<Student>(), true);
  ASSERT_EQ(std::is_copy_assignable<Student>(), true);
  ASSERT_EQ(std::is_move_assignable<Student>(), true);
  ASSERT_EQ(std::is_destructible<Student>(), true);

  // 11. is_trivially_default_constructible<T>, 能够完成trivially defalut construction
  // 12. is_trivially_copy_constructible<T>, 能够完成trivially copy construction
  // 13. is_trivially_move_constructible<T>, 能够完成trivially move construction
  // 14. is_trivially_copy_assignable<T>, 能够完成trivially copy assignment
  // 15. is_trivially_move_assignable<T>, 能够完成trivially move assignment
  // 16. is_trivially_destructible<T> ,带有可被调用的trivially 析构函数(不能是deleted，protected，
  // private)
  ASSERT_EQ(std::is_trivially_default_constructible<Student>(), false);
  ASSERT_EQ(std::is_trivially_copy_constructible<Student>(), false);
  ASSERT_EQ(std::is_trivially_copy_constructible<ITeacher>(), false);
  ASSERT_EQ(std::is_trivially_move_constructible<Student>(), false);
  ASSERT_EQ(std::is_trivially_copy_assignable<Student>(), false);
  ASSERT_EQ(std::is_trivially_move_assignable<Student>(), false);
  ASSERT_EQ(std::is_trivially_destructible<Student>(), false);

  // 17. is_nothrow_default_constructible, 能够完成defalut construction，不抛出异常
  // 18. is_nothrow_copy_constructible<T>, 能够完成copy construction
  // 19. is_nothrow_move_constructible<T>, 能够完成move construction
  // 20. is_nothrow_copy_assignable<T>, 能够完成copy assignment
  // 21. is_nothrow_move_assignable<T>, 能够完成move assignment
  // 22. is_nothrow_destructible<T> ,带有可被调用的析构函数(不能是deleted，protected， private)
  ASSERT_EQ(std::is_nothrow_default_constructible<Student>(), true);
  ASSERT_EQ(std::is_nothrow_copy_constructible<ITeacher>(), false);
  ASSERT_EQ(std::is_nothrow_move_constructible<Student>(), true);
  ASSERT_EQ(std::is_nothrow_copy_assignable<Student>(), false);
  ASSERT_EQ(std::is_nothrow_move_assignable<Student>(), true);
  ASSERT_EQ(std::is_nothrow_destructible<Student>(), true);
}

typedef std::string Url;

void TestTypeRelationTrait() {
  // 1. is_same<T1, T2> T1,T2 是相同类型，包括const，volatile修饰符
  ASSERT_EQ((std::is_same<int, const int>()), false);
  ASSERT_EQ((std::is_same<int, volatile int>()), false);
  ASSERT_EQ((std::is_same<Student, person>()), false);
  ASSERT_EQ((std::is_same<Url, std::string>()), true);

  // 2. is_base_of(T,D) T 是 D 的base class
  ASSERT_EQ((std::is_base_of<person, Student>()), true);

  // 3. is_convertible<T, T2> 类型T可转换至T2
  ASSERT_EQ((std::is_convertible<Student, person>()), true);
  ASSERT_EQ((std::is_convertible<int, double>()), true);
  ASSERT_EQ((std::is_convertible<double, int>()), true);

  // 4. is_constructible<T, Args...> 可用类型Args初始化T
  ASSERT_EQ((std::is_constructible<double, int>()), true);
  ASSERT_EQ((std::is_constructible<Student, std::string, std::string>()), true);

  // 5. is_trivially_constructible<T, Args...> 可用类型Args 平凡初始化 T
  ASSERT_EQ((std::is_trivially_constructible<Student, std::string, std::string>()), false);
  ASSERT_EQ((std::is_trivially_constructible<Score, const Score&>()), true);
  ASSERT_EQ((std::is_trivially_constructible<Score, double, double, double>()), false);

  // 6. is_nothrow_constructible<T, Args>
  ASSERT_EQ((std::is_nothrow_constructible<Score, double, double, double>()), true);

  // 7. is_assignable<T, T2> 可用T2赋值给T
  // 第一个类型如果是nonclass类型，永远是false
  ASSERT_EQ((std::is_assignable<double, int>()), false);
  ASSERT_EQ((std::is_assignable<double&, int>()), true);
  ASSERT_EQ((std::is_assignable<double&&, int>()), false);
  ASSERT_EQ((std::is_assignable<person, Student>()), true);
  ASSERT_EQ((std::is_assignable<std::string, const char*>()), true);

  // 8. is_trivially_assignable<T, T2> 可将T2平凡赋值给T
  ASSERT_EQ((std::is_trivially_assignable<person, Student>()), false);
  ASSERT_EQ((std::is_trivially_assignable<person, const person&>()), false);

  // 9. is_nothrow_assignable<T,T2>
  ASSERT_EQ((std::is_nothrow_assignable<person, const person&>()), false);
  ASSERT_EQ((std::is_nothrow_assignable<double, double>()), false);

  // 10. use_allocator<T, Alloc> , alloc 可被转换为T::allocator_type
  ASSERT_EQ((std::uses_allocator<std::vector<int>, std::allocator<int>>()), true);
}

void TestTypeModifierTrait() {
  // 1. remove_const<T>
  // 2. remove_volatile<T>
  // 3. remove_cv<T>
  // 4. add_const<T>
  // 5. add_volatile<T>
  // 6. add_cv<T>
  // 7. make_signed<T> , 带正负号的nonreference类型
  // 8. make_unsigned<T>， 不带正负号的nonreference类型
  // 9. remove_reference<T>
  // 10. add_lvalue_reference<T>， rvalue 变成lvalue
  // 11. add_rvalue_reference<T>， lvalue 保持lvalue
  // 12. remove_pointer<T>， 若非pointer，获得原类型
  // 13. add_pointer<T>，一个pointer类型，指向对应之nonreference 类型
  typedef int T;
  ASSERT_EQ((std::is_same<typename std::remove_const<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_volatile<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_cv<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_const<T>::type, const int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_volatile<T>::type, volatile int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_cv<T>::type, const volatile int>()), true);
  ASSERT_EQ((std::is_same<typename std::make_signed<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::make_unsigned<T>::type, unsigned int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_reference<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_lvalue_reference<T>::type, int&>()), true);
  ASSERT_EQ((std::is_same<typename std::add_rvalue_reference<T>::type, int&&>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_pointer<T>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_pointer<T>::type, int*>()), true);

  typedef const int& TT;
  ASSERT_EQ((std::is_same<typename std::remove_const<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_volatile<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_cv<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::add_const<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::add_volatile<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::add_cv<TT>::type, const int&>()), true);
  // undefine behavior, make_signed, make unsigned 实参必须是整数类型或enum
  // ASSERT_EQ((std::is_same<typename std::make_signed<TT>::type, const int>()), true);
  // ASSERT_EQ((std::is_same<typename std::make_unsigned<TT>::type, const unsigned int>()), true);

  // 一个指向常量类型的reference本身并不是常量，所以不可以移除其常量性
  ASSERT_EQ((std::is_same<typename std::remove_reference<TT>::type, const int>()), true);
  ASSERT_EQ((std::is_same<typename std::add_lvalue_reference<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::add_rvalue_reference<TT>::type, const int&>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_pointer<TT>::type, const int&>()), true);
  // add_pointer 必然包含 remove_reference
  ASSERT_EQ((std::is_same<typename std::add_pointer<TT>::type, const int*>()), true);

  //将lvalue reference 转换成rvalue reference
  ASSERT_EQ((std::is_same<
                typename std::add_rvalue_reference<typename std::remove_reference<TT>::type>::type,
                const int&&>()),
            true);
}

double func_test(int a, char b) {
  std::cout << "call_func_test ..." << std::endl;
  return 1.1;
}
class Callable {
 public:
  double operator()(int a, char b) { return 1.1; }
  char operator()(char b, float c) { return '\0'; }
  float getfloat(double a, int b) { return 1.1; }
};

// 12. aligned_storage<Len,Align=/*default alignment*/> len byte加上默认对齐规则后所可容纳之类型
// 13. aligned_storage<Len, Align> Len byte且以size_t Align之约数进行对齐，所可容纳之类型
/**
 * 提供嵌套类型 type ，其为平凡的标准布局类型，
 * 适于作为任何大小至多为 Len 且对齐要求为 Align 的因数的对象的未初始化存储。
 * Align 的默认值是任何大小至多为 Len 的对象的最强（最大）对齐要求。若不使用默认值，
 * 则 Align 对于某类型 T 必须为 alignof(T) 的值，否则行为未定义。
 * 若 Len == 0 则行为未定义
 */
template <typename T, std::size_t N>
class static_vector {
  typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
  std::size_t m_size = 0;

 public:
  template <typename... Args>
  void emplace_back(Args... args) {
    if (m_size >= N) {
      throw std::bad_alloc{};
    }
    new (data + m_size) T(std::forward<Args>(args)...);
    ++m_size;
  }

  const T& operator[](std::size_t pos) { return *reinterpret_cast<const T*>(data + pos); }

  ~static_vector() {
    for (std::size_t pos = 0; pos < m_size; pos++) {
      reinterpret_cast<T*>(data + pos)->~T();
    }
  }
};

// aligned_union 的可能实现
template <std::size_t Len, class... Types>
struct aligned_union {
  static constexpr std::size_t alignment_value = std::max({alignof(Types)...});

  struct type {
    alignas(alignment_value) char _s[std::max({Len, sizeof(Types)...})];
  };
};

void TestOtherTypeTrait() {
  // 1. rank<T> Array类型的维度（或0）
  ASSERT_EQ(std::rank<int>(), 0);
  ASSERT_EQ(std::rank<int[5]>(), 1);
  ASSERT_EQ((std::rank<int[4][5]>()), 2);
  // 2. extent<T, I=0> 维度I(或0)的尺度（宽度，extent）
  ASSERT_EQ(std::extent<int>(), 0);
  ASSERT_EQ(std::extent<int[]>(), 0);
  ASSERT_EQ(std::extent<int[5]>(), 5);
  ASSERT_EQ(std::extent<int[][7]>(), 0);
  ASSERT_EQ(std::extent<int[5][7]>(), 5);
  ASSERT_EQ((std::extent<int[][7], 1>()), 7);
  ASSERT_EQ((std::extent<int[5][7], 1>()), 7);
  ASSERT_EQ((std::extent<int[5][7], 2>()), 0);
  // 3. remove_extent<T> 获得array的元素类型，若T不是array，则获得类型T
  ASSERT_EQ((std::is_same<typename std::remove_extent<int>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_extent<int[]>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_extent<int[5]>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_extent<int[][7]>::type, int[7]>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_extent<int[5][7]>::type, int[7]>()), true);
  // 4. remove_all_extent<T> 获得多维array的元素类型，若T不是多维array，则获得类型T
  ASSERT_EQ((std::is_same<typename std::remove_all_extents<int>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_all_extents<int[]>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_all_extents<int[5]>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_all_extents<int[][7]>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::remove_all_extents<int[5][7]>::type, int>()), true);

  // 5. underlying_type<T> 枚举类型的底层类型
  ASSERT_EQ((std::is_same<typename std::underlying_type<Color>::type, int>()), true);
  // 6. decay<T> 将T转换为对应之“实值类型”  （“by value" type）
  /**
   * 对类型 T 应用左值到右值、数组到指针及函数到指针隐式转换，移除 cv 限定符，并定义结果类型为成员
   * typedef type 。 正式而言： 若T 指名“U的数组” 或 “到U的数组的引用”类型，则成员 typedef type 为
   * U* 。 否则，若 T 为函数类型 F 或到它的引用，则成员 typedef type 为std::add_pointer<F>::type 。
   * 否则，成员 typedef type 为 std::remove_cv<std::remove_reference<T>::type>::type 。
   * 这些转换模仿在以值传递时，应用到所有函数参数的类型转换。
   */
  ASSERT_EQ((std::is_same<typename std::decay<const volatile int>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::decay<void()>::type, void (*)()>()), true);
  ASSERT_EQ((std::is_same<typename std::decay<int&>::type, int&&>()), false);
  ASSERT_EQ((std::is_same<typename std::decay<int&>::type, int>()), true);
  // 7. enable_if<B, T=void> 只有当B为true时会产出T
  ASSERT_EQ((std::is_same<typename std::enable_if<true, int>::type, int>()), true);
  // ASSERT_EQ((std::is_same<typename std::enable_if<false, int>::type, int>()), false);
  // 8. conditional<B,T,F>, 当B为true时产出T，否则产出F
  ASSERT_EQ((std::is_same<typename std::conditional<true, int, char>::type, int>()), true);
  ASSERT_EQ((std::is_same<typename std::conditional<false, int, char>::type, char>()), true);
  // 9. common_type<T1,...> 所有传入类型的共通类型
  ASSERT_EQ(
      (std::is_same<typename std::common_type<std::string, const char*>::type, std::string>()),
      true);
  // 10. result_of<F, ArgTypes> 调用F并传入实参类型ArgTypes后，所得结果的类型
  ASSERT_EQ((std::is_same<typename std::result_of<Callable(int, char)>::type, double>()), true);
  ASSERT_EQ((std::is_same<typename std::result_of<Callable(char, float)>::type, char>()), true);
  ASSERT_EQ(
      (std::is_same<
          typename std::result_of<decltype (&Callable::getfloat)(Callable, double, int)>::type,
          float>()),
      true);
  ASSERT_EQ(
      (std::is_same<typename std::result_of<decltype (&func_test)(int, char)>::type, double>()),
      true);
  struct A {
    char c;
    int i;
    double d;
    bool b;
  };
  // 11. alignment_of<T> 等价于 alignof(T)
  std::cout << "alignment of A: " << std::alignment_of<A>::value << std::endl;
  // 12. aligned_storage<Len,Align=/*default alignment*/> len byte加上默认对齐规则后所可容纳之类型
  // 13. aligned_storage<Len, Align> Len byte且以size_t Align之约数进行对齐，所可容纳之类型
  /**
   * 提供嵌套类型 type ，其为平凡的标准布局类型，
   * 适于作为任何大小至多为 Len 且对齐要求为 Align 的因数的对象的未初始化存储。
   * Align 的默认值是任何大小至多为 Len 的对象的最强（最大）对齐要求。若不使用默认值，
   * 则 Align 对于某类型 T 必须为 alignof(T) 的值，否则行为未定义。
   * 若 Len == 0 则行为未定义
   *
   * 参考上述static_vector 的实现
   */
  static_vector<std::string, 10> sv;
  sv.emplace_back(5, '*');
  sv.emplace_back(10, 'a');
  std::cout << sv[0] << std::endl << sv[1] << std::endl;
  // 14. aligned_union<Len, Types...> len byte且以 a union of Types... 进行对齐，所可容纳之类型
  /**
   * 提供嵌套类型type
   * ，它是平凡的标准布局类型，且其大小和对齐适合用作任何列于Types的类型的一个对象的未初始化存储。
   * 存储的大小至少为 Len 。 std::aligned_union 亦确定所有 Types
   * 中最严格（最大）的对齐要求，使之可用作常量 alignment_value 。 若 sizeof...(Types) == 0 或若
   * Types 中的任何类型不是完整对象类型，则行为未定义。
   */
  typename std::aligned_union<16, int, double, std::string>::type data;
  *reinterpret_cast<int*>(&data) = 20;
  std::cout << "aligned_union value: " << *reinterpret_cast<int*>(&data) << std::endl;
}

}  //  namespace

TEST(ttTest, foo) {
  int a = 5;
  int* p = &a;
  foo(a);
  foo(p);
}

TEST(ttTest, min) {
  auto min_v = min(5, 6.2);
  auto min_s = min(std::string("abcd"), "efgh");
  std::cout << "min_v: " << min_v << std::endl;
  std::cout << "min_s: " << min_s << std::endl;
}

TEST(ttTest, std_traits) {
  TestTypePropertyTrait();
  TestClassTypePropertyTrait();
  TestTypeRelationTrait();
  TestTypeModifierTrait();
  TestOtherTypeTrait();
}