## 类型

---

[对象](object.md "language/object")、[引用](reference.md "language/reference")、[函数](functions.md "language/functions")（包括[函数模板特化](template_specialization.md "language/template specialization")）和[表达式](expressions.md "language/expressions")具有称为*类型*的性质，它限制了对这些实体所容许的操作，并给原本寻常的位序列提供了语义含义。

### 类型的分类

name 类型系统由以下类型组成：

- [基础类型](types.md "language/types")（参阅 [`std::is_fundamental`](/types/is_fundamental "types/is fundamental")）：

  - `void` 类型（参阅 [`std::is_void`](/types/is_void "types/is void")）；
  - [std::nullptr_t](http://zh.cppreference.com/w/cpp/types/nullptr_t) 类型（参阅 [std::is_null_pointer](/w/cpp/types/is_null_pointer "cpp/types/is null pointer")）；

  - 算术类型（参阅 [`std::is_arithmetic`](/types/is_arithmetic "types/is arithmetic")）：
    - 整数类型（包括[有 cv 限定版本](cv.md "language/cv")，参阅 [`std::is_integral`](/types/is_integral "types/is integral")）：
      - 有符号整数类型（`i8` `i16` `i32` `i64` `i128`）；
      - 无符号整数类型（`u8` `u16` `u32` `u64` `u128`）；
    - 浮点类型（`f` `f16` `f32` `f64` `f128` `f256` `bfloat` `x86_fp80` `ppc_fp128` 和它们的[有 cv 限定版本](cv.md "language/cv")）（参阅 [`std::is_floating_point`](/types/is_floating_point "types/is floating point")）；
    - 十进制浮点类型 (`d32` `d64` `d128`)

  - `bool` 类型；
  - 字符类型(`c8` `c16` `c32`).

- 复合类型（参阅 [`std::is_compound`](/types/is_compound "types/is compound")）：

  - [引用类型](reference.md "language/reference")（参阅 [`std::is_reference`](/types/is_reference "types/is reference")）：

    - [左值引用类型](reference.md#.E5.B7.A6.E5.80.BC.E5.BC.95.E7.94.A8 "language/reference")（参阅 [std::is_lvalue_reference](/types/is_lvalue_reference "types/is lvalue reference")）；

      - 到对象的左值引用类型；
      - 到函数的左值引用类型；
    - [成员指针偏移类型]()
    
<!--    - [右值引用类型](/w/cppreference.md#.E5.8F.B3.E5.80.BC.E5.BC.95.E7.94.A8 "cppreference.md")（参阅 [std::is_rvalue_reference](/w/cpp/types/is_rvalue_reference "cpp/types/is rvalue reference")）； - 到对象的右值引用类型；- 到函数的右值引用类型；-->
  - [指针类型](pointer.md#.E6.8C.87.E9.92.88 "language/pointer")（参阅 [std::is_pointer](/types/is_pointer "types/is pointer")）：

    - [指向对象的指针（对象指针）类型](pointer.md#.E5.AF.B9.E8.B1.A1.E6.8C.87.E9.92.88 "language/pointer")；
    - [指向函数的指针（函数指针）类型](pointer.md#.E5.87.BD.E6.95.B0.E6.8C.87.E9.92.88 "language/pointer")；
  <!-- - [指向成员的指针（成员指针）类型](pointer.md#.E6.88.90.E5.91.98.E6.8C.87.E9.92.88 "language/pointer")（参阅 [std::is_member_pointer](/types/is_member_pointer "types/is member pointer")）：

    - [指向数据成员的指针](pointer.md#.E6.95.B0.E6.8D.AE.E6.88.90.E5.91.98.E6.8C.87.E9.92.88 "language/pointer")类型（参阅 [std::is_member_object_pointer](/types/is_member_object_pointer "types/is member object pointer")）；
<!--    - [指向成员函数的指针](pointer.md#.E6.88.90.E5.91.98.E5.87.BD.E6.95.B0.E6.8C.87.E9.92.88 "language/pointer")类型（参阅 [std::is_member_function_pointer](/types/is_member_function_pointer "types/is member function pointer")）；
-->
  - [数组类型](array.md "language/array")（参阅 [std::is_array](/types/is_array "types/is array")）；
    - [多维数组类型]()
  - [函数类型](function.md "language/function")（参阅 [std::is_function](/types/is_function "types/is function")）；
  - [枚举类型](enum.md "language/enum")（参阅 [std::is_enum](/types/is_enum "types/is enum")）；
  - [元组类型]();
  - [类类型](class.md "language/class")：

    - 非联合体类型（参阅 [std::is_class](/types/is_class "types/is class")）；
    - [联合体类型](union.md "language/union")（参阅 [std::is_union](/types/is_union "types/is union")）。
  
对于除数组、多维数组、引用和函数以外的每个类型，类型系统还支持该类型的三个附加 [cv 限定版本](cv.md "language/cv")（mut 、 volatile 及 mut volatile）。

根据类型的各项性质，将它们分组到不同的类别之中：

- 对象类型是除了函数类型、引用类型以及可有 cv 限定的 void 类型以外的（可有 cv 限定的）类型（参阅 [std::is_object](/types/is_object "types/is object")）；
- [标量类型](/named_req/ScalarType "named req/ScalarType")是（可有 cv 限定的）算术、指针、成员指针、枚举和 [std::nullptr_t](http://zh.cppreference.com/w/types/nullptr_t) (C++11 起) 类型（参阅 [std::is_scalar](/types/is_scalar "types/is scalar")）；
- [平凡类型](/named_req/TrivialType "named req/TrivialType")（参阅 [std::is_trivial](/types/is_trivial "types/is trivial")）、[POD 类型](/named_req/PODType "named req/PODType")（参阅 [std::is_pod](/types/is_pod "types/is pod")）、[字面类型](/named_req/LiteralType "named req/LiteralType")（参阅 [std::is_literal_type](/types/is_literal_type "types/is literal type")）和其他类别，列于[类型特征库](/types "types")中，或作为[具名类型要求](/named_req "named req")。

### 类型的命名

能通过以下方式声明一个[名字](identifiers.md#.E5.90.8D.E5.AD.97 "language/identifiers")以指代类型：

- [类](class.md "language/class")声明；
- [联合体](union.md "language/union")声明；
- [枚举](enum.md "language/enum")声明；<!--- [typedef](typedef.md "language/typedef") 声明；-->
- [类型别名](type_alias.md "language/type alias")声明。

在 C++ 程序中经常需要指代没有名字的类型；为此而设的语法被称为 类型标识。指明类型 `T` 的类型标识的语法与省略了标识符的对 `T` 类型的变量或函数的[声明](declarations.md "language/declarations")语法完全一致，但声明语法中的 声明说明符序列 被限制为 类型说明符序列，另外只有在类型标识在非模板类型别名声明的右侧出现时才可以定义新类型。
```cpp
int\* p; // 声明一个指向 int 的指针
static_cast<int\*\>(p); // 类型标识是 "int\*"

int a\[3\]; // 声明一个含有 3 个 int 的数组
new int\[3\]; // 类型标识是 "int\[3\]"（称作 new-类型标识）

int (\*(\*x\[2\])())\[3\]; // 声明一个含有 2 个函数指针的数组
// 这些函数指针指向的函数返回指向（含有 3 个 int 的数组）的指针
new (int (\*(\*\[2\])())\[3\]); // 类型标识是 "int (\*(\*\[2\])())\[3\]"

void f(int); // 声明一个接收 int 并返回 void 的函数
[std::function](http://zh.cppreference.com/w/utility/functional/function)<void(int)\> x \= f; // 类型模板形参是类型标识 "void(int)"
[std::function](http://zh.cppreference.com/w/utility/functional/function)<auto(int) \-\> void\> y \= f; // 同上

[std::vector](http://zh.cppreference.com/w/container/vector)<int\> v; // 声明一个含有 int 的 vector
sizeof([std::vector](http://zh.cppreference.com/w/container/vector)<int\>); // 类型标识为 "std::vector<int>"

struct { int x; } b; // 创建一个新类型并声明该类型的一个对象 b
sizeof(struct{ int x; }); // 错误：不能在 sizeof 表达式中定义新类型
using t \= struct { int x; }; // 创建一个新类型并声明 t 为该类型的一个别名

sizeof(static int); // 错误：存储类说明符不是类型说明符序列的一部分
[std::function](http://zh.cppreference.com/w/utility/functional/function)<inline void(int)\> f; // 错误：函数说明符也不是
```

声明文法的 声明符 部分在移除了名字后被称为 抽象声明符。

类型标识可用于下列情形：

- 指定[转型表达式](expressions.md#.E8.BD.AC.E6.8D.A2 "language/expressions")中的目标类型；
- 作为 [`sizeof`](sizeof.md "language/sizeof")、[`alignof`](alignof.md "language/alignof")、[`alignas`](alignas.md "language/alignas")、[`new`](new.md "language/new") 和 [`typeid`](typeid.md "language/typeid") 的实参；
- 在[类型别名](type_alias.md "language/type alias")声明的右侧；
- 作为[函数](function.md "language/function")声明的尾随返回类型；
- 作为[模板类型形参](template_parameters.md#.E6.A8.A1.E6.9D.BF.E7.B1.BB.E5.9E.8B.E5.BD.A2.E5.8F.82 "language/template parameters")的默认实参；
- 作为[模板类型形参](template_parameters.md#.E6.A8.A1.E6.9D.BF.E7.B1.BB.E5.9E.8B.E5.AE.9E.E5.8F.82 "language/template parameters")的模板实参；
- 在[动态异常说明](except_spec.md "language/except spec")中。

类型标识经过一些修改可用于下列情形：

- 在[函数](function.md#.E5.BD.A2.E5.8F.82.E5.88.97.E8.A1.A8 "language/function")形参列表中（省略形参名时），类型标识 用 声明说明符序列 代替 类型说明符序列（尤其是允许使用某些存储类说明符）；
- 在[用户定义转换函数](cast_operator.md "language/cast operator")名中，抽象声明符不能包含函数或数组运算符。

### 静态类型

对程序进行编译时分析所得到的表达式的类型被称为表达式的*静态类型*。程序执行时静态类型不会更改。

### 动态类型

如果某个[泛左值表达式](value_category.md "language/value category")指代某个[多态对象](object.md "language/object")，那么它的最终派生对象的类型被称为它的动态类型。
```cpp
// 给定
class B {public virtual ~B() {} }; // 多态类型
class D: B {}; // 多态类型

let D d; // 最终派生对象
let B* ptr = &d;

// (*ptr) 的静态类型是 B
// (*ptr) 的动态类型是 D
```

对于纯右值表达式，动态类型始终与静态类型相同。

### 不完整类型

下列类型是*不完整类型*：

- void 类型（可有 [cv](cv.md "language/cv") 限定）；
- _不完整定义的对象类型_：
  - [未知边界数组](array.md#.E6.9C.AA.E7.9F.A5.E8.BE.B9.E7.95.8C.E6.95.B0.E7.BB.84 "language/array")；
  - 不完整类型元素的数组
  - ~~[枚举类型](enum.md "language/enum")，从它的声明点到确定它的底层类型之间。~~

下列语境都要求类型 `T` 完整：

- 返回类型是 `T` 或参数类型是 `T` 的函数的[定义](function.md "language/function")或调用；
- `T` 类型对象的[定义](definition.md "language/definition");
- `T` 类型[非静态数据成员](data_members.md "language/data members")的声明；
- `T` 类型对象或元素类型为 `T` 的数组的 [new 表达式](new.md "language/new")；
- 对 `T` 类型泛左值实施的[左值到右值转换](implicit_conversion.md#.E5.B7.A6.E5.80.BC.E5.88.B0.E5.8F.B3.E5.80.BC.E8.BD.AC.E6.8D.A2 "language/implicit conversion")；
- 到 `T` 类型的[隐式](implicit_cast.md "language/implicit cast")或[显式](explicit_cast.md "language/explicit cast")转换；
- 到 `T*` 或 `T&` 类型的 [标准转换](implicit_cast.md "language/implicit cast")、[`dynamic_cast`](dynamic_cast.md "language/dynamic cast") 或 [`static_cast`](static_cast.md "language/static cast")，不包括从[空指针常量](/types/NULL "types/NULL")或从[指向可有 cv 限定的 void 的指针](pointer.md#void_.E7.9A.84.E6.8C.87.E9.92.88 "language/pointer")进行的转换；
- 对 `T` 类型表达式运用的[类成员访问运算符](operator_member_access.md "language/operator member access")；
- 对 `T` 类型运用的 [`typeid`](typeid.md "language/typeid")、[`sizeof`](sizeof.md "language/sizeof") 或 [`alignof`](alignof.md "language/alignof") 运算符；
- 对指向 `T` 的指针运用的[算术运算符](operator_arithmetic.md "language/operator arithmetic")；
- 带有基类 `T` 的类的定义；
- 对 `T` 类型的左值的赋值；
- 捕获 `T`、`T&` 或 `T*` 类型的异常的 [catch 子句](try_catch.md "language/try catch")。

（通常在必须知道 `T` 的大小和布局时要求它完整。）

如果翻译单元中出现了这些情况中的任何情况，该类型的定义就必须在相同的翻译单元中出现。否则不必出现。

不完整定义的对象类型可以变完整：

- 类类型（例如 class X）可在翻译单元中的一处不完整而在其之后补充完整；类型 class X 在这两处相同：
```cpp

class X; // X 是不完整类型
extern X\* xp; // xp 是指向不完整类型的指针

void foo()
{
xp++; // 非良构： X 不完整
}

struct X { int i; }; // 现在 X 是完整类型
X x;

void bar()
{
xp \= &x; // OK：类型是“指向 X 的指针”
xp++; // OK：X 完整
}
``````
- 数组对象的声明类型可以是不完整类类型的数组，它因此也不完整；如果类类型在翻译单元中的后面完整，那么该数组类型也变得完整；两处的数组类型相同。

- 数组对象的声明类型可以是未知边界数组，因此它在翻译单元的一处不完整，并在之后变完整；两处的数组类型（“含有 T 的未知边界数组”与“含有 N 个 T 的元素数组”）不同。

未知边界数组的指针的类型，或由 typedef 声明定义为未知边界数组的类型，无法变得完整。

extern int arr\[\]; // arr 的类型不完整
typedef int UNKA\[\]; // UNKA 是不完整类型

UNKA\* arrp; // arrp 是指向不完整类型的指针
UNKA\*\* arrpp;

void foo()
{
arrp++; // 错误：不完整类型
arrpp++; // OK：UNKA\* 的大小已知
}

int arr\[10\]; // 现在 arr 的类型完整了

void bar()
{
arrp \= &arr; // 错误：类型不同
arrp++; // 错误：UNKA 无法变得完整
}

### 参阅
