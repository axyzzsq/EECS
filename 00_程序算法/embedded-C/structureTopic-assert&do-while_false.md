# Assert and do-while-false

## Assert

C Code：

```shell
#define configASSERT(x) do { \
    if((x) == 0){ \
        char *pcAssertTask = "NoTsk"; \
        if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) \
        { \
            pcAssertTask = pcTaskGetName( NULL );\
        } \
        DiagPrintf("\n\r[%s]Assert(" #x ") failed on line %d in file %s", pcAssertTask, __LINE__, __FILE__); \
        cli(); for(;;);}\
    } while(0)
```

`configASSERT `是一个宏定义，用于在代码中插入断言。断言是一种编程技术，用于在开发阶段检查程序的正确性。如果断言的条件为假`(0)`，则程序会在那里停止执行，并打印出错误信息。

这个宏接受一个参数` x`，这是要检查的条件。如果` x `为假`(0)`，则执行以下操作：

1. 定义一个字符串指针 `pcAssertTask`，并将其初始化为 `NoTsk`。
2. 检查任务调度器的状态。如果任务调度器已经启动，那么 `pcAssertTask` 将被设置为当前任务的名称。
3. 使用 DiagPrintf 函数打印出错误信息，包括任务名称、断言失败的行号和文件名。
4. 调用 cli 函数并进入无限循环，使程序停止执行。

`do{...}while(0)`是一种常见的宏定义技巧，用于确保宏的主体在任何情况下都能正确地作为一个单独的语句来使用。 



## do{...} while(0)

`do{...}while(0)` 是一种常见的在 C 语言中定义宏的技巧。这种结构可以确保宏的主体在任何情况下都能正确地作为一个单独的语句来使用。

例如，假设有以下的宏定义：

```c
#define MACRO(x) foo(x); bar(x)
```

如果在一个` if `语句中使用这个宏，就可能会出现问题：

```C
if (condition)
    MACRO(x);
else
    baz();
```

这段代码实际上会被扩展为：

```c
if (condition)
    foo(x); bar(x);
else
    baz();
```

这并不是我们想要的结果，因为 `bar(x);` 不在` if `语句的控制范围内。

但是，如果使用` do{...}while(0)` 结构来定义宏，就可以避免这个问题：

```C
#define MACRO(x) do { foo(x); bar(x); } while(0)
```

现在，即使在` if `语句中使用这个宏，也能得到正确的结果：

```C
if (condition)
    MACRO(x);
else
    baz();
```

这段代码会被扩展为：

```C
if (condition)
    do { foo(x); bar(x); } while(0);
else
    baz();
```

这样，`foo(x); `和 `bar(x); `都在` if `语句的控制范围内，这就是我们想要的结果。

注意，`do { ... } while(0) `结构只会执行一次，因为` while(0)` 的条件始终为假。这个结构的目的不是为了循环，而是为了创建一个单独的语句块。 