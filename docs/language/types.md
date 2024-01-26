## 基础类型

（类型系统概览见[类型](type.md)，以及参阅 C++ 库提供的[类型相关的工具列表](https://zh.cppreference.com/w/types "types")）

###  空类型

- void 
    
    值为空集的类型。它是无法变为完整类型的[不完整类型](type.md#不完整类型 "/language/type")（从而不允许存在 void 类型的对象）。不存在含有 void 的[数组](array "/language/array")和[多维数组]()以及到 void 的[引用](reference "language/reference")。然而 [指向 void 的指针](pointer#void_.E7.9A.84.E6.8C.87.E9.92.88 "language/pointer")和返回 void 类型的[函数](function.md "language/function")是允许存在的（其他语言中的*过程*）。

###  [std::nullptr\_t](https://zh.cppreference.com/w/types/nullptr_t "types/nullptr t")

[std::nullptr\_t](https://zh.cppreference.com/w/types/nullptr_t "types/nullptr t") 是空指针字面量 [`nullptr`](nullptr "language/nullptr") 的类型。它是独立类型，既不是指针类型，也不是成员指针类型。

###  整数类型

####  标准整数类型

标准整数类型均由补码实现。

##### 有符号整数类型
有符号整数类型：
`i8` `i16` `i32` `i64` `i128` `isize`

类型`i32`是有符号整数类型的默认类型。
也是整数字面量的默认类型，

###### 允许的内建操作
- [隐式转换](implicit_conversion.md "language/implicit_conversion")

    有符号整数类型可且仅可[整形提升](implicit_conversion.md#整型提升 "language/implicit_conversion")为更大的有符号整数类型的[纯右值](value_category.md#纯右值 "language/value_category")。

    不同于C++，有符号整数类型不会隐式转换为无符号整数类型，bool类型，浮点类型等其他类型。

- 赋值和初始化
    仅可用相同类型或经过转型后相同的类型对自身赋值和初始化。

- 算数运算

    | operator | 说明|
    | --- | --- |
    | `+a` | 一元正运算符|
    | `-a` | 一元负运算符|
    |`a + b` | 加法运算符|
    |`a - b` | 减法运算符|
    | `a * b`| 乘法运算符|
    |`a / b` | 整除运算符|
    | `a % b` | 取余运算符|

    注： `a`和`b`均是有符号整数类型的变量，内建运算符不支持与其他类型混合使用。

- 位运算
    有符号整数类型内建不支持位运算，请使用无符号整数类型。

- 比较运算

    | operator | 说明|
    | --- | --- |
    | `a == b` | 相等|
    | `a != b` | 不等|
    |`a > b` | 大于|
    |`a < b` | 小于|
    | `a >= b`| 大于等于|
    |`a <= b` | 小于等于|
    | `a <=> b` | 三路运算|    

    注： `a`和`b`均是有符号整数类型的变量，内建运算符不支持与其他类型混合使用。

- 逻辑运算
    有符号整数不会隐式转换为bool， 不可用于逻辑运算。
###### 取值范围
| 类型 | 位数 | 取值范围 |
| --- | --- | --- |
|`i8`| 8 | **\-128**到**127**|
| `i16`| 16 | **\-32768** 到 **32767** |
|`i32`| 32  | **\-2,147,483,648** 到 **2,147,483,647** |
|`i64`| 64 | **\-9,223,372,036,854,775,808** 到 **9,223,372,036,854,775,807** |

##### 无符号整数类型
无符号整数类型：
`u8` `u16` `u32` `u64` `u128` `usize`

类型`u32`是无符号整数类型的默认类型。

###### 允许的内建操作
- [隐式转换](implicit_conversion.md "language/implicit_conversion")

    无符号整数类型可且仅可[整形提升](implicit_conversion.md#整型提升 "language/implicit_conversion")为更大的无符号整数类型的[纯右值](value_category.md#纯右值 "language/value_category")。


    不同于C++，无符号整数类型不会隐式转换为有符号整数类型，bool类型，浮点类型等其他类型

- 赋值和初始化
    仅可用相同类型或经过转型后相同的类型对自身赋值和初始化。

- 算数运算

    | operator | 说明|
    | --- | --- |
    | `+a` | 一元正运算符|
    |`a + b` | 加法运算符|
    | `a * b`| 乘法运算符|
    |`a / b` | 整除运算符|
    | `a % b` | 取余运算符|

    注： `a`和`b`均是无符号整数类型的变量，内建运算符不支持与其他类型混合使用， 无符号数内建不支持取负，减法运算。

- 位运算

    | operator | 说明|
    | --- | --- |
    | `~a` | 逐位非(NOT)|
    |`a & b` | 逐位与(AND)|
    | `a \| b`| 逐位或(OR)|
    |`a ^ b` | 逐位异或(XOR)|
    | `a << b` | 向左移位|
    | `a >> b` | 向右移位 |

    注： `a`和`b`均是无符号整数类型的变量。

- 比较运算

    | operator | 说明|
    | --- | --- |
    | `a == b` | 相等|
    | `a != b` | 不等|
    |`a > b` | 大于|
    |`a < b` | 小于|
    | `a >= b`| 大于等于|
    |`a <= b` | 小于等于|
    | `a <=> b` | 三路运算|    
    
    注： `a`和`b`均是无符号整数类型的变量，内建运算符不支持与其他类型混合使用。

- 逻辑运算
    无符号整数不会隐式转换为bool， 不可用于逻辑运算。

###### 取值范围
| 类型 | 位数 | 取值范围 |
| --- | --- | --- |
| `u8` | 8 | **0** 到 **255** |
| `u16` | 16 | **0** 到 **65535** |
| `u32` | 32 | **0** 到 **4,294,967,295** |
| `u64` | 64 | **0** 到 **18,446,744,073,709,551,615** |

#### 扩展整数类型
由实现定义的整数类型

###  布尔类型

bool - 足以存放两个值 [`true`](bool_literal "language/bool literal") 或 [`false`](bool_literal "language/bool literal") 之一的类型。~~`sizeof(bool)` 的值由实现定义，而且不一定是 1。~~

#### 允许的内建操作

| operator | 说明|
| --- | --- |
|`!a`  | 非|
| `a && b` | 与|
|`a \|\| b` | 或|

###  字符类型
- `c8`

    UTF-8 字符表示的类型，要求大到足以表示任何 UTF-8 编码单元（8 位）。

- `c16`

   UTF-16 字符表示的类型，要求大到足以表示任何 UTF-16 编码单元（16 位）。
- `c32`

    UTF-32 字符表示的类型，要求大到足以表示任何 UTF-32 编码单元（32 位）。

#### 取值范围
|类型| 位数 | 取值范围 |
|---  | --- | ---|
|`c8`| /    | / |
|`c16`| 16  | **0** 到 **65535**(**0xffff**) |
|`c32`| 32   | **0** 到 **1114111**(**0x10ffff**) |
###  浮点类型
#### 允许的内建操作
- [隐式转换](implicit_conversion.md "language/implicit_conversion")

    浮点数类型可且仅可[浮点提升](implicit_conversion.md#浮点提升 "language/implicit_conversion")为更大的浮点数类型的[纯右值](value_category.md#纯右值 "language/value_category")。

    不同于C++，浮点数类型不会隐式转换为有符号整数类型，bool类型等其他类型

- 赋值和初始化
    仅可用相同类型或经过转型后相同的类型对自身赋值和初始化。

- 算数运算

    | operator | 说明|
    | --- | --- |
    | `+a` | 一元正运算符|
    | `-a` | 一元负运算符|
    |`a + b` | 加法运算符|
    |`a - b` | 减法运算符|
    | `a * b`| 乘法运算符|
    |`a / b` | 整除运算符|
    | `a % b` | 取余运算符|

    注： `a`和`b`均是浮点数类型的变量，内建运算符不支持与其他类型混合使用。

- 比较运算

    | operator | 说明|
    | --- | --- |
    | `a == b` | 相等|
    | `a != b` | 不等|
    |`a > b` | 大于|
    |`a < b` | 小于|
    | `a >= b`| 大于等于|
    |`a <= b` | 小于等于|
    | `a <=> b` | 三路运算|    

    注： `a`和`b`均是浮点数类型的变量，内建运算符不支持与其他类型混合使用。

- 逻辑运算
    浮点数不会隐式转换为bool， 不可用于逻辑运算。

####  标准浮点类型

下列三种类型和它们的 cv 限定版本统称为标准浮点类型。
- `f16`
    半精度浮点格式。如果支持就会匹配 [IEEE-754 binary16 格式](https://en.wikipedia.org/wiki/Half-precision_floating-point_format)。

- `f32`

    单精度浮点类型。如果支持就会匹配 [IEEE-754 binary32 格式](https://en.wikipedia.org/wiki/Single-precision_floating-point_format "enwiki:Single-precision floating-point format")。

- `f64`
    
    双精度浮点类型。如果支持就会匹配 [IEEE-754 binary64 格式](https://en.wikipedia.org/wiki/Double-precision_floating-point_format "enwiki:Double-precision floating-point format")。

- `f128` 
    四精度浮点类型。如果支持就会匹配[IEEE-754 binary128 格式](https://en.wikipedia.org/wiki/Quadruple-precision_floating-point_format)。

- `f256`
    八精度二进制浮点类型。如果支持就会匹配[IEEE-754 binary256 格式](https://en.wikipedia.org/wiki/Octuple-precision_floating-point_format)。

##### 取值范围

下表提供常用数值表示的界限的参考。

浮点类型 `T` 的范围定义如下：

+   最低限度保证的范围是从 `T` 能表示的最负有限浮点数到 `T` 能表示的最正有限浮点数。
+   如果 `T` 可以表示负无穷，那么 `T` 的范围扩展到所有负实数。
+   如果 `T` 可以表示正无穷，那么 `T` 的范围扩展到所有正实数。

因为 [ISO/IEC/IEEE 60559](https://www.iso.org/standard/80985.html) 格式可以表示正无穷和负无穷，所以遵守 ISO/IEC/IEEE 60559 的浮点类型可以表示值的范围包含所有实数。

[IEEE-754](https://en.wikipedia.org/wiki/Single-precision_floating-point_format "enwiki:Single-precision floating-point format")
| 类型 | 位数 | 最小非正规 |最小正规|最大|
| --- | --- | --- | --- | --- |
| `f16` | 16 | **±0x1p-24**| **±0x1p-14**| **±0x1.3ffp+15**|
| `f32` | 32 |   **±0x1p-149**|**±0x1p-126**|**±0x1.fffffp+127**|
| `f64` | 64 | **±0x1p-1074**|**±0x1p-1022**|**±0x1.fffffffffffffp+1023**|
| `f128` | 128 |**±0x1p-16494**|**±0x1p-16382**|**±0x1.ffffffffffffffffffffffffffffp+16383**|
| `f256` | 256 | **±0x1p-262378**|**±0x1p-262142**|**±0x1.fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffp+262143**|

####  扩展浮点类型

扩展浮点类型由实现定义。可能会包含[定宽浮点类型](https://zh.cppreference.com/w/types/floating-point "types/floating-point")。


- `x86_fp80_type_t`
    
    x86扩展格式。如果支持就会匹配[x86 extended precision format](https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format)

 `x86_fp80_type_t` 在 32/64 位平台上对象表示通常占用 96/128 位。

##### 取值范围

| 类型 | 位数 | 最小非正规 |最小正规|最大|
| --- | --- | --- | --- | --- |
| `x86_fp80_type_t` | 80 | **±0x1p-16446**|**±0x1p-16382**|**±0x1.fffffffffffffffep+16383**|

#### 十进制浮点类型

- `d32`
    十进制 32 浮点格式。如果支持就会匹配[decimal32 floating-point format](https://en.wikipedia.org/wiki/Decimal32_floating-point_format)

- `d64`
    十进制 64 浮点格式。如果支持就会匹配[decimal64 floating-point format](https://en.wikipedia.org/wiki/Decimal64_floating-point_format)

- `d128`
    十进制 128 浮点格式。如果支持就会匹配[decimal128 floating-point format](https://en.wikipedia.org/wiki/Decimal128_floating-point_format)

##### 取值范围

| 类型 | 位数 | 最小 |最大|
| --- | --- | --- | --- | 
| `d32` | 32 | **±0.000000×10E−95**|**±9.999999×10E96**|
| `d64` | 64 | **±0.000000000000000×10E−383**|**±9.999999999999999×10E384**|
| `d128` | 128 | **±0.000000000000000000000000000000000×10E−6143**|**±9.999999999999999999999999999999999×10E6144**|

#### 性质

浮点类型可以支持一些[特殊值](https://zh.cppreference.com/w/types/numeric_limits "types/numeric limits")：

+   *无穷*（正和负），见 [INFINITY](https://zh.cppreference.com/w/numeric/math/INFINITY "numeric/math/INFINITY")
+   *负零*，\-0.0。它与正零比较相等，但在某些算术运算中有意义，例如 1.0 / 0.0 \== [INFINITY](http://zh.cppreference.com/w/numeric/math/INFINITY)，但 1.0 / \-0.0 \== \-[INFINITY](http://zh.cppreference.com/w/numeric/math/INFINITY))，而且对某些数学函数有意义，例如 [sqrt(std::complex)](https://zh.cppreference.com/w/numeric/complex/sqrt "numeric/complex/sqrt")
+   *非数*（NaN），它与任何值（包括自身）比较都不相等。多个位模式都表示 NaN，见 [std::nan](https://zh.cppreference.com/w/numeric/math/nan "numeric/math/nan")、[NAN](https://zh.cppreference.com/w/numeric/math/NAN "numeric/math/NAN")。注意 C++ 并会不对发信 NaN 进行特殊对待，它会把所有 NaN 都当做静默 NaN，但提供了 [std::numeric\_limits::has\_signaling\_NaN](https://zh.cppreference.com/w/types/numeric_limits/has_signaling_NaN "types/numeric limits/has signaling NaN") 用以检测它们的支持情况。

实浮点数可以用于[算术运算符](operator_arithmetic "language/operator arithmetic") + - / \* 和各种来自 [<cmath>](https://zh.cppreference.com/w/header/cmath "header/cmath") 的数学函数。内建运算符和库函数都可能会引发浮点异常，并按 [math errhandling](https://zh.cppreference.com/w/numeric/math/math_errhandling "numeric/math/math errhandling") 中的描述设置 [errno](https://zh.cppreference.com/w/error/errno "error/errno")。

浮点表达式可以拥有大于它的类型所指定的值域和精度，见 [FLT\_EVAL\_METHOD](https://zh.cppreference.com/w/types/climits/FLT_EVAL_METHOD "types/climits/FLT EVAL METHOD")。浮点表达式也可以*收缩*，即在计算时将所有中间值视为拥有无限范围和精度，见 [#pragma STDC FP\_CONTRACT](https://zh.cppreference.com/w/preprocessor/impl#.23pragma_STDC "preprocessor/impl")。标准 C++ 不限制浮点操作的准确度。

浮点数上的某些运算受[浮点环境](https://zh.cppreference.com/w/numeric/fenv "numeric/fenv")的状态影响，并可以对它进行修改（最值得注意的是舍入方向）。

浮点类型的其他细节、界限和性质见[浮点类型的界限](https://zh.cppreference.com/w/types/climits#.E6.B5.AE.E7.82.B9.E7.B1.BB.E5.9E.8B.E7.9A.84.E7.95.8C.E9.99.90 "types/climits")和 [std::numeric\_limits](https://zh.cppreference.com/w/types/numeric_limits "types/numeric limits")。


注意：这些类型能够表示的值的实际界限（与保证的最小界限相对）可以通过 [std::numeric\_limits](https://zh.cppreference.com/w/types/numeric_limits "types/numeric limits") 获知。
