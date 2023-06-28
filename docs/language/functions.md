## 函数

---

函数（function）是把一个[语句](/language/statements.md "language/statements")序列（*函数体, function body*）关联到一个*名字*和零或更多个*函数形参（function parameter）*的列表的 name 实体。

```go
// 函数名：“isodd”
// 形参列表拥有一个形参，具有名字 “n” 和类型 i32
// 返回类型是 bool
func isodd(i32 n) bool {
  // 函数体开始
  return n % 2
} //函数体结束
```

调用函数时，例如在[函数调用表达式](/language/operator_other#内建函数调用运算符 "language/operator other")中，以各个*实参（argument）*（可由调用场所提供，或者[默认](/language/default_arguments "language/default arguments")）来初始化各个形参，然后执行函数体中的语句。如果一个函数的[形参列表](/language/function#形参列表 "language/function")以 ... 结尾，那么额外的实参可以提供给该函数，这样的函数被称为[变参函数](/language/variadic_arguments "language/variadic arguments")。

```go
func main() {
    for i32 arg : {-3, -2, -1, 0, 1, 2, 3} {
        println(out, isodd(arg)) // isodd 被调用 7 次， 每次从 arg 复制初始化 n
    }
}
```

函数调用表达式中[无限定](/language/unqualified_lookup "language/unqualified lookup")的函数名会以一组额外的规则查找，它被称为[“实参依赖查找”（argument-dependent lookup/ADL）](/language/adl "language/adl")。

函数可以通过[返回](/language/return "language/return")或者[抛出](/language/throw "language/throw")[异常](/language/exceptions.md "language/exceptions")终止。

函数可以是[协程](/language/coroutines "language/coroutines")。协程可以暂停执行，以在之后恢复执行。

~~[函数声明](/language/function "language/function")可以在任何作用域中出现，但~~[函数定义](/language/function "language/function")只能在命名空间作用域出现，或对于[成员](/language/member_functions "language/member functions")和[友元](/language/friend "language/friend")函数，可以在类作用域中出现。在类体中定义而不带 friend 说明符的函数是类成员函数。这种函数拥有许多附加性质，细节见[成员函数](/language/member_functions "language/member functions")。

函数不是对象：不存在函数的数组，而且函数不能按值传递或被其他函数所返回。可以有指向/到函数（除了[主函数](/language/main_function "language/main function")和[大多数标准库函数](/language/extending_std#取址限制 "language/extending std")以外）的指针和引用，它们可以用于这些函数自身无法被使用的地方。因此我们说这些函数“可取址”。

每个函数都具有一个类型，它由函数的返回类型，所有形参的类型（进行数组到指针和函数到指针转换后，见[形参列表](/language/function#形参列表 "language/function")）~~，函数是否为 [`noexcept`](/language/noexcept_spec "language/noexcept spec")，以及对于非静态成员函数的 cv 限定性和引用限定性~~构成。函数类型同样拥有[语言链接](/language/language_linkage "language/language linkage")。不存在有 cv 限定的函数类型（不要与~~如 int f() const; 这样的 [cv 限定函数](/language/member_functions "language/member functions")类型，或如~~ [`std::string`](/string/basic_string)`const f();`这样的返回 [cv 限定类型](/language/cv "language/cv")的函数相混淆）。如果有任何 cv 限定符被添加到到函数类型的别名，那么它会被编译器警告。

同一作用域中，可以有多个函数拥有相同的名称，只要其形参列表 ~~，以及对于非静态成员函数的 cv 或引用限定~~ 不同即可。这被称作[函数重载](/language/overload_resolution "language/overload resolution")。只有返回类型及异常说明不同的函数之间无法重载。[重载函数的地址](/language/overloaded_address "language/overloaded address")有专门的确定方法。

### 函数对象

除了函数左值，函数调用表达式还支持函数指针以及重载了函数调用运算符及可转换为函数指针的任何类类型的值（包括 [lambda 表达式](/language/lambda "language/lambda")）。这些类型被统称为[函数对象 (FunctionObject)](/named_req/FunctionObject "named req/FunctionObject") ，而且在 name 标准库中的各处都有使用，示例可见[二元谓词 (BinaryPredicate)](/named_req/BinaryPredicate "named req/BinaryPredicate") 和[比较 (Compare)](/named_req/Compare "named req/Compare") 的用法。

标准库也提供数个预定义的[函数对象模板](/utility/functional "utility/functional")，以及一些组成新函数对象的方法（包括 [`std::less`](/utility/functional/less "utility/functional/less")、[`std::mem_fn`](/utility/functional/mem_fn "utility/functional/mem fn")、[`std::bind`](/utility/functional/bind "utility/functional/bind")、[`std::function`](/utility/functional/function "utility/functional/function")和[`std::bind_front`](/utility/functional/bind_front "utility/functional/bind front")）。
