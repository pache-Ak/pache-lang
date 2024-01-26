## 标识符

---

*标识符*是一个由数字，下划线，大小写拉丁字母和大多数 Unicode 字符组成的任意长度的序列。

有效的标识符的首个字符必须是以下之一：

+   大写拉丁字母 A-Z
+   小写拉丁字母 a-z
+   下划线
+   具有 Unicode 属性 [XID\_Start](https://www.unicode.org/reports/tr31/#Table_Lexical_Classes_for_Identifiers) 的 Unicode 字符

有效的标识符的其他字符必须是以下之一：

+   数字 0-9
+   大写拉丁字母 A-Z
+   小写拉丁字母 a-z
+   下划线
+   具有 Unicode 属性 [XID\_Continue](https://www.unicode.org/reports/tr31/#Table_Lexical_Classes_for_Identifiers) 的 Unicode 字符

具有 XID\_Start 或 XID\_Continue 属性的字符在 [DerivedCoreProperties.txt](https://www.unicode.org/Public/UCD/latest/ucd/DerivedCoreProperties.txt) 列出。

标识符区分大小写（小写和大写字母是不同的），而且每一个字符都是起作用的。每个标识符都必须遵循[正规化形式 C](https://www.unicode.org/charts/normalization/)。

~~注意：大多数实现中对 Unicode 标识符的支持是受限的，比如 [gcc （10 之前）](https://gcc.gnu.org/wiki/FAQ#What_is_the_status_of_adding_the_UTF-8_support_for_identifier_names_in_GCC.3F)。~~

### 在声明中

可以用标识符来[命名](declarations.md "language/declarations")对象、引用、函数、枚举项、类型、类成员、命名空间、模板、模板特化、形参包 、goto 标号，以及其他实体，但有以下例外：

+   [关键词](../keyword.md "keyword")标识符不能用于其他目的；

-   关键词只有在属性记号中可做他用。（例如 `@delete` 是一个合法[属性]()）
+   作为特定运算符与标点符的[代用表示](https://zh.cppreference.com/w/language/operator_alternative "language/operator alternative")不能用于其他目的；

+   有以下形式之一的，作为记号（即不在形如 operator ""id 的*用户定义字符串字面量*中）出现的 标识符被保留：
    +   任何位置带有双下划线的标识符；
    +   以单下划线后随一个大写字母开始的标识符；
    +   在全局命名空间中，以一个下划线开始的标识符。

-   在<a href="https://zh.cppreference.com/w/language/user_literal#.E5.AD.97.E9.9D.A2.E9.87.8F.E8.BF.90.E7.AE.97.E7.AC.A6" title="language/user literal">字面量运算符</a>中：不以单个下划线开始的字面量后缀标识符为未来的标准化而被保留；含有双下划线的字面量后缀标识符为实现使用而被保留。

这里“被保留”的意思是，标准库的头文件可能声明这样的标识符以便它在内部使用，编译器可能会预先定义这种非标准的标识符，而且名字重整算法可能会假定某些这样的标识符是没有被使用的。如果程序员使用了这样的标识符的话，程序非良构，不要求诊断。

#### 僵尸标识符

C++14 起，某些标识符被从 C++ 标准库移除。它们列于[僵尸名列表](https://zh.cppreference.com/w/symbol_index/zombie_names "symbol index/zombie names")。

然而，这些标识符仍然在某些语境中为以前的标准化保留。可移植的代码中，被移除的成员函数名不可用做函数式宏的名字，而其他被移除的成员名不可用作对象式宏的名字。

### 在表达式中

命名某个变量、函数、[概念](https://zh.cppreference.com/w/language/constraints "language/constraints")的特化 (C++20 起)或枚举项的标识符可以作为[表达式](https://zh.cppreference.com/w/language/expressions "language/expressions")使用。仅由这个标识符组成的表达式的结果，是该标识符所命名的实体。如果该标识符命名的是某个函数、变量、[模板形参对象](https://zh.cppreference.com/w/language/template_parameters#.E9.9D.9E.E7.B1.BB.E5.9E.8B.E6.A8.A1.E6.9D.BF.E5.BD.A2.E5.8F.82 "language/template parameters") (C++20 起)或数据成员，那么表达式的[值类别](https://zh.cppreference.com/w/language/value_category "language/value category")是*左值*，否则是*纯右值* （例如[枚举项](https://zh.cppreference.com/w/language/enum "language/enum")是纯右值表达式，概念的特化是 bool 纯右值 (C++20 起)）。该表达式的类型以下列方式确定：

-   如果该（无限定）标识符所命名的实体是局部实体，且在该标识符所出现的声明区之外命名它时将导致它被穿插其间的某个 <a href="https://zh.cppreference.com/w/language/lambda" title="language/lambda">lambda 表达式</a> 按复制俘获，那么该表达式的类型是命名最内层的这种穿插其间的 lambda 表达式的闭包对象中为这种俘获所声明的非静态数据成员的<a href="https://zh.cppreference.com/w/language/operator_member_access" title="language/operator member access">类成员访问表达式</a>的类型。

```cpp
void f()
{
    float x, &r = x;
 
    [=]
    {
        decltype(x) y1;        // y1 拥有 float 类型
        decltype((x)) y2 = y1; // y2 拥有 float const& 类型
                               // 因为此 lambda 非 mutable 而 x 是左值
        decltype(r) r1 = y1;   // r1 拥有 float& 类型
        decltype((r)) r2 = y2; // r2 拥有 float const& 类型
    };
}
```
-   如果所命名的实体是某个 <code>T</code> 类型模板形参的模板形参对象，那么表达式的类型是 <span class="mw-geshi cpp source-cpp"><span class="kw4">const</span> T</span>。

+   否则，表达式的类型与被命名的实体的类型相同。

#### 无限定的标识符

除了适当声明了的标识符之外，以下各项也可以以相同方式用在表达式中：

+   函数写法的[重载运算符](operators.md "language/operators")名，比如 `operator+` 或 [`operator new`](http://zh.cppreference.com/w/memory/new/operator_new)；
+   [用户定义转换函数](https://zh.cppreference.com/w/language/cast_operator "language/cast operator")的名字，比如 `operator bool`；

+   [模板](https://zh.cppreference.com/w/language/templates "language/templates")的名字后随它的实参列表，比如 `MyTemplate<int>`；
+   ~ 字符后随类名，比如 `~MyClass`；

-   字符后随 <a href="https://zh.cppreference.com/w/language/decltype" title="language/decltype">decltype</a> 说明符，比如 `~decltype(str)`。

这些和标识符一起，被称作*无限定的标识表达式*。

#### 有限定的标识符

*有限定的标识表达式*是在无限定的标识表达式前面带上作用域解析运算符 `::`，以及以作用域解析运算符分隔的包含以下任意内容的序列（可以为空）：

+   命名空间的名字
+   类的名字

例如表达式 [`std::string::npos`](http://zh.cppreference.com/w/string/basic_string/npos) 是命名在命名空间 `std` 内的类 `string` 中的静态成员 npos 的表达式。表达式 ::tolower 命名的是全局命名空间内的函数 tolower。表达式 ::[std::cout](http://zh.cppreference.com/w/io/cout) 指名 std 命名空间（顶层命名空间）中的全局变量 cout。表达式 boost::signals2::connection 指名的是在 signals2 命名空间中声明的类型 connection，前者则在命名空间 boost 声明。

有限定标识符中，可能会需要以关键词 [template](https://zh.cppreference.com/w/keyword/template "keyword/template") 来消除[待决模板名](https://zh.cppreference.com/w/language/dependent_name "language/dependent name")的歧义。

关于为有限定的标识符进行的名字查找的细节，请参见[有限定的名字查找](https://zh.cppreference.com/w/language/qualified_lookup "language/qualified lookup")。

### 名字

*名字*是以下各项之一，用来代表某个实体或者某个标号：

+   标识符；
+   函数写法的重载运算符的名字（`operator+`、[`operator new``](memory/new/operator_new)）；
+   用户定义的转换函数的名字（`operator bool`）；
-   用户定义的字面量运算符的名字（`operator "" _km`）；
+   模板的名字后随它的实参列表（`MyTemplate<int>`）。

每个代表实体的名字都是由[声明](https://zh.cppreference.com/w/language/declarations "language/declarations")引入到程序中来的。代表标号的名字则既可以通过 [goto 语句](https://zh.cppreference.com/w/language/goto "language/goto")，也可以通过[带标号语句](https://zh.cppreference.com/w/language/statements#.E6.A0.87.E5.8F.B7 "language/statements")引入到程序中来。在多个翻译单元中使用的相同名字，可以根据它的[链接](https://zh.cppreference.com/w/language/storage_duration#.E9.93.BE.E6.8E.A5 "language/storage duration")代表相同或者不同的实体。

每当编译器在程序中遇到一个未知的名字时，它就会通过进行[名字查找](https://zh.cppreference.com/w/language/lookup "language/lookup")来将它与引入这个名字的声明联系起来，但对模板的声明和定义中的[待决名](https://zh.cppreference.com/w/language/dependent_name "language/dependent name")不会这样做。（对于这些名字，编译器需要确定它们命名的是类型、模板还是某些其他实体，这可能需要[显式消歧义](https://zh.cppreference.com/w/language/dependent_name "language/dependent name")）。

### 参阅