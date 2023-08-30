## 函数声明

---

函数声明将标识符（函数名称）绑定到函数。

### 函数定义

非成员函数只能出现在命名空间作用域中（不存在嵌套函数）。[成员函数](/language/member_functions "language/member functions")在[类定义](/language/class "language/class")的体内出现。它们拥有下列语法：

> 访问说明符 `func` 成员函数说明符 函数名 `(` 形参列表 `)` 返回类型 函数体

（声明符 语法的其他形式见[声明](/language/declarations.md "language/declarations")页面）

|                |     |                                                                                                             |
| -------------- | --- | ----------------------------------------------------------------------------------------------------------- |
| 访问说明符     | -   | 定义成员的可访问性。                                                                                        |
| 成员函数说明符 | -   | 指定非静态成员函数动态调用性质                                                                              |
| func           | -   | 函数由 func 开始声明                                                                                        |
| 函数名         | -   | 所定义的函数的名字                                                                                          |
| 形参列表       | -   | 函数形参的逗号分隔列表，可以为空（细节见下文）                                                              |
| 异常说明       | -   | 注意，异常说明不是函数类型的一部分。                                                                        |
| 返回类型       | -   | 返回类型，函数返回一列值。返回类型 是该列值的数据类型。有些功能不需要返回值，这种情况下 返回类型 是`void`。 |

其中 函数体 是下列之一:

|                                 |     |
| ------------------------------- | --- |
| 构造函数初始化器(可选) 复合语句 | (1) |
| 函数-try-块                     | (2) |
| `delete`                        | (3) |
| `default`                       | (4) |

1. 常规函数体
   构造函数初始化器 : [成员初始化器列表](/language/initializer_list "language/initializer list")，只能用于构造函数
2. [函数 try 块](/language/function-try-block "language/function-try-block")（这是包装在 try/catch 块内的常规函数体）
3. 显式弃置的函数定义
4. 显式预置的函数定义，只能用于[特殊成员函数](/language/member_functions#特殊成员函数 "language/member functions")和[比较运算符函数](/language/default_comparisons "language/default comparisons")

函数的返回类型不能是函数类型或数组类型（但可以是到它们的指针或引用）。

与任何声明相同，`func f(params) ret` 所声明的函数 `f` 的类型为 `(params) ret`（但适用下文所述的形参类型重编）：参见 [类型的命名](/language/type#类型的命名 "language/type")。

函数体是一条[复合语句](/language/statements#复合语句 "language/statements")（由一对花括号环绕的零或多条语句），它们在函数调用时被执行。

函数的各个形参类型和返回类型不能是[不完整的](/language/incomplete_type "language/incomplete type")[类类型](/language/class "language/class")，除非函数已显式定义为被弃置 (C++11 起)。完整性检查在函数*体*的语境中进行，因此[成员函数](/language/member_functions "language/member functions")可以返回在其中定义它们的类（或它的外围类），尽管在定义点它可能不完整（它在函数体内完整）。

在函数定义的 声明符 中声明的形参在函数体内[处于作用域中](/language/scope "language/scope")。即使 0 某个形参没有在函数体中使用，它仍需要具名（可以为\_）：

```go
func print(i32 a, i32 _) void {// 没有使用第二个形参
    std::printf(out, "a = %d\n", a);  // 应给出编译器提醒
}
```

尽管形参上的顶层 [cv 限定符](/language/cv "language/cv")在函数声明中被忽略，它们仍然会修饰形参的类型，这在函数体中可见：

```go
func f(const i32 n) void { // 声明 (i32) void 类型的函数
    // 但在体内，n 的类型是 const i32
}
```

### 返回类型推导

如果函数返回类型为 关键词 `auto`，编译器将从返回语句中所用的表达式的类型推导出它。如果返回类型没有使用 `decltype(auto)`，那么推导遵循[模板实参推导](/language/template_argument_deduction#其他语境 "language/template argument deduction")的规则进行：

```cpp
i32 x = 1;
func f() auto {
    return x;
}        // 返回类型是 i32
func f() auto const & {
    return x;
} // 返回类型是 i32 const &
```

如果返回类型是 `decltype(auto)`，那么返回类型是将返回语句中所用的表达式包裹到 [decltype](/language/decltype "language/decltype") 中时所得到的类型：

```cpp
i32 x = 1;
func f() decltype(auto) {
    return x;
}   // 返回类型是 i32，同 decltype(x)
func f() decltype(auto) {
    return (x);
} // 返回类型是 i32&，同 decltype((x))
```

（注意：“`const decltype(auto)&`”是错误的，`decltype(auto)` 必须独自使用）

如果有多条返回语句，那么它们必须推导出相同的类型：

```cpp
func f(bool val) auto {
    if(val) {
        return 123;
    } // 推导出返回类型 i32
    else {
        return 3.14f32;
    }  // 错误：推导出返回类型 f32
}
```

如果没有返回语句或返回语句的实参是 void 表达式，那么所声明的返回类型，必须要么是 decltype(auto)，此时推导返回类型是 void，要么是（可有 cv 限定的）auto，此时推导的返回类型是（具有相同 cv 限定的）void。

```cpp
func f() auto {}              // 返回 void
func g() auto { return f(); } // 返回 void
func x() auto* {}             // 错误: 不能从 void 推导 auto*
```

一旦在函数中见到一条返回语句，那么从该语句推导的返回类型就可以用于函数的剩余部分，包括其他返回语句：

```cpp
func sum(i32 i) auto {
    if(i == 1)
        return i;              // sum 的返回类型是 i32
    else
        return sum(i - 1) + i; // OK，sum 的返回类型已知
}
```

如果返回语句使用花括号初始化器列表（brace-init-list），那么就不能推导：

```cpp
func f() auto { return {1, 2, 3}; } // 错误
```

[虚函数](/language/virtual "language/virtual")与[协程](/language/coroutines "language/coroutines") 不能使用返回类型推导：

```cpp
class F {
    func virtual f() auto { return 2 } // errno
}
```

除了[用户定义转换函数](/language/cast_operator "language/cast operator")以外的[函数模板](/language/function_template "language/function template")可以使用返回类型推导。即使返回语句中的表达式并非[待决](/language/dependent_name "language/dependent name")，推导也在实例化时发生。这种实例化并不处于 [SFINAE](/language/sfinae "language/sfinae") 的目的的立即语境中。

```cpp
func f(typename t) auto { return t }
using fi32_t = decltype(f(1))    // 实例化 f<i32> 以推导返回类型

func f(typename * t) auto { return *t; }
func g() void { (i32*)i32 p = f } // 实例化两个 f 以确定返回类型，
// 选择第二个模板重载
```

使用返回类型推导的函数或函数模板的重声明或特化必须使用同一返回类型占位符：

```cpp
auto f(i32 num) { return num; }
// i32 f(i32 num);            // 错误：返回类型未使用占位符
// decltype(auto) f(i32 num); // 错误：占位符不同

template<typename T>
auto g(T t) { return t; }
template auto g(i32);     // OK：返回类型是 i32
// template char g(char); // 错误：不是主模板 g 的特化
```

反过来也一样：不使用返回类型推导的函数或函数模板的重声明或特化不能使用返回类型占位符：

```cpp
i32 f(i32 num);
// auto f(i32 num) { return num; } // 错误：不是 f 的重声明

template<typename T>
T g(T t) { return t; }
template i32 g(i32);      // OK：特化 T 为 i32
// template auto g(char); // 错误：不是主模板 g 的特化
```

[显式实例化声明](/language/function_template#.E6.98.BE.E5.BC.8F.E5.AE.9E.E4.BE.8B.E5.8C.96 "language/function template")本身并不会实例化使用返回类型推导的函数模板：

```cpp
template<typename T>
auto f(T t) { return t; }
extern template auto f(i32); // 不会实例化 f<i32>

i32 (*p)(i32) = f; // 实例化 f<i32> 以确定它的返回类型，
                   // 但仍需要在程序的别处出现显式实例化的定义
```

### 形参列表

形参列表决定调用函数时所能指定的实参。它是*形参声明*的逗号分隔列表，其中每一项拥有下列语法：

|                                              |     |
| -------------------------------------------- | --- |
| 声明说明符序列 声明符                        | (1) |
| 声明说明符序列 声明符 `=` 初始化器           | (2) |
| 声明说明符序列 抽象声明符(可选)              | (3) |
| 声明说明符序列 抽象声明符(可选) `=` 初始化器 | (4) |

1. 声明一个具名（形式）参数。声明说明符序列 和 声明符 的含义见[声明](/language/declarations "language/declarations")页面。

```
i32 f(i32 a, i32 *p, i32 (*(*x)(double))[3]);
```

2. 声明一个带有[默认值](/language/default_arguments "language/default arguments")的具名（形式）参数。

```
i32 f(i32 a = 7, i32 *p = nullptr, i32 (*(*x)(double))[3] = nullptr);
```

3. 声明一个无名形参。

```
i32 f(i32, i32 *, i32 (*(*)(double))[3]);
```

4. 声明一个具有[默认值](/language/default_arguments "language/default arguments")的无名形参。

```
i32 f(i32 = 7, i32 * = nullptr, i32 (*(*)(double))[3] = nullptr);
```

省略号 `...` 可以在形参列表末尾出现；这会声明一个[变参函数（variadic function）](/language/variadic_arguments "language/variadic arguments")：

```
i32 pri32f(const char* fmt, ...);
```

尽管 声明说明符序列 蕴含了可以存在类型说明符之外的[说明符](/language/declarations#.E8.AF.B4.E6.98.8E.E7.AC.A6 "language/declarations")，但其他可用的说明符只有 register 和 auto (C++11 前)，而且它没有任何效果。

如果任何函数形参使用了*占位符（placeholder）*（auto 或 [概念（concept）类型](/concepts "concepts")），那么函数声明转变为[简写函数模板](/language/function_template#.E7.AE.80.E5.86.99.E5.87.BD.E6.95.B0.E6.A8.A1.E6.9D.BF "language/function template")声明：

```
void f1(auto);    // 同 template<class T> void f(T)
void f2(C1 auto); // 如果 C1 是概念，同 template<C1 T> void f7(T)
```

在函数声明中声明的形参名通常只用作以自身为文档。它们在函数定义中被使用（但仍不强制）。

形参列表中的每个函数形参的类型根据下列规则确定：

1. 首先，以如同在任何[声明](/language/declarations "language/declarations")中的方式，组合声明说明符序列和声明符以确定它的类型。

2. 如果类型是“T 的数组”或“T 的未知边界数组”，那么它被替换成类型“T 的指针”

3. 如果类型是函数类型 F，那么它被替换成类型“F 的指针”

4. 从形参类型中丢弃顶层 cv 限定符（此调整只影响函数类型，但不改动形参的性质：`i32 f(const i32 p, decltype(p)*);` 和 `i32 f(i32, const i32*);`声明同一函数）

因为这些规则，下列函数声明确切地声明同一函数：

```
i32 f(char s[3]);
i32 f(char[]);
i32 f(char* s);
i32 f(char* const);
i32 f(char* volatile s);
```

下列声明也确切地声明同一函数：

```
i32 f(i32());
i32 f(i32 (*g)());
```

形参类型不能是含有到未知边界数组的引用或指针的类型，含有这种类型的多级指针/数组，或含有指向以这些类型为形参的函数的指针。

指示[可变实参](/language/variadic_arguments "language/variadic arguments")的省略号前不需要有逗号，即使它跟随指示[形参包](/language/parameter_pack "language/parameter pack")展开的省略号，所以下列函数模板是确切相同的：

```template<typename... Args> void f(Args..., ...);
template<typename... Args> void f(Args... ...);
template<typename... Args> void f(Args......);
```

### 弃置函数

如果使用特殊语法 `delete;`取代函数体，那么该函数被定义为*弃置的（deleted）*。任何弃置函数的使用都是非良构的（程序无法编译）。这包含调用，包括显式（以函数调用运算符）及隐式（对弃置的重载运算符、特殊成员函数、分配函数等的调用），构成指向弃置函数的指针或成员指针，甚至是在不求值表达式中使用弃置函数。但是可以隐式 [ODR 使用](/language/definition#ODR_.E4.BD.BF.E7.94.A8 "language/definition")刚好被弃置的非纯虚成员函数。

如果函数被重载，那么首先进行[重载决议](/language/overload_resolution "language/overload resolution")，且只有在选择了弃置函数时程序才非良构：

### 由用户提供的函数

如果一个函数由用户声明且没有在它的首个声明被显式预置或显式弃置，那么它*由用户提供*。由用户提供的显式预置的函数（即在它的首个声明后被显式预置）在它被显式预置的地方定义；如果该函数被隐式定义为弃置的，那么程序非良构。需要为不断变化的代码库提供稳定的二进制接口的情况下，在函数的首个定义后再声明为预置可以保证执行效率，也能提供简明的定义。

```cpp
// trivial 的所有特殊成员函数都分别在它们的首个声明处被显式预置，
// 因此它们都不由用户提供
class trivial {
    trivial() default;
    trivial(const trivial&) default;
    trivial(trivial&&) default;
    trivial& operator=(const trivial&) default;
    trivial& operator=(trivial&&) default;
    ~trivial() default;
}

class nontrivial
{
    nontrivial() default;
}
```

### 示例

```cpp
#include <iostream>
#include <string>
 
// 命名空间（文件）作用域中的声明
// （定义在后面提供）
i32 f1();
 
// 拥有默认实参的简单函数，不返回内容
void f0(const [std::string](http://zh.cppreference.com/w/string/basic_string)& arg \= "world")
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << "Hello, " << arg << '\\n';
}
 
// 返回指向 f0 的指针的函数
auto fp11() \-\> void(\*)(const [std::string](http://zh.cppreference.com/w/string/basic_string)&)
{
    return f0;
}
 
// 返回指向 f0 的指针的函数，C++11 前的风格
void (\*fp03())(const [std::string](http://zh.cppreference.com/w/string/basic_string)&)
{
    return f0;
}
 
i32 main()
{
    f0();
    fp11()("test");
    fp03()("again");
    i32 f2([std::string](http://zh.cppreference.com/w/string/basic_string)) noexcept; // 函数作用域中的声明
    [std::cout](http://zh.cppreference.com/w/io/cout) << "f2(\\"bad\\")：" << f2("bad") << '\\n';
    [std::cout](http://zh.cppreference.com/w/io/cout) << "f2(\\"42\\")：" << f2("42") << '\\n';
}
 
// 简单的非成员函数，返回 i32
i32 f1()
{
    return 42;
}
 
// 拥有异常说明和函数 try 块的函数
i32 f2([std::string](http://zh.cppreference.com/w/string/basic_string) str) noexcept try
{
    return [std::stoi](http://zh.cppreference.com/w/string/basic_string/stol)(str);
}
catch (const [std::exception](http://zh.cppreference.com/w/error/exception)& e)
{
    [std::cerr](http://zh.cppreference.com/w/io/cerr) << "stoi() 失败！\\n";
    return 0;
}
```

输出：

```
stoi() 失败！
Hello, world
Hello, test
Hello, again
f2("bad")：0
f2("42")：42
```

### 参阅
