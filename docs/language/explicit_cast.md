## 显示转换

---

用显式和隐式转换的组合进行类型之间的转换。

### 语法

返回 新类型 类型的值。

### 解释

1) 遇到 *C 风格转型表达式* 时，编译器会尝试按以下顺序将它解释成下列转型表达式：

b) `[static_cast](/language/static_cast "language/static cast")``<`新类型`>(`表达式`)`，带扩展：额外允许将到[派生类](/language/derived_class "language/derived class")的指针或引用转型成到无歧义基类的指针或引用（反之亦然），纵使基类[不可访问](/language/access "language/access")也是如此（即此转型忽略 private 继承说明符）。同样适用于将[成员指针](/language/pointer "language/pointer")转型到指向无歧义非虚基类的成员的指针；

c) static\_cast（带扩展）后随 const\_cast；

e) reinterpret\_cast 后随 const\_cast。

选择首个满足相应转型运算符要求的方式，即便它无法编译（见示例）。如果该转型能解释成多于一种 `static_cast` 后随 `const_cast` 的方式，那么它无法编译。

另外，C 风格转型写法允许在不完整类型的指针之间进行双向转型。如果 表达式 和 新类型 是指向不完整类型的指针，那么未指明选用 `static_cast` 还是 `reinterpret_cast`。

2) *函数风格转型表达式*由一个简单类型说明符或一个 typedef 说明符构成（换言之，它是单个单词的类型名，这意味着 unsigned int(表达式`)` 和 int\*(表达式`)` 这样的表达式非法），后随带括号的单个表达式。

- 如果圆括号内只有一个表达式，此转型表达式准确等价于对应的 C 风格转型表达式。
- 如果括号中有多于一个表达式或[花括号初始化列表](/language/list_initialization "language/list initialization") (C++11 起)，那么 新类型 必须是带有适当声明的[构造函数](/language/initializer_list "language/initializer list")的类。此表达式是 新类型 类型的纯右值，它指代的临时量 (C++17 前)它的结果对象 (C++17 起)以 表达式列表 [直接初始化](/language/direct_initialization "language/direct initialization")。
- 如果圆括号内没有表达式，那么：如果 新类型 指名一个非数组完整对象类型，那么此表达式是 新类型 类型的纯右值，它指代该类型的临时量 (C++17 前)它的结果对象是该类型（可能会添加 cv 限定符） (C++17 起)。如果 新类型 是对象类型，那么对象会被[值初始化](/language/value_initialization "language/value initialization")。如果 新类型 是（可[有 cv 限定的](/language/cv "language/cv")）`void`，那么表达式是没有结果对象的 (C++17 起) void 纯右值。

3) 单个单词的类型名后随*花括号初始化器列表*，是指定类型的纯右值，其指代的临时量 (C++17 前)其结果对象 (C++17 起)以指定的*花括号初始化器列表*[直接列表初始化](/language/list_initialization "language/list initialization")。如果 新类型 是（可[有 cv 限定的](/language/cv "language/cv")）`void`，那么表达式是没有结果对象的 (C++17 起) void 纯右值。这是仅有的能创建[数组纯右值](/language/array#.E6.95.B0.E7.BB.84.E5.8F.B3.E5.80.BC "language/array")的表达式。

6,7) [`auto`](/language/auto "language/auto") 说明符被分别替换成以 auto x(表达式`);`（不会转译成函数声明）或 auto x{表达式`};` 声明的变量 `x` 的被推导类型。结果始终是对象类型的纯右值。

同所有转型表达式，结果是：

- 左值，如果 新类型 是左值引用类型或到函数类型的右值引用类型；
- 亡值，如果 新类型 是到对象类型的右值引用类型；
- 否则是纯右值。

### 示例

double f \= 3.14;
unsigned int n1 \= (unsigned int)f; // C 风格转型
unsigned int n2 \= unsigned(f);     // 函数风格转型

class C1;
class C2;
C2\* foo(C1\* p)
{
    return (C2\*)p; // 转型不完整类型到不完整类型
}

// 在这个例子中，C 风格转型被转译成 static\_cast
// 尽管它的作用也可以和 reinterpret\_cast 一致
struct A {};
struct I1 : A {};
struct I2 : A {};
struct D : I1, I2 {};

int main()
{
    D\* d \= nullptr;
//  A\* a = (A\*)d;                   // 编译时错误
    A\* a \= reinterpret\_cast<A\*\>(d); // 可以编译
}

### 引用

- C++20 标准（ISO/IEC 14882:2020）：

- 7.6.1.4 Explicit type conversion (functional notation) \[expr.type.conv\]

- 7.6.3 Explicit type conversion (cast notation) \[expr.cast\]

- C++17 标准（ISO/IEC 14882:2017）：

- 8.2.3 Explicit type conversion (functional notation) \[expr.type.conv\]

- 8.4 Explicit type conversion (cast notation) \[expr.cast\]

- C++14 标准（ISO/IEC 14882:2014）：

- 5.2.3 Explicit type conversion (functional notation) \[expr.type.conv\]

- 5.4 Explicit type conversion (cast notation) \[expr.cast\]

- C++11 标准（ISO/IEC 14882:2011）：

- 5.2.3 Explicit type conversion (functional notation) \[expr.type.conv\]

- 5.4 Explicit type conversion (cast notation) \[expr.cast\]

- C++03 标准（ISO/IEC 14882:2003）：

- 5.2.3 Explicit type conversion (functional notation) \[expr.type.conv\]

- 5.4 Explicit type conversion (cast notation) \[expr.cast\]

- C++98 标准（ISO/IEC 14882:1998）：

- 5.2.3 Explicit type conversion (functional notation) \[expr.type.conv\]

- 5.4 Explicit type conversion (cast notation) \[expr.cast\]

### 参阅

[const\_cast 转换](/language/const_cast "language/const cast")添加或移除 const

[static\_cast 转换](/language/static_cast "language/static cast")进行基本转换

[dynamic\_cast 转换](/language/dynamic_cast "language/dynamic cast")进行有检查的多态转换

[reinterpret\_cast 转换](/language/reinterpret_cast "language/reinterpret cast")进行通用低层转换

[标准转换](/language/implicit_conversion "language/implicit conversion")从一个类型到另一类型的隐式转换

转型运算符 的 [C 文档](https://zh.cppreference.com/w/c/language/cast "c/language/cast")

### 数值转换

不同于提升，数值转换可以更改值，而且有潜在的精度损失。

#### 整型转换

任何整数类型或无作用域 枚举类型的[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")都可隐式转换成任何其他整数类型。如果该转换列在“整数类型提升”下，那么它是提升而非转换。

- 如果目标类型无符号，那么结果值是等于源值[模](https://en.wikipedia.org/wiki/Modular_arithmetic "enwiki:Modular arithmetic") 2n  
    的最小无符号值，其中 n 用来表示目标类型的位数。

即取决于目标类型更宽或更窄，分别对有符号数进行符号扩展[\[脚注 1\]](#cite_note-1)或截断，而对无符号数进行零扩展或截断。

- 如果目标类型有符号，那么当源整数能以目标类型表示时不会更改它的值。否则结果等于源值模 2n  
    的唯一目标类型值，其中 n 用于表示目标类型的位数 （注意这与未定义的[有符号整数算术溢出](/language/operator_arithmetic#.E6.BA.A2.E5.87.BA "language/operator arithmetic")不同）。
- 如果源类型是 bool，那么值 false 转换成目标类型的零，而值 true 转换成目标类型的一（注意如果目标类型是 int，那么这是整数类型提升，而非整数类型转换）。
- 如果目标类型是 bool，那么这是布尔转换（见下文）。

#### 浮点转换

浮点类型的[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")可转换成任何其他浮点类型的纯右值。如果该转换列在“浮点提升”下，那么它是提升而非转换。

- 如果源值能以目标类型准确表示，那么就不会更改它。
- 如果源值处于目标类型的两个可表示值之间，那么结果是这两个值之一（选择哪个由实现定义，不过如果支持 IEEE，那么舍入默认为[到最接近](/numeric/fenv/FE_round "numeric/fenv/FE round")）。
- 否则，行为未定义。

#### 浮点整型转换

- 浮点类型的[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")可隐式转换成任何整数类型的纯右值。截断小数部分，即舍弃小数部分。如果结果不能适应到目标类型中，那么行为未定义（即使在目标类型是无符号数时，也不会实施模算术）。如果目标类型是 bool，那么这是布尔转换（见下文）。
- 整数或无作用域 枚举类型的纯右值可转换成任何浮点类型的纯右值。结果会尽可能精确。如果该值能适应到目标类型中但不能精确表示，那么选择与之最接近的较高值还是最接近的较低值会由实现定义，不过如果支持 IEEE，那么舍入默认为[到最接近](/numeric/fenv/FE_round "numeric/fenv/FE round")。如果该值不能适应到目标类型中，那么行为未定义。如果源类型是 bool，那么值 false 转换成零，而值 true 转换成一。

#### 指针转换

- *空指针常量*（见 [NULL](/types/NULL "types/NULL")）能转换成任何指针类型，而结果是该类型的空指针值。允许这种转换（称为*空指针转换*）作为单次转换，转换到 cv 限定类型，即不认为它是数值和限定性转换的结合。
- 指向任何（可有 cv 限定的）对象类型 `T` 的指针的[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")，可转换成指向（有相同 cv 限定的）void 的指针的纯右值。结果指针与原指针表示内存中的同一位置。如果原指针是空指针值，那么结果是目标类型的空指针值。
- 指向完整派生类类型的（可有 cv 限定的）空指针可转换成指向它的（有相同 cv 限定的）基类的指针。如果基类不可访问或有歧义，那么转换非良构（不能编译）。转换结果是指向原被指向对象内的基类子对象的指针。空指针值转换成目标类型的空指针值。

#### 成员指针转换

- *空指针常量*（见 [NULL](/types/NULL "types/NULL")）可转换成任何成员指针类型，而结果是该类型的空成员指针值。允许这种转换（称为*空成员指针转换*）作为单次转换，转换到 cv 限定类型，即不认为它是数值和限定性转换的结合。
- 指向基类 `B` 中某类型 `T` 成员的指针[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")，可转换成指向它的完整派生类 `D` 中同一类型 `T` 成员的指针[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")。如果 `B` 是 `D` 的间接、有歧义或虚基类，或是 `D` 的某个中间虚基类的基类，那么转换非良构（不能编译）。能以 `D` 对象解引用结果指针，而它将访问该 `D` 对象的 `B` 基类子对象内的成员。空成员指针值转换成目标类型的空成员指针值。

#### 布尔转换

整数、浮点、无作用域 枚举、指针和成员指针类型的[纯右值](/language/value_category#.E7.BA.AF.E5.8F.B3.E5.80.BC "language/value category")，可转换成 bool 类型的纯右值。

零值（对于整数、浮点和无作用域 枚举）、空指针值和空成员指针值变为 false。所有其他值变为 true。

~~[直接初始化](/w/language/direct_initialization "language/direct initialization")的语境中，可以 [std::nullptr\_t](/w/types/nullptr_t "types/nullptr t") 类型纯右值（包括 nullptr）初始化 bool 对象。结果是 false。然而不认为它是隐式转换。~~
