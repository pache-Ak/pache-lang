## 语句

---

语句（statement）是依序执行的 **name** 程序片段。任何函数体都是语句的序列。例如：

```c++
fun main(c8*[] args) void {
 let auto n = 1                  // 声明语句
 n = n + 1                       // 表达式语句
 printf(out, "n = {}\n", n)      // 表达式语句
 return                          // return 语句
}
```

**name** 包含下列类型的语句：

1. 表达式语句（expression statement）；

2. 复合语句（compound statement）；

3. 选择语句（selection statement）；

4. 循环语句（iteration statement）；

5. 跳转语句（jump statement）；

6. 声明语句（declaration statement）；

7. try 块；

8. atomic 与 synchronized 块。

9. 声明语句 let;

### 标签

通过在语句自身之前提供后随冒号的标签，任何语句都可*带有标签*。
|      |      |
| :--- | :--- |
| `lable` 标识符 `:` 语句 | (1) |
| `case` 常量表达式 `:` 语句 | (2) |
| `default :` 语句 | (3) |

1. [goto](language/goto "language/goto") 的目标；

2. [switch](language/switch "language/switch") 语句中的 case 标签；

3. [switch](language/switch "language/switch") 语句中的 default 标签。

一条语句可以带有多个标签。

声明于函数内并带标识符的标签（且仅有标签）在该函数内、所有内嵌块、其自身声明的前后每处都在作用域内。

一个函数中的两个标签不得拥有同一标识符。

[无限定查找](language/unqualified_lookup "language/unqualified lookup")找不到标签：标签能与程序中任何其他实体拥有相同的名字。

```c++
func f() void {
 {
  goto Label // Label 在作用域内，尽管它在后面才声明
  lable Label:
 }
 goto Label // Label 忽略块作用域
}

fun g() void {
 goto Label // 错误： label 不在 g() 的作用域内
}
```

### 表达式语句

一行表达式是一条语句。

将多个语句写在同一行，它们则必须使用 ; 人为区分，

不鼓励，或禁止将多个语句放在一行  TODO

|      |      |
| :--- | :--- |
|  表达式(可选)  | (1) |

|      |       |      |
| :--- | :---: | :--- |
| 表达式 | - | 一个[表达式](/language/expressions.md "language/expressions") |>

典型 name 程序的大部分语句都是表达式语句，例如赋值和函数调用。

没有表达式的表达式语句被称作*空语句（null statement）*。它通常用来为 [for](language/for "language/for") 或 [while](language/while "language/while") 循环提供空体。它也可用于在复合语句的末尾引入标签。

### 复合语句

复合语句或（代码）块是花括号环绕的语句序列。

|      |      |
| :--- | :--- |
|  { 语句...(可选) } | (1) |

当需要一条语句，但要按顺序执行多条语句时，可以使用复合语句：

```C++
if x > 5 {
 auto n = 1
 println(out, n)
}

```

每个复合语句都引入其自身的块[作用域](/language/scope "language/scope")；在块中声明的变量在闭花括号处以逆序销毁：

```c++
fun main() {
    {                                // 块的开始
        std::ofstream f("test.txt") // 声明语句
        println(f, "abc")                // 表达式语句
    }                                // 块结束，刷新并关闭 f
    std::ifstream f("test.txt")
    std::string str
    scan(f, str)
}
```

### 选择语句

选择语句在数个控制流中选择一个。
|      |      |
| :--- | :--- |
| `if constexpr`(可选)  初始化语句 (可选) `;` 条件  true分支复合语句 | (1) |
| `if constexpr`(可选)  初始化语句(可选) `;` 条件  true分支复合语句 `else` false分支复合语句 | (2) |
| `switch`  初始化语句(可选) `;` 条件  复合语句 |(3) |
| `if !`(可选) `consteval` 复合语句 |(4)|
| `if !`(可选) `consteval` 复合语句 `else` 复合语句 | (5)|

1. [if](/language/if "language/if") 语句；
2. 带 else 子句的 [if](/language/if "language/if") 语句；
3. [switch](/language/switch.md language/switch) 语句；
4. [consteval if](/language/if.md#consteval_if "language/if") 语句；
5. 带 else 子句的 [consteval if](/language/if.md#consteval_if "language/if") 语句。

### 循环语句

循环语句重复执行一些代码。
|      |      |
| :--- | :--- |
| `loop` 复合语句 | (1) |
| `for`  初始化语句 `;` 条件(可选) `;` 表达式(可选)  复合语句 |(3) |
| `for` for-范围声明 `:` for-范围初始化器  复合语句| (4)|

1. [loop](/language/loop.md "language/loop") 循环；
2. [for](/language/for.md "language/for") 循环；
3. [范围 for](/language/range-for.md "language/range-for") 循环。

### 跳转语句

跳转语句无条件地转移控制流。
|      |      |
| :--- | :--- |
| `break` | (1)  |
| `continue` | (2)  |
| `return` 表达式(可选) | (3)  |
| `return` 花括号初始化器列表 | (4) |
| `goto` 标识符 | (5) |

1. [break](/language/break.md "language/break") 语句；
2. [continue](/language/continue.md "language/continue") 语句；
3. 可带表达式的 [return](/language/return.md "language/return") 语句；
4. 使用[列表初始化](/language/list_initialization.md "language/list initialization")的 [return](/language/return.md "language/return") 语句；
5. [goto](/language/goto.md "language/goto") 语句。

注意：对于所有跳转语句，转移出循环、出块或回到被初始化且具有自动存储期的变量之前，会牵涉到对“ 转移发起点在作用域中而目标点不在，且具有自动存储期 ”的对象的销毁。如果有多个对象被初始化，那么销毁顺序与初始化顺序相反。

### 声明语句

声明语句在块中引入一个或多个标识符。

|      |      |
| :--- | :--- |
| 块声明 | (1) |

1. 细节见[声明](/language/declarations.md "language/declarations")及[初始化](/language/initialization.md "language/initialization")。

### try 块

try 块提供当执行其他语句时捕获其所抛出的异常的能力。

|      |      |
| :--- | :--- |
| `try` 复合语句 处理块序列 | (1) |

1. 细节见[try/catch](/language/try_catch.md "language/try catch")。

### atomic 与 synchronized 块

atomic 与 synchronized 块用来实现[事务性内存](/language/transactional_memory.md "language/transactional memory")。

|      |      |
| :--- | :--- |
| `synchronized` 复合语句 | (1)  |
| `atomic_noexcept` 复合语句 | (2)  |
| `atomic_cancel` 复合语句 | (3)  |
| `atomic_commit` 复合语句 | (4)  |

1. [synchronized 块](/language/transactional_memory.md#synchronized_块 "language/transactional memory")，与所有 synchronized 块在一个全序中执行；
2. 在发生异常时中止的 [atomic 块](/language/transactional_memory.md#atomic_块 "language/transactional memory")；
3. 在发生异常时回滚的 [atomic 块](/language/transactional_memory.md#atomic_块 "language/transactional memory")；
4. 在发生异常时提交的 [atomic 块](/language/transactional_memory.md#atomic_块 "language/transactional memory")。

### 参阅
