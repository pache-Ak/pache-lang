## 作用域

---

C++ 程序中出现的每个[声明](declarations.md "language/declarations")，只在某些可能不连续的*作用域*中有效。

在作用域内，能用[无限定名字查找](lookup "language/lookup")将名字与它的声明关联起来。

### 通用说明

每个程序都有一个*全局作用域*，它*包含* ﻿了整个程序。

其他每个作用域 `S` 会以以下之一引入：

+   一个[声明](declarations.md "language/declarations")。
+   [形参列表](function.md#形参列表 "language/function")中的一个形参。
+   一条[语句](statements.md "language/statements")。
+   一个[异常处理块](try_catch "language/try catch")。

`S` 会始终在其他作用域中出现，这些作用域从而*包含* `S`。

程序某一点的*外围作用域*是任何包含该点的作用域；该点的*立即作用域* ﻿是所有外围作用域中最小的作用域。

给定程序点 `P` 和作用域 `S`，如果一个作用域是 `S` 本身或包含 `S`，并且不包含 `P`，那么该作用域*介入* `P` 和 `S`。

对于[模板形参作用域|模板形参作用域](https://zh.cppreference.com/w/)以外的任何作用域 `S`，它的*父作用域* ﻿是包含作用域 `S` 且不是模板形参作用域的最小作用域。

除非另外说明：

+   声明在它的[声明点](#.E5.A3.B0.E6.98.8E.E7.82.B9)*居于* ﻿该点的立即作用域中。
+   声明的*目标作用域* ﻿是该声明居于的作用域。
+   由声明（重新）引入的名字在目标作用域中*绑定* ﻿到该声明。

如果一个实体的声明的目标作用域是 `S`，那么该实体*属于* `S`。
```cpp
//                全局     作用域 作用域
//                作用域     S      T
int x;         //   ─┐                 // 程序点 X
               //    │
{              //    │      ─┐
    {          //    │       │     ─┐
        int y; //    │       │      │  // 程序点 Y
    }          //    │       │     ─┘
}              //   ─┘      ─┘
```

在以上程序中：

+   全局作用域、作用域 `S` 和作用域 `T` 都包含程序点 `Y`。

+   也就是说，在程序点 `Y` 这三个作用域都是该点的外围作用域。

+   全局作用域包含作用域 `S` 和作用域 `T`，并且作用域 `S` 包含作用域 `T`。

+   因此，作用域 `T` 在三者中最小，这意味着：

+   在程序点 `Y` 的立即作用域是作用域 `T`。
+   变量 y 的声明在它的声明点居于作用域 `T`。
+   作用域 `T` 是 y 的声明的目标作用域。
+   变量 y 属于作用域 `T`。

+   作用域 `S` 是作用域 `T` 的父作用域，而全局作用域是作用域 `S` 的父作用域。

+   作用域 `S` 介入程序点 `X` 和作用域 `T`。

### 块作用域

每个

+   [选择语句](statements.md#选择语句 "language/statements")（[if](if "language/if")、[switch](switch "language/switch")），
+   [循环语句](statements.md#循环语句 "language/statements")~~（[for](for "language/for")、~~[范围 for](range-for "language/range-for")、[while](while "language/while")、[do\-while](do "language/do")），
+   [异常处理块](try_catch "language/try catch")，或者
+   异常处理块中的复合语句以外的其他[复合语句](statements#.E5.A4.8D.E5.90.88.E8.AF.AD.E5.8F.A5 "language/statements")

都会引入一个包含该语句或处理块的*块作用域*。

属于块作用域的变量是*块变量*。

```cpp
int i \= 42;
int a\[10\];
 
for (int i \= 0; i < 10; i++) // 内部 “i” 居于 for 语句引入的块作用域
    a\[i\] \= i;
 
int j \= i; // j = 42
```

如果一个不[与名字无关](conflicting_declarations#.E6.9C.89.E6.BD.9C.E5.9C.A8.E5.86.B2.E7.AA.81.E7.9A.84.E5.A3.B0.E6.98.8E "language/conflicting declarations")的 (C++26 起)声明的目标作用域是由以下复合语句或处理块之一引入的某个块作用域 `S`，并且该声明与目标作用域是 `S` 的父作用域的另一声明[有潜在冲突](conflicting_declarations#.E6.9C.89.E6.BD.9C.E5.9C.A8.E5.86.B2.E7.AA.81.E7.9A.84.E5.A3.B0.E6.98.8E "language/conflicting declarations")，那么程序非良构：

+   [函数体](function#.E5.87.BD.E6.95.B0.E5.AE.9A.E4.B9.89 "language/function")或[函数 try 块](function-try-block "language/function-try-block")的复合语句。

+   自身不是选择语句或循环语句，但是是选择语句或循环语句的子语句的复合语句。
+   函数 try 块的处理块。

if (int x \= f()) // 声明了 “x”
{ // 此 if 块是 if 语句的子语句
    int x;       // 错误：重声明了 “x”
}
else
{ // 此 else 块也是 if 语句的子语句
    int x;       // 错误：重声明了 “x”
}

### 函数形参作用域

每个[形参声明](function#.E5.BD.A2.E5.8F.82.E5.88.97.E8.A1.A8 "language/function") `P` 都会引入一个包含 `P` 的*函数形参作用域*。

+   如果声明的形参所在的形参列表是[函数声明](function "language/function")的形参列表：

+   如果该函数声明是[函数定义](function#.E5.87.BD.E6.95.B0.E5.AE.9A.E4.B9.89 "language/function")，那么引入的作用域会延长到该函数定义的末尾。
+   否则（该函数声明是函数原型），引入的作用域会延长到该函数声明的声明符的末尾。
+   不管属于哪种情况，引入的作用域都不会包含该函数声明的[声明点](#.E5.A3.B0.E6.98.8E.E7.82.B9)。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><ul><li>如果声明的形参所在的形参列表是 <a href="lambda" title="language/lambda">lambda 表达式</a>的形参列表，那么引入的作用域会延长到 <code><b>{</b></code> <span class="t-spar">函数体</span> <code><b>}</b></code> 的末尾。</li></ul></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr><tr class="t-rev t-since-cxx17"><td><ul><li>如果声明的形参所在的形参列表是<a href="deduction_guide" title="language/deduction guide" class="mw-redirect">推导指引</a>的形参列表，那么引入的作用域会延长到该推导指引的末尾。</li></ul></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr><tr class="t-rev t-since-cxx20"><td><ul><li>如果声明的形参所在的形参列表是 <a href="requires" title="language/requires"><span class="mw-geshi cpp source-cpp">requires</span> 表达式</a>的形参列表，那么引入的作用域会延长到 <code><b>{</b></code> <span class="t-spar">要求序列</span> <code><b>}</b></code> 的末尾。</li></ul></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

int f(int n)  // 形参 “n” 的声明引入了一个函数形参作用域
{
    /\* ... \*/
}             // 引入的函数形参作用域在此结束

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx14"><td><h3><span class="mw-headline" id="lambda_.E4.BD.9C.E7.94.A8.E5.9F.9F">lambda 作用域</span></h3><p>每个 <a href="lambda" title="language/lambda">lambda 表达式</a>都会引入一个 <i>lambda 作用域</i>，它在 <code><b>[</b></code><span class="t-spar">捕获</span> ﻿<code><b>]</b></code> 之后立即开始，并延长到 <code><b>{</b></code> <span class="t-spar">函数体</span> <code><b>}</b></code> 的末尾。</p><p>某个 lambda 表达式 <span class="t-c"><span class="mw-geshi cpp source-cpp">E</span></span> 中带有初始化器的<a href="lambda#.E6.8D.95.E8.8E.B7" title="language/lambda">捕获</a>居于由 <span class="t-c"><span class="mw-geshi cpp source-cpp">E</span></span> 引入的 lambda 作用域。</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw4">auto</span> lambda <span class="sy1">=</span> <span class="br0">[</span>x <span class="sy1">=</span> <span class="nu0">1</span>, y<span class="br0">]</span><span class="br0">(</span><span class="br0">)</span> <span class="co1">// 此 lambda 表达式引入了一个 lambda 作用域，</span>
<span class="br0">{</span>                          <span class="co1">// 它是捕获 “x” 的目标作用域</span>
    <span class="coMULTI">/* ... */</span>
<span class="br0">}</span><span class="sy4">;</span>                         <span class="co1">// 引入的 lambda 作用域在分号前结束</span></pre></div></td><td><span class="t-mark-rev t-since-cxx14">(C++14 起)</span></td></tr></tbody></table>

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/scope&action=edit&section=5 "编辑章节：命名空间作用域")\] 命名空间作用域

命名空间 `N` 的每个 [命名空间定义](namespace "language/namespace")都会引入一个*命名空间作用域* `S`，它包含 `N` 的所有命名空间定义中的声明序列。

对于目标作用域是 `S` 或者被 `S` 包含的另一作用域的非友元重声明或特化，作用域 `S` 也包含以下部分：

+   [类](class "language/class")（模板）重声明或类模板特化中类头名 ﻿之后的部分。
+   [枚举](enum "language/enum")重声明中枚举头名 ﻿之后的部分。
+   其他重声明或中特化中[声明符](declarations#.E5.A3.B0.E6.98.8E.E7.AC.A6 "language/declarations")的无限定标识 ﻿或有限定标识 ﻿之后的部分。

[全局作用域](#.E9.80.9A.E7.94.A8.E8.AF.B4.E6.98.8E)是[全局命名空间](namespace "language/namespace")引入的命名空间作用域。

namespace V   // “V” 的命名空间定义引入了一个命名空间作用域 “S”
{
    // 作用域 “S” 的第一部分在此开始
    void f();
    // 作用域 “S” 的第一部分在此结束
}
 
void V::f()   // “f” 之后的部分也是作用域 “S” 的一部分
{
    void h(); // 声明了 V::h
}             // 作用域 “S” 的第二部分在此结束

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/scope&action=edit&section=6 "编辑章节：类作用域")\] 类作用域

类或类模板 `C` 的每个声明都会引入一个*类作用域* `S`，它包含 `C` 的所有[类定义](class "language/class")中的成员说明。

对于目标作用域是 `S` 或者被 `S` 包含的另一作用域的非友元重声明或特化，作用域 `S` 也包含以下部分：

+   [类](class "language/class")（模板）重声明或类模板特化中类头名 ﻿之后的部分。
+   [枚举](enum "language/enum")重声明中枚举头名 ﻿之后的部分。
+   其他重声明或中特化中[声明符](declarations#.E5.A3.B0.E6.98.8E.E7.AC.A6 "language/declarations")的无限定标识 ﻿或有限定标识 ﻿之后的部分。

class C       // “C” 的类定义引入了一个类作用域 “S”
{
    // 作用域 “S” 的第一部分在此开始
    void f();
    // 作用域 “S” 的第一部分在此结束
}
 
void C::f()   // “f” 之后的部分也是作用域 “S” 的一部分
{
    /\* ... \*/
}             // 作用域 “S” 的第二部分在此结束

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/scope&action=edit&section=7 "编辑章节：枚举作用域")\] 枚举作用域

枚举 `E` 的每个声明都会引入一个*枚举作用域*，它包含 `E` 的非不可见 (C++11 起)[声明](enum "language/enum")中的枚举项列表（如果存在）。

enum class E // “E” 的枚举定义引入了一个枚举作用域 “S”
{
    // 作用域 “S” 在此开始
    e1, e2, e3
    // 作用域 “S” 在此结束
}

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/scope&action=edit&section=8 "编辑章节：模板形参作用域")\] 模板形参作用域

每个[模板模板形参](template_parameters#.E6.A8.A1.E6.9D.BF.E6.A8.A1.E6.9D.BF.E5.BD.A2.E5.8F.82 "language/template parameters")都会引入一个*模板形参作用域*，它包含该模板模板形参的整个模板形参列表以及[require 子句](constraints#requires_.E5.AD.90.E5.8F.A5 "language/constraints") (C++20 起)。

每个模板声明 `D` 也都会引入一个*模板形参作用域* `S`，它从 `D` 的模板形参列表开始，到 `D` 的末尾结束。在该模板形参列表之外且按之前的语法居于 `S` 的声明会改为居于 `D` 居于的作用域。

// “X” 的类模板声明引入了一个模板形参作用域 “S1”
template
<
    // 作用域 “S1” 在此开始
    template // 模板模板形参 “T” 引入了另一个模板形参作用域 “S2”
    <
        typename T1
        typename T2
    \> requires std::convertible\_from<T1, T2\> // 作用域 “S2” 在此结束
    class T,
    typename U
\>
class X; // 作用域 “S1” 在分号前结束

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=language/scope&action=edit&section=9 "编辑章节：声明点")\] 声明点

作用域从*声明点* ﻿开始，它定位如下：

对于简单声明引入的变量和其他名字，声明点紧随该名字的[声明符](declarations#.E5.A3.B0.E6.98.8E.E7.AC.A6 "language/declarations")之后，且在它的初始化器之前（如果存在）：

int x \= 32; // 外部 x 的作用域中
 
{
    int x \= x; // 内部 x 的作用域在初始化器（= x）前就已经开始
               // 这导致内部 x 不以外部 x 的值（32）初始化，
               // 而是以自己持有的（不确定）值初始化
}
 
[std::function](http://zh.cppreference.com/w/utility/functional/function)<int(int)\> f \= \[&\](int n){ return n \> 1 ? n \* f(n \- 1) : n; };
// lambda 函数体在函数对象 f 的名字的作用域中
// 因此 f 能正确地被按引用捕获，给出递归函数

const int x \= 2; // 外部 x 的作用域中
 
{
    int x\[x\] \= {}; // 内部 x 的作用域在初始化器（= {}）前，但在声明符（x\[x\]）后开始。
                   // 声明符内仍在外部 x 的作用域中，这声明了一个含有 2 个 int 元素的数组。
}

类或类模板的声明点紧随它的[类头](classes "language/classes")中出现的命名该类的标识符（或命名该模板特化的 [模板标识](templates#template-id "language/templates")）之后。也就是说基类列表已经在该类名或类模板名的作用域中。

[enum 说明符](enum "language/enum")或不可见枚举声明 (C++11 起)的声明点紧随命名它的标识符之后：

enum E : int // E 的作用域从冒号开始
{
    A \= sizeof(E)
};

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><p><a href="type_alias" title="language/type alias">类型别名或别名模板</a>的声明点紧随该别名代表的类型标识之后：</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw1">using</span> T <span class="sy1">=</span> <span class="kw4">int</span><span class="sy4">;</span> <span class="co1">// 外部 T 的作用域从分号开始</span>
&nbsp;
<span class="br0">{</span>
    <span class="kw1">using</span> T <span class="sy1">=</span> T<span class="sy2">*</span><span class="sy4">;</span> <span class="co1">// 内部 T 的作用域从分号开始，</span>
                  <span class="co1">// 但分号前还在外部 T 的作用域中，</span>
                  <span class="co1">// 因此等同于 T = int*</span>
<span class="br0">}</span></pre></div></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

不指名构造函数的 [using 声明](using_declaration "language/using declaration")内的声明符的声明点紧随该声明符之后：

template<int N\>
class Base
{
protected:
    static const int next \= N + 1;
    static const int value \= N;
};
 
struct Derived: Base<0\>, Base<1\>, Base<2\>
{
    using Base<0\>::next,     // next 的作用域从逗号开始
          Base<next\>::value; // Derived::value 是 1
};

枚举项的声明点紧随它的定义之后（而不是在初始化器之前，这点与变量不同）：

const int x \= 12;
 
{
    enum
    {
        x \= x + 1, // 枚举项 x 的作用域从逗号开始，
                   // 但逗号前还在外部 x 的作用域中，
                   // x 初始化为 13
        y \= x + 1  // y 初始化为 14
    };
}

[注入类名](injected-class-name "language/injected-class-name")的声明点紧随它的类定义或类模板定义的左花括号之后：

template<typename T\>
struct Array
//  : std::enable\_shared\_from\_this<Array> // 错误：不在注入类名的作用域中
    : [std::enable\_shared\_from\_this](http://zh.cppreference.com/w/memory/enable_shared_from_this)< Array<T\> \> // OK ：在模板名 Array 的作用域中
{ // 现在在注入类名 Array 的作用域中，如同它是公开成员名
    Array\* p; // 指向 Array<T> 的指针
};

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><p>函数局部预定义变量 <span class="t-c"><span class="mw-geshi cpp source-cpp">__func__</span></span> 的声明点紧接函数定义的函数体之前。</p></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx17"><td><p><a href="structured_binding" title="language/structured binding">结构化绑定</a>的声明点紧随该结构化绑定声明的<span class="t-spar">标识符列表</span>之后，但结构化绑定的初始化器禁止提及其所引入的任何名字。</p></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><p>声明于<a href="range-for" title="language/range-for">范围 <span class="mw-geshi cpp source-cpp"><span class="kw1">for</span></span> 语句</a>的<span class="t-spar">范围声明</span> ﻿的变量<span class="t-rev-inl t-since-cxx17"><span>或结构化绑定</span> <span><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></span></span>的声明点紧随相应的<span class="t-spar">范围表达式</span> ﻿之后：</p><div class="mw-geshi" dir="ltr"><pre class="de1"><a href="http://zh.cppreference.com/w/container/vector"><span class="kw1272">std::<span class="me2">vector</span></span></a><span class="sy1">&lt;</span><span class="kw4">int</span><span class="sy1">&gt;</span> x<span class="sy4">;</span>
&nbsp;
<span class="kw1">for</span> <span class="br0">(</span><span class="kw4">auto</span> x <span class="sy4">:</span> x<span class="br0">)</span> <span class="co1">// 右括号前仍然属于 vector x 的作用域，</span>
                 <span class="co1">// 而 auto x 的作用域从右括号开始</span>
<span class="br0">{</span>
    <span class="co1">// auto x 的作用域中</span>
<span class="br0">}</span></pre></div></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

[模板形参](template_parameters "language/template parameters")的声明点紧随其完整模板形参（包括可选的默认实参）之后：
```cpp
typedef unsigned char T;
 
template<
    class T \= T, // 模板形参 T 的作用域从逗号开始，
                 // 但逗号前还在 unsigned char 的 typedef 名的作用域中，
    T, // 模板形参 T 的作用域中
    N \= 0
\>
struct A {};
```

### 参阅