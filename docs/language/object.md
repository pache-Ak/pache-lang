## 对象 

---

C++ 程序可以创建、销毁、引用、访问并操作*对象*。

在 C++ 中，一个对象拥有这些性质：

+   大小（可以使用 [`sizeof`](https://zh.cppreference.com/w/language/sizeof "language/sizeof") 获取）；
+   对齐要求（可以使用 [`alignof`](https://zh.cppreference.com/w/language/alignof "language/alignof") 获取）；
+   [存储期](https://zh.cppreference.com/w/language/storage_duration "language/storage duration")（自动、静态、动态、线程局部）；
+   [生存期](https://zh.cppreference.com/w/language/lifetime "language/lifetime")（与存储期绑定或者临时）
+   [类型](type.md "language/type")；
+   值（可能是不确定的，例如[默认初始化](https://zh.cppreference.com/w/language/default_initialization "language/default initialization")的非类类型）；
+   [名字](identifiers.md#名字 "language/identifiers")（可选）。

以下实体都不是对象：值，引用，函数，枚举项，类型，类的非静态成员，模板，类或函数模板的特化，命名空间，形参包，和 this。

*变量*由[声明](declarations.md "language/declarations")引入，是一个对象或不是对非静态数据成员的引用。

### 对象创建

对象能由[定义](definition "language/definition")、[new 表达式](new.md "language/new")、[throw 表达式](https://zh.cppreference.com/w/language/throw "language/throw")、更改[联合体](https://zh.cppreference.com/w/language/union "language/union")的活跃成员和求值要求[临时对象](https://zh.cppreference.com/w/language/lifetime#.E4.B8.B4.E6.97.B6.E5.AF.B9.E8.B1.A1.E7.94.9F.E5.AD.98.E6.9C.9F "language/lifetime")的表达式显式创建。显式对象创建中创建的对象是唯一定义的。

[隐式生存期类型](https://zh.cppreference.com/w/named_req/ImplicitLifetimeType "named req/ImplicitLifetimeType")的对象也可以由下列操作隐式创建：

+   开始 unsigned char 或 [std::byte](https://zh.cppreference.com/w/types/byte "types/byte") (C++17 起) 数组生存期的操作，此时在该数组中创建这种对象，
+   调用下列分配函数，此时在分配的存储中创建这种对象：

+   [operator new](https://zh.cppreference.com/w/memory/new/operator_new "memory/new/operator new")
+   [operator new\[\]](https://zh.cppreference.com/w/memory/new/operator_new "memory/new/operator new")
+   [std::calloc](https://zh.cppreference.com/w/memory/c/calloc "memory/c/calloc")
+   [std::malloc](https://zh.cppreference.com/w/memory/c/malloc "memory/c/malloc")
+   [std::realloc](https://zh.cppreference.com/w/memory/c/realloc "memory/c/realloc")

+   调用下列对象表示复制函数，此时在目标存储区域或结果中创建这种对象：

+   [std::memcpy](https://zh.cppreference.com/w/string/byte/memcpy "string/byte/memcpy")
+   [std::memmove](https://zh.cppreference.com/w/string/byte/memmove "string/byte/memmove")

只要在能给予程序有定义的行为时，同一存储区域中可创建零或多个对象。如果无法这样创建，例如操作冲突，那么程序行为未定义。如果多个这种隐式创建的对象的集合会给予程序有定义行为，那么不指定这些集合中的哪一个被创建。换言之，不要求隐式创建的对象是唯一定义的。

在指定的存储区域内隐式创建对象后，一些操作会生成指向*适合的已创建对象* ﻿的指针。适合的已创建对象与存储区域拥有相同地址。类似地，当且仅当不存在能给予程序有定义行为的指针值时，行为才未定义；而如果有多个给予程序有定义行为的值，那么不指定产生哪个值。
```cpp
#include <cstdlib>
 
struct X { int a, b; };
 
X\* MakeX()
{
    // 可能的有定义行为之一：
    // 调用 std::malloc 隐式创建一个 X 类型对象和它的子对象 a 与 b，并返回指向该 X 对象的指针
    X\* p \= static\_cast<X\*\>([std::malloc](http://zh.cppreference.com/w/memory/c/malloc)(sizeof(X)));
    p\-\>a \= 1;
    p\-\>b \= 2;
    return p;
}
```

调用 [std::allocator::allocate](https://zh.cppreference.com/w/memory/allocator/allocate "memory/allocator/allocate") 或[联合体](https://zh.cppreference.com/w/language/union "language/union")类型的隐式定义的复制/移动特殊成员函数也能创建对象。

### 对象表示与值表示

某些类型和对象具有*对象表示* ﻿和*值表示*，它们在下表中定义：

| 实体 | 对象表示 | 值表示 |
| --- | --- | --- |
| 完整对象类型 `T` | 一个 `T` 类型的非[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")完整对象占据的 N 个 `byte` 对象，其中 N 是 sizeof(T) | `T` 的对象表示中有参与表示 `T` 的值的所有位的集合 |
| `T` 类型的非位域完整对象 obj | obj 中对应 `T` 的对象表示的字节 | obj 中对应 `T` 的值表示的位 |
| 位域对象 bf | bf 占据的 N 个位的序列，其中 N 是该位域的宽度 | bf 的对象表示中有参与表示 bf 的值的所有位的集合 |

类型或对象的对象表示中不属于值表示的位是*填充位*。

对于[可平凡复制类型](https://zh.cppreference.com/w/named_req/TriviallyCopyable "named req/TriviallyCopyable")，它的值表示是对象表示的一部分，这意味着复制该对象在存储中所占据的字节就足以产生另一个具有相同值的对象（除非这个值是该类型的一个“陷阱表示”，将它读取到 CPU 中会产生一个硬件异常，就像浮点值的 SNaN（“Signaling NaN 发信非数”）或整数值的 NaT（“Not a Thing 非事物”）。

尽管大多数实现都不允许整数的陷阱表示、填充位或多重表示，也还存在例外；例如 Itanium 上的整数类型值[就可以是陷阱表示](https://web.archive.org/web/20170830125905/https://blogs.msdn.microsoft.com/oldnewthing/20040119-00/?p=41003)。

反过来不一定是对的：[可平凡复制类型](https://zh.cppreference.com/w/named_req/TriviallyCopyable "named req/TriviallyCopyable")的两个具有不同对象表示的对象可能表现出相同的值。例如，浮点数有多种位模式都表示相同的特殊值 [NaN](https://zh.cppreference.com/w/numeric/math/NAN "numeric/math/NAN") 。更常见的是，会为了满足[对齐要求](https://zh.cppreference.com/w/language/object#.E5.AF.B9.E9.BD.90 "language/object")，[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")的大小等得以满足而引入填充位。
```cpp
#include <cassert>
 
struct S
{
    char c;  // 1 字节值
             // 3 字节填充位（假设 alignof(float) == 4）
    float f; // 4 字节值    （假设 sizeof(float) == 4）
 
    bool operator\==(const S& arg) const
    { // 基于值的相等
        return c \== arg.c && f \== arg.f;
    }
};
 
void f()
{
    static\_assert(sizeof(S) \== 8);
    S s1 \= {'a', 3.14};
    S s2 \= s1;
    reinterpret\_cast<unsigned char\*\>(&s1)\[2\] \= 'b'; // 修改部分填充位
    [assert](http://zh.cppreference.com/w/error/assert)(s1 \== s2); // 值并未更改
}
```

对于 char，signed char，和 unsigned char 类型的对象，除非它们是大小过大的[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")，否则它的对象表示的每个位都参与它的值表示，而且每一种位模式都表示一个独立的值（没有填充位或陷阱位，不允许值的多种表示）。

### 子对象

一个对象可以拥有*子对象*。子对象包括：

+   成员对象
+   基类子对象
+   数组元素

不是其他任何对象的子对象的对象称为*完整对象*。

如果一个子对象是基类子对象或声明有 `[[[no_unique_address](https://zh.cppreference.com/w/language/attributes/no_unique_address "language/attributes/no unique address")]]` 属性的非静态数据成员 (C++20 起)，那么它*潜在重叠*。

完整对象、成员对象和数组元素也被称为*最终派生对象*，以便和基类子对象区分开。既非潜在重叠亦非[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")的对象的大小不能为零（基类子对象的大小可能为零，即使无 `[[no_unique_address]]` 也是如此 (C++20 起)：参见[空基类优化](https://zh.cppreference.com/w/language/ebo "language/ebo")）。

一个对象能含有其他对象，该情况下被含有的对象*内嵌于* ﻿前述对象。如果符合下列条件，那么对象 a 内嵌于另一对象 b：

+   a 是 b 的子对象，或
+   b 为 a [提供存储](https://zh.cppreference.com/w/language/lifetime#.E6.8F.90.E4.BE.9B.E5.AD.98.E5.82.A8 "language/lifetime")，或
+   存在对象 c，其中 a 内嵌于 c 而 c 内嵌于 b。

任何两个具有交叠的[生存期](https://zh.cppreference.com/w/language/lifetime "language/lifetime")的（非[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")）对象必然有不同的地址，除非其中一个对象内嵌于另一个对象，或者两个对象都是同一个完整对象中的不同类型的子对象，且其中一个是大小为零的子对象。

static const char c1 \= 'x';
static const char c2 \= 'x';
[assert](http://zh.cppreference.com/w/error/assert)(&c1 !\= &c2); // 值相同，地址不同

对于某个类，

+   它的非静态[数据成员](https://zh.cppreference.com/w/language/data_members "language/data members")，
+   它的非虚直接[基类](https://zh.cppreference.com/w/language/derived_class "language/derived class")，以及
+   它不是[抽象类](https://zh.cppreference.com/w/language/abstract_class "language/abstract class")时，它的[虚基类](https://zh.cppreference.com/w/language/derived_class#.E8.99.9A.E5.9F.BA.E7.B1.BB "language/derived class")

被统称为该类的*潜在构造的子对象*。

### 多态对象

声明或继承了至少一个虚函数的类类型的对象是多态对象。每个多态对象中，实现都会储存额外的信息（在所有现存的实现中，如果没被编译器优化掉的话，这就是一个指针），它被用于进行[虚函数](https://zh.cppreference.com/w/language/virtual "language/virtual")的调用，RTTI 功能特性（[`dynamic_cast`](https://zh.cppreference.com/w/language/dynamic_cast "language/dynamic cast") 和 [`typeid`](https://zh.cppreference.com/w/language/typeid "language/typeid")）也用它在运行时确定对象创建时所用的类型，而不管使用它的表达式是什么类型。

对于非多态对象，值的解释方式由使用对象的表达式所确定，这在编译期就已经决定了。

```cpp
#include <iostream>
#include <typeinfo>
 
struct Base1
{
    // 多态类型：声明了虚成员
    virtual ~Base1() {}
};
struct Derived1 : Base1
{
    // 多态类型：继承了虚成员
};
 
struct Base2
{
    // 非多态类型
};
struct Derived2 : Base2
{
    // 非多态类型
};
 
int main()
{
    Derived1 obj1; // object1 创建为类型 Derived1
    Derived2 obj2; // object2 创建为类型 Derived2
 
    Base1& b1 \= obj1; // b1 指代对象 obj1
    Base2& b2 \= obj2; // b2 指代对象 obj2
 
    [std::cout](http://zh.cppreference.com/w/io/cout) << "b1 的表达式类型：" << typeid(decltype(b1)).name() << '\\n'
              << "b2 的表达式类型：" << typeid(decltype(b2)).name() << '\\n'
              << "b1 的对象类型：" << typeid(b1).name() << '\\n'
              << "b2 的对象类型：" << typeid(b2).name() << '\\n'
              << "b1 的大小：" << sizeof b1 << '\\n'
              << "b2 的大小：" << sizeof b2 << '\\n';
}
```

可能的输出：
```
b1 的表达式类型：Base1
b2 的表达式类型：Base2
b1 的对象类型：Derived1
b2 的对象类型：Base2
b1 的大小：8
b2 的大小：1
```

### 严格的别名使用

在很多情况下，通过类型与对象的创建类型不同的表达式来访问对象都是未定义行为，它的例子和例外请参考 [reinterpret\_cast](https://zh.cppreference.com/w/language/reinterpret_cast#.E7.B1.BB.E5.9E.8B.E5.88.AB.E5.90.8D.E4.BD.BF.E7.94.A8 "language/reinterpret cast")。

### 对齐

每个[对象类型](https://zh.cppreference.com/w/language/type "language/type")都具有被称为*对齐要求* ﻿的性质，它是一个整数（类型是 [std::size\_t](https://zh.cppreference.com/w/types/size_t "types/size t")，总是 2 的幂），表示这个类型的不同对象所能分配放置的连续相邻地址之间的字节数。

每个对象类型在该类型的所有对象上强制该类型的对齐要求；可以使用 [`alignas`](https://zh.cppreference.com/w/language/alignas "language/alignas") 来要求更严格的对齐（更大的对齐要求） (C++11 起)。尝试在没有满足某个对象类型的对齐要求的存储中创建该类型的对象是未定义行为。

为了使[类](https://zh.cppreference.com/w/language/class "language/class")中的所有非静态成员都符合对齐要求，会在一些成员后面插入一些[填充位](#.E5.AF.B9.E8.B1.A1.E8.A1.A8.E7.A4.BA.E4.B8.8E.E5.80.BC.E8.A1.A8.E7.A4.BA)。

#include <iostream>
 
// S 类型的对象可以在任何地址上分配
// 因为 S.a 和 S.b 都可以在任何地址上分配
struct S
{
    char a; // 大小：1，对齐：1
    char b; // 大小：1，对齐：1
}; // 大小：2，对齐：1
 
// X 类型的对象只能在 4 字节边界上分配
// 因为 X.n 必须在 4 字节边界上分配
// 因为 int 的对齐要求（通常）就是 4
struct X
{
    int n;  // 大小：4，对齐：4
    char c; // 大小：1，对齐：1
    // 三个字节的填充位
}; // 大小：8，对齐：4
 
int main()
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << "alignof(S) = " << alignof(S) << '\\n'
              << "sizeof(S)  = " << sizeof(S) << '\\n'
              << "alignof(X) = " << alignof(X) << '\\n'
              << "sizeof(X)  = " << sizeof(X) << '\\n';
}

可能的输出：

alignof(S) = 1
sizeof(S)  = 2
alignof(X) = 4
sizeof(X)  = 8

最弱的对齐（最小的对齐要求）是 char、signed char 和 unsigned char 的对齐，等于 1；所有类型中最大的*基础对齐* ﻿由实现定义，并等于 [std::max\_align\_t](https://zh.cppreference.com/w/types/max_align_t "types/max align t") 的对齐 (C++11 起)。

基础对齐对于所有类型的存储期的对象都得到支持。

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/object&action=edit&section=7 "编辑章节：注解")\] 注解

C++ 中的对象和[面向对象编程（OOP）](https://en.wikipedia.org/wiki/Object-oriented_programming "enwiki:Object-oriented programming")中的对象的含义不同：

| C++ 中的对象 | OOP 中的对象 |
| --- | --- |
| 可以具有任何对象类型  
（见 [std::is\_object](https://zh.cppreference.com/w/types/is_object "types/is object")） | 必须具有类类型 |
| 没有“实例”的概念 |  有“实例”的概念（并且有类似 `instanceof` 这样可以检测“是...的实例”关系的机制）  |
| 没有“接口”的概念 | 有“接口”的概念（并且有类似 `instanceof` 这样可以检测是否有实现接口的机制） |
|  需要通过虚成员显式启用多态  | 始终具有多态 |

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/object&action=edit&section=8 "编辑章节：缺陷报告")\] 缺陷报告

下列更改行为的缺陷报告追溯地应用于以前出版的 C++ 标准。

| 缺陷报告 | 应用于 | 出版时的行为 | 正确行为 |
| --- | --- | --- | --- |
| [CWG 633](https://cplusplus.github.io/CWG/issues/633.html) | C++98 | 变量只能是对象 | 也可以是引用 |
| [CWG 734](https://cplusplus.github.io/CWG/issues/734.html) | C++98 | 未指明在同一作用域内定义的多个变量  
在保证值相同时是否可以具有相同的地址 | 在它们的生存期交叠时地址  
保证不同，与值是否相同无关 |
| [CWG 1189](https://cplusplus.github.io/CWG/issues/1189.html) | C++98 | 两个类型相同的基类子对象可以有相同的地址 | 它们的地址不会相同 |
| [CWG 1861](https://cplusplus.github.io/CWG/issues/1861.html) | C++98 | 大小过大的窄字符类型的位域的对象表示的每个位依然都参与它的值表示 | 允许存在填充位 |
| [CWG 2489](https://cplusplus.github.io/CWG/issues/2489.html) | C++98 | char\[\] 不能提供存储，但是可以在它的存储中隐式创建对象 | 不能在 char\[\] 的存储中隐式创建对象 |
| [CWG 2519](https://cplusplus.github.io/CWG/issues/2519.html) | C++98 | 对象表示的定义没有考虑位域 | 考虑位域 |
| [CWG 2719](https://cplusplus.github.io/CWG/issues/2719.html) | C++98 | 在没有正确对齐的存储中创建对象的行为不明确 | 此时行为未定义 |
| [P0593R6](https://wg21.link/P0593R6) | C++98 | 先前的对象模型不支持标准库所要求的许多  
有用的手法，并且与 C 中的有效类型不兼容 | 添加了隐式对象创建 |

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/object&action=edit&section=9 "编辑章节：参考")\] 参考