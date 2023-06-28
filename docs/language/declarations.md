## 声明

---
*声明*将名字引入（或再引入）到 C++ 程序中。每种实体的声明方式都不同。
*声明*是引入、创建并可能会初始化一个或数个标识符（典型地为变量）的语句。[定义](/language/definition "language/definition")是足以使该名字所标识的实体被使用的声明。

声明是下列之一：

- [函数定义](/language/function#函数定义 "language/function")
- [模板声明](/language/templates "language/templates")（包括[部分模板特化](/language/partial_specialization "language/partial specialization")）
- ~~[显式模板实例化](/language/class_template#显式实例化 "language/class template")~~ 隐式化
- [显式模板特化](/language/template_specialization "language/template specialization")
- [命名空间定义](/language/namespace "language/namespace")
- [链接说明](/language/language_linkage "language/language linkage")
- 属性声明 （[属性](/language/attributes "language/attributes")；）

- *块声明*（能在[块](/language/statements.md#复合语句 "language/statements")中出现的声明），它可以是下列之一：

  - [汇编声明](/language/asm "language/asm")
  - [命名空间别名定义](/language/namespace_alias "language/namespace alias")
  - [using 声明](/language/using_declaration "language/using declaration")
  - [using 指令](/language/namespace#using_指令 "language/namespace")
  - [using enum 声明](/language/enum#using_enum_声明 "language/enum")
  - [static assert 声明](/language/static_assert "language/static assert")
  - [不可见 enum 声明](/language/enum "language/enum")

- 简单声明

### 简单声明

简单声明是引入、创建并可能会初始化一个变量的语句。

> `let` *类型说明符*  *名字*`;`

|   |    |    |
| ---: | --- |  :--- |
| 声明说明符序列<br>（decl-specifier-seq）|-|*说明符（specifier）*的序列（见下文） |
| 初始化声明符列表<br>（init-declarator-list）|-|可以带有*初始化器（initializer）*的*声明符（declarator）*的逗号分隔列表。 初始化声明符列表 在声明具名的 class/struct/union 或具名枚举时可以省略 |

[结构化绑定声明](/language/structured_binding "language/structured binding")也是简单声明。

### 说明符

**声明说明符**（声明说明符序列）是下列以空白分隔的说明符的序列，顺序不限：

- ~~[`typedef`](/language/typedef "language/typedef") 说明符。如果存在，那么整个声明是 [typedef 声明](/language/typedef "language/typedef")，且每个声明符引入一个新的类型名，而非对象或函数。~~
- 函数说明符（~~[`inline`](/language/inline "language/inline")~~、[`virtual`](/language/virtual "language/virtual")、[`explicit`](/language/explicit "language/explicit")），只能在[函数定义](/language/function#函数定义 "language/function")中使用。

- ~~[`inline`](/language/inline "language/inline") 说明符也可以用于变量声明。~~

- [`friend`](/language/friend "language/friend") 说明符，可以在类和函数声明中使用。

- [`constexpr`](/language/constexpr "language/constexpr") 说明符，只允许在变量定义，函数及函数模板声明，以及具有字面类型的静态数据成员的声明中使用。

- [`consteval`](/language/consteval "language/consteval") 说明符，只允许在函数和函数模板声明中使用。
- [`constinit`](/language/constinit "language/constinit") 说明符，只允许在拥有静态或线程存储期的变量声明中使用。`constexpr`、`consteval` 和 `constinit` 说明符在一个 声明说明符序列 中最多只能出现一个。

- [存储类说明符](/language/storage_duration "language/storage duration")（~~[register](/keyword/register "keyword/register")、 (C++17 前)~~ [static](/keyword/static "keyword/static")、[`thread_local`](/keyword/thread_local "keyword/thread local") 、~~[extern](/keyword/extern "keyword/extern")~~ ）只能使用一个存储类说明符。
- [mutable](/keyword/mutable "keyword/mutable")说明符， TODO :
- **类型说明符**（类型说明符序列），指名一个类型的说明符的序列。声明所引入的每个实体都是这个类型，并可以被声明符修饰（见下文）。这个说明符序列也用于[类型标识（type-id）](/language/type#类型的命名 "language/type")。只有下列说明符是 类型说明符序列 的一部分，顺序不限：

  - [类说明符](/language/class "language/class")
  - [枚举说明符](/language/enum "language/enum")
  - 简单类型说明符

    - [c8](/keyword/c8 "keyword/c8")、[c16](/keyword/c16 "keyword/c16")、[c32](/keyword/c32 "keyword/c32") 、[bool](/keyword/bool "keyword/bool")、[i8](/keyword/i8.md "keyword/i8")、 [i16](/keyword/short "keyword/short")、[i32](/keyword/int "keyword/int")、[i64](/keyword/long "keyword/long")、[i128](/keyword/i128 "keyword/i128")、[u8](/keyword/u8.md "keyword/u8")、[u16](/keyword/u6 "keyword/u6")、[u32](/keyword/u32 "keyword/u32")、[u64](/keyword/u64 "keyword/u64")、[u128](/keyword/u128 "keyword/u128")、[f16](/keyword/f16 "keyword/f16")、[f32](/keyword/f32 "keyword/f32")、[f64](/keyword/f64 "keyword/f64")、[f128](/keyword/f128 "keyword/f128")、 [void](/keyword/void "keyword/void")
    - [auto](/language/auto "language/auto")
    - [decltype 说明符](/language/decltype "language/decltype")
    - 先前定义过的类名（可[限定](/language/identifiers#有限定的标识符 "language/identifiers")）
    - 先前定义过的枚举名（可[限定](/language/identifiers#有限定的标识符 "language/identifiers")）
    - ~~先前声明过的 [typedef 名](/language/typedef "language/typedef")或[类型别名](/language/type_alias "language/type alias") （可[限定](/language/identifiers#有限定的标识符 "language/identifiers")）~~
    - 带模板实参的模板名（可[限定](/language/identifiers#有限定的标识符 "language/identifiers")，可使用 [template 消歧义符](/language/dependent_name "language/dependent name")）
    - 无模板实参的模板名（可[限定](/language/identifiers#有限定的标识符 "language/identifiers")）：参阅[类模板实参推导](/language/class_template_argument_deduction "language/class template argument deduction")
  - ~~[详述类型说明符](/language/elaborated_type_specifier "language/elaborated type specifier")~~ 修改名字查找规则，

  - [typename 说明符](/language/dependent_name "language/dependent name")
  - [cv 限定符](/language/cv "language/cv")

    每个 声明说明符序列 中只允许一个类型说明符，但有以下例外：

  - `const` 能与自身外的任何类型说明符组合。
  - `volatile` 能与自身外的任何类型说明符组合。

[属性](/language/attributes "language/attributes")可以在 声明说明符序列前出现，此时它们会应用于在它之前的说明符所确定的类型。

在 声明说明符序列 中重复任何说明符，如 `const static const`是错误。

### 声明符

初始化声明符列表 是一或多个 初始化声明符 的逗号分隔列表，它拥有下列语法：

声明符 初始化器(可选)(1)声明符 requires-子句(2

初始化声明符序列 S D1, D2, D3; 中的每个 初始化声明符，均按照如同它是拥有相同说明符的孤立声明来处理：S D1; S D2; S D3;。

每个声明符恰好引入一个对象、引用、函数或（对于 typedef 声明）类型别名，它的类型由 声明说明符序列 提供，并且可以被声明符中的运算符，如 &（～的引用）或 \[\]（～的数组）或 ()（返回～的函数）所修饰。可以递归应用这些声明符，如下所示。

声明符 是下列之一：

无限定标识 (1)
有限定标识 (2)
`...` 标识符 (3)
**`*`**  限定符(可选) 声明符(4)
嵌套名说明符 **`*`**  限定符(可选) 声明符(5)
**`&`**  声明符(6)
**`&&`**  声明符(7)
非指针声明符 **`[`** 常量表达式(可选) **`]`** (8)
非指针声明符 **`(`** 形参列表 **`)`** 异常说明(可选) (9)

1) 所声明的[名称](/language/identifiers#.E5.90.8D.E5.AD.97 "language/identifiers")。

2) 使用[有限定的标识符](/w/cpp/language/identifiers#.E6.9C.89.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "cpp/language/identifiers")（有限定标识）定义或重声明先前声明的[命名空间成员](/w/cpp/language/namespace#.E5.91.BD.E5.90.8D.E7.A9.BA.E9.97.B4 "cpp/language/namespace")或[类成员](/w/cpp/language/classes "cpp/language/classes")的声明符。

3) [形参包](/w/cpp/language/parameter_pack "cpp/language/parameter pack")，只会在[形参声明](/w/cpp/language/function#.E5.BD.A2.E5.8F.82.E5.88.97.E8.A1.A8 "cpp/language/function")中出现。

4) [指针声明符](/language/pointer "language/pointer")：声明 S \* D; 将 `D` 声明为指向 声明说明符序列 所确定的类型 `S` 的指针。

5) [成员指针声明](/w/cpp/language/pointer "cpp/language/pointer")：声明 S C::\* D; 将 `D` 声明为指向 `C` 中的类型是 声明说明符序列 所确定的类型 `S` 的成员的指针。嵌套名说明符 是[名字和作用域解析运算符 `**::**` 的序列](/w/cpp/language/identifiers#.E6.9C.89.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "cpp/language/identifiers")

6) [左值引用声明符](/language/reference "language/reference")：声明 S & D; 将 `D` 声明为到 声明说明符序列 所确定的类型 `S` 的左值引用。

7) [右值引用声明符](/language/reference "language/reference")：声明 S && D; 将 `D` 声明为到 声明说明符序列 所确定的类型 `S` 的右值引用。

8) [数组声明符](/language/array "language/array")。非指针声明符 是任意合法声明符，但如果它以 \*、& 或 && 起始，那么就必须用括号环绕它。

9) [函数声明符](/language/function "language/function")。非指针声明符 是任意合法声明符，但如果它以 \*、& 或 && 起始，那么就必须用括号环绕它。注意最外层函数声明符可以以尾随返回类型结尾。

限定符 是 [const 与 volatile](/language/cv "language/cv") 限定符的序列，其中任一限定符在序列中至多出现一次。

### 注解

块声明出现[在代码块内](/language/statements#.E5.A4.8D.E5.90.88.E8.AF.AD.E5.8F.A5 "language/statements")，而当声明中引入的标识符之前已在某个外层代码块中声明时，该代码块的剩余部分中[外层声明被隐藏](/language/scope "language/scope")。

如果一个声明引入了一个具有自动存储期的变量，那么它会在执行它的声明语句时被初始化。退出代码块时，所有在该块中声明的自动变量被以它的初始化顺序的相反顺序销毁（与如何退出代码块无关：通过[异常](/language/exceptions "language/exceptions")、[goto](/language/goto "language/goto") 或抵达它的结尾）。

### 示例

### 参阅
