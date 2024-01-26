## 赋值运算符

---

赋值运算符修改对象的值。

| 运算符名 | 语法 | [可重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators") | 原型示例（对于 class T） |
| --- | --- | --- | --- |
| 简单赋值 | `a = b` | 是 | `T::operator =(const T2& b) void;` |
| 加法赋值 | `a += b` | 是 | `T::operator +=(const T2& b) void;` |
| 减法赋值 | `a -= b` | 是 | `T::operator -=(const T2& b) void;` |
| 乘法赋值 | `a *= b` | 是 | `T::operator *=(const T2& b) void;` |
| 除法赋值 | `a /= b` | 是 | `T::operator /=(const T2& b) void;` |
| 取模赋值 | `a %= b` | 是 | `T::operator %=(const T2& b) void;` |
| 逐位与赋值 | `a &= b` | 是 | `T::operator &=(const T2& b) void;` |
| 逐位或赋值 | `a \|= b` | 是 | `T::operator \|=(const T2& b) void;` |
| 逐位异或赋值 | `a ^= b` | 是 | `T::operator ^=(const T2& b) void;` |
| 逐位左移赋值 | `a <<= b` | 是 | `T::operator <<=(const T2& b) void;` |
| 逐位右移赋值 | `a >>= b` | 是 | `T::operator >>=(const T2& b) void;` |

**注意**  

+   所有内建赋值运算符都返回`void`，[用户定义重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")也应返回 `void`，从而能以与内建版本相同的方式使用用户定义运算符。

+   `T2` 可以是包含 `T` 在内的任意类型


### 解释
~~*复制赋值运算符*以 `b` 内容的副本替换对象 `a` 的内容（不修改 `b`）。对于类类型，这是一种特殊成员函数，描述见[复制赋值运算符](https://zh.cppreference.com/w/cpp/language/copy_assignment "cpp/language/copy assignment")。~~

~~移动~~赋值运算符以 `b` 的内容替换对象 `a` 的内容，并尽可能避免复制，如果`b`是[左值]()，会隐式转换为为[亡值]()。

出现在直接赋值运算符右侧的左值表达式会隐式转换为亡值。

对于类类型，这是一种特殊成员函数，描述见[移动赋值运算符]("https://zh.cppreference.com/w/cpp/language/move_assignment" "cpp/language/move assignment")。

~~对于非类类型，对复制与移动赋值不加以区分，它们都被称作*直接赋值*。~~

*复合赋值*运算符以 `a` 的值和 `b` 的值间的二元运算结果替换对象 `a` 的内容。

#### 直接赋值

直接赋值表达式的形式为

- 左操作数`=`右操作数

- 左操作数`=` `{}`

- 左操作数`=` `{`右操作数`}`

对于内建运算符，左操作数 拥有`mut` 标量类型，而 右操作数 必须能隐式转换到 左操作数 的类型。

直接赋值运算符期待以一个可修改左值作为它的左操作数，以一个右值表达式或*花括号初始化器列表* 作为它的右操作数，赋值运算符没有返回值。

当右操作数为左值时， 直接赋值运算符会将右操作数[移动]()到左操作数，并将右操作数变为[亡值]()。

对于非类类型，首先将右操作数[隐式转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion "cpp/language/implicit conversion")到左操作数的无 cv 限定的类型，然后复制它的值到左操作数所标识的对象中。

当左操作数拥有引用类型时，赋值运算符修改被引用的对象。

如果左右操作数标识的对象之间有重叠，那么行为未定义（除非二者严格重叠且类型相同）。

当右运算数是<i>花括号初始化器列表 (brace-init-list)</i>时

- 如果表达式 `E1` 拥有标量类型，那么

    - 表达式 `E1{}` 与 `E1 = T`等价，其中 `T` 是 `E1` 的类型。

    - 表达式`E1={E2}` 与 `E1=T{E2}` 等价，其中 `T` 是 `E1` 的类型。

- 如果表达式 `E1` 拥有类类型，那么语法 `E1={args...}` 会生成以<i>花括号初始化器列表</i>为实参对赋值运算符的一次调用，然后遵循<a href="https://zh.cppreference.com/w/cpp/language/overload_resolution" title="cpp/language/overload resolution">重载决议</a>规则选取适合的赋值运算符。需要注意的是，如果以某个非类类型为实参的非模板赋值运算符可用，那么它胜过 `E1 = {}` 中的复制/移动赋值，这是因为从 `{}` 到非类类型属于<a href="https://zh.cppreference.com/w/cpp/language/overload_resolution#.E5.88.97.E8.A1.A8.E5.88.9D.E5.A7.8B.E5.8C.96.E4.B8.AD.E7.9A.84.E9.9A.90.E5.BC.8F.E8.BD.AC.E6.8D.A2.E5.BA.8F.E5.88.97" title="cpp/language/overload resolution">恒等转换</a>，它优先于从 `{}` 到类类型的用户定义转换。

以 volatile 限定的非类类型左值为内建直接赋值运算符的左操作数被弃用，除非该赋值表达式在<a href="https://zh.cppreference.com/w/cpp/language/expressions#.E4.B8.8D.E6.B1.82.E5.80.BC.E8.A1.A8.E8.BE.BE.E5.BC.8F" title="cpp/language/expressions">不求值语境</a>或是<a href="https://zh.cppreference.com/w/cpp/language/expressions#.E5.BC.83.E5.80.BC.E8.A1.A8.E8.BE.BE.E5.BC.8F" title="cpp/language/expressions">弃值表达式</a>中出现。

[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对于每个类型 `T`，下列函数签名参与重载决议：

- `T*& operator=(T*&, T*);`

- `T*volatile & operator=(T*volatile &, T*);`

对于每个枚举或成员指针类型 `T`（可有 volatile 限定），下列函数签名参与重载决议：

对于每对 A1 和 A2，其中 A1 是算术类型（可有 volatile 限定）而 A2 是提升后的算术类型，下列函数签名参与重载决议：

####  示例
```cpp
#include <iostream>
 
int main()
{
    int n \= 0;        // 不是赋值
 
    n \= 1;            // 直接赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << ' ';
 
    n \= {};           // 零初始化，然后赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << ' ';
 
    n \= 'a';          // 整型提升，然后赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << ' ';
 
    n \= {'b'};        // 显式转型，然后赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << ' ';
 
    n \= 1.0;          // 浮点转换，然后赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << ' ';
 
//  n = {1.0};        // 编译错误（窄化转换）
 
    int& r \= n;       // 不是赋值
    int\* p;
 
    r \= 2;            // 通过引用赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << n << '\\n';
 
    p \= &n;           // 直接赋值
    p \= nullptr;      // 空指针转换，然后赋值
 
    struct { int a; [std::string](http://zh.cppreference.com/w/cpp/string/basic_string) s; } obj;
    obj \= {1, "abc"}; // 从花括号初始化器列表赋值
    [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << obj.a << ':' << obj.s << '\\n';
}
```
#### 复合赋值

复合赋值表达式的形式为

<table class="t-sdsc-begin"><tbody><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <span class="t-spar">运算符</span> <span class="t-spar">右操作数</span></td><td>(1)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <span class="t-spar">运算符</span> `<b>{}</b>`</td><td>(2)</td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <span class="t-spar">运算符</span> `<b>{ </b>`<span class="t-spar">右操作数</span> `<b>}</b>`</td><td>(3)</td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr></tbody></table>

<table class="t-par-begin"><tbody><tr class="t-par"><td><span class="t-spar">运算符</span></td><td>-</td><td><span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">*</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">/</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">%</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">+</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">-</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy1">&lt;&lt;=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy1">&gt;&gt;=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy3">&amp;</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy3">^</span><span class="sy1">=</span></span></span>、<span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy3">|</span><span class="sy1">=</span></span></span> 之一</td></tr><tr class="t-par"><td><span class="t-spar">左操作数</span></td><td>-</td><td>对于内建运算符，<span class="t-spar">左操作数</span> 可具有任何算术类型，但如果 <span class="t-spar">运算符</span> 是 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">+</span><span class="sy1">=</span></span></span> 或 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="sy2">-</span><span class="sy1">=</span></span></span>，那么它也接受指针类型，并与 + 和 - 有相同限制</td></tr><tr class="t-par"><td><span class="t-spar">右操作数</span></td><td>-</td><td>对于内建运算符，<span class="t-spar">右操作数</span> 必须可隐式转换成 <span class="t-spar">左操作数</span></td></tr></tbody></table>

每个内建复合赋值运算符表达式 E1 op\= E2（其中 E1 是可修改左值表达式，而 E2 是右值表达式或*花括号初始化器列表* (C++11 起)）的行为与表达式 E1 \= E1 op E2 的行为严格相同，但只对表达式 `E1` 进行一次求值，并且对于顺序不确定的函数而言是一次单个操作（例如 f(a +\= b, g()) 中，从 g() 内来看，+= 要么完全未开始，要么已完成）。

在[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对每对 A1 和 A2，其中 A1 是算术类型（可有 volatile 限定）而 A2 是提升后的算术类型，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A1<span class="sy3">&amp;</span> operator<span class="sy2">*</span><span class="sy1">=</span><span class="br0">(</span>A1<span class="sy3">&amp;</span>, A2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A1<span class="sy3">&amp;</span> operator<span class="sy2">/</span><span class="sy1">=</span><span class="br0">(</span>A1<span class="sy3">&amp;</span>, A2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A1<span class="sy3">&amp;</span> operator<span class="sy2">+</span><span class="sy1">=</span><span class="br0">(</span>A1<span class="sy3">&amp;</span>, A2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A1<span class="sy3">&amp;</span> operator<span class="sy2">-</span><span class="sy1">=</span><span class="br0">(</span>A1<span class="sy3">&amp;</span>, A2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

对于每对 I1 与 I2，其中 I1 是整型类型（可有 volatile 限定）而 I2 是提升后的整型类型，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy2">%</span><span class="sy1">=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy1">&lt;&lt;=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy1">&gt;&gt;=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy3">&amp;</span><span class="sy1">=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy3">^</span><span class="sy1">=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">I1<span class="sy3">&amp;</span> operator<span class="sy3">|</span><span class="sy1">=</span><span class="br0">(</span>I1<span class="sy3">&amp;</span>, I2<span class="br0">)</span><span class="sy4">;</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

对于每个可有 cv 限定的对象类型 `T`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span><span class="sy3">&amp;</span> operator<span class="sy2">+</span><span class="sy1">=</span><span class="br0">(</span>T<span class="sy2">*</span><span class="sy3">&amp;</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span><span class="sy4">;</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span><span class="sy3">&amp;</span> operator<span class="sy2">-</span><span class="sy1">=</span><span class="br0">(</span>T<span class="sy2">*</span><span class="sy3">&amp;</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span><span class="sy4">;</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span><span class="kw4">volatile</span> <span class="sy3">&amp;</span> operator<span class="sy2">+</span><span class="sy1">=</span><span class="br0">(</span>T<span class="sy2">*</span><span class="kw4">volatile</span> <span class="sy3">&amp;</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span><span class="sy4">;</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span><span class="kw4">volatile</span> <span class="sy3">&amp;</span> operator<span class="sy2">-</span><span class="sy1">=</span><span class="br0">(</span>T<span class="sy2">*</span><span class="kw4">volatile</span> <span class="sy3">&amp;</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span><span class="sy4">;</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

#### 示例

### 参阅

[运算符优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")

[运算符重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")
