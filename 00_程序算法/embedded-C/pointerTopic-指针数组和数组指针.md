# 指针数组和数组指针

## 指针数组

> 指针数组的本质是数组，数组的元素是指针；
>
> 一般形式： 类型名 * 数组名 [数组长度]；
>
> eg: int * p[4]；

### 例程1

> 例程1主要验证指针数组的表达形式

```C
#include "stdio.h"
int a = 0;
int b = 1;
int c = 2;
int d = 3;
int *p[4];
int main()
{
    p[0] = &a;
    p[1] = &b;
    p[2] = &c;
    p[3] = &d;
    printf("%d\n", *p[0]);
    printf("%d\n", *p[1]);
    printf("%d\n", *p[2]);
    printf("%d\n", *p[3]);
    return 0;
}
```

### 例程2

> 二维数组与二维指针

```C
#include "stdio.h"
int array[2][3] =
{
	{ 0, 1, 2 },
	{ 3, 4, 5 }
};
int *p[2][3];
int main()
{
	for (int i = 0; i < 2; i++) //两行
    {
        for (int j = 0; j < 3; j++) //三列
        {
            p[i][j] = &array[i][j];
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", *p[i][j]);
        }
        printf("\n");
    }
    return 0;
}
```

整型多维数组表示的是很数组中每一个元素的类型都是整型的变量，而多维指针数组中表示的是数组中每一个元素的类型都是指针型变量。而在循环赋值时我们将数组指针数组中的每一个指针都指向了整型数组中的每一个元素。

## 数组指针

> 数组指针的本质是指针，指针指向一个数组；
>
> 一般形式: 类型名 (* 数组名) [数组长度]
>
> eg: int (*p)[4]

### 例程

```C
#include "stdio.h"
int array[2][3] =
{
	{ 0, 1, 2 },
	{ 3, 4, 5 }
};
int main()
{
    //指针数组
    int *p[2][3];

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            p[i][j] = &array[i][j];
        }
    }
    //数组指针
    int (*q)[3] = array;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", q[i][j]);
        }
        printf("\n");
    }
    return 0;
}
```

