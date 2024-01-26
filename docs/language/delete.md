## delete 表达式

---

销毁先前由 [new 表达式](new.md "language/new")分配的对象，并释放获得的内存区域。

### 语法

- `::`(optional)`delete` expression

表达式以下之一：

-   具有[可隐式转换](implicit_conversion.md "language/implicit_conversion")到指向对象的指针的类类型的表达式

-   指向对象的指针类型的纯右值

-   指向对象的指针类型的左值

    指向对象的类指针类型的左值变量在调用*delete 表达式*后会变为[亡值](value_category.md#亡值 "language/value_category")。

###  解释

给定从*表达式*求值（并可能经过转换后）得到的指针为 `ptr`。

1. `ptr` 必须是以下之一：

    +   空指针。

    - 指向 [new 表达式](new.md "language/new")所创建的对象的指针。

    +   指向 [new 表达式](new.md "language/new")所创建的对象的基类子对象的指针。

ptr 的被指向类型必须与该对象（或基类子对象）的类型[相似](https://zh.cppreference.com/w/cpp/language/reinterpret_cast#.E7.B1.BB.E5.9E.8B.E5.88.AB.E5.90.8D.E5.8C.96 "cpp/language/reinterpret cast")。如果 ptr 是其他值，包括它是通过 [new 表达式](new.md "language/new")的数组形式获得的指针的情况，那么行为[未定义](https://zh.cppreference.com/w/cpp/language/ub "cpp/language/ub")。

2) ptr 必须是空指针值或先前由[分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_new "cpp/memory/new/operator new")不是不分配形式（即重载 (10)）的 [new 表达式](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new")的数组形式所获得的指针值。

表达式的结果始终具有 **void** 类型。

如果被删除的对象在删除点拥有不完整类类型，且完整类类型拥有不平凡的析构函数或解分配函数，那么行为未定义。

如果 ptr 不是空指针且[解分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_delete "cpp/memory/new/operator delete")不是销毁 delete (C++20 起)，那么 delete 表达式会对被销毁的对象，或对要被销毁的数组的每个元素（从数组的最后元素行进到首元素），调用它的[析构函数](https://zh.cppreference.com/w/cpp/language/destructor "cpp/language/destructor")（如果存在）。z

然后，除非它匹配的 new 表达式已经与另一 new 表达式合并，否则 (C++14 起)无论析构函数是否抛出异常，delete 表达式都会调用[解分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_delete "cpp/memory/new/operator delete")：[operator delete](http://zh.cppreference.com/w/cpp/memory/new/operator_delete)（版本一）或 [operator delete](http://zh.cppreference.com/w/cpp/memory/new/operator_delete)\[\]（版本二）。

在 ptr 所指向对象的动态类型的作用域中[查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")解分配函数的名字，这表示如果存在类特有解分配函数，那么它将会在全局版本之前被找到。如果 delete 表达式中存在 `**::**`，那么查找中只检查全局命名空间。

如果查找找到了多于一个解分配函数，那么按以下方式选择所调用的函数（有关这些函数及它们的效果的更详细描述见[解分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_delete "cpp/memory/new/operator delete")）：

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx20"><td><ul><li>如果至少一个解分配函数是销毁 delete，那么忽略所有非销毁 delete。</li></ul></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr><tr class="t-rev t-since-cxx17"><td><ul><li>如果类型的对齐要求超出 <code>__STDCPP_DEFAULT_NEW_ALIGNMENT__</code>，那么优先采用具对齐解分配函数（有一个 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/memory/new/align_val_t" title="cpp/memory/new/align val t">std::align_val_t</a></span> 类型的形参）。对于其他类型，优先采用不具对齐解分配函数（没有 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/memory/new/align_val_t" title="cpp/memory/new/align val t">std::align_val_t</a></span> 类型的形参）。</li></ul><dl><dd><ul><li>如果找到了多于一个优先函数，那么在下一步中只考虑优先函数。</li><li>如果找不到优先函数，那么在下一步中考虑非优先函数。</li></ul></dd></dl><ul><li>如果只剩下一个函数，那么选择该函数。</li></ul></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr></tbody></table>

+   如果找到的解分配函数是类特有的，那么优先采用不具大小的类特有解分配函数（无 [std::size\_t](https://zh.cppreference.com/w/cpp/types/size_t "cpp/types/size t") 类型的形参）而不是具大小的类特有解分配函数（带 [std::size\_t](https://zh.cppreference.com/w/cpp/types/size_t "cpp/types/size t") 类型的形参）

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx14"><td><ul><li>否则，查找抵达全局作用域，且：</li></ul><dl><dd><ul><li>如果类型完整，且只对于数组形式，如果它的操作数是指向拥有不平凡的析构函数的类或它的（可以是多维的）数组的指针，那么就会选择具大小的全局函数（带 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/types/size_t" title="cpp/types/size t">std::size_t</a></span> 类型的形参）</li><li>否则，选择全局具大小解分配函数（带 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/types/size_t" title="cpp/types/size t">std::size_t</a></span> 类型的形参）还是全局不具大小解分配函数（无 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/types/size_t" title="cpp/types/size t">std::size_t</a></span> 类型的形参）是未指明的。</li></ul></dd></dl></td><td><span class="t-mark-rev t-since-cxx14">(C++14 起)</span></td></tr></tbody></table>

指向要被回收的存储块的指针会作为首个实参，传递给按上述方式所选择的[解分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_delete "cpp/memory/new/operator delete")。块大小作为可选的 [std::size\_t](https://zh.cppreference.com/w/cpp/types/size_t "cpp/types/size t") 实参传递。对齐要求作为可选的 [std::align\_val\_t](https://zh.cppreference.com/w/cpp/memory/new/align_val_t "cpp/memory/new/align val t") 实参传递。 (C++17 起)

如果 ptr 是空指针值，那么不会调用析构函数，且可能会也可能不会调用解分配函数（这是未指明的），但默认的解分配函数保证在传递了空指针时不做任何事。

如果 ptr 是指向 [new](https://zh.cppreference.com/w/cpp/language/new "cpp/language/new") 所分配的对象的基类子对象的指针，那么基类的析构函数必须是[虚函数](https://zh.cppreference.com/w/cpp/language/virtual "cpp/language/virtual")，否则行为未定义。

### 注解

不能删除指向 void 的指针，因为它不是指向完整对象类型的指针。


### 关键词

[delete](https://zh.cppreference.com/w/cpp/keyword/delete "keyword/delete")


### 参阅

+   [new](new.md "language/new")