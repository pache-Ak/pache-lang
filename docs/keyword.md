## 关键字 （keyword）

try catch final throw  异常

loop break continue for if else goto lable  while case switch return default 流程控制

~~override~~??(@属性) abstract?? virtual  default delete 成员函数说明

new delete 动态内存

mut volatile?? &(ref) * ~~mutable~~ 类型限定符

true false null 常量

class ~~stuct~~ union func enum this operator let 类型定义

static_cast const_cast repinterpret_cast 
dynamic_cast 转换 TODO 不作为关键字 内建函数

atomic_cancel atomic_commot atomic_noexcept synchronized 事务性内存 TS

co_await co_return co_yield 协程 TS

package import ~~namespace~~ using ~~export~~ 包 作用域管理  TODO

sizeof alignof 对象大小（改为内置函数） TODO

constexpr 
consteval 

constinit(调成进断言) ~~inline~~ 编译器常量

static thread_local ~~register~~  ~~extern~~ 存储期说明

alignas 对齐说明

asm 汇编链接

concept ~~template~~ requirse  ~~typename~~ auto 元类型约束

implicit 隐式

public private friend protected 访问控制

static_assert 检验

reflexpr (反射 TS)

typeid 元类型反射

& | ! 重载 位运算 和 逻辑运算 ？？


还有，在任何位置含双下划线 \_\_ 的所有标识符，和以一个下划线后随一个大写字母开始的所有标识符，都始终受保留，而所有以单下划线开始的标识符，保留用作全局命名空间中的名称。更多细节见标识符。

命名空间 std 用于放置标准**name** 库中的名称。关于向它添加名称的规则，见扩充命名空间 std 。

名称 posix 保留作为未来的顶级命名空间。若程序于该命名空间声明或定义任何内容，则行为未定义。
