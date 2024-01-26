## 值类别

---

每个 C++ [表达式](expressions.md "language/expressions")（带有操作数的操作符、字面量、变量名等）可按照两种独立的特性加以辨别：*类型*和*值类别 (value category)*。每个表达式都具有某种非引用类型，且每个表达式只属于四种基本值类别中的一种：*纯右值 (prvalue)*、*亡值 (xvalue)*、*左值 (lvalue)*、void。

-   **泛左值 (glvalue)**（“泛化 (generalized)”的左值）是一个表达式，其值可确定某个对象或函数的标识；
    +   **左值 (lvalue)**（如此称呼的历史原因是，左值可以在赋值表达式的左边出现）是非亡值的泛左值；
    +   **亡值 (xvalue)**（“将亡 (expiring)”的值）是代表它的资源能够被重新使用的对象或位域的泛左值；
+   **右值 (rvalue)**（如此称呼的历史原因是，右值可以在赋值表达式的右边出现）是纯右值或者void。
    -   **纯右值 (prvalue)** （“纯 (pure)”的右  值）是求值符合下列之一的表达式：
        -   计算某个运算符的操作数的值（这种纯右值没有*结果对象*）
        -   初始化某个对象（称这种纯右值有一个*结果对象*）。

            结果对象可以是变量，由 [new 表达式](https://zh.cppreference.com/w/language/new "language/new")创建的对象，由[临时量实质化](https://zh.cppreference.com/w/language/implicit_conversion#.E4.B8.B4.E6.97.B6.E9.87.8F.E5.AE.9E.E8.B4.A8.E5.8C.96 "language/implicit conversion")创建的临时对象，或者前述三类对象的成员。注意非 void 的[弃值表达式](https://zh.cppreference.com/w/language/expressions#.E5.BC.83.E5.80.BC.E8.A1.A8.E8.BE.BE.E5.BC.8F "language/expressions")有结果对象（即被实质化的临时量）。并且在作为 [decltype](https://zh.cppreference.com/w/language/decltype "language/decltype") 的操作数以外的情况下，每个类类型或数组类型的纯右值都有结果对象；

    -   **void**表达式没有结果对象。
### 混合类别

#### 泛左值
下列表达式是*左值表达式*：

+   变量、函数、~~[模板形参对象](https://zh.cppreference.com/w/language/template_parameters#.E9.9D.9E.E7.B1.BB.E5.9E.8B.E6.A8.A1.E6.9D.BF.E5.BD.A2.E5.8F.82 "language/template parameters")~~ 或数据成员的名字，不论类型，例如 [std::cin](http://zh.cppreference.com/w/io/cin) 或 [std::endl](http://zh.cppreference.com/w/io/manip/endl)。

+   返回类型是引用的函数调用或重载运算符表达式，例如 [std::getline](http://zh.cppreference.com/w/string/basic_string/getline)([std::cin](http://zh.cppreference.com/w/io/cin), str)、[std::cout](http://zh.cppreference.com/w/io/cout) << 1、str1 \= str2；
+   `*p`，内建的[间接寻址](operator_member_access.md "language/operator member access")表达式；
+   `a[n]` ，内建的[下标](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E4.B8.8B.E6.A0.87.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，当 `a[n]` 中的一个操作数是数组左值时；
+   `a.m`，[对象成员](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，除了以下两种情况： `m` 是成员枚举项或非静态成员函数，或者 `a` 是右值而 `m` 是对象类型的非静态数据成员的情况；
+   `p->m`，内建的[指针成员](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，除了 `m` 是成员枚举项或非静态成员函数的情况；

+   [字符串字面量](https://zh.cppreference.com/w/language/string_literal "language/string literal")，例如 "Hello, world!"；
+   转换到引用类型的转型表达式，例如 `static_cast<i32&>(x)`；
+   具有引用类型的非类型[模板形参](https://zh.cppreference.com/w/language/template_parameters "language/template parameters")；

*泛左值*表达式包括左值、亡值。

性质：

+   可以用来[初始化引用](https://zh.cppreference.com/w/language/reference_initialization "language/reference initialization")；这会将一个新名字关联给该表达式所标识的对象。
+   可修改的泛左值可用作内建赋值和内建复合赋值运算符的左操作数。
+   泛左值可以通过~~左值到右值、~~ ~~数组到指针~~或函数到指针[隐式转换](https://zh.cppreference.com/w/language/implicit_conversion "language/implicit conversion")转换成纯右值。
+   泛左值可以具有[不完整类型](https://zh.cppreference.com/w/language/types#.E4.B8.8D.E5.AE.8C.E6.95.B4.E7.B1.BB.E5.9E.8B "language/types")，只要表达式中容许。

+   可以通过内建的取址运算符取泛左值的地址：&[std::endl](http://zh.cppreference.com/w/io/manip/endl) 是合法表达式。
#### 右值

*右值表达式*包括纯右值、~~亡值~~、void。

性质：

+   右值不能由内建的取址运算符取地址：&int()、&i++[\[3\]](#cite_note-3)、&42 及 &std::move(x) 是非法的。
+   右值不能用作内建赋值运算符及内建复合赋值运算符的左操作数。
+   右值可以用来[初始化 const 左值引用](https://zh.cppreference.com/w/language/reference_initialization "language/reference initialization")，这种情况下该右值所标识的对象的生存期被[延长](https://zh.cppreference.com/w/language/reference_initialization#.E4.B8.B4.E6.97.B6.E5.AF.B9.E8.B1.A1.E7.9A.84.E7.94.9F.E5.AD.98.E6.9C.9F "language/reference initialization")到该引用的作用域结尾。

### 基本类别

#### 左值
左值是非亡值的泛左值。

性质：

+   与泛左值相同。
-   具有[所有权]()。
+   左值可以是[多态的](object#.E5.A4.9A.E6.80.81.E5.AF.B9.E8.B1.A1 "language/object")：它标识的对象的[动态类型](type.md#动态类型 "language/types")不必是该表达式的静态类型。


#### 亡值


- 出现在直接赋值运算符右侧的左值会变为亡值

性质：

+   ~~与右值相同（见下文）。~~
+   与泛左值相同（见下文）。
-   不具有所有权
#### 纯右值

下列表达式是*纯右值表达式*：

+   （除了[字符串字面量](string_literal "language/string literal")之外的）[字面量](expressions.md#字面量 "language/expressions")，例如 42、true 或 nullptr；
+   返回类型是非引用的函数调用或重载运算符表达式，例如 str.substr(1, 2)、str1 + str2 ；

+   a + b、a % b、a & b、a << b，以及其他所有内建的[算术](https://zh.cppreference.com/w/language/operator_arithmetic "language/operator arithmetic")表达式；
+   a && b、a || b、!a，内建的[逻辑](https://zh.cppreference.com/w/language/operator_logical "language/operator logical")表达式；
+   a < b、a \== b、a \>= b 以及其他所有内建的[比较](https://zh.cppreference.com/w/language/operator_comparison "language/operator comparison")表达式；
+   &a，内建的[取地址](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E5.8F.96.E5.9C.B0.E5.9D.80.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式；
+   a.m，[对象成员](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，其中 `m` 是成员枚举项或非静态成员函数[\[2\]](#cite_note-pmfc-2)；
+   p\-\>m，内建的[指针成员](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，其中 `m` 是成员枚举项或非静态成员函数[\[2\]](#cite_note-pmfc-2)；
+   转换到非引用类型的转型表达式，例如 static\_cast<double\>(x)、[std::string](http://zh.cppreference.com/w/string/basic_string){} 或 (int)42；
+   [`this`](https://zh.cppreference.com/w/language/this "language/this") 指针；
+   [枚举项](https://zh.cppreference.com/w/language/enum "language/enum");
+   具有标量类型的非类型[模板形参](https://zh.cppreference.com/w/language/template_parameters "language/template parameters")；
+   a.m，[对象成员](https://zh.cppreference.com/w/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")表达式，其中 `a` 是右值且 `m` 是对象类型的非静态数据成员；

- [lambda 表达式]()

- [requires 表达式]()

- [概念]()的特化

性质：

+   与右值相同（见下文）。
+   纯右值不具有[多态](https://zh.cppreference.com/w/language/object#.E5.A4.9A.E6.80.81.E5.AF.B9.E8.B1.A1 "language/object")：它所标识的对象的[动态类型](https://zh.cppreference.com/w/language/type#.E5.8A.A8.E6.80.81.E7.B1.BB.E5.9E.8B "language/type")始终是该表达式的类型。
+   ~~非类非数组的~~纯右值不能有 [cv 限定](https://zh.cppreference.com/w/language/cv "language/cv")，除非它被[实质化](https://zh.cppreference.com/w/language/implicit_conversion#.E4.B8.B4.E6.97.B6.E9.87.8F.E5.AE.9E.E8.B4.A8.E5.8C.96 "language/implicit conversion")以[绑定](https://zh.cppreference.com/w/language/reference_initialization "language/reference initialization")到 cv 限定类型的引用 (C++17 起)。（注意：函数调用或转型表达式可能生成非类的 cv 限定类型的纯右值，但它的 cv 限定符通常被立即剥除。）
+   纯右值不能具有[不完整类型](https://zh.cppreference.com/w/language/type#.E4.B8.8D.E5.AE.8C.E6.95.B4.E7.B1.BB.E5.9E.8B "language/type")（除了类型 void（见下文），或在 [`decltype`](https://zh.cppreference.com/w/language/decltype "language/decltype") 说明符中使用之外）
+   纯右值不能具有[抽象类类型](https://zh.cppreference.com/w/language/abstract_class "language/abstract class")或它的数组类型。

#### void

返回 void 的函数调用表达式，转换到 void 的转型表达式，以及 [throw 表达式](https://zh.cppreference.com/w/language/throw "language/throw")，被归类为右值表达式，但它们不能用来初始化引用或者作为函数实参。它们可以用在舍弃值的语境（例如自成一行）和返回 void 的函数中的 return 语句中。

void 表达式没有结果对象。

### 特殊类别

#### 未决成员函数调用

表达式 a.mf 与 p\-\>mf，其中 `mf` 是[非静态成员函数](https://zh.cppreference.com/w/language/member_functions "language/member functions")，被归类为纯右值表达式，但它们不能用来初始化引用，作为函数实参，或者用于除了作为函数调用运算符的左操作数（例如 (p\-\>\*pmf)(args)）以外的任何目的。

#### 位域

代表某个[位域](https://zh.cppreference.com/w/language/bit_field "language/bit field")的表达式（例如 a.m，其中 a 是类型 struct A { int m: 3; } 的左值）是泛左值表达式：它可用作赋值运算符的左操作数，但它不能被取地址，并且非 const 的左值引用不能绑定于它。const 左值引用或右值引用可以从位域泛左值初始化，但这会制造位域的一个临时副本：它不会直接绑定到位域。

#### 有移动资格的表达式

可隐式移动的实体是具有自动储存期的 mut 非 volatile 限定的变量。在以下情况中，[标识表达式](https://zh.cppreference.com/w/language/identifiers "language/identifiers")（可以有任意括号）是具有移动资格的

+   它是 [`return`](https://zh.cppreference.com/w/language/return "language/return") 语句或者 [`co_return`](https://zh.cppreference.com/w/language/coroutines "language/coroutines") 语句 (C++20 起) 的操作数，且它指代的可隐式移动实体在当前函数体或者参数列表中声明。参见[自动从局部变量和形参移动](https://zh.cppreference.com/w/language/return#.E8.87.AA.E5.8A.A8.E4.BB.8E.E5.B1.80.E9.83.A8.E5.8F.98.E9.87.8F.E5.92.8C.E5.BD.A2.E5.8F.82.E7.A7.BB.E5.8A.A8 "language/return")。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx17"><td><ul><li>它是 <a href="https://zh.cppreference.com/w/language/throw" title="language/throw"><code>throw</code></a> 表达式的操作数，且它指代的可隐式移动实体属于不包含最内层的 lambda 表达式、try 块 或函数 try 块（如果有）的复合语句或构造函数的初始化列表包含 throw 表达式的范围。</li></ul></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr></tbody></table>

如果一个表达式有移动资格，那么它在[重载决议](https://zh.cppreference.com/w/language/overload_resolution "language/overload resolution")时被视为 左值或者右值 (C++23 前)右值 (C++23 起) （因此可能会选择移动构造）。

### 参阅