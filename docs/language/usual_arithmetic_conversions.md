# 一般算数转换

---

许多期待具有[算术类型](https://zh.cppreference.com/w/cpp/language/type "cpp/language/type")的操作数的二元运算符会以相似的方法进行转换并产生结果类型。这样做的目的是产生公共类型，也就是结果类型。该模式被称为**一般算术转换**。

对于（除移位之外的）二元运算符，当提升后的操作数拥有不同类型时，实施一般算术转换。
### 定义

一般算数转换定义如下：

#### 阶段 1

对两个操作数进行[左值到右值转换](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E5.B7.A6.E5.80.BC.E5.88.B0.E5.8F.B3.E5.80.BC.E8.BD.AC.E6.8D.A2 "cpp/language/implicit conversion")，在后续过程中将转换结果纯右值代替原操作数。

#### 阶段 2
- 如果操作数均具有浮点类型，
    - 如果两个操作数具有相同类型，那么后续不会进行任何转换。
    - 否则，如果两个操作数具有的类型的浮点数转换等级有序但不相等，那么具有更低浮点数转换等级的类型的操作数会转换到另一个操作数具有的（浮点数转换等级更高的）类型。
    - 否则，表达式非良构。
- 如果两个操作数都具有有符号整形，
    - 如果两个操作数具有相同类型，那么后续不会进行任何转换。
    - 否则，如果两个操作数具有的类型的浮点数转换等级有序但不相等，那么具有更低浮点数转换等级的类型的操作数会转换到另一个操作数具有的（浮点数转换等级更高的）类型。
    - 否则，表达式非良构。
- 如果操作数均具有无符号整形，
    - 如果两个操作数具有相同类型，那么后续不会进行任何转换。
    - 否则，如果两个操作数具有的类型的浮点数转换等级有序但不相等，那么具有更低浮点数转换等级的类型的操作数会转换到另一个操作数具有的（浮点数转换等级更高的）类型。
    - 否则，表达式非良构。
- 否则，表达式非良构。

### 整数转换等级

每个[整数类型](https://zh.cppreference.com/w/cpp/language/types#.E6.95.B4.E6.95.B0.E7.B1.BB.E5.9E.8B "cpp/language/types")都有一个*整数转换等级*，定义如下：

+   除了 char 和 signed char（如果 char 有符号）以外的所有有符号整数类型的等级都不相同，即使它们的表示相同。
+   对于两个有符号整数类型，宽度更小的类型的等级低于宽度更大的类型的等级。
+   以下整数类型的等级递减：

+   long
+   int
+   short
+   signed char

+   每个无符号整数类型的等级都等于对应的有符号整数类型的等级。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><ul><li>每个标准整数类型的等级都高于具有相同宽度的扩展整数类型的等级。</li></ul></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

+   bool 的等级低于所有标准整数类型的等级。
+   编码字符类型（char、char8\_t (C++20 起)、char16\_t、char32\_t (C++11 起) 和 wchar\_t 的等级等于它们的[底层类型](https://zh.cppreference.com/w/cpp/language/types#.E5.AD.97.E7.AC.A6.E7.B1.BB.E5.9E.8B "cpp/language/types")的等级，也就是说：

+   char 的等级等于 signed char 和 unsigned char 的等级。

+   wchar\_t 的等级等于它的由实现定义的底层类型的等级。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><ul><li>扩展有符号整数类型的等级，相对于另一相同宽度的扩展有符号整数类型的高低由实现定义，但不能违背确定整数转换等级的其他规则。</li></ul></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

+   对于所有整数类型 `T1`、`T2` 和 `T3`，如果 `T1` 的等级高于 `T2` 的等级，并且 `T2` 的等级高于 `T3` 的等级，那么 `T1` 的等级高于 `T3` 的等级。

整数转换等级也用于定义[整数提升](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E6.95.B4.E6.95.B0.E6.8F.90.E5.8D.87 "cpp/language/implicit conversion")。

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/usual_arithmetic_conversions&action=edit&section=8 "编辑章节：浮点数转换等级与子等级")\] 浮点数转换等级与子等级

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/usual_arithmetic_conversions&action=edit&section=9 "编辑章节：浮点数转换等级")\] 浮点数转换等级

每个[浮点数类型](https://zh.cppreference.com/w/cpp/language/types#.E6.B5.AE.E7.82.B9.E6.95.B0.E7.B1.BB.E5.9E.8B "cpp/language/types")都有一个*浮点数转换等级*，定义如下：

+   以下标准浮点数类型的等级依次递减：
    +   long double
    +   double
    +   float

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx23"><td><ul><li>对于每对浮点数类型 <code>T1</code> 和 <code>T2</code>，如果 <code>T1</code> 能表示的所有值的集合是 <code>T2</code> 能表示的所有值的集合的真子集，那么 <code>T1</code> 的等级低于 <code>T2</code>。</li><li>两个能表示的值的集合相同的扩展浮数点类型的等级相等。</li><li>如果一个扩展浮点数类型能表示的所有值的集合与正好一个无 cv 限定的标准浮点数类型能表示的所有值的集合相同，那么该扩展浮点数类型的等级等于该标准浮点数类型的等级。</li><li>如果一个扩展浮点数类型能表示的所有值的集合与两个或更多无 cv 限定的标准浮点数类型能表示的所有值的集合相同，那么该扩展浮点数类型的等级等于 <span class="mw-geshi cpp source-cpp"><span class="kw4">double</span></span> 的等级。</li></ul></td><td><span class="t-mark-rev t-since-cxx23">(C++23 起)</span></td></tr></tbody></table>

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx23"><td><h4><span class="mw-headline" id=".E6.B5.AE.E7.82.B9.E6.95.B0.E8.BD.AC.E6.8D.A2.E5.AD.90.E7.AD.89.E7.BA.A7">浮点数转换子等级</span></h4><p>浮点数转换等级相等的浮点数类型按<i>浮点数转换子等级</i> ﻿有序。该子等级在所有等级相等的类型中组成全序。</p><p><code>std::float16_t</code>、<code>std::float32_t</code>、<code>std::float64_t</code> 和 <code>std::float128_t</code> 类型（<a href="https://zh.cppreference.com/w/cpp/types/floating-point" title="cpp/types/floating-point">定宽浮点数类型</a>）的浮点数转换子等级大于所有浮点数转换等级相等的标准整数类型的浮点数转换子等级。否则，浮点数转换子等级的高低由实现定义。</p></td><td><span class="t-mark-rev t-since-cxx23">(C++23 起)</span></td></tr></tbody></table>

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/usual_arithmetic_conversions&action=edit&section=11 "编辑章节：用途")\] 用途

浮点数转换等级和子等级也用于：

+   确定不同浮点数类型之间的转换是否[可以是隐式的](https://zh.cppreference.com/w/cpp/language/implicit_conversion#.E6.B5.AE.E7.82.B9.E6.95.B0.E8.BD.AC.E6.8D.A2 "cpp/language/implicit conversion")，或者是不是[窄化转换](https://zh.cppreference.com/w/cpp/language/list_initialization#.E7.AA.84.E5.8C.96.E8.BD.AC.E6.8D.A2 "cpp/language/list initialization")
+   在重载决议中[区分转换序列](https://zh.cppreference.com/w/cpp/language/overload_resolution#.E9.9A.90.E5.BC.8F.E8.BD.AC.E6.8D.A2.E5.BA.8F.E5.88.97.E7.9A.84.E5.88.86.E7.BA.A7 "cpp/language/overload resolution")

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx23"><td><ul><li>在使用 <a href="https://zh.cppreference.com/w/cpp/io/basic_istream/operator_gtgt" title="cpp/io/basic istream/operator gtgt"><tt>std::basic_istream::operator&gt;&gt;</tt></a> 提取扩展浮点数类型时确定用 <a href="https://zh.cppreference.com/w/cpp/locale/num_get/get" title="cpp/locale/num get/get"><tt>std::num_get::get()</tt></a> 实际提取的类型</li><li>在使用 <a href="https://zh.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt" title="cpp/io/basic ostream/operator ltlt"><tt>std::basic_ostream::operator&lt;&lt;</tt></a> 插入扩展浮点数类型时确定用 <a href="https://zh.cppreference.com/w/cpp/locale/num_put/put" title="cpp/locale/num put/put"><tt>std::num_put::put()</tt></a> 实际插入的类型</li></ul></td><td><span class="t-mark-rev t-since-cxx23">(C++23 起)</span></td></tr></tbody></table>

+   确定 [std::complex](https://zh.cppreference.com/w/cpp/numeric/complex "cpp/numeric/complex") 的 [转换构造函数](https://zh.cppreference.com/w/cpp/numeric/complex/complex "cpp/numeric/complex/complex")是不是显式的
+   在传递具有不同浮点数类型的实参到[常用](https://zh.cppreference.com/w/cpp/numeric/math "cpp/numeric/math")或[特殊](https://zh.cppreference.com/w/cpp/numeric/special_functions "cpp/numeric/special functions")数学函数时确定公共浮点数类型

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/usual_arithmetic_conversions&action=edit&section=12 "编辑章节：缺陷报告")\] 缺陷报告

下列更改行为的缺陷报告追溯地应用于以前出版的 C++ 标准。

| 缺陷报告 | 应用于 | 出版时的行为 | 正确行为 |
| --- | --- | --- | --- |
| [CWG 1642](https://cplusplus.github.io/CWG/issues/1642.html) | C++98 | 一般算数转换可能会涉及左值 | 先应用左值到右值转换 |
| [CWG 2528](https://cplusplus.github.io/CWG/issues/2528.html) | C++20 | unsigned char 和 unsigned int 之间  
的三路比较因为中途的整数提升而非良构[\[1\]](#cite_note-1) | 在实际没有进行整数提升的情况下  
根据提升后类型确定公共类型[\[2\]](#cite_note-2) |
| [CWG 2892](https://cplusplus.github.io/CWG/issues/2892.html) | C++98 | 当两个操作数具有相同类型时，“后续不需要再进行转换”的含义不明确 | 改成“后续不会进行任何转换” |

1.  [↑](#cite_ref-1) 在缺陷解决前，unsigned char 会在阶段 5 开始时提升到 int，然后再转换到 unsigned int。然而后者是窄化转换，导致三路比较非良构。
2.  [↑](#cite_ref-2) 在缺陷解决后，公共类型还是 unsigned int。但不同的地方在于 unsigned char 会在中途没有整数提升的情况下直接转换到 unsigned int。该转换不是窄化转换，因此三路比较良构。