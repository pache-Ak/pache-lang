## 异常

---

异常处理提供了一种可以使程序从执行的某点将控制流和信息转移到与执行先前经过的某点相关联的处理代码的方法（换言之，异常处理将控制权沿调用栈向上转移）。

[throw 表达式](/language/throw "language/throw")、[dynamic\_cast](/language/dynamic_cast "language/dynamic cast")、[typeid](/language/typeid "language/typeid")、[new 表达式](/language/new "language/new")、[分配函数](/memory/new/operator_new "memory/new/operator new")，以及专门用来抛出特定异常以指示特定错误状态的任何标准库函数（例如 [std::vector::at](/container/vector/at "container/vector/at")、[std::string::substr](/string/basic_string/substr "string/basic string/substr") 等）都可以抛出异常。

为捕获异常，throw 表达式必须在 [try 块](/language/try_catch "language/try catch")或者 try 块中所调用的函数中，而且必须有与异常对象的类型相匹配的 [catch 子句](/language/try_catch "language/try catch")。

在声明函数时，可以提供以下说明以限制函数能够抛出的异常类型：

异常处理过程中发生的错误由 [std::terminate](/error/terminate "error/terminate") 和 [std::unexpected](/error/unexpected "error/unexpected") (C++17 前) 处理。

### 用法

尽管 throw 表达式转移控制到执行栈上方的任意代码块时没有条件限制（类似 [std::longjmp](/utility/program/longjmp "utility/program/longjmp")），但它的预期用途还是进行错误处理。

#### 错误处理

异常的抛出用于从函数中为错误发信号，其中“错误”通常仅限于以下内容[\[1\]](#cite_note-1)[\[2\]](#cite_note-2)[\[3\]](#cite_note-3)：

1. 无法满足后置条件，例如不能产生有效的返回值对象
2. 无法满足另一个必须调用的函数的前置条件
3. （对于非私有成员函数）无法（再）建立类不变量

这意味着构造函数（参阅 [RAII](/language/raii "language/raii")）和大多数运算符应该通过抛出异常来报告程序错误。

另外，所谓*宽契约（wide contract）*函数用异常来指示非法输入，例如，[std::string::at](/string/basic_string/at "string/basic string/at") 没有前置条件，但它会抛出异常以指示下标越界。

#### 异常安全

在函数报告了错误状态后，可以提供附加保证以保障程序的状态。以下是四个被广泛认可的异常保证等级[\[4\]](#cite_note-4)[\[5\]](#cite_note-5)[\[6\]](#cite_note-6)，每个是另一个的严格超集：

1. *不抛出（nothrow）（或不失败）异常保证*——函数始终不会抛出异常。[析构函数](/language/destructor "language/destructor")和其他可能在栈回溯中调用的函数被期待为不会抛出（以其他方式报告或隐瞒错误）。[析构函数](/language/destructor "language/destructor")默认为 [noexcept](/language/noexcept "language/noexcept")。 (C++11 起)交换函数（swap），[移动构造函数](/language/move_constructor "language/move constructor")，及为提供强异常保证所使用的其他函数，都被期待为不会失败（函数总是成功）。
2. *强（strong）异常保证*——如果函数抛出异常，那么程序的状态会恰好被回滚到该函数调用前的状态。（例如 [std::vector::push\_back](/container/vector/push_back "container/vector/push back")）
3. *基础（basic）异常保证*——如果函数抛出异常，那么程序处于某个有效状态。不泄漏资源，而所有对象的不变式都保持完好。
4. *无异常保证*——如果函数抛出异常，那么程序可能不会处于有效的状态：可能已经发生了资源泄漏、内存损坏，或其他摧毁不变式的错误。

此外，泛型组件还可以提供*异常中性（exception neutral）保证*：如果从某个模板形参（例如从 [std::sort](/algorithm/sort "algorithm/sort") 的 `Compare` 函数对象，或从 [std::make\_shared](/memory/shared_ptr/make_shared "memory/shared ptr/make shared") 中 `T` 的构造函数）抛出异常，那么它会被无修改地传播给调用方。

### 异常对象

虽然任意完整类型和指向 void 的 cv 指针都能作为异常对象抛出，但所有标准库函数都以值抛出匿名临时对象，而且这些对象的类型都（直接或间接）派生于 [std::exception](/error/exception "error/exception")。用户定义的异常通常遵循此模式。[\[7\]](#cite_note-7)[\[8\]](#cite_note-8)[\[9\]](#cite_note-9)

为避免不必要的异常对象复制和对象切片，catch 子句在实践中最好以引用捕获。[\[10\]](#cite_note-10)[\[11\]](#cite_note-11)[\[12\]](#cite_note-12)[\[13\]](#cite_note-13)

### 引用

1. [↑](#cite_ref-1) H. Sutter (2004) [“何时及如何使用异常”](http://www.drdobbs.com/when-and-how-to-use-exceptions/184401836)于 Dr. Dobb's
2. [↑](#cite_ref-2) H.Sutter, A. Alexandrescu (2004)，“C++ 编码标准”，第 70 条
3. [↑](#cite_ref-3) C++ 核心指导方针 [I.10](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#i10-use-exceptions-to-signal-a-failure-to-perform-a-required-task)
4. [↑](#cite_ref-4) B. Stroustrup (2000)，“C++ 程序语言” [“附录 E”](http://stroustrup.com/3rd_safe.pdf)
5. [↑](#cite_ref-5) H. Sutter (2000) “Exceptional C++”
6. [↑](#cite_ref-6) D. Abrahams (2001) [“泛型组件中的异常安全”](http://www.boost.org/community/exception_safety.html)
7. [↑](#cite_ref-7) D. Abrahams (2001) [“错误与异常处理”](http://www.boost.org/community/error_handling.html)
8. [↑](#cite_ref-8) isocpp.org Super-FAQ [“我应该抛出什么？”](https://isocpp.org/wiki/faq/exceptions#what-to-throw)
9. [↑](#cite_ref-9) C++ 核心指导方针 [E.14](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#e14-use-purpose-designed-user-defined-types-as-exceptions-not-built-in-types)
10. [↑](#cite_ref-10) C++ 核心指导方针 [E.15](https://github.com/isoCppCoreGuidelines/blob/master/CppCoreGuidelines.md#e15-catch-exceptions-from-a-hierarchy-by-reference)
11. [↑](#cite_ref-11) S. Meyers (1996) “More Effective C++” 第 13 条
12. [↑](#cite_ref-12) isocpp.org 超级 FAQ [“我应该捕获什么？”](https://isocpp.org/wiki/faq/exceptions#what-to-catch)
13. [↑](#cite_ref-13) H.Sutter, A. Alexandrescu (2004) “C++ 编码标准” 第 73 条
