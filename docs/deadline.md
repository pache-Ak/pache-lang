# new program language and related components design

-----

## main features

规范语法结构 依靠格式化器， 交互执行  功能丰富 高安全性追求 涵盖matlab等高级语言，涵盖汇编，降低编程难度，

底层，强类型 支持反射等动态语言特性 静态类型，

规范goto 零开销抽象，无垃圾收集，无运行时成本。

~~宏~~ ~~\_\_line\_\_~~ (类似替代C++ source\_location)

结构化绑定 函数式 Lambda 对齐

不区分函数和函数模板 提供元类型支持

原生并发支持？？ 取消成员函数指针 this指代继承关系中的此对象类型而不是对象地址

注释解释器

类的成员函数 名称查找如同在类所在作用域定义

成员函数不同与普通函数的点在于用于支持继承和多态

函数即函数指针 函数是一等公民

指针语法 type\*   构成一个新类型

引用语法 type&  构成新类型

数组语法 type\[z\] 构成新类型

多维数组 `type[x, y, ...]` 构成新类型

数组的数组不是~~连续的~~多维数组

函数类型语法 func(args) return-type  func可省略？？

不支持前置声明 类 函数 模板等声明同时必须定义， 变量声明建议初始化

~~隐式转换~~ 类型转换运算符？？

[自立实现](/freestanding "freestanding")

**[语言](/language/language.md "language")**

[基本概念](/language/basic_concepts.md "language/basic concepts")  
[关键词](/keyword.md "keyword")  
[~~预处理器~~](/preprocessor "preprocessor")  
[表达式](/language/expressions.md "language/expressions")  
[声明](/language/declarations.md "language/declarations")  
[初始化](/language/initialization.md "language/initialization")  
[函数](/language/functions.md "language/functions")  
[语句](/language/statements.md "language/statements")  
[类](/language/classes.md "language/classes")  
[重载](/language/operators.md "language/operators")  
[模板](/language/templates "language/templates")  
[异常](/language/exceptions.md "language/exceptions")

**[~~头文件~~](/header "header")**

**[具名要求](/named_req "named req")**

**[~~功能特性测试宏~~](/feature_test "feature test")**

**[语言支持库](/utility#语言支持 "utility")**

[源代码信息](/utility/source_location "utility/source location")
[类型支持](/types "types")  
[程序工具](/utility/program "utility/program")  
[协程支持](/coroutine "coroutine")
[三路比较](/utility#三路比较 "utility")
[numeric\_limits](/types/numeric_limits "types/numeric limits")  −  [`type_info`](/types/type_info "types/type info")  
[`initializer_list`](/utility/initializer_list "utility/initializer list")

**[概念库](/concepts "concepts")**

**[元编程库](/meta "meta")**

[类型特性](/meta#类型特性 "meta")  −  [ratio](/numeric/ratio "numeric/ratio")  
[`integer_sequence`](/utility/integer_sequence "utility/integer sequence")

**[诊断库](/error "error")**

[`basic_stacktrace`](/utility/basic_stacktrace "utility/basic stacktrace")

**[内存管理库](/memory "memory")**

[`unique_ptr`](/memory/unique_ptr "memory/unique ptr")
[`shared_ptr`](/memory/shared_ptr "memory/shared ptr")

**[通用工具库](/utility#通用工具 "utility")**

[函数对象](/utility/functional "utility/functional")  −  [hash](/utility/hash "utility/hash")
[工具函数](/utility#swap_与类型运算 "utility")  
[pair](/utility/pair "utility/pair")  −   [tuple](/utility/tuple "utility/tuple")
[optional](/utility/optional "utility/optional")  −  [any](/utility/any "utility/any")  
[variant](/utility/variant "utility/variant")  
[字符串转换](/utility#初等字符串转换 "utility")  
[格式化](/utility/format "utility/format")
[位操纵](/numeric#位操纵 "numeric")

**[字符串库](/string "string")**

[basic\_string](/string/basic_string "string/basic string")  
[basic\_string\_view](/string/basic_string_view "string/basic string view")  
以空字符结尾的字符串：  
   [字节](/string/byte "string/byte")  −   [多字节](/string/multibyte "string/multibyte")  −   [宽](/stringide "stringide")

**[容器库](/container "container")**

[array](/container/array "container/array")   −  [vector](/container/vector "container/vector")  −  [deque](/container/deque "container/deque")  
[list](/container/list "container/list")  −  [forward\_list](/container/forward_list "container/forward list")
[map](/container/map "container/map")  −  [multimap](/container/multimap "container/multimap")  
[set](/container/set "container/set")  −  [multiset](/container/multiset "container/multiset")  
[unordered\_map](/container/unordered_map "container/unordered map")
[unordered\_multimap](/container/unordered_multimap "container/unordered multimap")
[unordered\_set](/container/unordered_set "container/unordered set")
[unordered\_multiset](/container/unordered_multiset "container/unordered multiset")
[stack](/container/stack "container/stack")  −  [queue](/container/queue "container/queue")  −  [priority\_queue](/container/priority_queue "container/priority queue")  
[span](/container/span "container/span")

**[迭代器库](/iterator "iterator")**

**[范围库](/ranges "ranges")**

**[算法库](/algorithm "algorithm")**

[受约束算法](/algorithm/ranges "algorithm/ranges")

**[数值库](/numeric "numeric")**

[常用数学函数](/numeric/math "numeric/math")  
[数学特殊函数](/numeric/special_functions "numeric/special functions")  
[数学常数](/numeric/constants "numeric/constants")
[数值算法](/numeric#%E6%95%B0%E5%80%BC%E7%AE%97%E6%B3%95 "numeric")  
[伪随机数生成](/numeric/random "numeric/random")  
[浮点环境](/numeric/fenv "numeric/fenv")
[complex](/numeric/complex "numeric/complex")  −  [valarray](/numeric/valarray "numeric/valarray")

**[日期时间库](/chrono "chrono")**

**[本地化库](/locale "locale")**

**[输入/输出库](/io "io")**

[基于流的 I/O](/io#%E5%9F%BA%E4%BA%8E%E6%B5%81%E7%9A%84_I%2FO "io")  
[同步输出](/io#%E5%90%8C%E6%AD%A5%E7%9A%84%E8%BE%93%E5%87%BA "io")
[I/O 操纵符](/io/manip "io/manip")

**[文件系统库](/filesystem "filesystem")**

**[正则表达式库](/regex "regex")**

[basic\_regex](/regex/basic_regex "regex/basic regex")  −  [算法](/regex#%E7%AE%97%E6%B3%95 "regex")

**[并发支持库](/thread "thread")**

[atomic](/atomic/atomic "atomic/atomic")  −  [atomic\_flag](/atomic/atomic_flag "atomic/atomic flag")  
[atomic\_ref](/atomic/atomic_ref "atomic/atomic ref")
[thread](/thread/thread "thread/thread") − [jthread](/thread/jthread "thread/jthread")
[mutex](/thread/mutex "thread/mutex")  
[condition\_variable](/thread/condition_variable "thread/condition variable")  
[future](/thread/future "thread/future")  −  [promise](/thread/promise "thread/promise")
