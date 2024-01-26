## new表达式

---

创建并初始化拥有动态[存储期](https://zh.cppreference.com/w/cpp/language/storage_duration "cpp/language/storage duration")的对象，这些对象的生存期不受它们创建时所在的作用域限制。

### 语法

- `::`(optional) `new` type init(optional)

    尝试创建[类型标识](https://zh.cppreference.com/w/cpp/language/type#.E7.B1.BB.E5.9E.8B.E7.9A.84.E5.91.BD.E5.90.8D "cpp/language/type") 类型 所指代的类型的一个对象，它可以是数组类型，可以包含[占位符类型说明符](https://zh.cppreference.com/w/cpp/language/auto "cpp/language/auto") (C++11 起)，或包含将由[类模板实参推导](https://zh.cppreference.com/w/cpp/language/class_template_argument_deduction "cpp/language/class template argument deduction")推出的类模板名 (C++17 起)。
  
符合下列条件之一时，必须要有 初始化器：

+   类型是[未知边界数组](https://zh.cppreference.com/w/cpp/language/array#.E6.9C.AA.E7.9F.A5.E8.BE.B9.E7.95.8C.E6.95.B0.E7.BB.84 "cpp/language/array")

- 类型中使用了[占位符](https://zh.cppreference.com/w/cpp/language/auto "cpp/language/auto")，即 `auto`或 `decltype (auto)`，可能与<a href="https://zh.cppreference.com/w/cpp/language/constrai32s#.E6.A6.82.E5.BF.B5" title="cpp/language/constrai32s">类型约束结合
- 类型</span> 或 中使用了类模板，且该模板有实参需要被<a href="https://zh.cppreference.com/w/cpp/language/class_template_argument_deduction" title="cpp/language/class template argument deduction">推导</a></li></ul></td><td><span class="t-mark-rev t-since-cxx17">

```
```


### 解释

`new` 表达式尝试申请存储空间，并在已申请的存储空间上，尝试构造并初始化一个无名的对象。`new` 表达式返回一个指向它构造的对象纯右值指针。

如果 类型  是数组类型，那么它第一维之外的所有维都必须指定为正的类型是 [std::size\_t](https://zh.cppreference.com/w/cpp/types/size_t "cpp/types/size t") 的[经转换的常量表达式](https://zh.cppreference.com/w/cpp/language/constant_expression "cpp/language/constant expression") (C++14 起)，但（仅当使用无括号语法 (2) 时）第一维可以是整数类型、枚举类型或拥有单个到整数或枚举类型的非 explicit 转换函数的类类型 (C++14 前)任何能转换成 [std::size\_t](https://zh.cppreference.com/w/cpp/types/size_t "cpp/types/size t") 的表达式 (C++14 起)。这是唯一直接创建大小在运行时定义的数组的方法，这种数组常被称作*动态数组*：
```
i32 n \= 42;
double a\[n\]\[5\]; // 错误
auto p1 \= new double\[n\]\[5\];   // OK
auto p2 \= new double\[5\]\[n\];   // 错误：只有第一维可以不是常量
auto p3 \= new (double\[n\]\[5\]); // 错误：语法 (1) 不能用于动态数组

```

第一维为零是可接受的，分配函数也会被调用。

注意：[std::vector](https://zh.cppreference.com/w/cpp/container/vector "cpp/container/vector") 提供了与一维的动态数组类似的功能。

#### 分配

new 表达式通过调用适当的[分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_new "cpp/memory/new/operator new")分配存储。

如[分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_new "cpp/memory/new/operator new")中所描述，C++ 程序可提供这些函数的全局和类特有替换函数。如果 new 表达式以 :: 运算符开始，如 `::new T` ，那么忽略类特有替换函数（在全局[作用域](https://zh.cppreference.com/w/cpp/language/scope "cpp/language/scope")中[查找](https://zh.cppreference.com/w/cpp/language/lookup "cpp/language/lookup")函数）。否则，如果 `T` 是类类型，那么就会从 `T` 的类作用域中开始查找。

在调用分配函数时，new 表达式将请求的字节数作为 [std::size\_t](http://zh.cppreference.com/w/cpp/types/size_t) 类型的第一参数传递给它，该参数对于非数组 `T` 恰好是 sizeof(T)。

数组的分配中可能带有一个未指明的开销（overhead），且每次调用 new 的这个开销可能不同，除非选择的分配函数是标准非分配形式。new 表达式所返回的指针等于分配函数所返回的指针加上该值。许多实现使用数组开销存储数组中的对象数量，[delete\[\]](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete") 表达式会用它进行正确数量的析构函数调用。另外，如果用 new 分配 char、unsigned char 或 [std::byte](https://zh.cppreference.com/w/cpp/types/byte "cpp/types/byte") (C++17 起)的数组，那么它可能从分配函数请求额外内存，以此保证所有不大于请求数组大小的类型的对象在放入所分配的数组中时能够正确对齐。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx14"><td><p>允许将各个 new 表达式通过可替换分配函数所进行的分配予以省略或合并。在省略的情况下，存储可以由编译器提供，而无需调用分配函数（这也允许优化掉不使用的 new 表达式）。在合并的情况下且满足以下所有条件时，new 表达式 <code>E1</code> 所做的分配可以被扩展，以提供另一个 new 表达式 <code>E2</code> 的额外存储：</p><p><span class="t-li">1)</span> <code>E1</code> 所分配对象的生存期严格包含 <code>E2</code> 所分配对象的生存期，</p><p><span class="t-li">2)</span> <code>E1</code> 与 <code>E2</code> 将调用同一可替换全局分配函数</p><p><span class="t-li">3)</span> 对于抛出异常的分配函数，<code>E1</code> 与 <code>E2</code> 中的异常将首先被同一处理块捕捉。</p>注意此优化仅在使用 new 表达式，而非调用可替换分配函数的任何其他方法时允许：<span class="t-c"><span class="mw-geshi cpp source-cpp">delete <span class="br0">[</span><span class="br0">]</span> new <span class="kw4">i32</span><span class="br0">[</span><span class="nu0">10</span><span class="br0">]</span><span class="sy4">;</span></span></span> 能被优化掉，但 <span class="t-c"><span class="mw-geshi cpp source-cpp"><a href="http://zh.cppreference.com/w/cpp/memory/new/operator_delete"><span class="kw692">operator delete</span></a><span class="br0">(</span><a href="http://zh.cppreference.com/w/cpp/memory/new/operator_new"><span class="kw690">operator new</span></a><span class="br0">(</span><span class="nu0">10</span><span class="br0">)</span><span class="br0">)</span><span class="sy4">;</span></span></span> 不能。</td><td><span class="t-mark-rev t-since-cxx14">(C++14 起)</span></td></tr><tr class="t-rev t-since-cxx20"><td><p>在<a href="https://zh.cppreference.com/w/cpp/language/constant_expression" title="cpp/language/constant expression">常量表达式</a>求值期间，始终省略对分配函数的调用。只有在其他情况下调用可替换全局分配函数的 new 表达式能在常量表达式中求值。</p></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

##### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/new&action=edit&section=4 "编辑章节：布置 new")\] 布置 new

如果提供了 布置参数，那么它们会作为额外实参传递给分配函数。这些分配函数被称作“布置 new”，这来源于标准分配函数 void\* [operator new](http://zh.cppreference.com/w/cpp/memory/new/operator_new)([std::size\_t](http://zh.cppreference.com/w/cpp/types/size_t), void\*)，它直接返回未更改的第二实参。它被用于在已分配的存储中构造对象：

// 在任何块作用域内……
{
    // 静态分配拥有自动存储期的存储，对任何对象类型 \`T\` 足够大。
    alignas(T) unsigned char buf\[sizeof(T)\];
 
    T\* tptr \= new(buf) T; // 构造一个 \`T\` 对象，将它直接置于
                          // 你预分配的位于内存地址 \`buf\` 的存储。
 
    tptr\-\>~T();           // 如果程序依赖对象的析构函数的副作用，你必须\*\*手动\*\*调用它。
}                         // 离开此块作用域自动解分配 \`buf\` 。

注意：[分配器 (Allocator)](https://zh.cppreference.com/w/cpp/named_req/Allocator "cpp/named req/Allocator") 类的各成员函数封装了此功能。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx17"><td><p>在分配具有超出 <code>__STDCPP_DEFAULT_NEW_ALIGNMENT__</code> 的对齐要求的对象或这种对象的数组时，new 表达式将对齐要求作为第二实参（以 <span class="t-lc"><a href="https://zh.cppreference.com/w/cpp/memory/new/align_val_t" title="cpp/memory/new/align val t">std::align_val_t</a></span> 包装）传递给分配函数（对于布置形式，<span class="t-spar">布置参数</span> 被放在对齐之后，作为第三、第四个实参等）。如果重载决议失败（当定义了具有不同签名的类特有分配函数时，会发生此种情况，因为它隐藏了全局版本），那么会以实参列表中不带对齐再次尝试进行重载决议。这允许不关心对齐的类特有分配函数优先于全局的关心对齐的分配函数。</p></td><td><span class="t-mark-rev t-since-cxx17">(C++17 起)</span></td></tr></tbody></table>

new T;      // 调用 operator new(sizeof(T))
            // (C++17) 或 operator new(sizeof(T), std::align\_val\_t(alignof(T))))
new T\[5\];   // 调用 operator new\[\](sizeof(T)\*5 + overhead)
            // (C++17) 或 operator new(sizeof(T)\*5+overhead, std::align\_val\_t(alignof(T))))
new(2,f) T; // 调用 operator new(sizeof(T), 2, f)
            // (C++17) 或 operator new(sizeof(T), std::align\_val\_t(alignof(T)), 2, f)

如果不抛出的分配函数（例如 new([std::nothrow](http://zh.cppreference.com/w/cpp/memory/new/nothrow)) T 所选择的）因为分配失败返回空指针，那么 new 表达式会立即返回，而不会试图初始化对象或调用解分配函数。如果将空指针作为实参传给不分配的布置 new 表达式，而这会使得被选择的标准不分配布置分配函数返回空指针，那么行为未定义。

#### \[[编辑](https://zh.cppreference.com/mwiki/index.php?title=cpp/language/new&action=edit&section=5 "编辑章节：构造")\] 构造

new 表达式所创建的对象按照下列规则初始化：

+   对于非数组的 类型 ，在所得内存区域中构造单个对象。

+   如果没有 初始化器，那么对象会被[默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization "cpp/language/default initialization")。
+   如果 初始化器 是带括号的实参列表，那么对象会被[直接初始化](https://zh.cppreference.com/w/cpp/language/direct_initialization "cpp/language/direct initialization")。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><dl><dd><ul><li>如果 <span class="t-spar">初始化器</span> 是花括号包围的实参列表，那么对象会被<a href="https://zh.cppreference.com/w/cpp/language/list_initialization" title="cpp/language/list initialization">列表初始化</a>。</li></ul></dd></dl></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr></tbody></table>

+   如果 类型  是数组类型，那么初始化一个数组的对象。

+   如果没有 初始化器，那么每个元素都会被[默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization "cpp/language/default initialization")。
+   如果 初始化器 是一对空括号，那么每个元素被[值初始化](https://zh.cppreference.com/w/cpp/language/value_initialization "cpp/language/value initialization")。

<table class="t-rev-begin"><tbody><tr class="t-rev t-since-cxx11"><td><dl><dd><ul><li>如果 <span class="t-spar">初始化器</span> 是花括号包围的实参列表，那么数组会被<a href="https://zh.cppreference.com/w/cpp/language/aggregate_initialization" title="cpp/language/aggregate initialization">聚合初始化</a>。</li></ul></dd></dl></td><td><span class="t-mark-rev t-since-cxx11">(C++11 起)</span></td></tr><tr class="t-rev t-since-cxx20"><td><dl><dd><ul><li>如果 <span class="t-spar">初始化器</span> 是带括号的实参列表，那么数组会被<a href="https://zh.cppreference.com/w/cpp/language/aggregate_initialization" title="cpp/language/aggregate initialization">聚合初始化</a>。</li></ul></dd></dl></td><td><span class="t-mark-rev t-since-cxx20">(C++20 起)</span></td></tr></tbody></table>

如果初始化因抛出异常而终止（例如来自构造函数），那么 new 表达式在已经分配了任何存储的情况下会调用适当的[解分配函数](https://zh.cppreference.com/w/cpp/memory/new/operator_delete "cpp/memory/new/operator delete")：对于非数组 类型  是 [operator delete](http://zh.cppreference.com/w/cpp/memory/new/operator_delete)，而对于数组 类型  是 [operator delete](http://zh.cppreference.com/w/cpp/memory/new/operator_delete)\[\]。如果 new 表达式使用 ::new 语法，那么会在全局作用域查找解分配函数，否则如果 `T` 是类类型，那么会在 `T` 的作用域查找。如果失败的是常规（非布置）分配函数，那么会遵循 [delete 表达式](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete")中所述的规则查找解分配函数。对于失败的布置 new，与之匹配的解分配函数中除第一个外的所有形参的类型，必须与布置 new 的各形参类型相同。对解分配函数的调用中，将先前从分配函数取得的值作为第一实参，将对齐作为可选的对齐参数， (C++17 起)并将 `布置参数`（如果存在）作为额外的布置参数。如果找不到解分配函数，那么不会解分配内存。

### 内存泄漏

new 表达式所创建的对象（拥有动态存储期的对象）会持续到将 new 表达式所返回的指针用于匹配的 [delete 表达式](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete")之时。如果指针的原值丢失，那么对象不可达且无法解分配：发生*内存泄漏 (memory leak)*。

对指针赋值时可能发生：
```cpp
let i32* p = new i32{7}; // 动态分配的 i32 带值 7
p = nullptr; // 内存泄漏

```

或指针离开作用域：
```cpp
func f() void {
    let i32* p = new i32{7};
} // 内存泄漏
```

或因为异常：
```cpp
void f()
{
    i32\* p \= new i32(7);
    g();      // 可能抛出异常
    delete p; // 如果没有异常则 ok
} // 如果 g() 抛出异常则内存泄漏
```

为了简化动态分配的对象管理，通常将 new 表达式的结果存储于*[智能指针](https://zh.cppreference.com/w/cpp/memory#.E6.99.BA.E8.83.BD.E6.8C.87.E9.92.88 "cpp/memory")*中：[std::unique\_ptr](https://zh.cppreference.com/w/cpp/memory/unique_ptr "cpp/memory/unique ptr") 或 [std::shared\_ptr](https://zh.cppreference.com/w/cpp/memory/shared_ptr "cpp/memory/shared ptr")。这些指针保证在上述情形中执行 delete 表达式。

### 关键词

[new](https://zh.cppreference.com/w/cpp/keyword/new "cpp/keyword/new")

### 注解

[Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#array-cookies) 要求数组分配开销在创建的数组元素类型为可平凡析构时为零。MSVC 也是这样。

某些实现（如 VS 2019 v16.7 前的 MSVC）在元素类型非可平凡析构的情况下非分配布置数组 new 时会有非零的数组分配开销，这从 [CWG2382](https://wg21.link/p1969#2382) 起不再遵从标准。

创建 char、unsigned char 或 [std::byte](https://zh.cppreference.com/w/cpp/types/byte "cpp/types/byte") (C++17 起) 的数组的非分配布置数组 new 能用于在给定的存储区域隐式创建对象：它结束与该数组重叠的对象的生存期，然后在该数组中隐式创建隐式生存期类型的对象。

### 参阅

+   [构造函数](https://zh.cppreference.com/w/cpp/language/constructor "cpp/language/constructor")
+   [复制消除](https://zh.cppreference.com/w/cpp/language/copy_elision "cpp/language/copy elision")
+   [delete](https://zh.cppreference.com/w/cpp/language/delete "cpp/language/delete")
+   [析构函数](https://zh.cppreference.com/w/cpp/language/destructor "cpp/language/destructor")
+   [初始化](https://zh.cppreference.com/w/cpp/language/initialization "cpp/language/initialization")
    +   [聚合初始化](https://zh.cppreference.com/w/cpp/language/aggregate_initialization "cpp/language/aggregate initialization")
    +   [默认初始化](https://zh.cppreference.com/w/cpp/language/default_initialization "cpp/language/default initialization")
    +   [直接初始化](https://zh.cppreference.com/w/cpp/language/direct_initialization "cpp/language/direct initialization")
    +   [列表初始化](https://zh.cppreference.com/w/cpp/language/list_initialization "cpp/language/list initialization")