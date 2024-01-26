## 表达式

---

表达式是*运算符*和它们的*操作数*的序列，它指定一项计算。

表达式的求值可以产生一个结果（比如 2 + 2 的求值产生结果 4），也可能产生副作用（比如对 [std::printf](/io/c/fprintf)("%d", 4) 的求值在标准输出上打印字符 '4'）。

#### 概述

- [值类别](language/value_category "language/value category")（左值 (lvalue)、右值 (rvalue)、泛左值 (glvalue)、纯右值 (prvalue)、亡值 (xvalue) ）是根据表达式的值所进行的分类
- 实参和子表达式的[求值顺序](language/eval_order "language/eval order")指定获得中间结果所用的顺序

### 运算符

|          常见运算符|                           | | | | | 
| :--- |:--- |:--- |:--- |:--- |:--- |
|~~自增<br>自减~~|算术|逻辑|比较|成员访问|其他          |
| ~~++a<br>--a<br>a++<br>a~~ | +a<br>-a<br>a + b<br>a - b<br>a * b<br>a / b<br>a % b<br>~a<br>a & b<br>a \| b<br>a ^ b<br>a << b<br>a >> b  | !a<br>a && b<br>a \|\| b  | a == b<br>a != b<br>a < b<br>a > b<br>a <= b<br>a >= b<br>a <=> b  | a[b]<br>*a<br>&a<br>a->b<br>a.b<br>~~a->*b~~<br>~~a.*b~~  | a(...)<br>~~a, b<br>? :~~  |

- [运算符优先级](language/operator_precedence "language/operator precedence")定义了运算符绑定到它的各个实参的顺序
- [运算符重载](operators.md "language/operators")允许对用户定义的类指定各运算符的行为。

#### 转换

- [标准转换](language/implicit_conversion "language/implicit conversion")是从一个类型到另一类型的隐式转换
- [`const_cast` 转换](language/const_cast "language/const cast")
- [`static_cast` 转换](language/static_cast "language/static cast")
- [`dynamic_cast` 转换](language/dynamic_cast "language/dynamic cast")
- [`reinterpret_cast` 转换](language/reinterpret_cast "language/reinterpret cast")
- [显式转型](language/explicit_cast "language/explicit cast")转换，使用 C 风格写法和函数式写法
- [用户定义转换](language/cast_operator "language/cast operator")使得可以指定源自用户定义类的转换

#### 内存分配

- [new 表达式](language/new "language/new")动态地分配内存
- [delete 表达式](language/delete "language/delete")动态地解分配内存

#### 其他

- [常量表达式](language/constant_expression "language/constant expression")能在编译期求值并在编译期语境（如模板实参、数组大小等等）中使用
- [`sizeof`](language/sizeof "language/sizeof")
- [`alignof`](language/alignof "language/alignof")
- [`typeid`](language/typeid "language/typeid")
- [throw 表达式](language/throw "language/throw")

### 初等表达式

任何运算符的操作数都可以是其他的表达式或初等表达式（例如，1 + 2 \* 3 中 operator+ 的操作数是子表达式 2 \* 3 和初等表达式 1）。

初等表达式包括以下各项：

- [`this`](language/this "language/this")
- 字面量（例如 2 或 "Hello, world"）
- 标识表达式，包括
  - 经过适当声明的[无限定的标识符](language/identifiers#.E6.97.A0.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "language/identifiers")（例如 n 或 cout），
  - 经过适当声明的[有限定的标识符](language/identifiers#.E6.9C.89.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "language/identifiers")（例如 [std::string::npos](string/basic_string/npos "string/basic string/npos")），以及
  - 在[声明符](language/declarations#.E5.A3.B0.E6.98.8E.E7.AC.A6 "language/declarations")中将要声明的标识符

括号中的任何表达式也被归类为初等表达式：这确保了括号具有比任何运算符更高的优先级。括号保持值、类型和值类别不变。

#### 字面量

字面量是 name 程序中用以表现嵌入到源代码中的常量值的记号。

- [整数字面量](language/integer_literal "language/integer literal")是整数类型的十进制、八进制、十六进制或二进制的数值
- [字符字面量](language/character_literal "language/character literal")是有下列类型之一的单个字符：
- `c8`, `c16`, `c32`
- [浮点字面量](language/floating_literal "language/floating literal")是 `f32` `f64` `f128` 类型的值
- [十进制浮点字面量](/language/decimal_literal "language/decimal literal) 是 `d32` `d64` `d128` 类型的值
- [字符串字面量](language/string_literal "language/string literal")是有下列类型之一的字符序列：
- `const c8`, `const c16`, `const c32`
- [布尔字面量](language/bool_literal "language/bool literal")是 bool 类型的值，即 true 和 false

### 不求值表达式

运算符 [`typeid`](language/typeid "language/typeid")、[`sizeof`](language/sizeof "language/sizeof")、[`noexcept`](language/noexcept "language/noexcept") 和 [`decltype`](language/decltype "language/decltype")  的操作数是不求值表达式（除非运算符是 `typeid` 且操作数是多态泛左值），因为这些运算符只会查询它们的操作数的编译期性质。因此，[std::size\_t](/types/size_t) n \= sizeof([std::cout](/io/cout) << 42); 不进行控制台输出。

不求值的运算数会被当做*完整表达式*，即便它们在语法上是某个更大的表达式的操作数也是如此（例如，这意味着 sizeof(T()) 要求 `T::~T` 可访问）

[requires 表达式](language/constraints "language/constraints")也是不求值表达式。

### 弃值表达式

*弃值表达式*是只用来实施其副作用的表达式。从这种表达式计算的值被舍弃。这样的表达式包括任何[表达式语句](language/statements#.E8.A1.A8.E8.BE.BE.E5.BC.8F.E8.AF.AD.E5.8F.A5 "language/statements")的完整表达式，内建逗号运算符的左边的实参，以及转型到类型 void 的转型表达式的实参。

弃值表达式的计算结果永远不会进行数组到指针和函数到指针转换。只有在该表达式是 [有 volatile 限定的](language/cv "language/cv")泛左值 ，并具有下列形式之一（必须为它的内建含义，可以有括号）时才会进行左值到右值转换：

- 标识表达式 (id-expression)
- 数组下标表达式
- 类成员访问表达式
- 间接寻址
- 成员指针操作
- 条件表达式，它的第二个和第三个操作数都是这些表达式中的一种
- 逗号表达式，它的右操作数是这些表达式中的一种。

此外，如果该左值拥有具有 volatile 限定的类类型，那么要求用 volatile 复制构造函数来初始化作为结果的右值临时量。

如果表达式（经过可能会发生的任何左值向右值转换之后）是非 void 纯右值，那么就会进行[临时量实质化](language/implicit_conversion#.E4.B8.B4.E6.97.B6.E9.87.8F.E5.AE.9E.E8.B4.A8.E5.8C.96 "language/implicit conversion")。

当表达式声明为 [`[[nodiscard]]`](language/attributes/nodiscard "language/attributes/nodiscard") 的值，而它不是转型为 `void` 的表达式时，编译器可以发布警告。

### 参阅
