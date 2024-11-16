## 重载决议

---

为了编译函数调用，编译器必须首先进行[名字查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")，对于函数可能涉及[实参依赖查找](https://zh.cppreference.com/w/cpp/language/adl "cpp/language/adl")，而对于函数模板可能后随[模板实参推导](https://zh.cppreference.com/w/cpp/language/template_argument_deduction "cpp/language/template argument deduction")。

如果找到的名字指代了多个实体，那么该名字被*重载*，并且编译器必须决定要调用哪个重载。简单来说，决定调用的是各形参与各实参之间的匹配最紧密的重载。

具体来说，重载决议通过以下几个步骤进行：

1.  建立[候选函数](#.E5.80.99.E9.80.89.E5.87.BD.E6.95.B0)集合。
2.  从该集合去除函数，只保留[可行函数](#.E5.8F.AF.E8.A1.8C.E5.87.BD.E6.95.B0)。
3.  分析可行函数集合，以确定唯一的[最佳可行函数](#.E6.9C.80.E4.BD.B3.E5.8F.AF.E8.A1.8C.E5.87.BD.E6.95.B0)（可能会涉及[隐式转换序列的分级](#.E9.9A.90.E5.BC.8F.E8.BD.AC.E6.8D.A2.E5.BA.8F.E5.88.97.E7.9A.84.E5.88.86.E7.BA.A7)）。
```cpp
void f(long);
void f(float);
 
f(0L); // 调用 f(long)
f(0);  // 错误：重载有歧义
```

除函数调用外，其他语境可以出现重载函数名的语境，此时应用的规则不同，见[重载函数的地址](https://zh.cppreference.com/w/cpp/language/overloaded_address "cpp/language/overloaded address")。

如果函数无法被重载决议选择（例如它是有未被满足的[约束](https://zh.cppreference.com/w/cpp/language/constraints "cpp/language/constraints")的[模板化实体](https://zh.cppreference.com/w/cpp/language/templates#.E6.A8.A1.E6.9D.BF.E5.8C.96.E5.AE.9E.E4.BD.93 "cpp/language/templates")），那么不能使用它。

### 候选函数

在重载决议开始前，将名字查找和模板实参推导所选择的函数组成*候选函数*的集合。确切的判别标准取决于发生重载决议的语境。

#### 调用具名函数

如果 `E` 在[函数调用表达式](https://zh.cppreference.com/w/cpp/language/operator_other#.E5.86.85.E5.BB.BA.E7.9A.84.E5.87.BD.E6.95.B0.E8.B0.83.E7.94.A8.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator other") `E(args)` 中指名重载的函数和/或函数模板（但非可调用对象）的集合，那么遵循下列规则：

+   如果表达式 `E` 具有 `PA->B` 或 `A.B` 的形式（其中 A 具有类类型 cv T），那么将 `B` 作为 `T` 的成员函数[查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")。该查找所找到的函数声明都是候选函数。就重载决议而言，实参列表拥有 cv T 类型的隐含对象实参。
+   如果表达式 `E` 是[初等表达式](https://zh.cppreference.com/w/cpp/language/expressions#.E5.88.9D.E7.AD.89.E8.A1.A8.E8.BE.BE.E5.BC.8F "cpp/language/expressions")，那么遵循函数调用的正常规则[查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")它的名字（可能涉及 [实参依赖查找](https://zh.cppreference.com/w/cpp/language/adl "cpp/language/adl")）。该查找所找到的函数声明（取决于查找的工作方式）是下列之一：

a) 全部是非成员函数（该情况下，就重载决议而言，实参列表正是函数调用表达式中所用的实参列表）

b) 全部是某个类 `T` 的成员函数，该情况下，如果 [this](https://zh.cppreference.com/w/cpp/language/this "cpp/language/this") 在作用域中且它是指向 `T` 或从 `T` 派生的类的指针，那么以 `*this` 作为隐含对象实参。否则（如果 `this` 不在作用域中或不指向 `T`），以一个 `T` 类型的虚假对象作为隐含对象实参，而如果重载决议继而选择了非静态成员函数，那么程序非良构。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=3 "编辑章节：调用类对象")\] 调用类对象

如果 `E` 在[函数调用表达式](https://zh.cppreference.com/w/cpp/language/operator_other#.E5.86.85.E5.BB.BA.E7.9A.84.E5.87.BD.E6.95.B0.E8.B0.83.E7.94.A8.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator other") `E(args)` 中拥有类型 cv `T`，那么

+   在表达式 `(E).operator()` 的语境中，对名字进行 operator() 的通常[查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")获得 T 的函数调用运算符，并把每个找到的函数声明添加到候选函数集。
+   对于 `T` 或 `T` 的基类中每个（未被隐藏的）非 explicit 的[用户定义转换函数](https://zh.cppreference.com/w/cpp/language/cast_operator "cpp/language/cast operator")，且它的 cv 限定符与 `T` 的 cv 限定符相同或更多，并且该转换函数转换到：

+   函数指针
+   函数指针的引用
+   函数的引用

那么将一个拥有独有名称的*代表调用函数* ﻿添加到候选函数集，该函数的首个形参作为转换结果，剩余各形参作为转换结果所接受的形参列表，而它的返回类型作为转换结果的返回类型。如果后继的重载决议选择此代表函数，那么将调用用户定义转换函数，然后调用转换的结果。

任何情况下，就重载决议而言的实参列表，是函数调用表达式的实参列表，前面加上隐含对象实参 `E`（匹配到代表函数时，用户定义转换将自动将隐含对象实参转换成代表函数的首个实参）。

int f1(int);
int f2(float);
 
struct A
{
    using fp1 \= int(\*)(int);
    operator fp1() { return f1; } // 转换到函数指针的转换函数
    using fp2 \= int(\*)(float);
    operator fp2() { return f2; } // 转换到函数指针的转换函数
} a;
 
int i \= a(1); // 通过转换函数返回的指针调用 f1

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=4 "编辑章节：调用重载运算符")\] 调用重载运算符

如果表达式中某个运算符的至少一个实参具有类类型或枚举类型，那么[内建运算符](https://zh.cppreference.com/w/cpp/language/expressions#.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/expressions")和[用户定义的运算符重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")都参与重载决议，所选择的候选函数集如下：

对于实参具有类型 `T1`（移除 cv 限定后）的一元运算符 `**@**`，或左操作数具有类型 `T1` 而右操作数具有类型 `T2`（移除 cv 限定后）的二元运算符 `**@**`，准备下列候选函数集：

1) *成员候选*：如果 `T1` 是完整类或当前正在定义的类，那么成员候选集是对 `T1::operator@` 进行[有限定的名字查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")的结果。所有其他情况下，成员候选集为空。

2) *非成员候选*：对于[运算符重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")容许非成员形式的运算符，为在表达式的语境中对 `operator@` 进行[无限定名字查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")（可能涉及 [实参依赖查找](https://zh.cppreference.com/w/cpp/language/adl "cpp/language/adl")）所找到的所有声明，但忽略成员函数声明而且它不会阻止到下个外围作用域中继续进行查找。如果二元运算符的两个操作数，或一元运算符的唯一操作数具有枚举类型，那么只查找有形参具有该枚举类型（或到该枚举类型引用）的函数，成为非成员候选函数。

3) *内建候选*：对于 operator,、一元 operator& 和 operator\-\>，内建候选集为空。对于其他运算符，内建候选是[内建运算符页面](https://zh.cppreference.com/w/cpp/language/expressions#.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/expressions")中列出的函数，只要所有操作数都能隐式转换成它的各个形参。如果有任何内建候选拥有的形参列表与某个并非函数模板特化的非成员候选或非成员重写候选 (C++20 起)相同，那么该内建候选不会添加到内建候选列表。当考虑内建的赋值运算符时，限制从它的首个形参进行的转换：只考虑[标准转换序列](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><div class="t-li1"><p><span class="t-li">4)</span> <i>重写候选</i>：</p><dl><dd><ul><li>对于四个关系运算符表达式 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&lt;</span> y</span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&lt;=</span> y</span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&gt;</span> y</span></span> 及 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&gt;=</span> y</span></span>，添加所有找到的成员、非成员及内建 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">&lt;=&gt;</span></span> 到集合。</li><li>对于四个关系运算符表达式 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&lt;</span> y</span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&lt;=</span> y</span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&gt;</span> y</span></span> 及 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&gt;=</span> y</span></span> 还有三路比较运算符表达式 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">&lt;=&gt;</span> y</span></span>，对每个找到的成员、非成员及内建 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">&lt;=&gt;</span></span> 添加两个形参顺序相反的对应合成候选。</li><li>对于 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy3">!</span><span class="sy1">=</span> y</span></span>，在没有匹配的 <span class="mw-geshi cpp source-cpp">operator<span class="sy3">!</span><span class="sy1">=</span></span> 的情况下添加所有找到的成员、非成员及内建 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">==</span></span> 到集合。</li><li>对于相等运算符表达式 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy1">==</span> y</span></span> 与 <span class="t-c"><span class="mw-geshi cpp source-cpp">x <span class="sy3">!</span><span class="sy1">=</span> y</span></span>，在没有匹配的 <span class="mw-geshi cpp source-cpp">operator<span class="sy3">!</span><span class="sy1">=</span></span> 的情况下对每个找到的成员、非成员及内建 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">==</span></span> 添加两个形参顺序相反的对应合成候选。</li></ul></dd></dl></div><p><span class="t-li"></span>所有情况下，在重写表达式的语境中不考虑重写候选。对于所有其他运算符，重写候选集为空。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

提交给重载决议的候选函数集合是以上集合的并集。就重载决议而言的实参列表由运算符的各操作数组成，除了 `operator->` 的情况，它的第二个操作数并非函数调用的实参（见[成员访问运算符](https://zh.cppreference.com/w/cpp/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator member access")）。

struct A
{
    operator int();              // 用户定义转换
};
A operator+(const A&, const A&); // 非成员用户定义运算符
 
void m()
{
    A a, b;
    a + b; // 成员候选：无
           // 非成员候选：operator+(a, b)
           // 内建候选：int(a) + int(b)
           // 重载决议选择 operator+(a, b)
}

如果重载决议选择了内建候选，那么从类类型的操作数进行的[用户定义转换序列](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")不允许拥有第二个标准转换序列：用户定义转换函数必须直接给出期待的操作数类型：

struct Y { operator int\*(); }; // Y 可转换到 int\*
int \*a \= Y() + 100.0;          // 错误：指针和 double 之间没有 operator+

对于 operator,、一元 operator& 和 operator\-\>，如果候选函数集中没有可行函数（见后述），那么将运算符解释为内建运算符。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><p>如果对运算符 <code>@</code> 的重载决议选择了重写的 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">&lt;=&gt;</span></span> 候选，那么用重写的 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">&lt;=&gt;</span></span> 候选将 <span class="t-c"><span class="mw-geshi cpp source-cpp">x @ y</span></span> 解释为重写的表达式：当所选择的候选是具有逆序形参的合成候选时，解释为 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="nu0">0</span> @ <span class="br0">(</span>y <span class="sy1">&lt;=&gt;</span> x<span class="br0">)</span></span></span>，否则为 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="br0">(</span>x <span class="sy1">&lt;=&gt;</span> y<span class="br0">)</span> @ <span class="nu0">0</span></span></span>。</p><p>如果对运算符（<code>==</code> 或 <code>!=</code>）的重载决议选择了重写 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">==</span></span> 候选，那么它的返回类型必须是（可有 cv 限定的）<span class="mw-geshi cpp source-cpp"><span class="kw4">bool</span></span>，并使用选择的重写 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">==</span></span> 候选解释 <span class="t-c"><span class="mw-geshi cpp source-cpp">x @ y</span></span> 作为重写表达式：如果选择的候选是拥有逆序形参的合成候选时是 <span class="t-c"><span class="mw-geshi cpp source-cpp">y <span class="sy1">==</span> x</span></span> 或 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy3">!</span><span class="br0">(</span>y <span class="sy1">==</span> x<span class="br0">)</span></span></span>，否则是 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy3">!</span><span class="br0">(</span>x <span class="sy1">==</span> y<span class="br0">)</span></span></span>。</p><p>这种情况下的重载决议有一条决胜规则：非重写候选优于重写候选，且非合成重写候选优于合成重写候选。</p><p>这种具有逆序实参的查找使得可以只写 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">&lt;=&gt;</span><span class="br0">(</span><a href="http://zh.cppreference.com/w/cpp/string/basic_string"><span class="kw1233">std::<span class="me2">string</span></span></a>, <span class="kw4">const</span> <span class="kw4">char</span><span class="sy2">*</span><span class="br0">)</span></span> 与 <span class="mw-geshi cpp source-cpp">operator<span class="sy1">==</span><span class="br0">(</span><a href="http://zh.cppreference.com/w/cpp/string/basic_string"><span class="kw1233">std::<span class="me2">string</span></span></a>, <span class="kw4">const</span> <span class="kw4">char</span><span class="sy2">*</span><span class="br0">)</span></span> 就生成 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/string/basic_string" title="cpp/string/basic string">std::string</a></span> 和 <span class="mw-geshi cpp source-cpp"><span class="kw4">const</span> <span class="kw4">char</span><span class="sy2">*</span></span> 间的所有双向比较。更多细节见<a href="https://zh.cppreference.com/w/cpp/language/default_comparisons" title="cpp/language/default comparisons">默认比较</a>。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=5 "编辑章节：由构造函数初始化")\] 由构造函数初始化

当对类类型的对象进行[直接初始化](https://zh.cppreference.com/w/cpp/language/direct_initialization "cpp/language/direct initialization")或在[复制初始化](https://zh.cppreference.com/w/cpp/language/copy_initialization "cpp/language/copy initialization")之外的语境中进行[默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization "cpp/language/default initialization")时，候选函数是正在初始化的类的所有构造函数。实参列表是初始化器的表达式列表。

当对类类型对象从某个相同或派生类类型的对象进行复制初始化，或在复制初始化语境中进行默认初始化时，候选函数是正在初始化的类的所有[转换构造函数](https://zh.cppreference.com/w/cpp/language/converting_constructor "cpp/language/converting constructor")。实参列表是初始化器的表达式。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=6 "编辑章节：通过转换进行复制初始化")\] 通过转换进行复制初始化

如果类类型对象的[复制初始化](https://zh.cppreference.com/w/cpp/language/copy_initialization "cpp/language/copy initialization")要求调用某个用户定义转换以将 cv `S` 类型的初始化器表达式转换到正在初始化的对象的 cv `T` 类型，那么下列函数是候选函数：

+   `T` 的所有[转换构造函数](https://zh.cppreference.com/w/cpp/language/converting_constructor "cpp/language/converting constructor")
+   从 `S` 及它的各基类（除非隐藏）到 `T` 或 `T` 的派生类或到它们的引用的非 explicit 转换函数。如果此复制初始化是（可有 cv 限定的）`T` 的直接初始化序列的一部分（对于接受一个到 cv `T` 的引用的构造函数，初始化要绑定到它的首个形参的引用），那么也会考虑 explicit 转换函数。

无论哪种方式，就重载决议而言的实参列表均由单个实参组成，即初始化器表达式，它将会与构造函数的首个实参或转换函数的隐式对象实参相比较。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=7 "编辑章节：通过转换进行非类初始化")\] 通过转换进行非类初始化

当非类类型 cv1 `T` 对象的初始化要求某个[用户定义转换函数](https://zh.cppreference.com/w/cpp/language/cast_operator "cpp/language/cast operator")，以从类类型 cv `S` 的初始化器表达式转换时，下列函数是候选：

+   `S` 及它的基类（除非隐藏）中的，产生 `T` 类型，或可由[标准转换序列](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")转换到 `T` 的类型，或到这些类型的引用的非 explicit 用户定义转换函数。对于选择候选函数而言，忽略返回类型上的 cv 限定符。
+   如果这是[直接初始化](https://zh.cppreference.com/w/cpp/language/direct_initialization "cpp/language/direct initialization")，那么也会考虑 `S` 及它的基类（除非隐藏）中的，产生 `T` 类型，或可由[限定性转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")转换到 `T` 的类型，或到这些类型的引用的 explicit 用户定义转换函数。

无论哪种方式，就重载决议而言的实参列表均由单个实参组成，即初始化器表达式，它将会与转换函数的隐含对象实参相比较。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=8 "编辑章节：通过转换进行引用初始化")\] 通过转换进行引用初始化

在将指代 *cv1* `T` 的引用绑定到从初始化器表达式转换到类类型 *cv2* `S` 的左值或右值结果的[引用初始化](https://zh.cppreference.com/w/cpp/language/reference_initialization "cpp/language/reference initialization")期间，为候选集选择下列函数：

+   `S` 及它的基类（除非隐藏）中的到以下类型的非 explicit 用户定义转换函数：

+   （当初始化左值引用或到函数的右值引用时）到 *cv2* `T2` 的左值引用
+   （当初始化右值引用或到函数的左值引用时）*cv2* `T2` 或到 *cv2* `T2` 的右值引用

其中 *cv2* `T2` [引用兼容](https://zh.cppreference.com/w/cpp/language/reference_initialization#.E5.AE.9A.E4.B9.89 "cpp/language/reference initialization") *cv1* `T`。

+   对于直接初始化，如果 `T2` 与 `T` 类型相同或能以限定性转换转换到 `T`，那么也会考虑 explicit 用户定义转换函数。

无论哪种方式，就重载决议而言的实参列表均由单个实参组成，即初始化器表达式，它将会与转换函数的隐含对象实参相比较。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=9 "编辑章节：列表初始化")\] 列表初始化

当非聚合类类型 `T` 的对象进行[列表初始化](https://zh.cppreference.com/w/cpp/language/list_initialization "cpp/language/list initialization")时，进行两阶段的重载决议。

+   在阶段 1，候选函数是 `T` 的所有初始化器列表构造函数，而就重载决议而言的实参列表由单个初始化器列表实参组成
+   如果阶段 1 的重载决议失败，那么进入阶段 2，其中候选函数是 `T` 的所有构造函数，而就重载决议而言的实参列表由初始化器列表的各个单独元素所组成。

如果初始化器列表为空而 `T` 拥有默认构造函数，那么跳过阶段 1。

在复制列表初始化中，如果阶段 2 选择 explicit 构造函数，那么初始化非良构（与复制初始化的总体相反，它们甚至不考虑 explicit 构造函数）。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=10 "编辑章节：函数模板候选的额外规则")\] 函数模板候选的额外规则

如果名字查找找到了函数模板，那么为找到能在此时使用的模板实参值（如果存在）会进行[模板实参推导](https://zh.cppreference.com/w/cpp/language/template_argument_deduction "cpp/language/template argument deduction")和显式模板实参检查：

+   如果两者都成功，那么找到的模板实参会用来合成对应的函数模板特化的声明，那么这些特化会加入候选集，并且在决胜规则指定的情况以外都会被视为非模板函数。
+   如果模板实参推导失败或合成的函数模板特化非良构，那么这些函数不会加入候选集（参见 [SFINAE](https://zh.cppreference.com/w/cpp/language/sfinae "cpp/language/sfinae")）。

如果一个名字指代一个或多个函数模板，并且同时指代重载的非模板函数，那么这些函数和从模板生成的特化都是候选。

更多细节见[F函数模板重载](https://zh.cppreference.com/w/cpp/language/function_template#F.E5.87.BD.E6.95.B0.E6.A8.A1.E6.9D.BF.E9.87.8D.E8.BD.BD "cpp/language/function template")。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><p>如果构造函数模板或转换函数模板有<a href="https://zh.cppreference.com/w/cpp/language/dependent_name#.E5.80.BC.E5.BE.85.E5.86.B3.E8.A1.A8.E8.BE.BE.E5.BC.8F" title="cpp/language/dependent name">值待决</a>的<a href="https://zh.cppreference.com/w/cpp/language/explicit" title="cpp/language/explicit">条件性 explicit 说明符</a>，如果在推导后上下文要求候选不是 explicit 的而生成的特化是 explicit 的，那么它会从候选函数集合排除。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=11 "编辑章节：构造函数候选的额外规则")\] 构造函数候选的额外规则

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><p>被定义为弃置的预置<a href="https://zh.cppreference.com/w/cpp/language/move_constructor" title="cpp/language/move constructor">移动构造函数</a>和<a href="https://zh.cppreference.com/w/cpp/language/move_assignment" title="cpp/language/move assignment">移动赋值运算符</a>会从候选函数集合排除。。</p><p>在构造类型 <code>D</code> 的对象时，如果满足以下所有条件，那么从类类型 <code>C</code> <a href="https://zh.cppreference.com/w/cpp/language/using_declaration#.E7.BB.A7.E6.89.BF.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0" title="cpp/language/using declaration">继承</a>的首个形参类型是“到 <code>P</code> 的引用”的构造函数（包括从模板实例化的此类构造函数）会从候选函数集合排除：</p><ul><li>实参列表只有一个实参。</li><li><code>C</code> <a href="https://zh.cppreference.com/w/cpp/language/reference_initialization#.E5.AE.9A.E4.B9.89" title="cpp/language/reference initialization">引用关联于</a> <code>P</code>。</li><li><code>P</code> 引用关联于 <code>D</code>。</li></ul></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=12 "编辑章节：成员函数候选的额外规则")\] 成员函数候选的额外规则

如果有候选函数是除构造函数外且没有[显式对象形参](https://zh.cppreference.com/w/cpp/language/member_functions#.E6.98.BE.E5.BC.8F.E5.AF.B9.E8.B1.A1.E5.BD.A2.E5.8F.82 "cpp/language/member functions") (C++23 起)的[成员函数](https://zh.cppreference.com/w/cpp/language/member_functions "cpp/language/member functions")（静态或非静态），那么将它当做如同它有一个额外形参（*隐式对象形参*），代表调用函数所用的对象，并出现在首个实际形参之前。

类似地，调用成员函数所用的对象会作为*隐含对象实参* ﻿前附于实参列表。

对于类 `X` 的成员函数，隐含对象形参的类型受成员函数的 cv 限定和引用限定影响，如[成员函数](https://zh.cppreference.com/w/cpp/language/member_functions "cpp/language/member functions")中所述。

就确定*隐式对象形参* ﻿类型而言，用户定义转换函数被认为是*隐含对象实参* ﻿的成员。

就确定*隐式对象形参* ﻿类型而言，由 using 声明引入到派生类中的成员函数被认为是派生类的成员。

<table class="t-rev-begin"><tbody><tr class="t-rev t-until-cxx23"><td><p>对于静态成员函数，它的<i>隐式对象形参</i> ﻿被认为匹配任何对象：不检验它的类型，且不会为它尝试转换序列。</p></td><td><span class="t-mark-rev t-until-cxx23">(C++23 前)</span></td></tr></tbody></table>

对于重载决议的剩余部分，*隐含对象实参* ﻿与其他实参没有区别，但下列特殊规则适用于*隐式对象形参*：

1) 不能对隐式对象形参运用用户定义转换。

2) 右值能绑定到非 const 的隐式对象形参（除非是对引用限定的成员函数） (C++11 起)，且不影响隐式转换的等级。

struct B { void f(int); };
struct A { operator B&(); };
 
A a;
a.B::f(1); // 错误：不能对隐式对象形参运用用户定义转换
static\_cast<B&\>(a).f(1); // OK

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=13 "编辑章节：可行函数")\] 可行函数

给定以上述方式构造的候选函数集，重载决议的下一步骤是检验各个实参与形参，并将集合缩减为*可行函数* ﻿的集合。

为了被包含在可行函数集中，候选函数必须满足下列条件：

1) 如果有 `M` 个实参，那么刚好具有 `M` 个形参的候选函数可行。

2) 如果有 `M` 个实参且候选函数的形参少于 `M` 个，但具有一个[省略号形参](https://zh.cppreference.com/w/cpp/language/variadic_arguments "cpp/language/variadic arguments")，那么它可行。

3) 如果有 `M` 个实参且候选函数的形参多于 `M` 个，但是第 `M+1` 个形参和所有后随形参都具有默认实参，那么它可行。对于剩余的重载决议，形参列表被截断到 `M`。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><p><span class="t-li">4)</span> 如果函数拥有关联的<a href="https://zh.cppreference.com/w/cpp/language/constraints" title="cpp/language/constraints">约束</a>，那么必须满足它。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

5) 对于每个实参，必须至少存在一个隐式转换序列将它转换到对应的形参。

6) 如果任何形参具有引用类型，那么这一步负责引用绑定：如果右值实参对应非 const 左值引用形参，或左值实参对应右值引用形参，那么函数不可行。

用户定义的转换（[转换构造函数](https://zh.cppreference.com/w/cpp/language/converting_constructor "cpp/language/converting constructor")和[用户定义转换函数](https://zh.cppreference.com/w/cpp/language/cast_operator "cpp/language/cast operator")）不参与可能会应用多于一个用户定义的转换的隐式转换序列。具体而言，一个用户定义的转换不会被考虑，如果它的转换目标是一个构造函数的首个形参或用户定义转换函数的隐式对象参数，且这个构造函数/用户定义转换函数是下列初始化的候选函数：

+   [通过用户定义转换进行复制初始化](#.E9.80.9A.E8.BF.87.E8.BD.AC.E6.8D.A2.E8.BF.9B.E8.A1.8C.E5.A4.8D.E5.88.B6.E5.88.9D.E5.A7.8B.E5.8C.96)
+   [通过转换函数进行非类类型的初始化](#.E9.80.9A.E8.BF.87.E8.BD.AC.E6.8D.A2.E8.BF.9B.E8.A1.8C.E9.9D.9E.E7.B1.BB.E5.88.9D.E5.A7.8B.E5.8C.96)
+   [为直接引用绑定的通过转换函数进行的初始化](#.E9.80.9A.E8.BF.87.E8.BD.AC.E6.8D.A2.E8.BF.9B.E8.A1.8C.E5.BC.95.E7.94.A8.E5.88.9D.E5.A7.8B.E5.8C.96)
+   在类的[复制初始化](https://zh.cppreference.com/w/cpp/language/copy_initialization "cpp/language/copy initialization")的第二步（直接初始化）期间[通过构造函数所作的初始化](#.E7.94.B1.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0.E5.88.9D.E5.A7.8B.E5.8C.96)

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><ul><li>由列表初始化所作的初始化，其中的初始化器列表刚好拥有一个元素，且它自身是一个初始化器列表，且目标是类 <code>X</code> 的构造函数的首个实参，而该转换是到 <code>X</code> 或到（可有 cv 限定的）<code>X</code> 的引用的转换：</li></ul><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw1">struct</span> A <span class="br0">{</span> A<span class="br0">(</span><span class="kw4">int</span><span class="br0">)</span><span class="sy4">;</span> <span class="br0">}</span><span class="sy4">;</span>
<span class="kw1">struct</span> B <span class="br0">{</span> B<span class="br0">(</span>A<span class="br0">)</span><span class="sy4">;</span> <span class="br0">}</span><span class="sy4">;</span>
&nbsp;
B b<span class="br0">{</span><span class="br0">{</span><span class="nu0">0</span><span class="br0">}</span><span class="br0">}</span><span class="sy4">;</span> <span class="co1">// B 的列表初始化</span>
&nbsp;
<span class="co1">// 候选：B(const B&amp;)、B(B&amp;&amp;)、B(A)</span>
<span class="co1">// {0} -&gt; B&amp;&amp; 不可行：要调用 B(A)</span>
<span class="co1">// {0} -&gt; const B&amp; ：不可行：要绑定到右值，要调用 B(A)</span>
<span class="co1">// {0} -&gt; A 可行。调用 A(int)：不禁止到 A 的用户定义转换</span></pre></div></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=14 "编辑章节：最佳可行函数")\] 最佳可行函数

对于每对可行函数 `F1` 和 `F2`，对从第 `i` 实参到第 `i` 形参的转换做排行，以确定哪一个更好（除了首个实参，静态成员函数的*隐式对象实参* ﻿在排行上没有影响）。

如果 `F1` 的所有实参的隐式转换*不劣于* `F2` 的所有实参的隐式转换，且满足下列条件，那么确定 `F1` 是优于 `F2` 的函数：

1) 至少存在一个 `F1` 的实参，它的隐式转换*优于* `F2` 的该实参的对应的隐式转换。

2) 或若非如此，（只在通过转换进行非类初始化的语境中，）从 `F1` 的返回类型到要初始化的类型的标准转换序列*优于* ﻿从 `F2` 的返回类型到该类型的标准转换序列。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><p><span class="t-li">3)</span> 或若非如此，（仅在对函数类型的引用进行直接引用绑定所作的，通过转换函数进行初始化的语境中，）<code>F1</code> 的返回类型是与正在初始化的引用相同种类的引用（左值或右值），而 <code>F2</code> 的返回类型不是。</p></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

4) 或若非如此，`F1` 是非模板函数而 `F2` 是模板特化。

5) 或若非如此，`F1` 与 `F2` 都是模板特化，且按照[模板特化的偏序规则](https://zh.cppreference.com/w/cpp/language/function_template#.E5.87.BD.E6.95.B0.E6.A8.A1.E6.9D.BF.E9.87.8D.E8.BD.BD "cpp/language/function template")，`F1` 更特殊。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><div class="t-li1"><p><span class="t-li">6)</span> 或若非如此，<code>F1</code> 与 <code>F2</code> 是满足以下所有条件的的非模板函数：</p><ul><li>它们的<a href="https://zh.cppreference.com/w/cpp/language/function#.E5.BD.A2.E5.8F.82.E7.B1.BB.E5.9E.8B.E5.88.97.E8.A1.A8" title="cpp/language/function">形参类型列表</a>相同<span class="t-rev-inl t-since-cxx23"><span>，其中忽略<a href="https://zh.cppreference.com/w/cpp/language/member_functions#.E6.98.BE.E5.BC.8F.E5.AF.B9.E8.B1.A1.E5.BD.A2.E5.8F.82" title="cpp/language/member functions">显式对象形参</a>的类型</span> <span><span class="t-mark-rev t-since-cxx23">(C++23 起)</span></span></span>。</li><li>如果它们是成员函数，那么它们是相同类的直接成员。</li><li>如果它们都是非静态成员函数，那么它们的对象形参的类型相同。</li><li>按照<a href="https://zh.cppreference.com/w/cpp/language/constraints" title="cpp/language/constraints">约束的偏序</a>规则，<code>F1</code> 比 <code>F2</code> 更受约束。</li></ul></div></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><div class="t-li1"><p><span class="t-li">7)</span> 或若非如此，<code>F1</code> 是类 D 的构造函数，<code>F2</code> 是 D 的基类 B 的构造函数，且对应每个实参的 <code>F1</code> 和 <code>F2</code> 的形参均具有相同类型：</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw1">struct</span> A
<span class="br0">{</span>
    A<span class="br0">(</span><span class="kw4">int</span> <span class="sy1">=</span> <span class="nu0">0</span><span class="br0">)</span><span class="sy4">;</span>
<span class="br0">}</span><span class="sy4">;</span>
&nbsp;
<span class="kw1">struct</span> B<span class="sy4">:</span> A
<span class="br0">{</span>
    <span class="kw1">using</span> A<span class="sy4">::</span><span class="me2">A</span><span class="sy4">;</span>
&nbsp;
    B<span class="br0">(</span><span class="br0">)</span><span class="sy4">;</span>
<span class="br0">}</span><span class="sy4">;</span>
&nbsp;
B b<span class="sy4">;</span> <span class="co1">// OK，B::B()</span></pre></div></div></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><p><span class="t-li">8)</span> 或若非如此，<code>F2</code> 是重写的候选而 <code>F1</code> 不是。</p><p><span class="t-li">9)</span> 或若非如此，<code>F1</code> 和 <code>F2</code> 都是重写候选，但 <code>F2</code> 是带逆序形参的合成重写候选而 <code>F1</code> 不是。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx17"><td><p><span class="t-li">10)</span> 或若非如此，<code>F1</code> 是从<a href="https://zh.cppreference.com/w/cpp/language/class_template_argument_deduction#.E7.94.A8.E6.88.B7.E5.AE.9A.E4.B9.89.E6.8E.A8.E5.AF.BC.E6.8C.87.E5.BC.95" title="cpp/language/class template argument deduction">用户定义推导指引</a>所生成的而 <code>F2</code> 不是。</p><p><span class="t-li">11)</span> 或若非如此，<code>F1</code> 是<a href="https://zh.cppreference.com/w/cpp/language/class_template_argument_deduction#.E7.94.A8.E6.88.B7.E5.AE.9A.E4.B9.89.E6.8E.A8.E5.AF.BC.E6.8C.87.E5.BC.95" title="cpp/language/class template argument deduction">复制推导候选</a>而 <code>F2</code> 不是。</p><div class="t-li1"><p><span class="t-li">12)</span> 或若非如此，<code>F1</code> 是从非模板构造函数生成而 <code>F2</code> 是从构造函数模板生成：</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw1">template</span><span class="sy1">&lt;</span><span class="kw1">class</span> T<span class="sy1">&gt;</span>
<span class="kw1">struct</span> A
<span class="br0">{</span>
    <span class="kw1">using</span> value_type <span class="sy1">=</span> T<span class="sy4">;</span>
    A<span class="br0">(</span>value_type<span class="br0">)</span><span class="sy4">;</span>  <span class="co1">// #1</span>
    A<span class="br0">(</span><span class="kw4">const</span> A<span class="sy3">&amp;</span><span class="br0">)</span><span class="sy4">;</span>    <span class="co1">// #2</span>
    A<span class="br0">(</span>T, T, <span class="kw4">int</span><span class="br0">)</span><span class="sy4">;</span>   <span class="co1">// #3</span>
&nbsp;
    <span class="kw1">template</span><span class="sy1">&lt;</span><span class="kw1">class</span> U<span class="sy1">&gt;</span>
    A<span class="br0">(</span><span class="kw4">int</span>, T, U<span class="br0">)</span><span class="sy4">;</span>   <span class="co1">// #4</span>
<span class="br0">}</span><span class="sy4">;</span>                  <span class="co1">// #5 是 A(A)，它是复制推导候选</span>
&nbsp;
A x<span class="br0">(</span><span class="nu0">1</span>, <span class="nu0">2</span>, <span class="nu0">3</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 使用 #3，从非模板构造函数生成</span>
&nbsp;
<span class="kw1">template</span><span class="sy1">&lt;</span><span class="kw1">class</span> T<span class="sy1">&gt;</span>
A<span class="br0">(</span>T<span class="br0">)</span> <span class="sy2">-</span><span class="sy1">&gt;</span> A<span class="sy1">&lt;</span>T<span class="sy1">&gt;</span><span class="sy4">;</span>       <span class="co1">// #6，不如 #5 特殊</span>
&nbsp;
A a<span class="br0">(</span><span class="nu0">42</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 使用 #6 推出 A&lt;int&gt; 并用 #1 初始化</span>
A b <span class="sy1">=</span> a<span class="sy4">;</span> <span class="co1">// 使用 #5 推出 A&lt;int&gt; 并用 #2 初始化</span>
&nbsp;
<span class="kw1">template</span><span class="sy1">&lt;</span><span class="kw1">class</span> T<span class="sy1">&gt;</span>
A<span class="br0">(</span>A<span class="sy1">&lt;</span>T<span class="sy1">&gt;</span><span class="br0">)</span> <span class="sy2">-</span><span class="sy1">&gt;</span> A<span class="sy1">&lt;</span>A<span class="sy1">&lt;</span>T<span class="sy1">&gt;&gt;</span><span class="sy4">;</span> <span class="co1">// #7，和 #5 一样特殊</span>
A b2 <span class="sy1">=</span> a<span class="sy4">;</span> <span class="co1">// 使用 #7 推出 A&lt;A&lt;int&gt;&gt; 并用 #1 初始化</span></pre></div></div></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr></tbody></table>

对所有可行函数进行这些逐对比较。如果刚好有一个可行函数优于所有其他函数，那么重载决议成功并调用该函数。否则编译失败。

void Fcn(const int\*, short); // 重载 #1
void Fcn(int\*, int);         // 重载 #2
 
int i;
short s \= 0;
 
void f()
{
    Fcn(&i, 1L);  // 第 1 个实参：&i -> int\* 优于 &i -> const int\*
                  // 第 2 个实参：1L -> short 与 1L -> int 等价
                  // 调用 Fcn(int\*, int)
 
    Fcn(&i, 'c'); // 第 1 个实参：&i -> int\* 优于 &i -> const int\*
                  // 第 2 个实参：'c' -> int 优于 'c' -> short
                  // 调用 Fcn(int\*, int)
 
    Fcn(&i, s);   // 第 1 个实参：&i -> int\* 优于 &i -> const int\*
                  // 第 2 个实参：s -> short 优于 s -> int
                  // 无胜者，编译错误
}

如果最佳可行函数决议到了一个可以找到多个声明的函数，且这些声明其中的两个存在于不同的作用域并指定了使得该函数可行的默认参数，那么程序非良构。

namespace A
{
    extern "C" void f(int \= 5);
}
 
namespace B
{
    extern "C" void f(int \= 5);
}
 
using A::f;
using B::f;
 
void use()
{
    f(3); // OK，默认实参不会用于使函数可行
    f();  // 错误：找到两次默认实参
}

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=15 "编辑章节：隐式转换序列的分级")\] 隐式转换序列的分级

重载决议所考虑的实参-形参隐式转换序列与[复制初始化](https://zh.cppreference.com/w/cpp/language/copy_initialization "cpp/language/copy initialization")中（对于非引用形参）所用的[隐式转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")对应，但在到隐含对象形参或到赋值运算符的左侧操作数的转换时不考虑创建临时对象的转换。当形参为静态成员函数的隐式对象形参时，隐式转换序列是与任何其他标准转换序列相比既不更好也不更差的标准转换序列。 (C++23 起)

每种[标准转换序列的类型](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")都被赋予三个等级之一：

1) **准确匹配**：不要求转换、左值到右值转换、限定性转换、函数指针转换、 (C++17 起)类类型到相同类的用户定义转换

2) **提升**：整型提升、浮点提升

3) **转换**：整型转换、浮点转换、浮点整型转换、指针转换、成员指针转换、布尔转换、派生类到它的基类的用户定义转换

标准转换序列的等级是它包含的标准转换（至多可有[三次转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")）中的最差等级。

直接绑定引用形参到实参表达式是*恒等*或派生类到基类*转换*：

struct Base {};
struct Derived : Base {} d;
 
int f(Base&);    // 重载 #1
int f(Derived&); // 重载 #2
 
int i \= f(d); // d -> Derived& 拥有‘准确匹配’等级
              // d -> Base& 拥有‘转换’等级
              // 调用 f(Derived&)

因为转换序列的分级只会操作类型和值类别，所以就分级而言，[位域](https://zh.cppreference.com/w/cpp/language/bit_field "cpp/language/bit field")能绑定到引用形参，但如果选择了这个函数，那么程序非良构。

1) 标准转换序列始终*优于* ﻿用户定义转换序列或省略号转换序列。

2) 用户定义转换序列始终*优于* ﻿[省略号转换](https://zh.cppreference.com/w/cpp/language/variadic_arguments "cpp/language/variadic arguments")序列

3) 标准转换序列 `S1` *优于* ﻿标准转换序列 `S2`，条件是

a) `S1` 是 `S2` 的真子序列，排除左值变换。恒等转换序列被认为是任何非恒等转换的子序列；

b) 或若非如此，`S1` 的等级优于 `S2` 的等级；

c) 或若非如此，`S1` 和 `S2` 都绑定到某个引用形参，而它并非某个引用限定的成员函数的隐式对象形参，且 `S1` 绑定右值引用到右值而 `S2` 绑定左值引用到右值

int i;
int f1();
 
int g(const int&);  // 重载 #1
int g(const int&&); // 重载 #2
 
int j \= g(i);    // 左值 int -> const int& 是仅有的合法转换
int k \= g(f1()); // 右值 int -> const int&& 优于 右值 int -> const int&

d) 或若非如此，`S1` 和 `S2` 都绑定到引用形参，且 `S1` 绑定左值引用到函数而 `S2` 绑定右值引用到函数。

int f(void(&)());  // 重载 #1
int f(void(&&)()); // 重载 #2
 
void g();
int i1 \= f(g); // 调用 #1

e) 或若非如此，`S1` 和 `S2` 都绑定到仅在顶层 cv 限定性有别的引用形参，而 `S1` 的类型比 `S2` 的 cv 限定性*更少*。

int f(const int &); // 重载 #1
int f(int &);       // 重载 #2（都是引用）
 
int g(const int &); // 重载 #1
int g(int);         // 重载 #2
 
int i;
int j \= f(i); // 左值 i -> int& 优于 左值 int -> const int&
              // 调用 f(int&)
int k \= g(i); // 左值 i -> const int& 排行为准确匹配
              // 左值 i -> 右值 int 排行为准确匹配
              // 有歧义的重载：编译错误

f) 或若非如此，S1 与 S2 仅在限定性转换有区别，且

<table class="t-rev-begin"><tbody><tr class="t-rev t-until-cxx20"><td><p><code>S1</code> 的结果的 cv 限定是 <code>S2</code> 的结果的 cv 限定的真子集<span class="t-rev-inl t-until-cxx11"><span>，并且 <code>S1</code> 不是<a href="https://zh.cppreference.com/w/cpp/language/string_literal#.E6.B3.A8.E8.A7.A3" title="cpp/language/string literal">已弃用的字符串字面量数组到指针转换</a></span> <span><span class="t-mark-rev t-until-cxx11">(C++11 前)</span></span></span>。</p></td><td><span class="t-mark-rev t-until-cxx20">(C++20 前)</span></td></tr><tr class="t-rev t-since-cxx20"><td><p>能通过限定性转换将 <code>S1</code> 的结果转换为 <code>S2</code> 的结果。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

int f(const int\*);
int f(int\*);
 
int i;
int j \= f(&i); // &i -> int\* 优于 &i -> const int\*，调用 f(int\*)

4) 用户定义转换序列 `U1` *优于* ﻿用户定义转换序列 `U2`，如果它们调用相同的构造函数/用户定义转换函数，或以聚合初始化初始化相同的类，而任一情况下 `U1` 中的第二标准转换序列优于 `U2` 中的第二标准转换序列：

struct A
{
    operator short(); // 用户定义转换函数
} a;
 
int f(int);   // 重载 #1
int f(float); // 重载 #2
 
int i \= f(a); // A -> short，后随 short -> int（等级为‘提升’）
              // A -> short，后随 short -> float（等级为‘转换’）
              // 调用 f(int)

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11 t-until-cxx20"><td><p><span class="t-li">6)</span> 列表初始化序列 <code>L1</code> <i>优于</i> ﻿列表初始化序列 <code>L2</code>，如果对应形参是到数组的引用且 L1 转换到“N1 个 T 的数组”，L2 转换到“N2 个 T 的数组”，而 N1 小于 N2。</p></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span><br><span class="t-mark-rev t-until-cxx20">(C++20 前)</span></td></tr><tr class="t-rev t-since-cxx20"><td><div class="t-li1"><p><span class="t-li">6)</span> 列表初始化序列 <code>L1</code> <i>优于</i> ﻿列表初始化序列 <code>L2</code>，如果 L1 与 L2 均转换到相同元素类型的数组，且</p><ul><li>L1 所初始化的元素数 N1 小于 L2 所初始化的元素数 N2，或</li><li>N1 等于 N2，但 L2 转换到未知边界数组而 L1 没有。</li></ul><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw4">void</span> f<span class="br0">(</span><span class="kw4">int</span>    <span class="br0">(</span><span class="sy3">&amp;&amp;</span><span class="br0">)</span><span class="br0">[</span><span class="br0">]</span> <span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 重载 #1</span>
<span class="kw4">void</span> f<span class="br0">(</span><span class="kw4">double</span> <span class="br0">(</span><span class="sy3">&amp;&amp;</span><span class="br0">)</span><span class="br0">[</span><span class="br0">]</span> <span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 重载 #2</span>
<span class="kw4">void</span> f<span class="br0">(</span><span class="kw4">int</span>    <span class="br0">(</span><span class="sy3">&amp;&amp;</span><span class="br0">)</span><span class="br0">[</span><span class="nu0">2</span><span class="br0">]</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 重载 #3</span>
&nbsp;
f<span class="br0">(</span><span class="br0">{</span><span class="nu0">1</span><span class="br0">}</span><span class="br0">)</span><span class="sy4">;</span>        <span class="co1">// #1：由于转换优于 #2，由于边界优于 #3</span>
f<span class="br0">(</span><span class="br0">{</span><span class="nu16">1.0</span><span class="br0">}</span><span class="br0">)</span><span class="sy4">;</span>      <span class="co1">// #2：double -&gt; double 优于 double -&gt; int</span>
f<span class="br0">(</span><span class="br0">{</span><span class="nu16">1.0</span>, <span class="nu16">2.0</span><span class="br0">}</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// #2：double -&gt; double 优于 double -&gt; int</span>
f<span class="br0">(</span><span class="br0">{</span><span class="nu0">1</span>, <span class="nu0">2</span><span class="br0">}</span><span class="br0">)</span><span class="sy4">;</span>     <span class="co1">// #3：-&gt; int[2] 优于 -&gt; int[]，</span>
               <span class="co1">//     而 int -&gt; int 优于 int -&gt; double</span></pre></div></div></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

如果两个转换序列因为拥有相同等级而不可区分，那么应用下列额外规则：

1) 不涉及指针到 bool 或成员指针到 bool 的转换优于涉及这些的转换。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><div class="t-li1"><p><span class="t-li">2)</span> 如果底层类型固定的<a href="https://zh.cppreference.com/w/cpp/language/enum" title="cpp/language/enum">枚举</a>的底层类型与经提升后的底层类型不同，那么提升到它的底层类型的转换优于提升到提升后底层类型的转换。</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw2">enum</span> num <span class="sy4">:</span> <span class="kw4">char</span> <span class="br0">{</span> one <span class="sy1">=</span> <span class="st0">'0'</span> <span class="br0">}</span><span class="sy4">;</span>
<a href="http://zh.cppreference.com/w/cpp/io/cout"><span class="kw1762">std::<span class="me2">cout</span></span></a> <span class="sy1">&lt;&lt;</span> num<span class="sy4">::</span><span class="me2">one</span><span class="sy4">;</span> <span class="co1">// '0'，而不是 48</span></pre></div></div></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx23"><td><div class="t-li1"><p><span class="t-li">3)</span> 在浮点类型 <code>FP1</code> 和浮点类型 <code>FP2</code> 之间任一方向的转换，在满足以下条件时优于 <code>FP1</code> 和算术类型 <code>T3</code> 之间相同方向的转换：</p><ul><li><code>FP1</code> 的<a href="https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions#.E6.B5.AE.E7.82.B9.E8.BD.AC.E6.8D.A2.E7.AD.89.E7.BA.A7" title="cpp/language/usual arithmetic conversions">浮点转换等级</a>等于 <code>FP2</code> 的浮点转换等级，并且<ul><li><code>T3</code> 不是浮点类型，或者</li><li><code>T3</code> 是浮点转换等级与 <code>FP1</code> 的浮点转换等级不同的浮点类型，或者</li><li><code>FP2</code> 的<a href="https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions#.E6.B5.AE.E7.82.B9.E8.BD.AC.E6.8D.A2.E5.AD.90.E7.AD.89.E7.BA.A7" title="cpp/language/usual arithmetic conversions">浮点转换子等级</a>高于 <code>T3</code> 的浮点转换子等级。</li></ul></li></ul></div></td><td><span class="t-mark-rev t-since-cxx23">(C++23 起)</span></td></tr></tbody></table>

4) 派生类指针到基类指针的转换优于派生类指针到 void 指针的转换，而基类指针到 void 的转换优于派生类指针到 void 指针的转换。

5) 如果 `Mid`（直接或间接）从 `Base` 派生，而 `Derived`（直接或间接）从 `Mid` 派生，那么

a) `Derived*` 到 `Mid*` 优于 `Derived*` 到 `Base*`

b) `Derived` 到 `Mid&` 或 `Mid&&` 优于 `Derived` 到 `Base&` 或 `Base&&`

c) `Base::*` 到 `Mid::*` 优于 `Base::*` 到 `Derived::*`

d) `Derived` 到 `Mid` 优于 `Derived` 到 `Base`

e) `Mid*` 到 `Base*` 优于 `Derived*` 到 `Base*`

f) `Mid` 到 `Base&` 或 `Base&&` 优于 `Derived` 到 `Base&` 或 `Base&&`

g) `Mid::*` 到 `Derived::*` 优于 `Base::*` 到 `Derived::*`

h) `Mid` 到 `Base` 优于 `Derived` 到 `Base`

对有歧义的转换序列的分级与用户定义转换序列相同，因为一个实参的多个转换序列只有在它们涉及不同的用户定义转换时才能存在：

class B;
 
class A { A (B&);};         // 转换构造函数
class B { operator A (); }; // 用户定义转换函数
class C { C (B&); };        // 转换构造函数
 
void f(A) {} // 重载 #1
void f(C) {} // 重载 #2
 
B b;
f(b); // B -> A 经由构造函数或 B -> A 经由函数（有歧义转换）
      // b -> C 经由构造函数（用户定义转换）
      // 重载 #1 和 #2 的转换无法辨别；编译失败

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=16 "编辑章节：列表初始化中的隐式转换序列")\] 列表初始化中的隐式转换序列

在[列表初始化](https://zh.cppreference.com/w/cpp/language/list_initialization "cpp/language/list initialization")中，实参是 花括号初始化器列表，但它不是表达式，所以到就重载决议而言的形参类型的隐式转换序列以下列规则决定：

+   如果形参类型是某聚合体 `X` 且初始化器列表确切地由一个同类型或它的派生类（可有 cv 限定）的元素组成，那么隐式转换序列是将该元素转换到形参类型所要求的序列。
+   否则，如果形参类型是到字符数组的引用且初始化器列表拥有单个元素，元素是类型适当的字符串字面量，那么隐式转换序列是恒等转换。
+   否则，如果形参类型是 [std::initializer\_list](http://zh.cppreference.com/w/cpp/utility/initializer_list)<X\> 且存在从每个初始化器列表元素到 `X` 的非窄化隐式转换，那么就重载决议而言的隐式转换序列是所需的最坏转换。如果 花括号初始化器列表 为空，那么转换序列是恒等转换。

+   否则，如果形参类型是“N 个 T 的数组”（这只对到数组的引用发生），那么初始化器列表必须有 N 个或更少的元素，且所用的隐式转换序列是将列表（或空花括号对，如果 `{}` 小于 N）的每个元素转换到 `T` 所需的最坏隐式转换序列。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><ul><li>否则，如果形参类型是“T 的未知边界数组”（这只对到数组的引用发生），那么所用的隐式转换序列是将列表的每个元素转换到 <code>T</code> 所需的最坏隐式转换序列。</li></ul></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

typedef int IA\[3\];
 
void h(const IA&);
void g(int (&&)\[\])
 
h({1, 2, 3}); // int -> int 恒等转换
g({1, 2, 3}); // C++20 起同上

+   否则，如果形参类型是非聚合类类型 `X`，那么重载决议选取 X 的构造函数 C 以从实参初始化器列表初始化

+   如果 C 是非 initializer\_list 构造函数且而该初始化器列表拥有单个元素，它的类型是可有 cv 限定的 X，那么隐式转换序列具有*准确匹配* ﻿等级。如果该初始化器列表拥有单个元素，它具有可有 cv 限定的派生自 X 的类型，那么隐式转换序列具有*转换* ﻿等级。（注意它和聚合体的区别：聚合体在考虑[聚合初始化](https://zh.cppreference.com/w/cpp/language/aggregate_initialization "cpp/language/aggregate initialization")前直接从单元素初始化器列表进行初始化，而非聚合体在考虑任何其他构造函数之前先考虑 initializer\_list 构造函数）
+   否则，隐式转换序列是以恒等转换作为第二标准转换序列的用户定义转换序列。

如果有多个构造函数可行，但是没有一个优于其他所有构造函数，那么隐式转换序列是有歧义的转换序列。

struct A { A([std::initializer\_list](http://zh.cppreference.com/w/cpp/utility/initializer_list)<int\>); };
void f(A);
 
struct B { B(int, double); };
void g(B);
 
g({'a', 'b'});    // 调用 g(B(int,double))，用户定义转换
// g({1.0, 1,0}); // 错误：double->int 是窄化转换，不能在列表初始化中出现
 
void f(B);
// f({'a', 'b'}); // f(A) 与 f(B) 都是用户定义转换

+   否则，如果形参类型是可按照[聚合初始化](https://zh.cppreference.com/w/cpp/language/aggregate_initialization "cpp/language/aggregate initialization")从初始化器列表初始化的聚合体，那么隐式转换序列是以恒等转换作为第二标准转换序列的用户定义转换序列：

struct A { int m1; double m2; };
 
void f(A);
f({'a', 'b'}); // 调用 f(A(int, double))，用户定义转换

+   否则，如果形参是引用，那么应用引用初始化规则：

struct A { int m1; double m2; };
 
void f(const A&);
f({'a', 'b'}); // 创建临时量，调用 f(A(int, double))。用户定义转换

+   否则，如果形参类型不是类且初始化器列表拥有一个元素，那么隐式转换序列是将该元素转换到形参类型所要求者。
+   否则，如果形参类型不是类且初始化器列表没有元素，那么隐式转换序列是恒等转换。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><p>如果实参是指派初始化器列表，并且形参不是引用，那么只有在形参拥有聚合类型且该类型能按照<a href="https://zh.cppreference.com/w/cpp/language/aggregate_initialization" title="cpp/language/aggregate initialization">聚合初始化</a>的规则从初始化器列表初始化时，转换才可行。此时隐式转换序列是以恒等转换作为第二标准转换序列的用户定义转换序列。</p><p>如果在重载决议后，聚合体各成员的声明顺序与所选择的重载不匹配，那么形参的初始化非良构。</p><div class="mw-geshi" dir="ltr"><pre class="de1"><span class="kw1">struct</span> A <span class="br0">{</span> <span class="kw4">int</span> x, y<span class="sy4">;</span> <span class="br0">}</span><span class="sy4">;</span>
<span class="kw1">struct</span> B <span class="br0">{</span> <span class="kw4">int</span> y, x<span class="sy4">;</span> <span class="br0">}</span><span class="sy4">;</span>
&nbsp;
<span class="kw4">void</span> f<span class="br0">(</span>A a, <span class="kw4">int</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// #1</span>
<span class="kw4">void</span> f<span class="br0">(</span>B b, ...<span class="br0">)</span><span class="sy4">;</span> <span class="co1">// #2</span>
<span class="kw4">void</span> g<span class="br0">(</span>A a<span class="br0">)</span><span class="sy4">;</span>      <span class="co1">// #3</span>
<span class="kw4">void</span> g<span class="br0">(</span>B b<span class="br0">)</span><span class="sy4">;</span>      <span class="co1">// #4</span>
&nbsp;
<span class="kw4">void</span> h<span class="br0">(</span><span class="br0">)</span>
<span class="br0">{</span>
    f<span class="br0">(</span><span class="br0">{</span>.<span class="me1">x</span> <span class="sy1">=</span> <span class="nu0">1</span>, .<span class="me1">y</span> <span class="sy1">=</span> <span class="nu0">2</span><span class="br0">}</span>, <span class="nu0">0</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// OK：调用 #1</span>
    f<span class="br0">(</span><span class="br0">{</span>.<span class="me1">y</span> <span class="sy1">=</span> <span class="nu0">2</span>, .<span class="me1">x</span> <span class="sy1">=</span> <span class="nu0">1</span><span class="br0">}</span>, <span class="nu0">0</span><span class="br0">)</span><span class="sy4">;</span> <span class="co1">// 错误：选择 #1，初始化由于不匹配的成员顺序失败</span>
    g<span class="br0">(</span><span class="br0">{</span>.<span class="me1">x</span> <span class="sy1">=</span> <span class="nu0">1</span>, .<span class="me1">y</span> <span class="sy1">=</span> <span class="nu0">2</span><span class="br0">}</span><span class="br0">)</span><span class="sy4">;</span>    <span class="co1">// 错误：在 #3 和 #4 间有歧义</span>
<span class="br0">}</span></pre></div></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=17 "编辑章节：缺陷报告")\] 缺陷报告

下列更改行为的缺陷报告追溯地应用于以前出版的 C++ 标准。

| 缺陷报告 | 应用于 | 出版时的行为 | 正确行为 |
| --- | --- | --- | --- |
| [CWG 1](https://cplusplus.github.io/CWG/issues/1.html) | C++98 | 选择可能有（来自不同作用域的）不同默认实参的相同函数时的行为未指明 | 此时程序非良构 |
| [CWG 83](https://cplusplus.github.io/CWG/issues/83.html) | C++98 | 从字符串字面量到 char\* 的转换序列优于从它  
到 const char\* 的转换序列，即使前者已弃用 | 降低该已弃用的转换序列的排行  
（该隐式转换已在 C++11 移除） |
| [CWG 162](https://cplusplus.github.io/CWG/issues/162.html) | C++98 | 在 `&F(args)` 的场合下，`F` 命名的重载集包含非静态成员函数时不合法 | 该场合下在重载决议选择了  
非静态成员函数时才不合法 |
| [CWG 280](https://cplusplus.github.io/CWG/issues/280.html) | C++98 | 不会将在不可访问基类中声明的转换函数  
对应的代表调用函数添加到候选函数集 | 移除该访问约束，但如果重载决议选中了  
代表调用函数而它对应的转换函数  
无法被调用，那么程序非良构 |
| [CWG 415](https://cplusplus.github.io/CWG/issues/415.html) | C++98 | 当函数模板成为候选时，它的特化会通过模板实参推导进行实例化 | 此时不会进行实例化，  
改为合成这些特化的声明 |
| [CWG 495](https://cplusplus.github.io/CWG/issues/495.html) | C++98 | 但实参的隐式转换一样好时，非模板转换函数总是  
优于转换函数模板，即使后者的标准转换序列更好 | 先比较标准转换序列，再比较特化程度 |
| [CWG 1307](https://cplusplus.github.io/CWG/issues/1307.html) | C++11 | 未指定基于数组大小的重载决议 | 可能时较短的数组较好 |
| [CWG 1328](https://cplusplus.github.io/CWG/issues/1328.html) | C++11 | 在绑定引用到转换结果时，候选函数的确定方法不明确 | 使之明确 |
| [CWG 1374](https://cplusplus.github.io/CWG/issues/1374.html) | C++98 | 在比较标准转换序列时会先检查限定性转换，再检查引用绑定 | 调换顺序 |
| [CWG 1385](https://cplusplus.github.io/CWG/issues/1385.html) | C++11 | 声明带有引用限定符的非 explicit 的用户定义转换函数没有对应的代表函数 | 它现在有对应的代表函数 |
| [CWG 1467](https://cplusplus.github.io/CWG/issues/1467.html) | C++11 | 略去了聚合体和数组的同类型列表初始化 | 定义这种初始化 |
| [CWG 1601](https://cplusplus.github.io/CWG/issues/1601.html) | C++11 | 从 enum 转换到它的底层类型不偏好固定的底层类型 | 底层类型比提升后的该类型更受偏好 |
| [CWG 1608](https://cplusplus.github.io/CWG/issues/1608.html) | C++98 | 实参类型是 `T1` 的一元运算符 `@` 的成员  
候选集在 `T1` 是当前正在定义的类时为空 | 此时该集是 `T1::operator@`  
的有限定名字查找的结果 |
| [CWG 1687](https://cplusplus.github.io/CWG/issues/1687.html) | C++98 | 当重载决议选中内建候选时，操作数的转换没有限制 | 只转换类类型操作数，并  
禁用第二段标准转换序列 |
| [CWG 2052](https://cplusplus.github.io/CWG/issues/2052.html) | C++98 | 非良构的合成函数模板特化也会加入候选集，导致程序非良构 | 不会加入候选集 |
| [CWG 2076](https://cplusplus.github.io/CWG/issues/2076.html) | C++11 | [CWG 问题 1467](https://cplusplus.github.io/CWG/issues/1467.html) 的解决方案导致列表初始化中双层  
初始化器列表中的单个初始化器也适用用户定义转换 | 此时不适用用户定义转换 |
| [CWG 2137](https://cplusplus.github.io/CWG/issues/2137.html) | C++11 | 从 `{X`} 列表初始化 `X` 时，初始化器列表构造函数输给复制构造函数 | 非聚合体首先考虑初始化器列表 |
| [CWG 2273](https://cplusplus.github.io/CWG/issues/2273.html) | C++11 | 继承和非继承构造函数之间没有决胜规则 | 非继承构造函数胜出 |
| [CWG 2673](https://cplusplus.github.io/CWG/issues/2673.html) | C++20 | 拥有的形参列表与非成员重写候选的相同的内建候选会添加到内建候选列表 | 不会添加 |
| [CWG 2712](https://cplusplus.github.io/CWG/issues/2712.html) | C++98 | 当考虑内建的赋值运算符时，要求首个形参  
不能绑定临时量，但实际上已经无法绑定[\[1\]](#cite_note-1) | 移除该多余要求 |
| [CWG 2713](https://cplusplus.github.io/CWG/issues/2713.html) | C++20 | 有关指派初始化器列表的转换限制即使在形参是引用时也会应用 | 此时不会限制 |
| [CWG 2789](https://cplusplus.github.io/CWG/issues/2789.html) | C++23 | 在比较形参类型列表时也会包含显式对象形参 | 不包含 |
| [P2468R2](https://wg21.link/P2468R2) | C++20 | a !\= b 即使在有匹配的 operator!\= 的情况下  
也会添加基于 operator\== 的重写候选 | 此时不会添加重写候选 |

1.  [↑](#cite_ref-1) 内建的赋值运算符的首个形参的类型是“到可有 volatile 限定的类型 `T` 的引用”。具有该类型的引用无法绑定临时量。

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=18 "编辑章节：引用")\] 引用

+   C++23 标准（ISO/IEC 14882:2023）：

+   12.2 Overload resolution \[over.match\]

+   C++20 标准（ISO/IEC 14882:2020）：

+   12.4 Overload resolution \[over.match\]

+   C++17 标准（ISO/IEC 14882:2017）：

+   16.3 Overload resolution \[over.match\]

+   C++14 标准（ISO/IEC 14882:2014）：

+   13.3 Overload resolution \[over.match\]

+   C++11 标准（ISO/IEC 14882:2011）：

+   13.3 Overload resolution \[over.match\]

+   C++03 标准（ISO/IEC 14882:2003）：

+   13.3 Overload resolution \[over.match\]

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/overload_resolution&action=edit&section=19 "编辑章节：参阅")\] 参阅

+   [名字查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")
+   [实参依赖查找](https://zh.cppreference.com/w/cpp/language/adl "cpp/language/adl")
+   [模板实参推导](https://zh.cppreference.com/w/cpp/language/template_argument_deduction "cpp/language/template argument deduction")
+   [SFINAE](https://zh.cppreference.com/w/cpp/language/sfinae "cpp/language/sfinae")