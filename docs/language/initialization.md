## 初始化

----

变量的*初始化*会在构造时提供变量的初始值。

初始值可以由[声明符](/language/declarations.md "language/declarations")或 [new 表达式](/language/new.md "language/new")的初始化器部分提供。在函数调用时也会发生：函数形参及函数返回值也会被初始化。

对于每个声明符，初始化器必须是下列之一：

- ~~( 表达式列表 ) (1) ~~
- ~~= 表达式 (2)~~
- { 初始化器列表 } (3)

1. ~~括号中的以逗号分隔的含有任意表达式和花括号初始化器列表的列表~~ 因统一初始化废弃
2. ~~等号后面跟着一个表达式~~
3. 花括号初始化器列表：以逗号分隔且可以为空的含有表达式和其他花括号初始化器列表的列表

TODO : 2. 3. 中只保留一个
    类型没有对应字面量， 2. 依赖于转换和其他初始化语法
    `{}` 符号需要讨论， `<>` 不利于解析需要区分大于小于号， `[]` 常用于数组 `()` 使用广泛， 试图限制于函数
    初始化器列表 只能为结构值或返回此（动态）类型的工厂函数或表达式

根据上下文，初始化器可以调用：

- [值初始化](/language/value_initialization "language/value initialization")，例如 [`std::string`](/string/basic_string)`s{};`
- [直接初始化](/language/direct_initialization "language/direct initialization")，例如 ~~[`std::string`](/string/basic_string)`s("hello")`~~;
- ~~[复制初始化](/language/copy_initialization "language/copy initialization")，例如 [`std::string`](/string/basic_string)`s = "hello";`~~
- [列表初始化](/language/list_initialization "language/list initialization")，例如 [`std::string`](/string/basic_string)`s{'a', 'b', 'c'};`
- [聚合初始化](/language/aggregate_initialization "language/aggregate initialization")，例如`char a[3] = {'a', 'b'};`
- [引用初始化](/language/reference_initialization "language/reference initialization")，例如`char& c = a[0];`

如果不提供初始化器，那么就会应用[默认初始化](/language/default_initialization "language/default initialization")的规则， 并给出编译器提醒。

### 非局部变量

所有具有静态[存储期](/language/storage_duration "language/storage duration")
的非局部变量的初始化会作为程序启动的一部分在
[main 函数](/language/main_function "language/main function")的执行之前
进行（除非被延迟，见下文）。

所有具有线程局部存储期的非局部变量的初始化会作为线程启动的一部分进行，
按顺序早于线程函数的执行开始。对于这两种变量，初始化发生于两个截然不同的阶段：

#### 静态初始化

有两种静态初始化的形式：

1) 如果可能，那么应用[常量初始化](/language/constant_initialization "language/constant initialization")。

2) 否则非局部静态及线程局域变量会被[零初始化](/language/zero_initialization "language/zero initialization")。

实践中：

- 常量初始化通常在编译期进行。预先被计算的对象表示会作为程序映像的一部分存储下来。如果编译器没有这样做，那么它仍然必须保证该初始化发生早于任何动态初始化。
- 零初始化的变量将被置于程序映像的 `.bss` 段，它不占据磁盘空间，并在加载程序时由操作系统以零填充。

#### 动态初始化

在所有静态初始化完成后，在下列情形中进行非局部变量的动态初始化：

1) *无序的动态初始化*，仅适用于未被[显式特化](/language/template_specialization "language/template specialization")的（静态/线程局域）类模板的[静态数据成员](/language/static "language/static")及[变量模板](/language/variable_template "language/variable template") (C++14 起)。这些静态变量的初始化相对于所有其他动态初始化之间是顺序不确定的，除非程序在初始化某个变量之前开始了一个线程，此时初始化则是无顺序的。这些线程局域变量的初始化相对于所有其他动态初始化之间是无顺序的。

2) *部分有序的动态初始化*，适用于并未被隐式或显式实例化的特化的所有 inline 变量。如果一个部分有序的 V 在每个翻译单元中比有序或部分有序的 W 更早定义，那么 V 的初始化按顺序早于（或若程序启动了线程，则为先发生于）W 的初始化。

3) *有序的动态初始化*，适用于所有其他非局部变量：在单个翻译单元中，这些变量的初始化始终严格以其定义出现于源代码中的顺序[定序](/language/eval_order "language/eval order")。不同翻译单元中的静态变量的初始化之间是顺序不确定的。不同翻译单元中的线程局域变量的初始化之间是无顺序的。

当拥有静态或线程存储期的非局部变量的初始化通过异常退出时，调用 [std::terminate](/error/terminate "error/terminate")。

#### 提早动态初始化

在下列条件都满足的情况下，允许编译器将初始化动态初始化的变量作为静态初始化（实为编译期）的一部分：

1) 初始化的动态版本不改变命名空间作用域中任何先于其初始化的对象的值

2) 初始化的静态版本在被初始化变量中产生的值，与当所有不要求静态初始化的变量都被动态初始化时，由动态初始化所生成的值相同。

因为上述规则，如果某对象 `o1` 的初始化涉及到命名空间作用域对象 `o2`，而它潜在地要求动态初始化，但在同一翻译单元中在其之后定义，那么不指明所用的 `o2` 是完全初始化的 `o2` 的值（因为编译器把 `o2` 的初始化提升到编译时）还是 `o2` 仅被零初始化的值。

```
fun fd() f64 { return 1.0; }
 
extern f64 d1;
 
f64 d2 = d1;   // 未指明：
                  // 如果 d1 被动态初始化则动态初始化为 0.0，或
                  // 如果 d1 被静态初始化则动态初始化为 1.0，或
                  // 静态初始化为 0.0（因为当两个变量都被动态初始化时将为这个值）
 
f64 d1 = fd(); // 可能静态或动态初始化为 1.0
```

#### 延迟动态初始化

动态初始化是发生早于（对于静态变量）主函数或（对于线程局域变量）其线程的首个函数的首条语句，还是延迟到发生晚于它们，是由实现定义的。

如果非内联变量 的初始化延迟到发生晚于主/线程函数的首条语句，那么它发生早于与所初始化的变量定义于同一翻译单元中的任何拥有静态/线程存储期的变量的首次 [ODR 使用](/language/definition#ODR_.E4.BD.BF.E7.94.A8 "language/definition")。若给定翻译单元中没有 ODR 使用变量或函数，则定义于该翻译单元的非局部变量可能始终不被初始化（这模仿按需的动态库的行为）。然而，只要翻译单元中 ODR 使用了任何事物，就会初始化所有在初始化或销毁中拥有副作用的非局部变量，即使程序中没有用到它们。

如果内联变量的初始化被延迟，那么它早于这个特定变量的首次[ODR 使用](/language/definition.md#ORD "language/definition")发生。

```
// ============
// == 文件 1 ==
 
#include "a.h"
#include "b.h"
 
B b;
A::A() { b.Use(); }
 
// ============
// == 文件 2 ==
 
#include "a.h"
 
A a;
 
// ============
// == 文件 3 ==
 
#include "a.h"
#include "b.h"
 
extern A a;
extern B b;
 
int main()
{
    a.Use();
    b.Use();
}
 
// 如果 a 在进入 main 之前被初始化，那么 b 可能在 A::A() 使用它的时间点仍未被初始化
// （因为动态初始化在翻译单元间是顺序不确定的）
 
// 如果 a 在某个 main 的首条语句之后的时间点初始化
// （它 ODR 使用了定义于文件 1 的函数，强制其初始化得以运行），
// 那么 b 将在 A::A 使用它前初始化
```

### 静态局部变量

有关局部（即块作用域）的静态和线程局部变量，见[静态局部变量](/language/storage_duration#.E9.9D.99.E6.80.81.E5.B1.80.E9.83.A8.E5.8F.98.E9.87.8F "language/storage duration")。

~~拥有[外部或内部链接](/language/storage_duration#.E8.BF.9E.E6.8E.A5 "language/storage duration")的变量的块作用域声明中不允许初始化器。这种声明必须带 extern 出现而且不能为定义。~~

### 类成员

非静态数据成员可以[成员初始化器列表](/language/initializer_list "language/initializer list")或以[默认成员初始化器](/language/data_members#.E6.88.90.E5.91.98.E5.88.9D.E5.A7.8B.E5.8C.96 "language/data members")初始化。

### 注解

非局部变量的销毁顺序在 [std::exit](/utility/program/exit "utility/program/exit") 中描述。

### 参阅

- [复制消除](/language/copy_elision "language/copy elision")
- [转换构造函数](/language/converting_constructor "language/converting constructor")
- [复制构造函数](/language/copy_constructor "language/copy constructor")
- [默认构造函数](/language/default_constructor "language/default constructor")
- [`explicit`](/language/explicit "language/explicit")
- [移动构造函数](/language/move_constructor "language/move constructor")
- [new](/language/new "language/new")
