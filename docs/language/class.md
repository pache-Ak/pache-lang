## 类声明

---

类（class）是用户定义类型，以 类说明符 定义，它在[声明](/language/declarations "language/declarations")语法的 声明说明符序列 中出现。类说明符拥有下列语法：

类关键词 类头名 **`final`**(可选) 基类子句(可选) **`{`** 成员说明 **`}`**

定义具名类

类关键词 - [class](/keyword/class "keyword/class")，~~[struct](/keyword/struct "keyword/struct")~~ 和 [union](/keyword/union "keyword/union") 之一。~~除了默认[成员访问](/language/access "language/access")和默认[基类访问](/language/derived_class "language/derived class")之外，关键词 struct 和 class 是等同的。~~如果关键词是 union，那么声明引入一个[联合体类型](/language/union "language/union")。 属性 -  任意数量的[属性](/language/attributes "language/attributes")，可以包含 [alignas 指定符](/language/alignas "language/alignas") 类头名 - 所定义的类的名字，可以[有限定](/language/identifiers#.E6.9C.89.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "language/identifiers") **`final`** -  出现时，该类[无法被派生](/language/final "language/final") 基类子句 - 一个或多个基类以及各自所用的继承模型的列表（见[派生类](/language/derived_class "language/derived class")） 成员说明 - 访问说明符、成员对象及成员函数的声明和定义的列表（见下文）

### 成员说明

| [访问说明符](/language/access "language/access") | 解释 |
|--- |--- |
| `public` | 公开的|
| `proteced` |受保护的 |
| `private` | 私有的|

成员说明，或类定义的*体*，是花括号环绕的任何数量下列各项的序列：

1. [静态](/language/static "language/static")及非静态的[数据成员](/language/data_members "language/data members")

```cpp
class S {
  private let i32 d1
  private static let i32 d2
}
```

2. 函数定义，同时声明并定义[成员函数](/language/member_functions "language/member functions")或者[友元函数](/language/friend "language/friend")。

```cpp
class M {
  public M(std::size_t R, std::size_t C) : C(C), data(R\*C) {} // 构造函数定义

  public operatr()(this const &self, size_t r, size_t c) i32 { // 成员函数定义
    return this.data[r * C + c]
  }

  public operator()(this & self, size_t r, size_t c) i32 & {  // 另一个成员函数定义
    return this.data[r * C + c]
  }

  private std::size_t C
  private std::vector<i32> data
}
```

3. [using 声明](/language/using_declaration "language/using declaration")：

```cpp
class Base {
  protected let i32 d
}
 
class Derived : Base {
public:
    using Base::d // 令 Base 的受保护成员 d 成为 Derived 的公开成员
    using Base::Base // 继承基类的所有构造函数（C++11）
}
```

4. [static\_assert](/w/cpp/language/static_assert "cpp/language/static assert") 声明：

```cpp
class <typename T> Foo {
    static_assert(std::is_floating_point<T>::value, "Foo<T>: T 必须是浮点数")
}
```

5. [别名声明](/language/type_alias "language/type alias")：

```cpp
class <typename T> identity {
    using type = T
}
```

6. [using enum 声明](/language/enum#using_enum_.E5.A3.B0.E6.98.8E "language/enum")：

```cpp
enum class color { red, orange, yellow }
 
class highlight {
    using enum color
}
```

7. [友元声明](/language/friend "language/friend")

```cpp
class A { }
class F {
  friend A
}
```

8. 成员[枚举](/language/enum "language/enum")以及[嵌套类](/language/nested_types "language/nested types")

```cpp
class M　{
  enum e {a, b, c, }
  class I { }
}
```

### 局部类

类声明可以在函数体内出现，此时它定义*局部类*。这种类的名字只存在于函数作用域中，且无法在函数外访问。

- 局部类不能拥有静态数据成员
- 局部类的成员函数无链接
- 局部类的成员函数必须完全在类体内定义
- 除[闭包类型](/language/lambda "language/lambda")以外的 局部类不能拥有成员模板
- 局部类不能拥有[友元模板](/language/friend#.E6.A8.A1.E6.9D.BF.E5.8F.8B.E5.85.83 "language/friend")
- 局部类不能在类定义内定义[友元函数](/language/friend "language/friend")
- 函数（包括成员函数）内的局部类的外围函数能访问的名字也可以被该局部类访问

### 参阅
