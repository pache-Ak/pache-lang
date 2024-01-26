## 注释

---

注释是一种代码内文档。当被插入到程序中时，它们实际上会被编译器忽略；它们只需要被阅读源代码的人当作注记。一些工具可以解析各式注释。

### 语法

-   `//` comment

    称为“单行”注释。
-   `///` comment

    文档注释

在[翻译阶段 3](https://zh.cppreference.com/w/cpp/language/translation_phases "cpp/language/translation phases") 中，每段注释通过被替换为单个空白字符从程序中移除。

###  单行注释

单行注释通常用于注释单行文本；不过也可以把多个单行注释放在一起来组成多行注释。单行注释告诉编译器忽略 `//` 和换行符之间的所有内容。

### 文档注释

TODO

### 注解

除了把代码注释掉之外，用于排除源代码的其他机制还有：
```cpp
if false {
    std::println("this line will not print.");
}
```

### 示例
```cpp
#include <iostream>
// C++ 风格注释能注释一行
 
// 或者，能将
// 它们串在一起

 
int main()
{
    // 注释在预处理前被移除，
    // 下方代码不会运行
    // return 1;
 
    // 下方代码会运行
    return 0;
}
```

输出：
```
```
### 参阅