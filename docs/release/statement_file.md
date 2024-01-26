# 编译模块接口

---

## 作用

编译时可能对其他编译单元的符号产生依赖，并且在编译阶段不一定能提供完整定义。

定义变量需要类型时完整类型，
指针和引用不要求类型完整。

使用指针和引用时需要类型完整。

函数需要类型声明。

不需要函数完整定义。

## 包含内容

- 函数声明

    包含成员函数
    名字，类型

- 类定义
    名字， 变量成员

- 变量定义

    类型，名字，初始值

### 实现1
类型通过`name manging`映射为字符串。

函数声明定义为

```json
{
    "name":{
        "args_type":[
            "type",
            "type"
        ],
        "return_type":"type",
        "llvm_FunctionType":32
    }
}
```

类定义为

```json
{
    "name":{
        "member":[
            {
                "name":"type"
            },
            {
                "name":"type"
            }
        ],
        "llvm_StructType":5
    }
}
```

变量定义为

```json
{
    "name":{
        "type":"type",
        "llvm_Value":4
    }
}
```