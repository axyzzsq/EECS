# GCC常用选项
## 1.头文件包含
```c
#include <stdio.h>  //使用双尖括号表示去工具链所指定的目录下查找头文件
#include "sub.h"  //使用双引号表示在当前目录下来查找头文件
```

## 2.如何解决找不到头文件的报错

> 方法一：在工具链里面加入 `-I`参数指定头文件在哪里；
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326200459.png)
>
> 方法二：把头文件放到工具链指定的系统目录底下；
>
>   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326200515.png)

