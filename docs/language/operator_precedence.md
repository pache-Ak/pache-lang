## 运算符优先级

---

下表列出 C++ 运算符的优先级和结合性。各个运算符以优先级的降序从上至下列出。

| 优先级 | 运算符 | 描述 | 结合性 |
| --- | --- | --- | --- |
| 1 | `::` | [作用域解析](https://zh.cppreference.com/w/cpp/language/identifiers#.E6.9C.89.E9.99.90.E5.AE.9A.E7.9A.84.E6.A0.87.E8.AF.86.E7.AC.A6 "cpp/language/identifiers") | 从左到右 → |
| 2 | `a()` | [函数调用](https://zh.cppreference.com/w/cpp/language/operator_other#.E5.86.85.E5.BB.BA.E7.9A.84.E5.87.BD.E6.95.B0.E8.B0.83.E7.94.A8.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator other") |从右到左 ← |
|2 | `a[]` | [下标](https://zh.cppreference.com/w/cpp/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E4.B8.8B.E6.A0.87.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator member access") |从右到左 ← |
|2| `.`   `->` | [成员访问](https://zh.cppreference.com/w/cpp/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E6.88.90.E5.91.98.E8.AE.BF.E9.97.AE.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator member access") |从右到左 ← |
| 3 | `+a`   `-a` | 一元[加与减](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E4.B8.80.E5.85.83.E7.AE.97.E6.9C.AF.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") |从右到左 ← |
|3| `!`   `~` | [逻辑非](https://zh.cppreference.com/w/cpp/language/operator_logical "cpp/language/operator logical")和[逐位非](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E6.8C.89.E4.BD.8D.E9.80.BB.E8.BE.91.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") |从右到左 ← |
|3| `*a` | [间接](https://zh.cppreference.com/w/cpp/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E9.97.B4.E6.8E.A5.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator member access")（解引用） |从右到左 ← |
|3| `&a` | [取址](https://zh.cppreference.com/w/cpp/language/operator_member_access#.E5.86.85.E5.BB.BA.E7.9A.84.E5.8F.96.E5.9C.B0.E5.9D.80.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator member access") |从右到左 ← |
|3| [co\_await](https://zh.cppreference.com/w/cpp/keyword/co_await "cpp/keyword/co await") | [await 表达式](https://zh.cppreference.com/w/cpp/language/coroutines "cpp/language/coroutines") (C++20) |从右到左 ← |
|3| [`new`](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new")  | [动态内存分配](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new") |从右到左 ← |
|3| [`delete`](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete")   | [动态内存分配](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete") |从右到左 ← |
| 4 | `a*b` `a/b` `a%b` | [乘法、除法与余数](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E4.B9.98.E6.B3.95.E6.80.A7.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") | 从左到右 → |
| 5 | `a+b` `a-b` | [加法与减法](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E5.8A.A0.E6.B3.95.E6.80.A7.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") | 从左到右 → |
| 6 | `<<` `>>` | 逐位[左移与右移](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E7.A7.BB.E4.BD.8D.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") | 从左到右 → |
| 7 | `<=>` | [三路比较运算符](https://zh.cppreference.com/w/cpp/language/operator_comparison#.E4.B8.89.E8.B7.AF.E6.AF.94.E8.BE.83 "cpp/language/operator comparison")(C++20 起) | 从左到右 → |
| 8 | `<` `<=` `>` `>=` | 分别为 < 与 ≤ 与 > 与 ≥ 的[关系运算符](https://zh.cppreference.com/w/cpp/language/operator_comparison "cpp/language/operator comparison") | 从左到右 → |
| 9 | `==` `!=` | 分别为 = 与 ≠ 的[相等性运算符](https://zh.cppreference.com/w/cpp/language/operator_comparison "cpp/language/operator comparison") | 从左到右 → |
| 10 | `a&b` | [逐位与](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E6.8C.89.E4.BD.8D.E9.80.BB.E8.BE.91.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic") | 从左到右 → |
| 11 | `^` | [逐位异或](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E6.8C.89.E4.BD.8D.E9.80.BB.E8.BE.91.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic")（互斥或） | 从左到右 → |
| 12 | `\|` | [逐位或](https://zh.cppreference.com/w/cpp/language/operator_arithmetic#.E6.8C.89.E4.BD.8D.E9.80.BB.E8.BE.91.E8.BF.90.E7.AE.97.E7.AC.A6 "cpp/language/operator arithmetic")（可兼或） | 从左到右 → |
| 13 | `&&` | [逻辑与](https://zh.cppreference.com/w/cpp/language/operator_logical "cpp/language/operator logical") | 从左到右 → |
| 14 | `\|\|` | [逻辑或](https://zh.cppreference.com/w/cpp/language/operator_logical "cpp/language/operator logical") | 从左到右 → |
|15| [`throw`](https://zh.cppreference.com/w/cpp/language/throw "cpp/language/throw") | [throw 运算符](https://zh.cppreference.com/w/cpp/language/throw "cpp/language/throw") |从右到左 ← |
|15| [co\_yield](https://zh.cppreference.com/w/cpp/keyword/co_yield "cpp/keyword/co yield") | [yield 表达式](https://zh.cppreference.com/w/cpp/language/coroutines "cpp/language/coroutines") (C++20) | 从右到左 ← |


分析表达式时，运算符的优先级与结合性影响表达式中运算对象组合的方式。

  
对于优先级不同的运算符：

列于上面表中某行的运算符，将比列于低于它的行中拥有较低优先级的任何运算符，更紧密地与其实参相绑定（如同用了括号）。例如，表达式 [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << a & b 和 \*p++ 会被分析为 ([std::cout](http://zh.cppreference.com/w/cpp/io/cout) << a) & b 和 \*(p++)，而非 [std::cout](http://zh.cppreference.com/w/cpp/io/cout) << (a & b) 或 (\*p)++。

  
对于优先级相同的运算符：

拥有相同优先级的运算符以其结合性的方向与各参数绑定。例如表达式 a \= b \= c 会被分析为 a \= (b \= c) 而非 (a \= b) \= c，因为赋值具有从右到左结合性，但 a + b \- c 会被分析为 (a + b) \- c 而非 a + (b \- c)，因为加法和减法具有从左到右结合性。

  
结合性规定对于一元运算符是冗余的，只为完备而给出：

一元前缀运算符始终从右到左结合（delete ++\*p 为 delete(++(\*p))）
  
运算符优先级不受[运算符重载](https://zh.cppreference.com/w/cpp/language/operators "cpp/language/operators")影响。

例如，[std::cout](http://zh.cppreference.com/w/cpp/io/cout) << a ? b : c; 会被分析为 ([std::cout](http://zh.cppreference.com/w/cpp/io/cout) << a) ? b : c;，因为算术左移的优先级高于条件运算符。

### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/operator_precedence&action=edit&section=1 "编辑章节：注解")\] 注解

优先级和结合性是编译时概念，与[求值顺序](https://zh.cppreference.com/w/cpp/language/eval_order "cpp/language/eval order")无关，后者是运行时概念。

标准自身不指定优先级。它们是从文法导出的。

表中并未包括 [`const_cast`](https://zh.cppreference.com/w/cpp/language/const_cast "cpp/language/const cast")、[`static_cast`](https://zh.cppreference.com/w/cpp/language/static_cast "cpp/language/static cast")、[`dynamic_cast`](https://zh.cppreference.com/w/cpp/language/dynamic_cast "cpp/language/dynamic cast")、[`reinterpret_cast`](https://zh.cppreference.com/w/cpp/language/reinterpret_cast "cpp/language/reinterpret cast")、[`typeid`](https://zh.cppreference.com/w/cpp/language/typeid "cpp/language/typeid")、[`sizeof...`](https://zh.cppreference.com/w/cpp/language/sizeof... "cpp/language/sizeof...")、[`noexcept`](https://zh.cppreference.com/w/cpp/language/noexcept "cpp/language/noexcept") 及 [`alignof`](https://zh.cppreference.com/w/cpp/language/alignof "cpp/language/alignof")，因为它们决不会有歧义。

一些运算符拥有[代用写法](https://zh.cppreference.com/w/cpp/language/operator_alternative "cpp/language/operator alternative")（例如，`&&` 可为 and、`||` 可为 or、`!` 可为 not 等）。

### 参阅

| 常见运算符 |
| --- |
| [赋值](https://zh.cppreference.com/w/cpp/language/operator_assignment "cpp/language/operator assignment") | [自增/自减](https://zh.cppreference.com/w/cpp/language/operator_incdec "cpp/language/operator incdec") | [算术](https://zh.cppreference.com/w/cpp/language/operator_arithmetic "cpp/language/operator arithmetic") | [逻辑](https://zh.cppreference.com/w/cpp/language/operator_logical "cpp/language/operator logical") | [比较](https://zh.cppreference.com/w/cpp/language/operator_comparison "cpp/language/operator comparison") | [成员访问](https://zh.cppreference.com/w/cpp/language/operator_member_access "cpp/language/operator member access") | [其他](https://zh.cppreference.com/w/cpp/language/operator_other "cpp/language/operator other") |
| 
a \= b  
a +\= b  
a \-\= b  
a \*\= b  
a /\= b  
a %\= b  
a &\= b  
a |\= b  
a ^\= b  
a <<= b  
a \>>= b

 | 

++a  
\--a  
a++  
a\--

 | 

+a  
\-a  
a + b  
a \- b  
a \* b  
a / b  
a % b  
~a  
a & b  
a | b  
a ^ b  
a << b  
a \>> b

 | 

!a  
a && b  
a || b

 | 

a \== b  
a !\= b  
a < b  
a \> b  
a <= b  
a \>= b  
a <=> b

 | 

a\[b\]  
\*a  
&a  
a\-\>b  
a.b  
a\-\>\*b  
a.\*b

 | 函数调用 |
| a(...) |
| 逗号 |
| a, b |
| 条件 |
| a ? b : c |
| 特殊运算符 |
| 

[static\_cast](https://zh.cppreference.com/w/cpp/language/static_cast "cpp/language/static cast") 转换一个类型为另一相关类型  
[dynamic\_cast](https://zh.cppreference.com/w/cpp/language/dynamic_cast "cpp/language/dynamic cast") 在继承层级中转换  
[const\_cast](https://zh.cppreference.com/w/cpp/language/const_cast "cpp/language/const cast") 添加或移除 [cv](https://zh.cppreference.com/w/cpp/language/cv "cpp/language/cv") 限定符  
[reinterpret\_cast](https://zh.cppreference.com/w/cpp/language/reinterpret_cast "cpp/language/reinterpret cast") 转换类型到无关类型  
[C 风格转型](https://zh.cppreference.com/w/cpp/language/explicit_cast "cpp/language/explicit cast") 以 `static_cast`、`const_cast` 及 `reinterpret_cast` 的混合转换一个类型到另一类型  
[new](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new") 创建有动态存储期的对象  
[delete](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete") 销毁先前由 new 表达式创建的对象，并释放其所拥有的内存区域  
[sizeof](https://zh.cppreference.com/w/cpp/language/sizeof "cpp/language/sizeof") 查询类型的大小  
[sizeof...](https://zh.cppreference.com/w/cpp/language/sizeof... "cpp/language/sizeof...") 查询[形参包](https://zh.cppreference.com/w/cpp/language/parameter_pack "cpp/language/parameter pack")的大小(C++11 起)  
[typeid](https://zh.cppreference.com/w/cpp/language/typeid "cpp/language/typeid") 查询类型的类型信息  
[noexcept](https://zh.cppreference.com/w/cpp/language/noexcept "cpp/language/noexcept") 查询表达式是否能抛出异常(C++11 起)  
[alignof](https://zh.cppreference.com/w/cpp/language/alignof "cpp/language/alignof") 查询类型的对齐要求(C++11 起)

 |