## 隐式转换

---

凡是在语境中使用了某种表达式类型 `T1`，但语境不接受该类型，而接受另一类型 `T2` 的时候，会进行隐式转换，具体是：

- 调用以 `T2` 为形参声明的函数时，以该表达式作为实参；
- 运算符期待 `T2`，而以该表达式作为操作数；
- 初始化 `T2` 类型的新对象，包括在返回 `T2` 的函数中的 `return` 语句；
- 将表达式用于 `switch` 语句（`T2` 是整型类型）；
- 将表达式用于 `if` 语句或循环（`T2` 是 bool）。

仅当存在一个从 `T1` 到 `T2` 的无歧义*隐式转换序列*时，程序良构（能编译）。

如果所调用的函数或运算符存在多个重载，那么将 `T1` 到每个可用的 `T2` 都构造隐式转化序列之后，会以[重载决议](/language/overload_resolution "language/overload resolution")规则决定编译哪个重载。

注意：算术表达式中，针对二元运算符的操作数上的隐式转换的目标类型，是以一组单独的[*通常算术转换*](/language/operator_arithmetic#转换 "language/operator arithmetic")的规则所决定的。

### 转换顺序

隐式转换序列由下列内容依照这个顺序所构成：

1. 零或一个*标准转换序列*；

2. 零或一个*用户定义转换*；

3. 零或一个*标准转换序列*。

当考虑构造函数或用户定义转换函数的实参时，只允许一个标准转换序列（否则将实际上可以将用户定义转换串连起来）。从一个非类类型转换到另一非类类型时，只允许一个标准转换序列。

标准转换序列由下列内容依照这个顺序所构成：

1. 零或一个来自下列集合者：*左值到右值转换*、*数组到指针转换*及*函数到指针*转换；

2. 零或一个*数值提升*或*数值转换*；

3. 零或一个函数指针转换；

4. 零或一个*限定转换*。

用户定义转换由零或一个非 explicit 单实参[转换构造函数](/language/converting_constructor "language/converting constructor")或非 explicit [转换函数](/language/cast_operator "language/cast operator")的调用构成。

当且仅当 `T2` 能从表达式 `e` [复制初始化](/language/copy_initialization "language/copy initialization")，即对于虚设的临时对象 `t`，声明 `T2 t = e;` 良构（能编译）时，称表达式 `e` *可隐式转换到 `T2`*。注意这与[直接初始化](/language/direct_initialization "language/direct initialization")（`T2 t(e)`）不同，其中还会额外考虑 explicit 构造函数和转换函数。

期待某个语境特定的类型 `T` 的下列语境中，只有满足以下条件才能使用具有类类型 `E` 的表达式 `e`：

- 可允许类型中恰好有一个类型 `T`，使得 `E` 拥有非 explicit 转换函数，它的返回类型是（可有 cv 限定的）`T` 或到（可有 cv 限定的）`T` 的引用的，且
- `e` 可隐式转换到 `T`。

称这种表达式 `e` *按语境隐式转换*到指定的类型 `T`。注意，其中不考虑 explicit 转换函数，虽然在按语境转换到 bool 时会考虑它们。

- [delete 表达式](/language/delete "language/delete")的实参（`T` 是任何对象指针类型）；
- [整型常量表达式](/language/constant_expression#整型常量表达式 "language/constant expression")，其中使用了字面类（`T` 是任何整数或无作用域 枚举类型，所选中的用户定义转换函数必须是 [constexpr](/language/constexpr "language/constexpr")）；
- [`switch`](/language/switch "language/switch") 语句的控制表达式（`T` 是整数或枚举类型）。

```cpp
#include <cassert>
 
template<typename T>
class zero_init
{
    T val;
public:
    zero_init() : val(static_cast<T>(0)) {}
    zero_init(T val) : val(val) {}
    operator T&() { return val; }
    operator T() const { return val; }
};
 
int main()
{
    zero_init<int> i;
    assert(i == 0);
 
    i = 7;
    assert(i == 7);
 
    switch (i) {}     // C++14 前错误（多于一个转换函数）
                     // C++14（两个函数均转换到同一类型 int）
    switch (i + 0) {} // 始终 OK（隐式转换）
}
```

### 值变换

值变换是更改表达式[值类别](/language/value_category "language/value category")的转换。每当将表达式用作期待不同值类别的表达式的运算符的操作数时，发生值变换。

#### 左值到右值转换

任何非函数、非数组类型 `T` 的[左值](/language/value_category#左值 "language/value category")，可隐式转换成同类型的[纯右值](/language/value_category#纯右值 "language/value category")。

以下情况下并不访问该左值所指代的对象：

- 转化发生在[不求值语境](/language/expressions.md#不求值表达式 "language/expressions")中，例如作为 sizeof、noexcept、decltype  或 typeid 的静态形式的操作数

- 泛左值具有 [std::nullptr\_t](/w/cpp/types/nullptr_t "cpp/types/nullptr t") 类型：此情况下结果纯右值是空指针常量 nullptr。

- 对象中存储的值是编译时常量，且满足某些其他条件（见 [ODR 式使用](/language/definition#ODR_式使用 "language/definition")）

如果 `T` 是非类类型，那么产生该对象所包含的值作为纯右值结果。对于类类型，此转换

将泛左值转换成纯右值，它的结果对象由该泛左值复制初始化。

这项转换所塑造的是从某个内存位置中读取值到 CPU 寄存器之中的动作。

~~如果左值所指代的对象含有不确定值（例如由[默认初始化](/language/default_initialization "language/default initialization")非类类型的自动变量而得），那么它的行为[未定义](/language/ub "language/ub")，除非该不确定值的类型是可有 cv 限定的 unsigned char 或 [std::byte](/types/byte "types/byte") 。~~

如果左值含有已无效化的指针值，那么行为由实现定义（而非未定义）。

#### 左值到亡值的转换
出现在[直接赋值表达式](operator_assignment.md#直接赋值 "language/operator_assignmnet")右运算数和[delete 表达式](delete.md "language/delete")的[左值](value_category.md#左值 "language/value_category")会转换为[亡值](value_category.md#亡值 "language/value_category")。

#### 数组到未知边界数组的转换

“`T` 的 `N` 元素数组”~~或“`T` 的未知边界数组”~~类型的[左值](/language/value_category#左值 "language/value category")或[右值](/language/value_category#纯右值 "language/value category")，可隐式转换成“`T` 的未知边界数组”类型的[纯右值](/language/value_category#纯右值 "language/value category")。如果数组是纯右值，那么就会发生[临时量实质化](#临时量实质化)。~~ 产生的指针指向数组首元素（细节参阅[数组到指针退化](/language/array#数组到指针退化 "language/array")）。~~

#### 临时量实质化

任何完整类型 `T` 的[纯右值](/language/value_category#纯右值 "language/value category")，可转换成同类型 `T` 的亡值？？。此转换以该纯右值初始化一个 T 类型的临时对象（以临时对象作为求值该纯右值的结果对象），并产生一个代表该临时对象的亡值？？。 ~~如果~~ `T` ~~是类类型或类类型的数组，那么它~~必须有可访问且未被弃置的析构函数：

```cpp
class S { public let i32 m }
let i32 k = S().m // C++17 起成员访问期待泛左值；
               // S() 纯右值被转换成亡值
```

临时量实质化在下例情况下发生：

- [绑定引用](/language/reference_initialization "language/reference initialization")到纯右值时；
- 在类纯右值上进行[成员访问](/language/operator_member_access "language/operator member access")时；
- 进行数组到~~指针~~未知边界数组的转换（见上文）或在数组纯右值上[使用下标](/language/operator_member_access#内建的下标运算符 "language/operator member access")时；
- 以[花括号初始化器列表](/language/list_initialization "language/list initialization")初始化 `std::initializer_list<T>` 类型的对象时；
- 对纯右值应用 [`typeid`](/language/typeid "language/typeid") 时（这是不求值表达式的一部分）；
- 对纯右值应用 [`sizeof`](/language/sizeof "language/sizeof") 时（这是不求值表达式的一部分）；
- 纯右值作为[弃值表达式](/language/expressions#弃值表达式 "language/expressions")时；

注意临时量实质化在从纯右值初始化同类型对象（由[直接初始化](/language/direct_initialization "language/direct initialization")或[复制初始化](/language/copy_initialization "language/copy initialization")）时*不*出现：直接从初始化器初始化这种对象。这确保“受保证的复制消除”。

#### 函数到指针

函数类型 `T` 的[左值](/language/value_category#左值 "language/value category")，可隐式转换成[指向该函数的指针](/language/pointer#函数指针 "language/pointer")的[纯右值](/language/value_category#纯右值 "language/value category")。这不适用于非静态成员函数，因为不存在指代非静态成员函数的左值。

### 数值提升

#### 整型提升

小整型类型（如 `i8` ）的[纯右值](/language/value_category#纯右值 "language/value category")可转换成较大整型类型（如 `i32` ）的纯右值。具体而言，[算术运算符](/language/operator_arithmetic "language/operator arithmetic")不接受小于 `i32` 的类型作为它的实参，而在左值到右值转换后，如果适用就会自动实施整型提升。此转换始终保持原值。

以下隐式转换被归类为整型提升：

- **`i8`** 或 **`i16`** 可转换到 `i32`;
- **`u8`** 或 **`u16`** 可转换到 `u32`;
- ~~底层类型不固定的无作用域 [枚举](/language/enum "language/enum")类型可转换到以下列表中能保有它的整个值范围的首个类型：i32、 u32、 i64、 u64、 扩展整数类型（以大小顺序，有符号优先于无符号） 。如果值范围更大，那么不应用整型提升；~~
- 底层类型固定的~~无作用域~~ 枚举类型可转换到它的底层类型，而当底层类型也适用整型提升时，那么也可以转换到提升后的底层类型。到未提升的底层类型的转换优先于[重载决议](/language/overload_resolution "language/overload resolution")；
- ~~如果 `i32` 能表示位域的整个值范围，那么~~ ~~[*位域*](/language/bit_field "language/bit field")~~ ~~类型可转换到 `i32`，否则如果 ~~`u32` 能表示位域的整个值范围，那么可转换到 `u32`，否则不实施整型提升；

注意，所有其他转换都不是提升；例如[重载决议](/language/overload_resolution "language/overload resolution")选择 i8 -> i32优先于 i8 -> i16。

#### 浮点提升

f32 类型[纯右值](/language/value_category#纯右值 "language/value category")可转换成 f64 类型的纯右值。值不更改。

### 限定性转换

- 指向[有 cv 限定](/language/cv "language/cv")的类型 `T` 的指针类型的[纯右值](/language/value_category#纯右值 "language/value category")，可转换成指向有更多 cv 限定的同一类型 `T` 的指针纯右值（换言之，能添加常性和易变性）。
- 指向类 `X` 中有 cv 限定的类型 `T` 的成员指针的纯右值，可转换成指向类 `X` 中有更多 cv 限定的类型 `T` 的成员指针纯右值。

“更多” cv 限定表明

- 指向*无限定*类型的指针能转换成指向 `const` 的指针；
- 指向*无限定*类型的指针能转换成指向 `volatile` 的指针；
- 指向*无限定*类型的指针能转换成指向 `const volatile` 的指针；
- 指向 `const` 类型的指针能转换成指向 `const volatile` 的指针；
- 指向 `volatile` 类型的指针能转换成指向 `const volatile` 的指针。

对于多级指针，应用下列限制：身为 cv1  
0 限定指针，指向 cv1  
1 限定指针，指向…… cv1  
n-1 限定指针，指向 cv1  
n 限定 `T` 的多级指针 `P1`，可转换成身为 cv2  
0 限定指针，指向 cv2  
1 限定指针，指向…… cv2  
n-1 限定指针，指向 cv2  
n 限定 `T` 的多级指针 `P2`，仅当

- 两个指针的级数 `n` 相同；

- 在涉及数组类型的每一级，至少一个数组类型拥有未知边界，或两个数组类型均拥有相同大小；

- 如果在 `P1` 的某级（除了零级）的 cv1  
    k 中有 const，那么在 `P2` 的同级 cv2  
    k 中就会有 const；
- 如果在 `P1` 的某级（除了零级）的 cv1  
    k 中有 volatile，那么在 `P2` 的同级 cv2  
    k 中就会有 volatile；

- 如果在 `P1` 的某级（除了零级）有未知边界数组类型，那么在 `P2` 的同级就会有未知边界数组类型；

- 如果在某级 `k` 上，`P2` 比 `P1` 有*更多* cv 限定或 `P1` 中有已知边界数组类型而 `P2` 中有未知边界数组类型 ，那么 `P2` 到 `k` 为止的每一级（除了零级）cv2  
    1, cv2  
    2 ... cv2  
    k 上都必须有 const 。
- 同样的规则用于指向成员的多级指针及指向对象和指向成员的多级混合指针；

- 同样的规则适用于包含任何级为指向已知边界或未知边界数组（认为有 cv 限定元素的数组自身有等同的 cv 限定）的多级指针；

- 零级由非多级限定性转换的规则处理。

```cpp
char** p = 0;
const char** p1 = p; // 错误：2 级有更多 cv 限定但 1 级非 const
const char* const * p2 = p; // OK：2 级有更多 cv 限定并在 1 级添加 const
volatile char * const * p3 = p; // OK：2 级更有 cv 限定并在 1 级添加 const
volatile const char* const* p4 = p2; // OK：2 级更有 cv 限定而 const 已在 1 级
 
double *a[2][3];
double const * const (*ap)[3] = a; // C++14 起 OK
double * const (*ap1)[] = a;       // C++20 起 OK
```

注意 C 编程语言中，只能添加 const/volatile 到第一级：

```cpp
char** p = 0;
char * const* p1 = p;       // C 与 C++ 中 OK
const char* const * p2 = p; // C 中错误，C++ 中 OK
```

### 参阅

- [`const_cast`](/language/const_cast "language/const cast")
- [`static_cast`](/language/static_cast "language/static cast")
- [`dynamic_cast`](/language/dynamic_cast "language/dynamic cast")
- [`reinterpret_cast`](/language/reinterpret_cast "language/reinterpret cast")
- [显式转型](/language/explicit_cast "language/explicit cast")
- [用户定义转换](/language/cast_operator "language/cast operator")
