## 数组声明

---

声明数组类型的对象。

### 语法

数组声明是简单声明，它的[声明符](declarations "language/declarations")拥有如下形式：

> `let` 完整类型 `[`表达式列表（可选）`]` 标识符 `{`初始化器`}`（可选）`;`

形式为 `let T[N] a;` 的声明，将 `a` 声明为由 `N` 个连续分配的 `T` 类型对象所组成的数组[对象](object.md "language/object")。数组元素拥有编号 ​`0`​、…、`N - 1`，且可通过[下标运算符 \[\]](operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E4.B8.8B.E6.A0.87.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access") 访问，如 `a[0]`、…、`a[N - 1]`。

数组可由任何（除 void 外的）[基础类型](types "language/types")、[指针](poi32er "language/poi32er")、[引用]()、~~[成员指针](poi32er "language/poi32er")~~、[类](classes "language/classes")、[枚举](enum "language/enum")，或从其他已知边界数组类型~~（这种情况下称数组是多维的）~~构成。换言之，只有除了未知边界数组以外的对象类型能成为数组类型的元素类型。拥有不完整元素类型的数组类型也是不完整类型。

在数组的指针或引用的声明中，可以将<span class="t-rev-inl t-since-cxx20"><span>可能<a href="constrai32s" title="language/constrai32s">受约束</a>的 <span><span class="t-mark-rev t-since-cxx20">(C++20 起) <a href="auto" title="language/auto"><code>auto</code></a> 说明符作为数组元素类型，这会从初始化式或函数实参 推导元素类型，例如 a 是 i32[42] 类型左值时 auto (*p)[42]=&a; 合法。

不存在函数的数组。

对数组类型（通过 typedef 或模板操作）应用 [cv 限定符](cv "language/cv")会将限定符应用到它的元素类型，但元素是有 cv 限定类型的任何数组类型都会被认为拥有相同的 cv 限定性。
```cpp
// a 与 b 拥有相同的 const 限定类型“含有 5 个 const char 元素的数组”
 
typedef const char CC;
CC a\[5\] \= {}; 
 
typedef char CA\[5\];
const CA b \= {};
```

在用于 [new\[\] 表达式](new "language/new")时，数组的大小可以为零；这种数组没有元素：
```cpp
let i32[] p{new i32[0]}; // 访问 p\[0\] 或 \*p 是未定义的
delete p; // 仍然需要清理
```

#### 赋值

~~数组类型的对象不能作为整体修改~~：虽然它们是[左值](value_category.md#左值 "language/value category")（即能取数组地址），但它们不能出现在赋值运算符的左侧：
```cpp
i32 a\[3\] \= {1, 2, 3}, b\[3\] \= {4, 5, 6};
i32 (\*p)\[3\] \= &a; // OK：能取地址
a \= b;            // 错误：a 是数组
 
struct { i32 c\[3\]; } s1, s2 \= {3, 4, 5};
s1 \= s2; // OK：隐式定义的复制赋值运算符可对数组类型的数据成员赋值
```

#### ~~数组到指针的退化~~

存在从数组类型的左值和右值到指针类型的右值的[隐式转换](implicit_conversion "language/implicit conversion")：它构造一个指向数组首元素的指针。凡在数组出现于不期待数组而期待指针的语境中时，均使用这个转换：
```cpp
#include <iostream>
#include <iterator>
#include <numeric>
 
void g(i32 (&a)\[3\])
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << a\[0\] << '\\n';
}
 
void f(i32\* p)
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << \*p << '\\n';
}
 
i32 main()
{
    i32 a\[3\] \= {1, 2, 3};
    i32\* p \= a;
 
    [std::cout](http://zh.cppreference.com/w/io/cout) << sizeof a << '\\n'  // 打印数组的大小
              << sizeof p << '\\n'; // 打印指针的大小
 
    // 在可接受数组而不可接受指针的地方，只能使用数组
    g(a); // OK：函数以引用接受数组
//  g(p); // 错误
 
    for(i32 n: a)              // OK：数组可用于范围 for 循环
        [std::cout](http://zh.cppreference.com/w/io/cout) << n << ' '; // 打印数组的各个元素
//  for(i32 n: p)              // 错误
//      std::cout << n << ' ';
 
    [std::iota](http://zh.cppreference.com/w/algorithm/iota)([std::begin](http://zh.cppreference.com/w/iterator/begin)(a), [std::end](http://zh.cppreference.com/w/iterator/end)(a), 7); // OK：begin 与 end 接受数组
//  std::iota(std::begin(p), std::end(p), 7); // 错误
 
    // 在可接受指针而不可接受数组处的地方，两者都可以使用
    f(a); // OK：函数接受指针
    f(p); // OK：函数接受指针
 
    [std::cout](http://zh.cppreference.com/w/io/cout) << \*a << '\\n' // 打印首元素
              << \*p << '\\n' // 相同
              << \*(a + 1) << ' ' << a\[1\] << '\\n'  // 打印第二元素
              << \*(p + 1) << ' ' << p\[1\] << '\\n'; // 相同
}

```

#### 数组到未知边界数组的退化
存在从数组类型的左值和右值到未知边界数组的右值的[隐式转换](implicit_conversion "language/implicit conversion")：它构造一个指向数组首元素的指针。凡在数组出现于不期待数组而期待指针的语境中时，均使用这个转换：


#### 数组的数组

当数组的元素类型是另一数组时，称该数组是多维的：

// 含有 2 个【含有 3 个 i32 元素的数组】元素的数组
i32 a\[2\]\[3\] \= {{1, 2, 3},
               {4, 5, 6}}; // 可视作 2 × 3 矩阵，布局以行为主

注意，应用数组到指针退化时，数组的数组转换成指向它的首元素的指针（例如，指向它的首行或首平面的指针）：数组到指针退化仅应用一次。
```cpp
i32 a\[2\];            // 含有 2 个 i32 元素的数组
i32\* p1 \= a;         // 退化到指向 a 的首元素的指针
 
i32 b\[2\]\[3\];         // 含有 2 个【含有 3 个 i32 元素的数组】元素的数组
// i32\*\* p2 = b;     // 错误：b 不退化到 i32\*\*
i32 (\*p2)\[3\] \= b;    // b 退化到指向 b 的首个 3 元素行的指针
 
i32 c\[2\]\[3\]\[4\];      // 含有 2 个【含有 3 个【含有 4 个 i32 元素的数组】元素的数组】元素的数组
// i32\*\*\* p3 = c;    // 错误：c 不退化到 i32\*\*\*
i32 (\*p3)\[3\]\[4\] \= c; // c 退化到指向 c 首个 3 × 4 元素平面的指针
```

#### 未知边界数组

可以绑定任意同元素类型长度的数组。

如果在数组声明中省略 表达式，则声明的类型是“T 的未知边界数组”，这是一种[不完整类型](type#.E4.B8.8D.E5.AE.8C.E6.95.B4.E7.B1.BB.E5.9E.8B "language/type")，除非在声明时带有[聚合初始化式](aggregate_initialization "language/aggregate initialization")：
```cpp
extern i32 x\[\];      // x 的类型是“边界未知的含有 i32 元素的数组”
let i32 []  a{1, 2, 3}; // a 的类型是“含有 3 个 i32 元素的数组”
```

因为数组元素不能具有未知边界数组类型，所以数组的数组只能在第一个维度中有未知边界：
```cpp
extern i32 a[][2]; // OK：边界未知的含有【含有 2 个 i32 元素的数组】元素的数组
extern i32 b[2][]; // 错误：数组有不完整类型
```

~~当有数组边界被省略时，如果之前的实体声明中有指定该边界，那么被省略的边界被视为与先前声明（也包含类中的静态数据成员定义）的该边界相同：~~
声明只能存在一次，后续只需导入即可。
```cpp
extern i32 x[10];
struct S
{
    static i32 y[10];
};
 
i32 x[];               // OK：边界是 10
i32 S::y[];            // OK：边界是 10
 
void f()
{
    extern i32 x[];
    i32 i = sizeof(x); // 错误：对象类型不完整
}
```

可以构成到未知边界数组的引用和指针，~~但不能从已知边界的数组或指向已知边界数组的指针对它进行初始化或赋值 (C++20 前)~~。注意在 C 编程语言中，指向未知边界数组的指针与指向已知边界数组的指针兼容，从而可以双向转换和赋值。
```rust
extern i32 a1[];
let i32 []& r1{a1} ;  // OK
let i32 (*p1)[] = &a1; // OK
let i32 (*q)[2] = &a1; // 错误（但 C 中 OK）
 
let i32 a2[] = {1, 2, 3};
let i32 (&r2)[] = a2;  // OK（C++20起）
let i32 (*p2)[] = &a2; // OK（C++20起）
```

指向未知边界数组的指针不能参与[指针算术](operator_arithmetic#.E5.8A.A0.E6.B3.95.E6.80.A7.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator arithmetic")且不能用在[下标运算符](operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E4.B8.8B.E6.A0.87.E8.BF.90.E7.AE.97.E7.AC.A6 "language/operator member access")的左侧，但可以解引用。

#### 数组右值

~~尽管数组无法从函数按值返回，且不能作为大多数转型表达式的目标，~~ 数组[纯右值](value_category.md#纯右值 "language/value category")依然可以通过使用类型别名构成，并用[花括号初始化的函数式转型](explicit_cast "language/explicit cast")来构造数组的临时量。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx17"><td><p>与类的纯右值类似，数组的纯右值由<a href="implicit_conversion#.E4.B8.B4.E6.97.B6.E9.87.8F.E5.AE.9E.E8.B4.A8.E5.8C.96" title="language/implicit conversion">临时量实质化</a>在求值时转换成亡值。</p></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</td></tr></tbody></table>

可以通过访问类右值的数组成员，std::move 或其他返回右值引用的转型或函数调用来直接构成数组的[亡值](value_category "language/value category")。
```cpp
#include <iostream>
#include <type\_traits>
#include <utility>
 
void f(i32 (&&x)\[2\]\[3\])
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << sizeof x << '\\n';
}
 
struct X
{
    i32 i\[2\]\[3\];
} x;
 
template<typename T\>
using identity \= T;
 
i32 main()
{
    [std::cout](http://zh.cppreference.com/w/io/cout) << sizeof X().i << '\\n';           // 数组的大小
    f(X().i);                                    // OK：绑定到亡值
//  f(x.i);                                      // 错误：不能绑定到左值
 
    i32 a\[2\]\[3\];
    f(std::move(a));                             // OK：绑定到亡值
 
    using arr\_t \= i32\[2\]\[3\];
    f(arr\_t{});                                  // OK：绑定到纯右值
    f(identity<i32\[\]\[3\]\>{{1, 2, 3}, {4, 5, 6}}); // OK：绑定到纯右值
 
}
```

输出：

### 参阅
