## 算数运算符

---

返回特定算术运算的结果。

| 运算符名 | 语法 | 原型示例（对于 class T） |
| --- | --- | --- |
| 类内定义 | 类外定义 |
| 一元加 | +a | T T::operator+() const; | T operator+(const T &a); |
| 一元减 | \-a | T T::operator\-() const; | T operator\-(const T &a); |
| 加法 | a + b | T T::operator+(const T2 &b) const; | T operator+(const T &a, const T2 &b); |
| 减法 | a \- b | T T::operator\-(const T2 &b) const; | T operator\-(const T &a, const T2 &b); |
| 乘法 | a \* b | T T::operator\*(const T2 &b) const; | T operator\*(const T &a, const T2 &b); |
| 除法 | a / b | T T::operator/(const T2 &b) const; | T operator/(const T &a, const T2 &b); |
| 模 | a % b | T T::operator%(const T2 &b) const; | T operator%(const T &a, const T2 &b); |
| 逐位非 | ~a | T T::operator~() const; | T operator~(const T &a); |
| 逐位与 | a & b | T T::operator&(const T2 &b) const; | T operator&(const T &a, const T2 &b); |
| 逐位或 | a | b | T T::operator|(const T2 &b) const; | T operator|(const T &a, const T2 &b); |
| 逐位异或 | a ^ b | T T::operator^(const T2 &b) const; | T operator^(const T &a, const T2 &b); |
| 逐位左移 | a << b | T T::operator<<(const T2 &b) const; | T operator<<(const T &a, const T2 &b); |
| 逐位右移 | a \>> b | T T::operator\>>(const T2 &b) const; | T operator\>>(const T &a, const T2 &b); |
| 
**注解**  

+   此表中的所有运算符都[可重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")。
+   所有内建运算符都返回值，而大多数[用户定义重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")也会返回值，因此用户定义运算符能以与内建版本相同的方式使用。不过，用户定义重载中，任何类型都可以作为返回类型（包括 void）。特别是，operator<< 和 operator\>> 作为流插入和流提取的重载所返回的是 `T&`。
+   `T2` 可以是包括 `T` 在内的任何类型。

 |

### 通用解释

所有内建算术运算符计算特定算术运算的结果，并返回它的结果。不修改实参。

#### 转换

目前没有实现整形提升

如果传递给内建算术运算符的操作数是整数或无作用域枚举类型，那么在所有其他操作前（但在左值到右值转换后，如果适用）会对操作数实施[整数提升](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E6.95.B4.E6.95.B0.E6.8F.90.E5.8D.87 "cpp/language/implicit conversion")。如果操作数之一具有数组或函数类型，那么实施[数组到指针](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E6.95.B0.E7.BB.84.E5.88.B0.E6.8C.87.E9.92.88.E8.BD.AC.E6.8D.A2 "cpp/language/implicit conversion")和[函数到指针](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E5.87.BD.E6.95.B0.E5.88.B0.E6.8C.87.E9.92.88 "cpp/language/implicit conversion")转换。

对于（除移位之外的）二元运算符，当提升后的操作数拥有不同类型时，实施[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")。

#### 溢出

无符号整数算术始终进行 modulo 2n  
运算，其中 n 是该整数的位数。例如对于 unsigned int，向 [UINT\_MAX](https://zh.cppreference.com/w/cpp/types/climits "cpp/types/climits") 加一得到 ​0​，而从 ​0​ 减一得到 [UINT\_MAX](https://zh.cppreference.com/w/cpp/types/climits "cpp/types/climits")。

有符号整数算术运算溢出（结果类型无法容纳它的结果）时，行为未定义，此类操作可能会表现为：

+   按照表示法的规则（典型为[补码](https://zh.cppreference.com/w/cpp/language/types#.E5.80.BC.E7.9A.84.E8.8C.83.E5.9B.B4 "cpp/language/types")）发生回绕，
+   在某些平台上或由于编译器选项（例如 GCC 和 Clang 中的 `-ftrapv`）引发陷阱，
+   在某些时候饱和到最小或最大值（在许多 DSP 上），
+   完全[被编译器优化掉](https://blog.llvm.org/2011/05/what-every-c-programmer-should-know_14.html)。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=4 "编辑章节：浮点数环境")\] 浮点数环境

如果支持 [#pragma STDC FENV\_ACCESS](https://zh.cppreference.com/w/cpp/preprocessor/impl "cpp/preprocessor/impl") 并设置它为 `ON`，那么所有浮点数算术运算符都服从当前浮点数[舍入方向](https://zh.cppreference.com/w/cpp/numeric/fenv/FE_round "cpp/numeric/fenv/FE round")，并报告 [math\_errhandling](https://zh.cppreference.com/w/cpp/numeric/math/math_errhandling "cpp/numeric/math/math errhandling") 中指定的浮点数算术错误，除非其在[静态初始化器](https://zh.cppreference.com/w/cpp/language/initialization#.E9.9D.9E.E5.B1.80.E9.83.A8.E5.8F.98.E9.87.8F "cpp/language/initialization")中发生（此时不引发浮点数异常，且舍入模式是最近舍入）。

#### 浮点数缩略

除非支持 [#pragma STDC FP\_CONTRACT](https://zh.cppreference.com/w/cpp/preprocessor/impl "cpp/preprocessor/impl") 并设置它为 `OFF`，否则所有浮点数算术都可以如同它的中间结果拥有无限范围和精度一般进行，即允许实施省略舍入误差和浮点数异常的优化。例如 C++ 允许以单条融合乘加 CPU 指令来实现 (x \* y) + z，或把 a \= x \* x \* x \* x; 优化为 tmp \= x \* x; a \= tmp \* tmp。

无关乎缩略，浮点数算术的中间结果可拥有与它的类型所指定不同的范围和精度，见 [FLT\_EVAL\_METHOD](https://zh.cppreference.com/w/cpp/types/climits/FLT_EVAL_METHOD "cpp/types/climits/FLT EVAL METHOD")。

正式来讲，C++ 标准不在浮点数运算的精度上做任何保证。

### 一元算术运算符

一元算术运算符表达式的形式为

1) `+ exp` 一元加（提升）。

2) `- exp` 一元减（取反）。

一元 `+` 和 `-` 运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")高于所有二元算术运算符，所以*表达式*无法包含位于顶层的二元算术运算符。这些运算符从右到左关联：

```
+a - b; // 等价于 (+a) - b，而不是 +(a - b)
-c + d; // 等价于 (-c) + d，而不是 -(c + d)
 
+-e; // 等价于 +(-e)，在 e 是内建类型的情况下一元 + 是无操作，
     // 因为任何提升操作都已经在取反过程中进行
```

#### 内建一元算术运算符

1) 对于内建一元加运算符，*表达式*必须是算术、无作用域枚举或指针类型的纯右值。如果*表达式*具有整数或无作用域枚举类型，那么对它实施整数提升。结果的类型是（可能为提升后的）表达式 ﻿的类型。

内建的提升操作的结果是表达式 ﻿的值。如果操作数为提升后的整数或指针类型的纯右值，则内建的一元加运算为无操作。否则，整数提升或左值到右值、数组到指针、函数到指针或用户定义转换会更改操作数的类型或值类别。例如在一元加表达式中 char 会被转换为 int ，而非泛型且无捕获的 [lambda 表达式](https://zh.cppreference.com/w/cpp/language/lambda "cpp/language/lambda")会被转换为函数指针 (C++11 起)。

2) 对于内建一元减运算符，表达式 ﻿必须是一个算术或无作用域枚举类型的纯右值。对表达式 ﻿实施整数提升，结果的类型是提升后的表达式 ﻿的类型。

内建的取反操作的结果是提升后的表达式 ﻿的相反数。对于无符号的 a，\-a 的值是 2N  
\-a，其中 N 是提升后的位数。

+   也就是说，结果是操作数的补码（其中操作数和结果都视为无符号数）。

#### 重载

在[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对于每个无 cv 限定的提升后算术类型 `A` 和每个类型 `T`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A operator<span class="sy2">+</span><span class="br0">(</span>A<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span> operator<span class="sy2">+</span><span class="br0">(</span>T<span class="sy2">*</span><span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">A operator<span class="sy2">-</span><span class="br0">(</span>A<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

```
#include <iostream>
 
int main()
{
    char c = 0x6a;
    int n1 = 1;
    unsigned char n2 = 1;
    unsigned int n3 = 1;
    std::cout << "char：" << c << " int：" << +c << '\n'
              << "-1，当 1 的类型是 signed 时：" << -n1 << '\n'
              << "-1，当 1 的类型是 unsigned char 时：" << -n2 << '\n'
              << "-1，当 1 的类型是 unsigned int 时：" << -n3 << '\n';
    char a[3];
    std::cout << "数组大小：" << sizeof a << '\n'
              << "指针大小：" << sizeof +a << '\n';
}
```

输出：

```
char：j int：106
-1，当 1 的类型是 signed 时：-1
-1，当 1 的类型是 unsigned char 时：-1
-1，当 1 的类型是 unsigned int 时：4294967295
数组大小：3
指针大小：8
```

### 加性运算符

二元加性运算符表达式的形式为

<table class="t-sdsc-begin"><tbody><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>+</b></code> <span class="t-spar">右操作数</span></td><td>(1)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>-</b></code> <span class="t-spar">右操作数</span></td><td>(2)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr></tbody></table>

1) 二元加（加法）。

2) 二元减（减法）。

二元 `+` 和 `-` 运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")高于 `*`、`/` 和 `%` 以外的所有其他二元算术运算符。这些运算符从左到右结合：

```
a + b * c;  // 等价于 a + (b * c) ，而不是 (a + b) * c
d / e - f;  // 等价于 (d / e) - f ，而不是 d / (e - f)
g + h >> i; // 等价于 (g + h) >> i，而不是 g + (h >> i)
 
j - k + l - m; // 等价于 ((j - k) + l) - m
```

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=10 "编辑章节：内建加性运算符")\] 内建加性运算符

对于内建二元加和二元减运算符，左操作数 ﻿和右操作数 ﻿都必须是纯右值，并且必须满足以下条件之一：

+   两个操作数都具有算术或无作用域枚举类型。此时会对它们进行[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")。
+   恰有一个操作数具有整数或无作用域枚举类型。此时会对该操作数进行整数提升。

在本段的后续描述中，“操作数”、左操作数 ﻿和右操作数 ﻿均指代转换后或提升后的操作数。

1) 内建加法必须满足以下条件之一：

+   两个操作数都具有算术类型。此时结果是两个操作数的和。
+   一个操作数是指向完全定义的对象类型的指针，而另一个操作数具有整数类型。此时会从该指针加上该整数值（见[指针算术](#.E6.8C.87.E9.92.88.E7.AE.97.E6.9C.AF)）。

2) 内建减法必须满足以下条件之一：

+   两个操作数都具有算术类型。此时结果是两个操作数的差。
+   一个操作数是指向完全定义的对象类型的指针，而另一个操作数具有整数类型。此时会从该指针减去该整数值（见[指针算术](#.E6.8C.87.E9.92.88.E7.AE.97.E6.9C.AF)）。
+   两个操作数都是指向相同的完全定义的对象类型的有 cv 限定或无 cv 限定版本的指针。此时会从左操作数 ﻿减去右操作数（见[指针算术](#.E6.8C.87.E9.92.88.E7.AE.97.E6.9C.AF)）。

如果两个操作数都具有浮点数类型，并且该类型支持 IEEE 浮点数算术（见 [std::numeric\_limits::is\_iec559](https://zh.cppreference.com/w/cpp/types/numeric_limits/is_iec559 "cpp/types/numeric limits/is iec559")），那么会有以下特殊计算结果：

+   如果操作数之一是 NaN，那么结果是 NaN。
+   无穷减无穷是 NaN，并引发 [FE\_INVALID](https://zh.cppreference.com/w/cpp/numeric/fenv/FE_exceptions "cpp/numeric/fenv/FE exceptions")。
+   无穷加负无穷是 NaN，并引发 [FE\_INVALID](https://zh.cppreference.com/w/cpp/numeric/fenv/FE_exceptions "cpp/numeric/fenv/FE exceptions")。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=11 "编辑章节：指针算术")\] 指针算术

当从具有指针类型的表达式 P 加上或减去具有整数类型的表达式 J 时，结果的类型与 P 相同。

+   如果 P 求值为[空指针值](https://zh.cppreference.com/w/cpp/language/pointer#.E7.A9.BA.E6.8C.87.E9.92.88 "cpp/language/pointer")且 J 求值为 ​0​，那么结果是空指针值。
+   否则，如果 P 指向包含 n 个元素的数组对象 x 的第 `i` 个元素，那么给定 J 的值为 j，按以下方式从 P 加上或减去值：

+   表达式 P + J 和 J + P

+   在 i + j 在 `[`​0​`,` n`)` 中时指向 x 的第 `i+j` 个元素，并且
+   在 i + j 是 n 时是 x 的最后一个元素的尾后指针。

+   表达式 P \- J

+   在 i \- j 在 `[`​0​`,` n`)` 中时指向 x 的第 `i-j` 个元素，并且
+   在 i \- j 是 n 时是 x 的最后一个元素的尾后指针。

+   其他 j 值都会导致未定义行为。

+   否则，如果 P 指向完整对象，基类子对象或成员子对象 y，那么给定 J 的值为 j，按以下方式从 P 加上或减去值：

+   表达式 P + J 和 J + P

+   在 j 是 ​0​ 时指向 y，并且
+   在 j 是 1 时是 y 的尾后指针。

+   表达式 P \- J

+   在 j 是 ​0​ 时指向 y，并且
+   在 j 是 \-1 时是 y 的尾后指针。

+   其他 j 值都会导致未定义行为。

+   否则，如果 P 是对象 z 的尾后指针，那么给定 J 的值为 j：

+   如果 z 是指向包含 n 个元素的数组对象，那么按以下方式从 P 加上或减去值：

+   表达式 P + J 和 J + P

+   在 n + j 在 `[`​0​`,` n`)` 中时指向 z 的第 `n+j` 个元素，并且
+   在 j 是 ​0​ 时是 z 的最后一个元素的尾后指针。

+   表达式 P \- J

+   在 n \- j 在 `[`​0​`,` n`)` 中时指向 z 的第 `n-j` 个元素，并且
+   在 j 是 ​0​ 时是 z 的最后一个元素的尾后指针。

+   其他 j 值都会导致未定义行为。

+   否则，按以下方式从 P 加上或减去值：

+   表达式 P + J 和 J + P

+   在 j 是 \-1 时指向 z，并且
+   在 j 是 ​0​ 时是 z 的尾后指针。

+   表达式 P \- J

+   在 j 是 1 时指向 z，并且
+   在 j 是 ​0​ 时是 z 的尾后指针。

+   其他 j 值都会导致未定义行为。

+   否则行为未定义。

当两个指针表达式 P 和 Q 相减时，结果类型是 [std::ptrdiff\_t](https://zh.cppreference.com/w/cpp/types/ptrdiff_t "cpp/types/ptrdiff t")。

+   如果 P 和 Q 都求值为[空指针值](https://zh.cppreference.com/w/cpp/language/pointer#.E7.A9.BA.E6.8C.87.E9.92.88 "cpp/language/pointer")，那么结果是 ​0​。
+   否则，如果 P 和 Q 分别指向相同的数组对象 x 的第 `i` 个和第 `j` 个元素，那么表达式 P \- Q 的值是 i − j。

+   如果 i − j 无法以 [std::ptrdiff\_t](https://zh.cppreference.com/w/cpp/types/ptrdiff_t "cpp/types/ptrdiff t") 表示，那么行为未定义。

+   否则，如果 P 和 Q 指向相同的完整对象，基类子对象或成员子对象，那么结果是 ​0​。
+   否则行为未定义。

这些指针算术运算符使得指针满足[老式随机访问迭代器 (LegacyRandomAccessIterator)](https://zh.cppreference.com/w/cpp/named_req/RandomAccessIterator "cpp/named req/RandomAccessIterator") 的要求。

对于加法和减法，如果 P 或 Q 的类型是“指向（可有 cv 限定的）`T` 的指针”，并且 `T` 和数组元素类型不[相似](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E7.9B.B8.E4.BC.BC.E7.B1.BB.E5.9E.8B "cpp/language/implicit conversion")，那么行为未定义：

```
int arr[5] = {1, 2, 3, 4, 5};
unsigned int *p = reinterpret_cast<unsigned int*>(arr + 1);
unsigned int k = *p; // OK：k 的值是 2
unsigned int *q = p + 1; // 未定义行为：p 指向的是 int，而不是 unsigned int
```

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=12 "编辑章节：重载")\] 重载

在[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对于每对提升后的算术类型 `L` 和 `R` 和每个对象类型 `T`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LR operator<span class="sy2">+</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LR operator<span class="sy2">-</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span> operator<span class="sy2">+</span><span class="br0">(</span>T<span class="sy2">*</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span> operator<span class="sy2">+</span><span class="br0">(</span><a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a>, T<span class="sy2">*</span><span class="br0">)</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp">T<span class="sy2">*</span> operator<span class="sy2">-</span><span class="br0">(</span>T<span class="sy2">*</span>, <a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a><span class="br0">)</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><div><p><span class="mw-geshi cpp source-cpp"><a href="http://zh.cppreference.com/w/cpp/types/ptrdiff_t"><span class="kw108">std::<span class="me2">ptrdiff_t</span></span></a> operator<span class="sy2">-</span><span class="br0">(</span>T<span class="sy2">*</span>, T<span class="sy2">*</span><span class="br0">)</span></span></p></div></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

其中 `LR` 是对 `L` 和 `R` 实施[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")的结果。

```
#include <iostream>
 
int main()
{
    char c = 2;
    unsigned int un = 2;
    int  n = -10;
    std::cout <<  " 2 + (-10)，当 2 的类型是 char 时     = " << c + n << '\n'
              <<  " 2 + (-10)，当 2 的类型是 unsigned 时 = " << un + n << '\n'
              <<  " -10 - 2.12  = " << n - 2.12 << '\n';
 
    char a[4] = {'a', 'b', 'c', 'd'};
    char* p = &a[1];
    std::cout << "指针相加的例子：" << *p << *(p + 2)
              << *(2 + p) << *(p - 1) << '\n';
    char* p2 = &a[4];
    std::cout << "指针的差：" << p2 - p << '\n';
}
```

输出：

```
 2 + (-10)，当 2 的类型是 char 时     = -8
 2 + (-10)，当 2 的类型是 unsigned 时 = 4294967288
 -10 - 2.12  = -12.12
指针相加的例子：bdda
指针的差：3
```

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=13 "编辑章节：乘性运算符")\] 乘性运算符

乘性运算符表达式的形式为

<table class="t-sdsc-begin"><tbody><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>*</b></code> <span class="t-spar">右操作数</span></td><td>(1)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>/</b></code> <span class="t-spar">右操作数</span></td><td>(2)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>%</b></code> <span class="t-spar">右操作数</span></td><td>(3)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr></tbody></table>

1) 乘法。

2) 除法。

3) 取余。

乘性运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")高于所有其他二元算术运算符。这些运算符从左到右结合：

```
a + b * c;  // 等价于 a + (b * c) ，而不是 (a + b) * c
d / e - f;  // 等价于 (d / e) - f ，而不是 d / (e - f)
g % h >> i; // 等价于 (g % h) >> i，而不是 g % (h >> i)
 
j * k / l % m; // 等价于 ((j * k) / l) % m
```

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=14 "编辑章节：内建乘性运算符")\] 内建乘性运算符

对于内建的乘法和除法运算符，两个操作数都必须具有算术或无作用域枚举类型。对于内建的取余运算符，两个操作数都必须具有整数或无作用域枚举类型。对两个操作数应用[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")。

在本段的后续描述中，“操作数”、左操作数 ﻿和右操作数 ﻿均指代转换后的操作数。

1) 内建乘法的结果是两个操作数的积。

2) 内建除法的结果是左操作数 ﻿除以右操作数 ﻿的商。如果右操作数 ﻿为零，那么行为未定义。

如果两个操作数具有整数类型，那么结果是代数商（进行整数除法）：商向零取整（舍弃小数部分）。

3) 内建取余的结果是左操作数 ﻿按整数除法除以右操作数 ﻿的余数。如果右操作数 ﻿为零，那么行为未定义。

如果 a / b 能以结果类型表示，那么 (a / b) \* b + a % b \== a。

如果商 a / b 不能以结果类型表示，那么 `a / b` 和 `a % b` 的行为均未定义（这意味着在补码系统上 [INT\_MIN](http://zh.cppreference.com/w/cpp/types/climits) % \-1 未定义）。

注意：在 [CWG 问题 614](https://cplusplus.github.io/CWG/issues/614.html) 解决（[N2757](https://wg21.link/n2757)）之前，如果二元运算符 % 的一或两个操作数为负，那么余数符由实现定义，因为它取决于整数除法的舍入方向。函数 [std::div](https://zh.cppreference.com/w/cpp/numeric/math/div "cpp/numeric/math/div") 在该情况下提供良好定义的行为。

注意：对于浮点数余数，见 [std::remainder](https://zh.cppreference.com/w/cpp/numeric/math/remainder "cpp/numeric/math/remainder") 和 [std::fmod](https://zh.cppreference.com/w/cpp/numeric/math/fmod "cpp/numeric/math/fmod")。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=15 "编辑章节：重载")\] 重载

在[针对用户定义运算符的重载决议中](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")，对于每对提升后的算术类型 `LA` 与 `RA` 及每对提升后的整数类型 `LI` 与 `RI`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LRA operator<span class="sy2">*</span><span class="br0">(</span>LA, RA<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LRA operator<span class="sy2">/</span><span class="br0">(</span>LA, RA<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LRI operator<span class="sy2">%</span><span class="br0">(</span>LI, RI<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

其中 `LRx` 是对 `Lx` 和 `Rx` 实施[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")的结果。

```
#include <iostream>
 
int main()
{
    char c = 2;
    unsigned int un = 2;
    int  n = -10;
    std::cout <<  "2 * (-10)，当 2 的类型是 char 时     = " << c * n << '\n'
              <<  "2 * (-10)，当 2 的类型是 unsigned 时 = " << un * n << '\n'
              <<  "-10 / 2.12  = " << n / 2.12 << '\n'
              <<  "-10 / 21  = " << n / 21 << '\n'
              <<  "-10 % 21  = " << n % 21 << '\n';
}
```

输出：

```
2 * (-10)，当 2 的类型是 char 时     = -20
2 * (-10)，当 2 的类型是 unsigned 时 = 4294967276
-10 / 2.12  = -4.71698
-10 / 21  = 0
-10 % 21  = -10
```

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=16 "编辑章节：逐位逻辑运算符")\] 逐位逻辑运算符

逐位逻辑运算符表达式的形式为

<table class="t-sdsc-begin"><tbody><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><code><b>~</b></code> <span class="t-spar">右操作数</span></td><td>(1)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>&amp;</b></code> <span class="t-spar">右操作数</span></td><td>(2)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>|</b></code> <span class="t-spar">右操作数</span></td><td>(3)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>^</b></code> <span class="t-spar">右操作数</span></td><td>(4)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr></tbody></table>

1) 逐位非（NOT）。

2) 逐位与（AND）。

3) 逐位或（OR）。

4) 逐位异或（XOR）。

逐位非运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")高于所有二元算术运算符。它从右到左结合：

```
~a - b; // 等价于 (~a) - b，而不是 ~(a - b)
~c * d; // 等价于 (~c) * d，而不是 ~(c * d)
 
~-e; // 等价于 ~(-e)
```

~ 在后随[类型名](https://zh.cppreference.com/w/cpp/language/type#.E7.B1.BB.E5.9E.8B.E7.9A.84.E5.91.BD.E5.90.8D "cpp/language/type")或 [`decltype`](https://zh.cppreference.com/w/cpp/language/decltype "cpp/language/decltype") 说明符 (C++11 起)时在语法上有歧义：它可以是 operator~，也可以是[析构函数](https://zh.cppreference.com/w/cpp/language/destructor "cpp/language/destructor")标识符的开始。该歧义通过将 ~ 视为 operator~ 解决。只有在不能组成 operator~ 的地方才能用 ~ 作为析构函数标识符的开始。

所有其他逐位逻辑运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")低于所有其他二元算术运算符。此外，逐位与的优先级高于逐位异或，逐位异或的优先级高于逐位或。这些运算符从左到右结合：

```
a & b * c;  // 等价于 a & (b * c) ，而不是 (a & b) * c
d / e ^ f;  // 等价于 (d / e) ^ f ，而不是 d / (e ^ f)
g << h | i; // 等价于 (g << h) | i，而不是 g << (h | i)
 
j & k & l; // 等价于 (j & k) & l
m | n ^ o  // 等价于 m | (n ^ o)
```

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=17 "编辑章节：内建逐位逻辑运算符")\] 内建逐位逻辑运算符

对于内建的逐位非运算符，右操作数 ﻿必须是整数或无作用域枚举类型的纯右值，并且会对它进行整数提升。对于其他内建的逐位逻辑运算符，两个操作数都必须具有整数或无作用域枚举类型，并且会对它们进行[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")。

在本段的后续描述中，“操作数”、左操作数 ﻿和右操作数 ﻿均指代转换后或提升后的操作数。

1) 给定操作数为 x，内建逐位非操作的结果为 r。对于 x 的二进制表示的每个系数 x\_i，r 的二进制表示的对应系数 r\_i 在 x\_i 是 ​0​ 时是 1，否则是 ​0​。

+   也就是说，结果是操作数的反码（其中操作数和结果都视为无符号数）。

结果 r 的类型就是操作数 x 的类型。

2-4) 给定操作数为 x 和 y，内建逐位逻辑操作的结果为 r。对于 x 和 y 的二进制表示的每对系数 x\_i 和 y\_i，r 的二进制表示的对应系数 r\_i

2) 在 x\_i 和 y\_i 都是 1 时是 1，否则是 ​0​。

3) 在 x\_i 和 y\_i 其中至少一个是 1 时是 1，否则是 ​0​。

3) 在 x\_i 和 y\_i 其中一个是（但不能两个都是）1 时是 1，否则是 ​0​。

结果 r 的类型就是操作数 x 和 y 的类型。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=18 "编辑章节：重载")\] 重载

在[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对于每对提升后的整数类型 `L` 和 `R`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">R operator~<span class="br0">(</span>R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LR operator<span class="sy3">&amp;</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LR operator<span class="sy3">^</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">LR operator<span class="sy3">|</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

其中 `LR` 是对 `L` 和 `R` 实施[一般算术转换](https://zh.cppreference.com/w/cpp/language/usual_arithmetic_conversions "cpp/language/usual arithmetic conversions")的结果。

```
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
 
int main()
{
    uint16_t mask = 0x00f0;
    uint32_t x0 = 0x12345678;
    uint32_t x1 = x0 | mask;
    uint32_t x2 = x0 & ~mask;
    uint32_t x3 = x0 & mask;
    uint32_t x4 = x0 ^ mask;
    uint32_t x5 = ~x0;
    using bin16 = std::bitset<16>;
    using bin32 = std::bitset<32>;
    std::cout << std::hex << std::showbase
              << "掩码：" << mask << std::setw(39) << bin16(mask) << '\n'
              << "值：" << x0 << std::setw(33) << bin32(x0) << '\n'
              << "设置各位：" << x1 << std::setw(33) << bin32(x1) << '\n'
              << "清除各位：" << x2 << std::setw(33) << bin32(x2) << '\n'
              << "选择各位：" << x3 << std::setw(39) << bin32(x3) << '\n'
              << "异或各位：" << x4 << std::setw(33) << bin32(x4) << '\n'
              << "取反各位：" << x5 << std::setw(33) << bin32(x5) << '\n';
}
```

输出：

```
掩码：0xf0                           0000000011110000
值：0x12345678       00010010001101000101011001111000
设置各位：0x123456f8 00010010001101000101011011111000
清除各位：0x12345608 00010010001101000101011000001000
选择各位：0x70       00000000000000000000000001110000
异或各位：0x12345688 00010010001101000101011010001000
取反各位：0xedcba987 11101101110010111010100110000111
```

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=19 "编辑章节：移位运算符")\] 移位运算符

移位运算符表达式的形式为

<table class="t-sdsc-begin"><tbody><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>&lt;&lt;</b></code> <span class="t-spar">右操作数</span></td><td>(1)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr><tr class="t-sdsc"><td><span class="t-spar">左操作数</span> <code><b>&gt;&gt;</b></code> <span class="t-spar">右操作数</span></td><td>(2)</td><td class="t-sdsc-nopad"></td></tr><tr><td colspan="10" class="t-sdsc-sep"></td></tr></tbody></table>

1) 向左移位。

2) 向右移位。

移位运算符的[优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")高于逐位逻辑运算符，但低于加性和乘性运算符。这些运算符从左到右结合：

```
a >> b * c;  // 等价于 a >> (b * c) ，而不是 (a >> b) * c
d << e & f;  // 等价于 (d << e) & f ，而不是 d << (e & f)
 
g << h >> i; // 等价于 (g << h) >> i，而不是 g << (h >> i)
```

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=20 "编辑章节：内建移位运算符")\] 内建移位运算符

对于内建的移位运算符，两个操作数都必须具有整数或无作用域枚举类型，并且会对它们进行整数提升。

在本段的后续描述中，“操作数”、a、b、左操作数 ﻿和右操作数 ﻿均指代提升后的操作数。

任何情况下，如果右操作数 ﻿的值为负或不小于左操作数 ﻿中的位数，那么行为未定义。

<table class="t-rev-begin"><tbody><tr class="t-rev t-until-cxx20"><td><p>对于无符号 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span>，<span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&lt;&lt;</span> b</span></span> 的值是 <span class="texhtml">a * 2<span class="t-su">b<br></span></span>对 <span class="texhtml">2<span class="t-su">N<br></span></span>取模，其中 <span class="texhtml">N</span> 是返回类型中的位数（即进行逐位左移并舍弃移出目标类型的位）。</p><p>对于有符号的非负 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span>，如果 <span class="texhtml">a * 2<span class="t-su">b<br></span></span>能以返回类型的无符号版本表示，那么将该值<a href="https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E6.95.B4.E6.95.B0.E8.BD.AC.E6.8D.A2" title="cpp/language/implicit conversion">转换</a>到有符号后即是 <span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&lt;&lt;</span> b</span></span> 的值（这使得以 <span class="t-c"><span class="mw-geshi cpp source-cpp"><span class="nu0">1</span> <span class="sy1">&lt;&lt;</span> <span class="nu0">31</span></span></span> 创建 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/types/climits" title="cpp/types/climits">INT_MIN</a></span> 合法）；否则行为未定义。</p><p>对于负 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span>，<span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&lt;&lt;</span> b</span></span> 的行为未定义。</p><p>对于无符号 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span> 和有符号的非负 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span>，<span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&gt;&gt;</span> b</span></span> 的值是 <span class="texhtml">a/2<span class="t-su">b<br></span></span>的整数部分。</p><p>对于负 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span>，<span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&gt;&gt;</span> b</span></span> 的值由实现定义（大多数平台上进行算术右移，因此结果保留为负）。</p></td><td><span class="t-mark-rev t-until-cxx20">(C++20 前)</span></td></tr><tr class="t-rev t-since-cxx20"><td><p><span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&lt;&lt;</span> b</span></span> 的值是与 <span class="texhtml">a * 2<span class="t-su">b<br></span></span>对 <span class="texhtml">2<span class="t-su">N<br></span></span>同余的唯一值，其中 <span class="texhtml">N</span> 是返回类型中的位数（即进行逐位左移并舍弃移出目标类型的位）。</p><p><span class="t-c"><span class="mw-geshi cpp source-cpp">a <span class="sy1">&gt;&gt;</span> b</span></span> 的值是 <span class="texhtml">a/2<span class="t-su">b<br></span></span>向负无穷取整（换言之，有符号 <span class="t-c"><span class="mw-geshi cpp source-cpp">a</span></span> 上的右移是算术右移）。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

返回类型是左操作数 ﻿的类型。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=21 "编辑章节：重载")\] 重载

在[针对用户定义运算符的重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E8.B0.83.E7.94.A8.E9.87.8D.E8.BD.BD.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/overload resolution")中，对于每对提升后的整数类型 `L` 和 `R`，下列函数签名参与重载决议：

<table class="t-dcl-begin"><tbody><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">L operator<span class="sy1">&lt;&lt;</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl"><td class="t-dcl-nopad"><p><span class="mw-geshi cpp source-cpp">L operator<span class="sy1">&gt;&gt;</span><span class="br0">(</span>L, R<span class="br0">)</span></span></p></td><td class="t-dcl-nopad"></td><td class="t-dcl-nopad"></td></tr><tr class="t-dcl-sep"><td></td><td></td><td></td></tr></tbody></table>

```
#include <iostream>
 
enum { ONE = 1, TWO = 2 };
 
int main()
{
    std::cout << std::hex << std::showbase;
    char c = 0x10;
    unsigned long long ull = 0x123;
    std::cout << "0x123 << 1 = " << (ull << 1) << '\n'
              << "0x123 << 63 = " << (ull << 63) << '\n' // 无符号中的溢出
              << "0x10 << 10 = " << (c << 10) << '\n';   // 提升 char 到 int
    long long ll = -1000;
    std::cout << std::dec << "-1000 >> 1 = " << (ll >> ONE) << '\n';
}
```

输出：

```
0x123 << 1 = 0x246
0x123 << 63 = 0x8000000000000000
0x10 << 10 = 0x4000
-1000 >> 1 = -500
```

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=22 "编辑章节：标准库")\] 标准库

许多标准库类型都重载了算术运算符。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=23 "编辑章节：一元算术运算符")\] 一元算术运算符

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=24 "编辑章节：加性运算符")\] 加性运算符

<table class="t-dsc-begin"><tbody><tr class="t-dsc"><td><div class="t-dsc-member-div"><div><p><a href="https://zh.cppreference.com/w/cpp/chrono/time_point/operator_arith2" title="cpp/chrono/time point/operator arith2"><span class="t-lines"><span>operator+</span><span>operator-</span></span></a></p></div><p><span class="t-lines"><span><span class="t-mark-rev t-since-cxx11">(C++11)</span></span></span></p></div></td><td>实现涉及时间点的加法和减法运算<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/chrono/time_point/dsc_operator_arith2&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><div><p><a href="https://zh.cppreference.com/w/cpp/chrono/duration/operator_arith4" title="cpp/chrono/duration/operator arith4"><span class="t-lines"><span>operator+</span><span>operator-</span><span>operator*</span><span>operator/</span><span>operator%</span></span></a></p></div><p><span class="t-lines"><span><span class="t-mark-rev t-since-cxx11">(C++11)</span></span></span></p></div></td><td>实现以时长为实参的算术运算<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/chrono/duration/dsc_operator_arith4&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><div><p><a href="https://zh.cppreference.com/w/cpp/chrono/year_month_day/operator_arith_2" title="cpp/chrono/year month day/operator arith 2"><span class="t-lines"><span>operator+</span><span>operator-</span></span></a></p></div><p><span class="t-lines"><span><span class="t-mark-rev t-since-cxx20">(C++20)</span></span></span></p></div></td><td>将 <code>year_month_day</code> 与一定量的年数或月数相加或相减<br><span class="t-mark">(函数)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/chrono/year_month_day/dsc_operator_arith_2&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/string/basic_string/operator%2B" title="cpp/string/basic string/operator+"><span class="t-lines"><span>operator+</span></span></a></p></div></td><td>连接两个字符串，一个字符串和一个 <span class="mw-geshi cpp source-cpp"><span class="kw4">char</span></span>，或者一个字符串和一个 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/string/basic_string_view" title="cpp/string/basic string view">string_view</a></span><br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/string/basic_string/dsc_operator%2B&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/iterator/reverse_iterator/operator_arith" title="cpp/iterator/reverse iterator/operator arith"><span class="t-lines"><span>operator+</span><span>operator-</span></span></a></p></div></td><td>推进或减小迭代器<br><span class="t-mark">(<code>std::reverse_iterator&lt;Iter&gt;</code> 的公开成员函数)</span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/iterator/move_iterator/operator_arith" title="cpp/iterator/move iterator/operator arith"><span class="t-lines"><span>operator+</span><span>operator-</span></span></a></p></div></td><td>推进或减小迭代器<br><span class="t-mark">(<code>std::move_iterator&lt;Iter&gt;</code> 的公开成员函数)</span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/numeric/complex/operator_arith3" title="cpp/numeric/complex/operator arith3"><span class="t-lines"><span>operator+</span><span>operator-</span><span>operator*</span><span>operator/</span></span></a></p></div></td><td>在两个复数，或一个复数与一个标量上进行复数算术运算<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/numeric/complex/dsc_operator_arith3&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/numeric/valarray/operator_arith3" title="cpp/numeric/valarray/operator arith3"><span class="t-lines"><span>operator+</span><span>operator-</span><span>operator*</span><span>operator/</span><span>operator%</span><span>operator&amp;</span><span>operator|</span><span>operator^</span><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span><span>operator&amp;&amp;</span><span>operator||</span></span></a></p></div></td><td>应用二元运算符到两个 valarray 的每个元素，或一个 valarray 的每个元素和一个值<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/numeric/valarray/dsc_operator_arith3&amp;action=edit">[编辑]</a></span></td></tr></tbody></table>

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=25 "编辑章节：乘性运算符")\] 乘性运算符

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=26 "编辑章节：逐位逻辑运算符")\] 逐位逻辑运算符

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=27 "编辑章节：移位运算符")\] 移位运算符

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=28 "编辑章节：流插入/提取运算符")\] 流插入/提取运算符

整个标准库中，移位运算符常对输入/输出流（[std::ios\_base](http://zh.cppreference.com/w/cpp/io/ios_base)& 或从它派生的类之一）重载，以之为左运算数和返回类型。这种运算符被称为*流插入*和*流提取*运算符：

<table class="t-dsc-begin"><tbody><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/io/basic_istream/operator_gtgt" title="cpp/io/basic istream/operator gtgt"><span class="t-lines"><span>operator&gt;&gt;</span></span></a></p></div></td><td>提取带格式数据<br><span class="t-mark">(<code>std::basic_istream&lt;CharT,Traits&gt;</code> 的公开成员函数)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/io/basic_istream/dsc_operator_gtgt&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/io/basic_istream/operator_gtgt2" title="cpp/io/basic istream/operator gtgt2"><span class="t-lines"><span>operator&gt;&gt;<span class="t-dsc-small">(std::basic_istream)</span></span></span></a></p></div></td><td>提取字符和字符数组<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/io/basic_istream/dsc_operator_gtgt2&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt" title="cpp/io/basic ostream/operator ltlt"><span class="t-lines"><span>operator&lt;&lt;</span></span></a></p></div></td><td>插入带格式数据<br><span class="t-mark">(<code>std::basic_ostream&lt;CharT,Traits&gt;</code> 的公开成员函数)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/io/basic_ostream/dsc_operator_ltlt&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt2" title="cpp/io/basic ostream/operator ltlt2"><span class="t-lines"><span>operator&lt;&lt;<span class="t-dsc-small">(std::basic_ostream)</span></span></span></a></p></div></td><td>插入字符数据，或向右值流插入<br><span class="t-mark">(函数)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/io/basic_ostream/dsc_operator_ltlt2&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/numeric/complex/operator_ltltgtgt" title="cpp/numeric/complex/operator ltltgtgt"><span class="t-lines"><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span></span></a></p></div></td><td>复数的序列化和反序列化<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/numeric/complex/dsc_operator_ltltgtgt&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/utility/bitset/operator_ltltgtgt2" title="cpp/utility/bitset/operator ltltgtgt2"><span class="t-lines"><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span></span></a></p></div></td><td>执行 bitset 的流输入和输出<br><span class="t-mark">(函数)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/utility/bitset/dsc_operator_ltltgtgt2&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><p><a href="https://zh.cppreference.com/w/cpp/string/basic_string/operator_ltltgtgt" title="cpp/string/basic string/operator ltltgtgt"><span class="t-lines"><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span></span></a></p></div></td><td>执行字符串的流输入与输出<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/string/basic_string/dsc_operator_ltltgtgt&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><div><p><a href="https://zh.cppreference.com/w/cpp/numeric/random/linear_congruential_engine/operator_ltltgtgt" title="cpp/numeric/random/linear congruential engine/operator ltltgtgt"><span class="t-lines"><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span></span></a></p></div><p><span class="t-lines"><span><span class="t-mark-rev t-since-cxx11">(C++11)</span></span></span></p></div></td><td>执行伪随机数引擎的流输入和输出<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/numeric/random/engine/dsc_operator_ltltgtgt&amp;action=edit">[编辑]</a></span></td></tr><tr class="t-dsc"><td><div class="t-dsc-member-div"><div><p><a href="https://zh.cppreference.com/w/cpp/numeric/random/uniform_int_distribution/operator_ltltgtgt" title="cpp/numeric/random/uniform int distribution/operator ltltgtgt"><span class="t-lines"><span>operator&lt;&lt;</span><span>operator&gt;&gt;</span></span></a></p></div><p><span class="t-lines"><span><span class="t-mark-rev t-since-cxx11">(C++11)</span></span></span></p></div></td><td>执行伪随机数分布的流输入和输出<br><span class="t-mark">(函数模板)</span> <span class="editsection noprint plainlinks" title="编辑此模板"><a rel="nofollow" class="external text" href="https://zh.cppreference.com/mwiki/index.php?title=Template:cpp/numeric/random/distribution/dsc_operator_ltltgtgt&amp;action=edit">[编辑]</a></span></td></tr></tbody></table>

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=29 "编辑章节：缺陷报告")\] 缺陷报告

下列更改行为的缺陷报告追溯地应用于以前出版的 C++ 标准。

| 缺陷报告 | 应用于 | 出版时的行为 | 正确行为 |
| --- | --- | --- | --- |
| [CWG 614](https://cplusplus.github.io/CWG/issues/614.html) | C++98 | 整数除法的算术商的取整方向由实现定义 | 向零取整（舍弃小数部分） |
| [CWG 1450](https://cplusplus.github.io/CWG/issues/1450.html) | C++98 | 未指明 a / b 在无法以结果类型表示时的结果 | 此时 a / b 和 a % b 的行为均未定义 |
| [CWG 1457](https://cplusplus.github.io/CWG/issues/1457.html) | C++98 | 将正的有符号值的最左端的 1 位移入符号位的行为未定义 | 赋予良好定义 |
| [CWG 1504](https://cplusplus.github.io/CWG/issues/1504.html) | C++98 | 指向数组元素的基类子对象的指针可以用于指针算术 | 此时行为未定义 |
| [CWG 1515](https://cplusplus.github.io/CWG/issues/1515.html) | C++98 | 只有声明了 unsigned 的无符号整数需要遵循 modulo 2n  
算术规则 | 应用到所有无符号整数 |
| [CWG 1642](https://cplusplus.github.io/CWG/issues/1642.html) | C++98 | 算术运算符的操作数可以是左值 | 部分操作数必须是右值 |
| [CWG 1865](https://cplusplus.github.io/CWG/issues/1865.html) | C++98 | [CWG 问题 1504](https://cplusplus.github.io/CWG/issues/1504.html) 的解决方案导致指向数组元素的指针在指针指向  
类型和数组类型的非顶层 cv 限定不同时参与指针运算的行为未定义 | 赋予良好定义 |
| [CWG 1971](https://cplusplus.github.io/CWG/issues/1971.html) | C++98 | 不明确用来解决 ~ 的歧义的规则是否适用 ~X(0) 这种情况 | 适用这种情况 |
| [CWG 2419](https://cplusplus.github.io/CWG/issues/2419.html) | C++98 | 只有通过 & 获取的到非数组对象的指针才会在  
指针运算中被视为到大小为 1 的数组的首元素的指针 | 应用到所有到非数组对象的指针 |
| [CWG 2626](https://cplusplus.github.io/CWG/issues/2626.html) | C++98 | 内建 operator~ 的结果只是“反码”，而没有适当的定义 | 结果用二进制表示进行描述 |
| [CWG 2724](https://cplusplus.github.io/CWG/issues/2724.html) | C++20 | 向右移位的取整方向不明确 | 使之明确 |
| [CWG 2853](https://cplusplus.github.io/CWG/issues/2853.html) | C++98 | 对象末尾后指针无法与整数相加或相减 | 可以相加或相减 |

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_arithmetic&action=edit&section=30 "编辑章节：参阅")\] 参阅

[运算符优先级](https://zh.cppreference.com/w/cpp/language/operator_precedence "cpp/language/operator precedence")

[运算符重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")

| 常见运算符 |
| --- |
| [赋值](https://zh.cppreference.com/w/cpp/language/operator_assignment "cpp/language/operator assignment") | [自增/自减](https://zh.cppreference.com/w/cpp/language/operator_incdec "cpp/language/operator incdec") | **算术** | [逻辑](https://zh.cppreference.com/w/cpp/language/operator_logical "cpp/language/operator logical") | [比较](https://zh.cppreference.com/w/cpp/language/operator_comparison "cpp/language/operator comparison") | [成员访问](https://zh.cppreference.com/w/cpp/language/operator_member_access "cpp/language/operator member access") | [其他](https://zh.cppreference.com/w/cpp/language/operator_other "cpp/language/operator other") |
| 
a \= b  
a +\= b  
a \-\= b  
a \*\= b  
a /\= b  
a %\= b  
a &\= b  
a |\= b  
a ^\= b  
a <<= b  
a \>>= b

 | 

++a  
\--a  
a++  
a\--

 | 

+a  
\-a  
a + b  
a \- b  
a \* b  
a / b  
a % b  
~a  
a & b  
a | b  
a ^ b  
a << b  
a \>> b

 | 

!a  
a && b  
a || b

 | 

a \== b  
a !\= b  
a < b  
a \> b  
a <= b  
a \>= b  
a <=> b

 | 

a\[b\]  
\*a  
&a  
a\-\>b  
a.b  
a\-\>\*b  
a.\*b

 | 函数调用 |
| a(...) |
| 逗号 |
| a, b |
| 条件 |
| a ? b : c |
| 特殊运算符 |
| 

[static\_cast](https://zh.cppreference.com/w/cpp/language/static_cast "cpp/language/static cast") 转换一个类型为另一相关类型  
[dynamic\_cast](https://zh.cppreference.com/w/cpp/language/dynamic_cast "cpp/language/dynamic cast") 在继承层级中转换  
[const\_cast](https://zh.cppreference.com/w/cpp/language/const_cast "cpp/language/const cast") 添加或移除 [cv](https://zh.cppreference.com/w/cpp/language/cv "cpp/language/cv") 限定符  
[reinterpret\_cast](https://zh.cppreference.com/w/cpp/language/reinterpret_cast "cpp/language/reinterpret cast") 转换类型到无关类型  
[C 风格转换](https://zh.cppreference.com/w/cpp/language/explicit_cast "cpp/language/explicit cast") 以 `static_cast`、`const_cast` 及 `reinterpret_cast` 的混合转换一个类型到另一类型  
[new](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new") 创建有动态存储期的对象  
[delete](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete") 销毁先前由 new 表达式创建的对象，并释放其所拥有的内存区域  
[sizeof](https://zh.cppreference.com/w/cpp/language/sizeof "cpp/language/sizeof") 查询类型的大小  
[sizeof...](https://zh.cppreference.com/w/cpp/language/sizeof... "cpp/language/sizeof...") 查询[形参包](https://zh.cppreference.com/w/cpp/language/parameter_pack "cpp/language/parameter pack")的大小(C++11 起)  
[typeid](https://zh.cppreference.com/w/cpp/language/typeid "cpp/language/typeid") 查询类型的类型信息  
[noexcept](https://zh.cppreference.com/w/cpp/language/noexcept "cpp/language/noexcept") 查询表达式是否能抛出异常(C++11 起)  
[alignof](https://zh.cppreference.com/w/cpp/language/alignof "cpp/language/alignof") 查询类型的对齐要求(C++11 起)

 |