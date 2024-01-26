## **name**语言

---

这是一份核心 **name** 语言构造的参考手册。

**[基本概念](/language/basic_concepts.md "language/basic concepts")**

[注释](comment.md "comment")  
[ASCII 码表](/language/ascii "language/ascii")  
[标点](/language/punctuators "language/punctuators")  
[名字与标识符](identifiers.md "language/identifiers")  
[类型](type.md "language/type") – [基础类型](types.md "language/types")  
[对象](object.md "language/object") – [作用域](/language/scope "language/scope") – [生存期](/language/lifetime "language/lifetime")  
[定义与 ODR](/language/definition "language/definition")  
[名字查找](/language/lookup "language/lookup")  
[有限定](/language/qualified_lookup "language/qualified lookup") – [无限定](/language/unqualified_lookup "language/unqualified lookup")（[实参依赖](/language/adl "language/adl")）  
[“如同”规则](/language/as_if "language/as if")  
[未定义行为](/language/ub "language/ub")  
[内存模型与数据竞争](/language/memory_model "language/memory model")  
[字符集与编码](/language/charset "language/charset")  
[翻译阶段](/language/translation_phases "language/translation phases")  
[`main` 函数](/language/main_function "language/main function")  
[模块](/language/modules "language/modules")  

**[关键词](/keyword.md "keyword")**

**[表达式](expressions.md "language/expressions")**

[值类别](value_category.md "language/value category")  
[求值顺序与定序](/language/eval_order "language/eval order")  
[常量表达式](/language/constant_expression "language/constant expression")  
[运算符](expressions.md#运算符 "language/expressions")  
[赋值](operator_assignment.md "language/operator assignment") – [算术](/language/operator_arithmetic "language/operator arithmetic")  
[逻辑](/language/operator_logical "language/operator logical") – [比较](/language/operator_comparison "language/operator comparison")  
[成员访问与间接](/language/operator_member_access "language/operator member access")  
[调用](/language/operator_call "language/operator call")  
[`sizeof`](/language/sizeof "language/sizeof") – [`alignof`](/language/alignof "language/alignof")  
[`new`](/language/new "language/new") – [`delete`](/language/delete "language/delete") – [`typeid`](/language/typeid "language/typeid")  
[运算符重载](/language/operators.md "language/operators")  
[默认比较](/language/default_comparisons "language/default comparisons")  
[运算符优先级](/language/operator_precedence "language/operator precedence")  
[转换](/language/expressions.md#转换 "language/expressions")  
[隐式](/language/implicit_conversion "language/implicit conversion") – [显式](/language/explicit_cast "language/explicit cast") – [用户定义](/language/cast_operator "language/cast operator")  
 [`static_cast`](/language/static_cast "language/static cast") – [`dynamic_cast`](/language/dynamic_cast "language/dynamic cast")  
 [`const_cast`](/language/const_cast "language/const cast") – [`reinterpret_cast`](/language/reinterpret_cast "language/reinterpret cast")  
[字面量](/language/expressions.md#字面量 "language/expressions")  
[布尔](/language/bool_literal "language/bool literal") – [整数](/language/integer_literal "language/integer literal") – [浮点](/language/floating_literal "language/floating literal")  
[字符](/language/character_literal "language/character literal") – [字符串](/language/string_literal "language/string literal")  
 [`nullptr`](/language/nullptr "language/nullptr")  
 [用户定义](/language/user_literal "language/user literal")  

**[声明](/language/declarations.md "language/declarations")**

[命名空间声明](/language/namespace "language/namespace")  
[命名空间别名](/language/namespace_alias "language/namespace alias")  
[引用](/language/reference "language/reference") – [指针](/language/pointer "language/pointer") – [数组](/language/array "language/array")  
[结构化绑定](/language/structured_binding "language/structured binding")  
[枚举与枚举项](/language/enum "language/enum")  
[存储期与链接](/language/storage_duration "language/storage duration")  
[翻译单元局部](/language/tu_local "language/tu local")  
[语言链接](/language/language_linkage "language/language linkage")  
[`inline` 说明符](/language/inline "language/inline")  
[内联汇编](/language/asm "language/asm")  
[`const`/`volatile`](/language/cv "language/cv") – [`constexpr`](/language/constexpr "language/constexpr")  
[`consteval`](/language/consteval "language/consteval") – [`constinit`](/language/constinit "language/constinit")  
[`decltype`](/language/decltype "language/decltype") – [`auto`](/language/auto "language/auto")  
[`typedef`](/language/typedef "language/typedef") – [类型别名](/language/type_alias "language/type alias")  
[详述类型说明符](/language/elaborated_type_specifier "language/elaborated type specifier")  
[属性](/language/attributes "language/attributes") – [`alignas`](/language/alignas "language/alignas")  
[`static_assert`](/language/static_assert "language/static assert")  

**[初始化](/language/initialization.md "language/initialization")**

[默认初始化](/language/default_initialization "language/default initialization")  
[值初始化](/language/value_initialization "language/value initialization") (C++03)  
[复制初始化](/language/copy_initialization "language/copy initialization")  
[~~直接初始化~~](/language/direct_initialization "language/direct initialization")  
[聚合初始化](/language/aggregate_initialization "language/aggregate initialization")  
[列表初始化](/language/list_initialization "language/list initialization")  
[引用初始化](/language/reference_initialization "language/reference initialization")  
[静态非局部初始化](/language/initialization.md#非局部变量 "language/initialization")  
[零](/language/zero_initialization "language/zero initialization") – [常量](/language/constant_initialization "language/constant initialization")  
[动态非局部初始化](/language/initialization.md#非局部变量 "language/initialization")  
[有序](/language/initialization.md#非局部变量 "language/initialization") – [无序](/language/initialization.md#非局部变量 "language/initialization")  
[复制消除](/language/copy_elision "language/copy elision")  

**[函数](/language/functions "language/functions")**

[函数声明](/language/function "language/function")  
[默认实参](/language/default_arguments "language/default arguments")  
[变长实参](/language/variadic_arguments "language/variadic arguments")  
[Lambda 表达式](/language/lambda "language/lambda")  
[实参依赖查找](/language/adl "language/adl")  
[重载决议](/language/overload_resolution "language/overload resolution")  
[运算符重载](/language/operators "language/operators")  
[重载集的地址](/language/overloaded_address "language/overloaded address")  
[协程](/language/coroutines "language/coroutines")  

**[语句](/language/statements.md "language/statements")**

[`if`](/language/if "language/if") – [`switch`](/language/switch "language/switch")  
[`for`](/language/for "language/for") – [范围 `for`](/language/range-for "language/range-for")  
[`while`](/language/while "language/while") – [`do`\-`while`](/language/do "language/do")  
[`continue`](/language/continue "language/continue") – [`break`](/language/break "language/break") – [`goto`](/language/goto "language/goto") – [`return`](/language/return "language/return")  
[`synchronized` 与 `atomic`](/language/transactional_memory "language/transactional memory") (TM TS)  

**[类](/language/classes "language/classes")**

[类类型](/language/class "language/class") – [联合类型](/language/union "language/union")  
[注入类名](/language/injected-class-name "language/injected-class-name")  
[数据成员](/language/data_members "language/data members") – [位域](/language/bit_field "language/bit field")  
[成员函数](/language/member_functions "language/member functions") – [`this` 指针](/language/this "language/this")  
[静态成员](/language/static "language/static") – [嵌套类](/language/nested_types "language/nested types")  
[派生类](/language/derived_class "language/derived class") – [`using` 声明](/language/using_declaration "language/using declaration")  
[空基类优化](/language/ebo "language/ebo")  
[虚函数](/language/virtual "language/virtual") – [抽象类](/language/abstract_class "language/abstract class")  
[`override`](/language/override "language/override") – [`final`](/language/final "language/final")  
[成员访问](/language/access "language/access") – [`friend`](/language/friend "language/friend")  
[构造函数与成员初始化器列表](/language/constructor "language/constructor")  
[默认构造函数](/language/default_constructor "language/default constructor") – [析构函数](/language/destructor "language/destructor")  
[复制构造函数](/language/copy_constructor "language/copy constructor") – [复制赋值](/language/copy_assignment "language/copy assignment")  
[移动构造函数](/language/move_constructor "language/move constructor")  
[移动赋值](/language/move_assignment "language/move assignment")  
[转换构造函数](/language/converting_constructor "language/converting constructor") – [`explicit` 说明符](/language/explicit "language/explicit")  

**[模板](/language/templates "language/templates")**

[模板形参与实参](/language/template_parameters "language/template parameters")  
[类模板](/language/class_template "language/class template") – [函数模板](/language/function_template "language/function template")  
[变量模板](/language/variable_template "language/variable template") (C++14)  
[类成员模板](/language/member_template "language/member template")  
[模板实参推导](/language/template_argument_deduction "language/template argument deduction")  
[类模板实参推导](/language/class_template_argument_deduction "language/class template argument deduction")  
[显式特化](/language/template_specialization "language/template specialization") – [部分特化](/language/partial_specialization "language/partial specialization")  
[形参包](/language/parameter_pack "language/parameter pack") – [`sizeof...`](/language/sizeof... "language/sizeof...")  
[折叠表达式](/language/fold "language/fold")  
[待决名](/language/dependent_name "language/dependent name") – [SFINAE](/language/sfinae "language/sfinae")  
[约束与概念](/language/constraints "language/constraints")  

**[异常](/lunguage/exceptions.md "language/exceptions")**

[`throw` 表达式](/language/throw "language/throw")  
[`try`\-`catch` 块](/language/try_catch "language/try catch")  
[函数 `try` 块](/language/function-try-block "language/function-try-block")  
[`noexcept` 说明符](/language/noexcept_spec "language/noexcept spec")  
[`noexcept` 运算符](/language/noexcept "language/noexcept")  
[动态异常说明](/language/except_spec "language/except spec") (C++17 前)  

**杂项**

[C++ 的历史](/language/history "language/history")  
[扩充命名空间 std](/language/extending_std "language/extending std")  
[字母缩写](/language/acronyms "language/acronyms")

**习语**

[资源获取即初始化](/language/raii "language/raii")  
[三/五/零法则](/language/rule_of_three "language/rule of three")  
[指向实现的指针](/language/pimpl "language/pimpl")  
[零开销原则](/language/Zero-overhead_principle "language/Zero-overhead principle")
