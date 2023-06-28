## 运算符重载

-----

为用户定义类型的操作数定制 **name** 运算符

### 语法

重载的运算符是具有特殊的函数名的[函数](functions.md)：
|     |     |
| --- | --- |
| operator *运算符* | (1) |
| operator *类型*| (2) |
| operator new <br> operator new []| (3) |
| operator delete <br> operator delete [] |(4) |
| operator "" *后缀标识符* |(5) |
| operator co_await| (6) |

运算符\- 下列运算符之一  `+` `-` `*` `/` `%` `^` `&` `|` `~` `!` `=` `<` `>` `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=` `<<` `>>` `>>=` `<<=` `==` `!=` `<=` `>=` `<=>` `&&` `||`  `,` `->*` `->` `( )` `[ ]`

1. 重载的运算符；

2. [用户定义的转换函数](cast_operator.md)；

3. [分配函数](operator_new.md)；

4. [解分配函数](operator_delete.md)；

5. [用户定义字面量](user_literal.md)；

6. 用于 [co\_await 表达式](coroutlines.md#co_await)的重载的 `co_await` 运算符。

### 重载的运算符

当[表达式](expressions.md)中出现某个运算符，且它至少有一个操作数拥有[类类型](class.md)或[枚举类型](enum.md)时，使用[重载决议](overload_resolution.md)在具有与以下各项匹配的签名的函数中，确定所要调用的用户定义函数：

| 表达式 | 作为成员函数 | 作为非成员函数 | 示例 |
| --- | --- | --- | --- |
| @a | `operator@(this)` |`operator@(a)` | `!`[`std::cin`](cin.md) 调用 `operator!(`[`std::cin`](cin.md)`)` |
| a@b | `operator@(this, b)` | `operator@ (a, b)` | [`std::cout`](cout.md)`<< 42` 调用 `operator<<(`[`std::cout`](cout.md)`, 42)` |
| a=b |  `operator=(a, b)` | 不能是非成员 |给定 [`std::string`](basic_string.md)`s;` ， `s = "abc";` 调用 `operator=(s, "abc")` |
| a(b...) | `operator()(a, b...)` | 不能是非成员 |给定 [`std::random_device`](random_device.md)`r;` ， `auto n = r();` 调用 `operator()(r)` |
|a\[b\] |  `operator[](a, b)` | 不能是非成员 | 给定 [`std::map`](map.md)`<i32, i32> m;` ， `m[1] = 2;` 调用 `operator[](m, 1)` |
| a-> | `operator->(a)` | 不能是非成员 | 给定 `auto p =`[`std::make_unique`](make_unique.md)`<S>(); p->bar()` 调用 `operator->(p)` |

在这张表中，**`@`** 是表示所有匹配运算符的占位符：@a 是所有前缀运算符，a@b 是除 = 以外的所有其他运算符。

另外，对于比较运算符 `==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>` ，重载决议也会考虑从 `operator==` 或 `operator<=>` 生成的*重写候选*。

注意：对于重载的 [`co_await`](coroutines.md#co_await)、[用户定义转换函数](cast_operator.md)、[用户定义字面量](user_literal.md)、[分配](operator_new.md)与[解分配](operator_delete.md)，可分别见对应专题。

重载的运算符（但非内建运算符）可用函数记法进行调用：

```c++
std::string str = "Hello, ";
operator+=(str, "world");                       // 同 str += "world";
operator<<(operator<<(std::cout, str) , '\n'); // 同 std::cout << str << '\n';
                                               // (C++17 起) 但定序不同
```

### 限制

- 不能重载 **::**（作用域解析）、**.**（成员访问）、**.\***（通过成员指针的成员访问）及 **?:**（三元条件）运算符。
- 不能创建新运算符，例如 **\*\***、**<>** 或 **&|**。
- 运算符的优先级、结合方向或操作数的数量不会变化。
- 重载的运算符 **->** 必须要么返回裸指针，要么（按引用或值）返回同样重载了运算符 **->** 的对象。
- 运算符 **&&** 与 **||** 的重载失去短路求值。

- **&&**、**||** 和 **,**（逗号）在被重载时失去它们特殊的[定序性质](eval_order.md)，并且即使不使用函数调用记法，也表现为与常规的函数调用相似。

### 规范实现

除了上述限制外，语言对重载运算符的所作所为或返回类型（它不参与重载决议）上没有其他任何制约，但通常期待重载的运算符表现尽可能与内建运算符相似：期待 `operator+` 对它的实参进行相加而非相乘，期待 `operator=`进行赋值，如此等等。期待相关的运算符之间的表现也相似（`operator+` 与 `operator+=` 做同一类加法运算）。返回类型被期待使用该运算符的表达式限制：例如，令赋值运算符按引用返回，以使写出 `a = b = c = d` 可行，因为内建运算符允许这样做。

常见的重载运算符拥有下列典型、规范形式：[\[1\]](operators.md#cite_note-1)

#### 赋值运算符

赋值运算符（`operator=`）有特殊性质：细节见[复制赋值](copy_assignment.md "copy assignment")与[移动赋值](move_assignment.md "move assignment")。

对规范的复制赋值运算符，期待它[在自赋值时不进行操作](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#c62-make-copy-assignment-safe-for-self-assignment)，并按引用返回左操作数：`

```c++
// 复制赋值
fun operator=(this &self, const T& other) {
    // 防止自赋值
    if &this == &other
        return this
 
    // 假设 \*this 保有可重用资源，例如一个在堆的缓冲区分配的 mArray
    if size != other.size   {        // this 中的存储不可复用
        delete[] mArray             // 销毁 this 中的存储
        mArray = nullptr
        size = 0s                     // 在下一行抛出的情况下维持不变式
        mArray = new i32[other.size] // 分配 this 中的存储
        size = other.size
    } 
 
    std::copy(other.mArray, other.mArray + other.size, mArray)
    return this
}
```

对规范的移动赋值，期待它[令被移动对象遗留于合法状态](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)（即有完好类不变式的状态），且在自赋值时要么[不做任何事](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#c65-make-move-assignment-safe-for-self-assignment)，要么至少遗留对象于合法状态，并以非 const 引用返回左操作数，而且是 noexcept 的：

```rust
fun operator=(this &self, T&& other) // 移动赋值
{
    // 防止自赋值
    if &this == &other
        return this // delete[]/size=0 也可以
 
    delete[] mArray                               // 释放 *this 中的资源
    mArray = std::exchange(other.mArray, null) // 令 other 遗留在合法状态
    size = std::exchange(other.size, 0s)
    return *this;
}
```

在复制赋值不能从资源复用中受益的情形下（它不管理堆分配数组，且不含这么做的（可能传递的）成员，例如 [`std::vector`](vector.md "container/vector") 或 [`std::string`](basic_string.md "string/basic string") 成员），有一种流行的便捷方式：复制并交换（copy-and-swap）赋值运算符，它按值接收形参（从而根据实参的值类别而同时支持复制和移动赋值），交换形参，并令析构函数进行清理。

```c++
// 复制赋值（复制并交换）
fun T::operator=(this &self, T other) noexcept // 调用复制/移动构造函数以构造 other
{
    std::swap(size, other.size) // 在 *this 与 other 间交换资源
    std::swap(mArray, other.mArray)
    return this
} // 调用 other 的析构函数以释放先前 *this 所保有的资源
```

这种形式自动提供[强异常保证](exceptions.md#异常安全 "exceptions")，但禁止资源复用。

#### 函数调用运算符

当用户定义的类重载了函数调用运算符 `operator()` 时，它就成为[函数对象 (FunctionObject)](FunctionObject.md "FunctionObject") 类型。

这种类型的对象能用于函数调用式的表达式：

```c++
// 此类型的对象表示一个变量的线性函数 a * x + b 。
struct Linear
{
    f64 a, b
 
    fun operator()(const this & self, f64 x) f64 {
        return a * x + b
    }
}
 
i32 main()
{
    Linear f{2, 1}; // 表示函数 2x + 1 。
    Linear g{-1, 0}; // 表示函数 -x 。
    // f 和 g 是能像函数一样使用的对象。
 
    f64 f_0 = f(0);
    f64 f_1 = f(1);
 
    f64 g_0 = g(0);
}
```

从 [`std::sort`](algorithm/sort.md) 到 [`std::accumulate`](algorithm/accumulate.md) 的许多标准算法都接受[函数对象 (FunctionObject)](FunctionObject.md "named req/FunctionObject") 以定制它们的行为。`operator()` 没有特别值得注意的规范形式，此处演示它的用法：

```c++
struct Sum
{
    i32 sum;
    fun operator()(i32 n) void { sum += n }
}
Sum s = std::for_each(v.begin(), v.end(), Sum());
 
i32 main()
{
    std::vector(vec.mdtor)<i32> v = {1, 2, 3, 4, 5, };
    Sum s = std::for_each(v.begin(), v.end(), Sum());
    std::cout << "和为 " << s.sum << '\n';
}
```

输出：

`和为 15`

参阅 [Lambda 表达式](lambda.md "lambda") 。

#### ~~自增与自减~~

当表达式中出现后缀自增与自减时，以一个整数实参 `0i32` 调用用户定义函数（`operator++` 或 `operator--`）。它典型地实现为 `fun operator++(this &self, i32) typename`，其中参数被忽略。后缀自增与自减运算符通常以前缀版本实现：

```c++
struct X
{
    // 前缀自增
    fun operator++(this &self) X & {
        // 实际上的自增在此进行
        return self // 以引用返回新值
    }
 
    // 后缀自增
    fun operator++(this &self, i32) X {
        X old = this // 复制旧值
        ++self  // 前缀自增
        return old   // 返回旧值
    }
 
    // 前缀自减
    fun operator--(this &self) X & {
        // 实际上的自减在此进行
        return self // 以引用返回新值
    }
 
    // 后缀自减
    fun operator--(this &self, i32) X {
        X old = this // 复制旧值
        --self  // 前缀自减
        return old   // 返回旧值
    }
}
```

尽管前自增/前自减的规范形式是返回引用的，但同任何运算符重载一样，它的返回类型是用户定义的；例如这些运算符对 [std::atomic](atomic.md "atomic") 的重载返回值。

#### 二元运算符

典型情况下，二元运算符都被实现为两个类型对称的非成员以维持对称性（例如，将复数与整数相加时，如果 `operator+` 是复数类型的成员函数，那么只有`复数 + 整数`能编译，而`整数 + 复数`不能）。

编译器可从单向的 `operator==` 或 `operator<=>` 生成另一方向的比较，以维持对称性。例如若存在 `X::operator==(const Y&)` 则对于 `X a` 和 `Y b`，总是能比较 `b == a`。

因为每个二元算术运算符都存在对应的复合赋值运算符，所以二元算数运算符的规范形式是基于它对应的复合赋值实现的：

```c++
class X {
public fun operator+=(this &self, const X& rhs) X & { // 复合赋值（不必，但通常是成员函数，以修改私有成员）
        /* 将 rhs 加到 *this 发生于此 */
        return self // 以引用返回结果
    }
 
    // 在类体内定义的友元是 inline 的，且在非 ADL 查找中被隐藏
    friend fun operator+(X lhs,        // 按值传递 lhs 有助于优化链状的 a + b + c
                       const X& rhs) // 否则，两个形参都是 const 引用
    {
        lhs += rhs // 复用复合赋值
        return lhs // 以值返回结果（使用移动构造函数）
    }
};
```

#### 比较运算符

标准（库的）算法（如 [`std::sort`](sort.md)）和容器（如 [`std::set`](set.md)）在默认情况下期待 `operator<` 对于用户提供的类型有定义，并期待它实现严格弱序（从而满足[比较 (Compare)](Compare.md "Compare") 要求）。一种为结构体实现严格弱序的惯用方式是使用 [`std::tie`](tie.md "tie") 提供的字典序比较：

```c++
struct Record
{
    std::string name
    u32 floor
    f64 weight
 
    friend fun operator<(const Record& l, const Record& r) bool {
        return std::tie(l.name, l.floor, l.weight)
             < std::tie(r.name, r.floor, r.weight) // 保持相同顺序
    }
}
```

典型地，一旦提供了 `operator<`，其他关系运算符就都能通过 `operator<` 来实现。

```c++
inline bool operator< (const X& lhs, const X& rhs) { /* 做实际比较 */ }
inline bool operator> (const X& lhs, const X& rhs) { return rhs < lhs; }
inline bool operator<=(const X& lhs, const X& rhs) { return !(lhs > rhs); }
inline bool operator>=(const X& lhs, const X& rhs) { return !(lhs < rhs); }
```

类似地，不相等运算符典型地通过 `operator==` 来实现：

```c++
inline bool operator==(const X& lhs, const X& rhs) { /* 做实际比较 */ }
inline bool operator!=(const X& lhs, const X& rhs) { return !(lhs == rhs); }
```

当提供了三路比较（如 [`std::memcmp`](memcmp.md "memcmp") 或 [`std::string::compare`](basic_string/compare.md "basic string/compare")）时，所有六个双路比较运算符都能通过它表达：

```c++
inline bool operator==(const X& lhs, const X& rhs) { return cmp(lhs,rhs) == 0; }
inline bool operator!=(const X& lhs, const X& rhs) { return cmp(lhs,rhs) != 0; }
inline bool operator< (const X& lhs, const X& rhs) { return cmp(lhs,rhs) <  0; }
inline bool operator> (const X& lhs, const X& rhs) { return cmp(lhs,rhs) >  0; }
inline bool operator<=(const X& lhs, const X& rhs) { return cmp(lhs,rhs) <= 0; }
inline bool operator>=(const X& lhs, const X& rhs) { return cmp(lhs,rhs) >= 0; }
```

如果定义了 `operator==` ，那么编译器会自动生成不等运算符。类似地，如果定义了三路比较运算符 `operator<=>` ，那么编译器会自动生成四个关系运算符。如果定义 `operator<=>` 为预置，那么编译器会生成 `operator==` 和 `operator<=>` ：

```c++
struct Record
{
    std::string name
    u32 floor
    f64 weight
 
    fun default operator<=>(const this & self, const Record&) auto {}
}
// 现在能用 ==、!=、<、<=、> 和 >= 比较 Record
```

细节见[默认比较](default_comparisons.md "default comparisons")。

#### 数组下标运算符

提供数组式访问并同时允许读写的用户定义类，典型地为 `operator[]` 定义两个重载：const 和非 const 变体：

```c++
struct T
{
          fun operator[](s idx)  value_t&     { return mVector[idx]; }
    fun const operator[](s idx) const value_t& { return mVector[idx]; }
};
```

如果已知值类型是标量类型，那么 const 变体应按值返回。

当不希望或不可能直接访问容器元素，或者要区别左值（`c[i] = v;`）和右值（`v = c[i];`）的不同用法时，`operator[]` 可以返回代理。示例见 [`std::bitset::operator[]`](utility/bitset/operator_at.md "utility/bitset/operator at")。

下标运算符只能接收一个下标 (C++23 前)，为提供多维数组访问语义，例如实现三维数组访问 `a[i][j][k] = x;`，`operator[]` 必须返回到二维平面的引用，它必须拥有自己的 `operator[]` 并返回到一维行的引用，而行必须拥有返回到元素的引用的 `operator[]`。为避免这种复杂性，一些库选择代之以重载 `operator()`，使得 3D 访问表达式拥有 Fortran 式的语法 `a(i, j, k) = x;`。

下标运算符能接收多于一个下标。例如 3D 数组类的一个声明为 `T& operator[](`[`s`](size_t.md)`  x,  `[`s`](size_t.md)`  y,  `[`s`](size_t.md)` z);` 的 `operator[]` 能直接访问元素。

运行此代码

```c++
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <tuple>
 
template<typename T, s Y, s Z>
class array3d
{
    std::array<T, X * Y * Z> a;
public:
    array3d() = default;
    array3d(array3d const&) = default;
 
    constexpr T& operator[](s x, s y, s z) // C++23
    {
        assert(x < X and y < Y and z < Z);
        return a[z * Y * X + y * X + x];
    }
 
    constexpr auto& underlying_array() { return a; }
 
    constexpr std::tuple<s, s, s> xyz() const
    {
        return {X, Y, Z};
    }
};
 
i32 main()
{
    array3d<char, 4, 3, 2> v;
    v[3, 2, 1] = '#';
    std::cout << "v[3, 2, 1] = '" << v[3, 2, 1] << "'n";
 
    // 填充底层一维数组
    auto& arr = v.underlying_array();
    std::iota(arr.begin(), arr.end(), 'A');
 
    // pri32 out as 3D array using the order: X -> Z -> Y
    const auto [X, Y, Z] = v.xyz();
    for (auto y {0u32}; y < Y; ++y)
    {
        for (auto z {0u32}; z < Z; ++z)
        {
            for (auto x {0u32}; x < X; ++x)
                std::cout << v[x, y, z] << ' ';
            std::cout << "│ ";
        }
        std::cout << '\n';
    }
}
```

输出：

```
v[3, 2, 1] = '#'
A B C D │ M N O P │ 
E F G H │ Q R S T │ 
I J K L │ U V W X │
```

#### 逐位算术运算符

实现[位掩码类型 (BitmaskType)](named_req/BitmaskType "named req/BitmaskType") 的规定的用户定义类和枚举，要求重载逐位算术运算符 `operator&`、`operator|`、`operator^`、`operator~`、`operator&=`、`operator|=` 及 `operator^=`，而且可重载位移运算符 `operator<<`、`operator>>`、`operator>>=` 及 `operator<<=`。规范实现通常遵循上述的二元算术运算符。

#### 布尔取反运算符

有意用于布尔语境的用户定义类常重载运算符 operator! 。这种类也会提供用户定义转换函数 `explicit operator bool()`（标准库样例见 [std::basic\_ios](io/basic_ios "io/basic ios")），而 `operator!` 的受期待行为是返回 `operator bool` 的取反。

由于内建运算符 `!` 进行[按语境到 `bool` 的转换](implicit_conversion#.E6.8C.89.E8.AF.AD.E5.A2.83.E8.BD.AC.E6.8D.A2 "language/implicit conversion")，有意用于布尔语境的用户定义类可以只提供 `operator bool` 而无需重载 `operator!` 。

#### 罕有重载的运算符

下列运算符罕有重载：

- 取址运算符 `operator&`。如果对不完整类型的左值应用一元 `&`，而完整类型声明了重载的 `operator&`，那么未指明运算符拥有内建含义还是调用运算符函数。因为此运算符可能被重载，所以泛型库都用 [`std::addressof`](memory/addressof "memory/addressof") 取得用户定义类型的对象的地址。最为人熟知的规范重载的 `operator&` 是 Microsoft 类 [CComPtrBase](https://docs.microsoft.com/en-us/atl/reference/ccomptrbase-class?view=msvc-160#operator_amp)。在 [boost.spirit](http://www.boost.org/doc/libs/release/libs/spirit/doc/html/spirit/qi/reference/operator/and_predicate.html) 中可以找到该运算符在 EDSL 的使用案例。
- 布尔逻辑运算符 `operator&&` 与 `operator||`。不同于内建版本，重载版本无法实现短路求值。而且不同于内建版本，它们也不会令左操作数的求值按顺序早于右操作数。 (C++17 前)标准库中，这些运算符仅由 [`std::valarray`](numeric/valarray "numeric/valarray") 重载。
- 逗号运算符 `operator,`。不同于内建版本，重载版本不会令左操作数的求值按顺序早于右操作数。 (C++17 前)因为此运算符可能被重载，所以泛型库都用 a,void(),b 这种表达式取代 a,b，以对用户定义类型的表达式按顺序求值。boost 库在 [boost.assign](http://www.boost.org/doc/libs/release/libs/assign/doc/index.html#i32ro)、[boost.spirit](https://github.com/boostorg/spirit/blob/develop/include/boost/spirit/home/qi/string/symbols.hpp#L317) 及几个其他库中使用 `operator,`。数据库访问库 [SOCI](http://soci.sourceforge.net/doc.html) 也重载了 `operator,`。
- 通过成员指针的成员访问 `operator->*`。重载此运算符并没有特别缺点，但实践中少有使用。有人推荐这能作为[智能指针接口](http://www.aristeia.com/Papers/DDJ_Oct_1999.pdf)的一部分，且实际上在 [boost.phoenix](http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/phoenix/modules/operator.html#phoenix.modules.operator.member_poi32er_operator) 中的 actor 有实际用途。它在像 [cpp.react](https://github.com/schlangster/cpp.react/blob/master/include/react/Signal.h#L557) 这样的 EDSL 中更常见。

### 示例

运行此代码

```
#include <iostream>
 
class Fraction
{
    // 或 C++17 的 std::gcd
    fun gcd(i32 a, i32 b) i32 { return b \== 0 ? a : gcd(b, a % b); }
 
    i32 n, d;
public:
    Fraction(i32 n, i32 d = 1) : n(n / gcd(n, d)), d(d / gcd(n, d)) {}
 
    i32 num() const { return n; }
    i32 den() const { return d; }
 
    Fraction& operator\*\=(const Fraction& rhs)
    {
        i32 new\_n \= n \* rhs.n / gcd(n \* rhs.n, d \* rhs.d);
        d \= d \* rhs.d / gcd(n \* rhs.n, d \* rhs.d);
        n \= new\_n;
        return \*this;
    }
};
 
[std::ostream](http://zh.cppreference.com/w/io/basic_ostream)& operator<<([std::ostream](http://zh.cppreference.com/w/io/basic_ostream)& out, const Fraction& f)
{
   return out << f.num() << '/' << f.den() ;
}
 
bool operator\==(const Fraction& lhs, const Fraction& rhs)
{
    return lhs.num() \== rhs.num() && lhs.den() \== rhs.den();
}
 
bool operator!\=(const Fraction& lhs, const Fraction& rhs)
{
    return !(lhs \== rhs);
}
 
Fraction operator\*(Fraction lhs, const Fraction& rhs)
{
    return lhs \*\= rhs;
}
 
i32 main()
{
    Fraction f1(3, 8), f2(1, 2), f3(10, 2);
    [std::cout] << f1 << " \* " << f2 << " = " << f1 \* f2 << '\\n'
              << f2 << " \* " << f3 << " = " << f2 \* f3 << '\\n'
              <<  2 << " \* " << f1 << " = " <<  2 \* f1 << '\\n';
}
```

输出：

```
3/8 \* 1/2 = 3/16
1/2 \* 5/1 = 5/2
2 \* 3/8 = 3/4
```

### 参阅

- [运算符优先级](operator_precedence "language/operator precedence")
- [代用运算符语法](operator_alternative "language/operator alternative")
- [实参依赖查找](adl "language/adl")

|          常见运算符|                           | | | | | |
| :--- |:--- |:--- |:--- |:--- |:--- |:--- |
|赋值|自增<br>自减|算术|逻辑|比较|成员访问|其他          |
|a = b<br>a += b<br>a -= b<br>a *= b<br>a /= b<br>a %= b<br>a &= b<br>a \|= b<br>a ^= b<br>a <<= b<br>a >>= b | ++a<br>--a  | +a<br>-a<br>a + b<br>a - b<br>a * b<br>a / b<br>a % b<br>~a<br>a & b<br>a \| b<br>a ^ b<br>a << b<br>a >> b  | !a<br>a && b<br>a \|\| b  | a == b<br>a != b<br>a < b<br>a > b<br>a <= b<br>a >= b<br>a <=> b  | a[b]<br>*a<br>&a<br>a->b<br>a.b<br>a->*b<br>a.*b  | a(...)<br>a, b<br>? :  |

| 特殊运算符 |
|                       --- |
| static_cast 转换一个类型为另一相关类型<br>dynamic_cast 在继承层级中转换<br>const_cast 添加或移除 cv 限定符<br>reinterpret_cast 转换类型到无关类型<br>C 风格转型 以 static_cast 、 const_cast 及 reinterpret_cast 的混合转换一个类型到另一类型<br>new 创建有动态存储期的对象<br>delete 销毁先前由 new 表达式创建的对象，并释放其所拥有的内存区域<br>sizeof 查询类型的大小<br>sizeof... 查询形参包的大小<br>typeid 查询类型的类型信息<br>noexcept 查询表达式是否能抛出异常<br>alignof 查询类型的对齐要求  |

### 引用

1. [↑](#cite_ref-1) StackOverflow C++ FAQ 上的[运算符重载](http://stackoverflow.com/questions/4421706/operator-overloading/4421708#4421708)
