## 类

---

类是一种用户定义类型。

类的类型可以由类说明符定义，它在[声明](/language/declarations "language/declarations")语法的 声明说明符序列 中出现。类说明符的语法参见[类声明](/language/class "language/class")。

类可以拥有下列种类的成员：

1. 数据成员：
    1. 非静态数据成员，包括位域。
    2. 静态数据成员
2. 成员函数：
    1. 非静态成员函数
    2. 静态成员函数
3. 嵌套类型：
    1. 在类定义之中定义的嵌套类及枚举
    2. 既存类型的别名，~~以 typedef 或~~类型别名 声明定义
    3. 就[查找](/language/unqualified_lookup#.E6.B3.A8.E5.85.A5.E7.B1.BB.E5.90.8D "language/unqualified lookup")而言 ~~（除非在用作~~ ~~[构造函数](/language/initializer_list "language/initializer list")~~ ~~名时）~~，类的名字在该类自身的定义中，表现为一个作为其自身的类型别名的公开成员：这被称作*[注入类名](/language/injected-class-name "language/injected-class-name")*
4. ~~来自定义于类中的所有无作用域枚举，或由 using 声明或 using enum 声明引入的枚举项~~
5. [成员模板](/language/member_template "language/member template")（变量模板、类模板或函数模板）可以在任何非局部 class/ ~~struct/~~ union 定义体内出现。

所有成员都在类定义中一次性定义，不能向已定义的类添加成员（与命名空间成员不同）

类 `T` 的成员不能使用 `T` 作为其名字。

声明或继承了至少一个[虚](/language/virtual "language/virtual")成员函数的类是*多态*的。此类型的对象是[多态对象](/language/object#.E5.A4.9A.E6.80.81.E5.AF.B9.E8.B1.A1 "language/object")，并拥有作为其对象表示一部分存储的运行时类型信息，它可以通过 [`dynamic_cast`](/language/dynamic_cast "language/dynamic cast") 和 [`typeid`](/language/typeid "language/typeid") 查询到。虚成员函数参与动态绑定。

声明或继承了至少一个纯虚函数的类是[抽象类](/language/abstract_class "language/abstract class")。不能创建此类型的对象。

### 类的性质

#### 可平凡复制类

*可平凡复制类* 是满足以下所有条件的类：

- 至少有一个合格的[复制构造函数](/language/copy_constructor#.E5.90.88.E6.A0.BC.E7.9A.84.E5.A4.8D.E5.88.B6.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/copy constructor")，[移动构造函数](/language/move_constructor#.E5.90.88.E6.A0.BC.E7.9A.84.E7.A7.BB.E5.8A.A8.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/move constructor")，[复制赋值运算符](/language/copy_assignment#.E5.90.88.E6.A0.BC.E7.9A.84.E5.A4.8D.E5.88.B6.E8.B5.8B.E5.80.BC.E8.BF.90.E7.AE.97.E7.AC.A6 "language/copy assignment")或[移动赋值运算符](/language/move_assignment#.E5.90.88.E6.A0.BC.E7.9A.84.E7.A7.BB.E5.8A.A8.E8.B5.8B.E5.80.BC.E8.BF.90.E7.AE.97.E7.AC.A6 "language/move assignment")，
- 每个合格的复制构造函数都是[平凡的](/language/copy_constructor#.E5.B9.B3.E5.87.A1.E7.9A.84.E5.A4.8D.E5.88.B6.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/copy constructor")，
- 每个合格的移动构造函数都是[平凡的](/language/move_constructor#.E5.B9.B3.E5.87.A1.E7.9A.84.E7.A7.BB.E5.8A.A8.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/move constructor")，
- 每个合格的复制赋值运算符都是[平凡的](/language/copy_assignment#.E5.B9.B3.E5.87.A1.E7.9A.84.E5.A4.8D.E5.88.B6.E8.B5.8B.E5.80.BC.E8.BF.90.E7.AE.97.E7.AC.A6 "language/copy assignment")，
- 每个合格的移动赋值运算符都是[平凡的](/language/move_assignment#.E5.B9.B3.E5.87.A1.E7.9A.84.E7.A7.BB.E5.8A.A8.E8.B5.8B.E5.80.BC.E8.BF.90.E7.AE.97.E7.AC.A6 "language/move assignment")，并且
- 有一个未被弃置的[平凡析构函数](/language/destructor#.E5.B9.B3.E5.87.A1.E6.9E.90.E6.9E.84.E5.87.BD.E6.95.B0 "language/destructor")。

#### 平凡类

*平凡类* 是满足以下所有条件的类：

- 有一个或多个[合格的默认构造函数](/language/default_constructor#.E5.90.88.E6.A0.BC.E7.9A.84.E9.BB.98.E8.AE.A4.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/default constructor")，它们全部都是[平凡的](/language/default_constructor#.E5.B9.B3.E5.87.A1.E9.BB.98.E8.AE.A4.E6.9E.84.E9.80.A0.E5.87.BD.E6.95.B0 "language/default constructor")。

#### 标准布局类

*标准布局类* 是满足以下所有条件的类：

- 没有具有非标准布局类类型（或这种类型的数组）或到它们的引用的[非静态数据成员](/language/data_members "language/data members")，
- 没有[虚函数](/language/virtual "language/virtual")和[虚基类](/language/derived_class#.E8.99.9A.E5.9F.BA.E7.B1.BB "language/derived class")，
- 所有非静态数据成员都具有相同的[可访问性](/language/access "language/access")，
- 没有非标准布局的基类
- 该类和它的所有基类中的非静态数据成员和[位域](/language/bit_field "language/bit field")都在相同的类中首次声明，并且
- 给定该类为 S，且作为基类时集合 M(S) 没有元素，其中 M(X) 对于类型 X 定义如下：

- 如果 X 是没有（可能继承来的）非静态数据成员的非联合体类类型，那么集合 M(X) 为空。
- 如果 X 是首个非静态数据成员（可能是匿名联合体）具有 X0 类型的非联合体类类型，那么集合 M(X) 包含 X0 和 M(X0) 中的元素。
- 如果 X 是联合体类型，集合 M(X) 是包含所有 Ui 的集合与每个 M(Ui) 集合的并集，其中每个 Ui 是 X 的第 i 个非静态数据成员的类型。
- 如果 X 是元素类型是 Xe 的数组类型，集合 M(X) 包含 Xe 和 M(Xe) 中的元素。
- 如果 X 不是类类型或数组类型，那么集合 M(X) 为空。

*标准布局结构体* 是以类关键词 [struct](/keyword/struct "keyword/struct") 或类关键词 [class](/keyword/class "keyword/class") 定义的标准布局类。*标准布局联合体* 是以类关键词 [union](/keyword/union "keyword/union") 定义的标准布局类。

#### 隐式生存期类

*隐式生存期类* 是满足以下条件之一的类：

- 它是[聚合体](/language/aggregate_initialization "language/aggregate initialization")，或
- 至少有一个平凡且合格的构造函数和一个平凡且未被弃置的析构函数。
