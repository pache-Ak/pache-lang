## 基本概念
本节定义了描述 **name** 编程语言时所使用的一些专门的术语与概念。

一个 **name** 程序是一个含有[声明](/language/declarations.md "language/declarations")的文本文件序列。它们被[翻译](/language/translation_phases "language/translation phases")成一个可执行程序，**name** 实现通过调用其[主 (main) 函数](/language/main_function "language/main function")执行这一程序。

在 **name** 程序中，一些被称为[关键词](/keyword.md "keyword")的词语有着特殊的含义。其它词语可以被用作[标识符](/language/identifiers "language/identifiers")。在翻译的过程中，[注释](/comment.md "comment")会被忽略。**name** 程序也包含[字面量](/language/expressions.md#字面量 "language/expressions")，其中的字符的值由[字符集与编码](/language/charset "language/charset")指定。程序中的某些特定字符必须通过[转义序列](/language/escape "language/escape")表示。

**name** 程序中的*实体*包括值、[对象](/language/object "language/object")、[引用](/language/reference "language/reference")[、结构化绑定](/language/structured_binding "language/structured binding")、[函数](/language/functions "language/functions")、[枚举项](/language/enum "language/enum")、[类型](/language/type "language/type")、类成员、[模板](/language/templates "language/templates")、[模板特化](/language/template_specialization "language/template specialization")、[命名空间](/language/namespace "language/namespace")和[形参包](/language/parameter_pack "language/parameter pack")。

*[声明](/language/declarations.md "language/declarations")*可以引入实体，将它们与[名字](/language/identifiers#.E5.90.8D.E5.AD.97 "language/identifiers")关联起来，并定义其属性。能够定义使用一个实体所需的所有属性的声明是[定义](/language/definition "language/definition")。对任何被 [ODR 使用](/language/definition#ODR_.E4.BD.BF.E7.94.A8 "language/definition")的非内联函数或变量，程序中必须只含有其一个定义。

函数的定义通常包括一系列的[语句](/language/statements "language/statements")，其中部分会包含[表达式](/language/expressions "language/expressions")。表达式指定了程序需要进行的运算。

程序中遇到的名字通过[名字查找](/language/lookup "language/lookup")与引入它们的声明关联起来。每个名字都只在称为其[作用域](/language/scope "language/scope")的程序部分中有效。有些名字有[链接](/language/storage_duration "language/storage duration")，这使得它们即使出现在不同的作用域或翻译单元时也代表相同的实体。

**name** 中的每一个对象、引用、函数和表达式都会关联一个[类型](/language/type "language/type")。类型可以分为[基础类型](/language/types "language/types")，复合类型，或[自定义类型](/language/classes "language/classes")，以及完整或[不完整](/language/type#.E4.B8.8D.E5.AE.8C.E6.95.B4.E7.B1.BB.E5.9E.8B "language/type")的类型等。

被声明的且不是[非静态数据成员](/language/data_members "language/data members")的对象和引用是*变量*。

### 参阅