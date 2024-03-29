# TODO

-----

## 禁止指针运算

使用迭代器， 范围。

## 元类型

`concept` 关键字 定义一个概念或称元类型，概念定义一组约束， 如果一个类型满足这些约束则称类型满足概念或类型是这个元类型， 元类型或概念可作为函数参数类型及模板类形参， 做函数参数时， 此函数成为模板函数或称元函数。 在实例化时， 对参数进行检查是否满足约束， 不满足时， 实例化失败， 并报告一个错误。错误信息应当包含此类型所需约束， 在定义类时， 应当有工具告知一个概念的约束， 以指导实现。 以谓词的方式建模元类型， 在选择函数重载时， 如果类型满足概念A及其概念B（满足A-\>B）则选择B。

类型是值？？
仅定义时可逻辑运算

## 类

### deadline

支持友元

EBO优化?? 变为以下形式

若一个 final 类不含有非静态成员变量， 建议改为模块；

``` 
可以用 using 引入模块， 访问权限取复合
```

若一个 多态 类不含有非静态成员变量， 只给虚表指针分配内存

若类含有不含有非静态成员变量的类成员， 不分配内存， 如同以访问限定符包含类内的成员函数

``` 
建议以 using 引入成员函数 访问权限取复合
```

对不含非静态成员变量的类变量取地址非法or提醒

### 确定性的定义顺序
所有仍在流行的编译器都会严格按照定义顺序来布局成员变量。
不涉及继承关系中父类和子类的定义顺序。

#### 考虑到对齐，按照字节大小排列变量也许会更节省空间。
此问题目前缺乏有关研究。

### 继承构造函数
``` cpp
class A {

};

class B : A {
  using A::A;
};
```
以此语法继承父类的构造函数，并解决参数完美转发等问题。

### enum

枚举（enumeration）是独立的类型，它的值被限制在一个取值范围内（细节见下文），它可以包含数个明确命名的常量（“枚举项（enumerator）”）。各常量的值是某个整型类型（称为该枚举的底层类型（underlying type））的值。??

## 错误处理

### 异常处理

### errno code

## allocate_at_least
申请（无论是operator new还是分配器）内存的时候，得到的往往比申请的多；比如你operator new(12)申请12个字节，很多内存池会返回给你一个16个字节大小的内存空间（向上取整）。

这段额外的分配空间却并不能为用户所用——现标准中，申请了12个字节你就只能访问12个字节，即使后面送了你4个字节，访问那些还是UB。自然，我们希望能访问所有字节。

一个最容易想到的办法就是让内存池提供一个接口，回答“当我申请N个字节的时候，实际上我会得到多少字节？”（也就是TCMalloc的nallocx API）。不过，这个API有三个问题：

效率不高，很多时候知道“能得到多少字节”这个信息的唯一途径就是实际分配一遍内存（比如遍历一遍链表），而后面我们实际分配的时候就相当于做了两遍同样的工作。
返回值可能比较保守。N -> 实际字节这个映射实际上并不是常数，有可能两次malloc(12)一次返回16字节，一次32字节，但是nallocx(12)永远只能返回最保守的猜测。
损失实际大小。nallocx得到数值之后，我们每次只会使用这个数值的字节做分配请求（比如想要12/13/14字节都会申请16字节），这可能会让内存池对于实际内存需求的预估出现偏差从而损失效率。
说到这里，实际可行的解决办法其实已经呼之欲出了——只要在分配的时候，除了返回指向分配内存的指针，还返回实际分配的大小就行了；这样三个问题都解决了。P0401采用的就是这个方法——在分配器API既有的allocate()函数之外，增加了allocate_at_least()函数：
```cpp
template<class Pointer>
struct allocation_result {
    Pointer ptr;
    size_t count;
};

template<class Pointer>
func allocate_at_least(size s) allocation_result<Pointer> {
  // definition
}
```
## 类型

### 类型推导

统一初始化

#### 用户定义字面量 UDL

constexpr operator""*function-name*(*arg-type*) -\> *return-type* { }

#### 原始字符串字面量

R"(...)"
正则块

#### 数学表达式块

## 标准库

### IO

抽象流
方法在流中抽象实现

文件流
in out err 是文件流

缓冲流

字节流
二进制流

文件需显示关闭在final(or finally)块中

to\_string
from\_strinh

标准输入 in
标准输出 out
标准错误 err

## inteligence

### 自动注释生成

``` 
抛出异常类型
模板函数分析实例化后分析异常
TODO
```

### 异常处理提醒

### 静态分析

尽可能的错误分析，警告， 静态检验

#### 显示资源回收提醒 （close， delete……）

`@mustused`??

### “零”法则

~~IDE自动生成所有默认函数??~~

若类的特殊成员函数都是`default`定义， 则编译器建议不进行定义~~??~~

### “五”法则

若类的特殊成员函数有一个被用户定义， 且有其余未被定义的， 则编译器应给出建议

## 编译工具链

### 格式化器

### 构建系统

启用异常选项？？

### 包管理器

~~include~~

采用树形目录存储
一个文件之含有一个类， 类模板， 枚举类 或模块文件名为模块名；
一个文件夹为一个包
可用\*包含次包内的所有子包；

名称冲突警告在引入包时， 名字查找冲突时程序无法编译

### 解释器

### 分析器

### 优化器

## 模式匹配

## REPL（交互式）

## GUI

## Markdown字符串

## 构造函数推导 推到指引隐式

## 考虑浮点数扩展（非IEE754类型）等其他浮点数

## 成员函数 方法
语法
`a.fun();`
定义语法：
```cpp
class A {
  fun(this & self);
}
```
### 成语函数和非成语函数
例子：
`v.begin()` and `bengin(v)`
两者在多数情况下等价，但两者并非对所有情况等价。

在cpp中对于内置类型数组前者不可用。

#### 解决思路1
将所有类型都作为类（class）处理。

但成员函数的扩展是个问题，
函数时编程时更好的扩展方式。

目前不倾向此方案。
#### 解决思路2
将例如`begin(v)`等函数作为成语函数处理。

需处理多态和作用域限制等问题。

目前倾向于此， 计划更激进的废弃`v.begin()`写法。
需要更进一步讨论。

## 运算符重载
目前运算符的 **`this`** 参数如何传递的语法还未确定。
倾向于在定义时放在第一个参数。
例如：

```cpp
a();

// definition
operator()(this & self) {
  // function body
}
```

### 可以静态（static）
可以静态是c++概念， 在此意为不包含this参数，是函数（function）不是（method）。

#### operator()() 确认可以 

## 函数 函数指针 函数对象 函数是一等公民

## 定义确定类型 声明引入对象

## cv限定符 右结合 声明符中()只用于函数 声明符右结合

`i32 const * const`
`(i32 const [] * const) i32 const *`

## 注释

### 嵌套注释 ？？

`/* /* */ */`

### 注释标签

`@`

### 异常说明

`@throw ...`

## 网络

## 云运行环境

## 默认常量

去除 `const` ??

`mutable` 在常量类内的含义??

## 默认移动？默认拷贝？

## 不默认final类 ~~？？~~

~~final -\> base??~~

IDE自动补全final

语言设计or编译器警告??

编译器提醒  `@`标签

不默认final函数

## 跨平台？？？？？

## IDE

### 显示糖

## 在线编译器

## 解析友好

禁止`<>` `<``>`只用于比较，
减少其他符号重载
`(``)`减少重载
当前包含 函数声明 函数调用  优先级

## 初始化语法

[初始化](/language/initialization.md "language/initialization")
只保留一种初始化语法

构造函数

工厂函数

> 以具名的函数区分初始化含义

[\#1659](https://github.com/carbon-language/carbon-lang/discussions/1659)

## 元组

[结构化绑定](/language/structured_binding "language/structured binding")

## 取消代用语法？？ 慎重考虑符号

### 指针语法

`->` 替换为 ??

### 等于 复制 不等于 符号

\= == := \!= \<\>

# finish

## 不要 定义类或枚举并在同一语句中声明其类型的变量

原因 在同一声明中混合使用类型定义和另一个实体的定义是令人困惑和不必要的。示例，不好

``` c++
struct Data { /*...*/ } data{ /*...*/ };
```

示例，良好

``` cpp
struct Data { /*...*/ };
Data data{ /*...*/ };
```

不支持此语法

额外影响 取消无名类 无名枚举

## 修改名字查找规则  消除详述类型说明符用法

用关键字表示声明语法
声明语法:
`var (keyword) type name`
`keyword name : type`
keyword : let

## 创建对象（变量）关键字

`let`

## an idea about loop \#1768

[loop](/language/statements.md "language/statements")

``` 
loop {
  // ...
  if /* */ {
    // ...
    break
  }
  // ...
  if /* */ {
    // ...
    continue
  }

}
```

## 隐式转换

### 取消整型到bool的隐式转换

### 取消无符号与有符号之间的隐式转换

### 取消整型与浮点型之间的隐式准换

## 删除后缀自增自减运算符

## 弃用自增自减运算符

详见 [imcrement operator](../ideas/increment-operator.md)
