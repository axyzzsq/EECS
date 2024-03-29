# 回调函数

- 回调函数可以实现高耦合系统的解耦，使用函数指针来实现插件化，屏蔽内部代码细节。

### 定义

1. 维基百科：把一段可执行的代码像参数传递那样传给其他代码，而这段代码会在某个时刻被调用执行，这就叫做回调。如果代码立即被执行就称为同步回调，如果在之后晚点的某个时间再执行，则称之为异步回调

2. Stack Overflow:函数 F1 调用函数 F2 的时候，函数 F1 通过参数给 函数 F2 传递了另外一个函数 F3 的指针，在函数 F2 执行的过程中，函数F2 调用了函数 F3，这个动作就叫做回调（Callback），而先被当做指针传入、后面又被回调的函数 F3 就是回调函数。

   **回调函数本质上是通过函数指针把一段程序作为参数进行传递。**

### 优点

​        回调函数的优点是解耦。

​		![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326185803.png)

​        回调中，主程序把回调函数像参数一样传入库函数。这样一来，只要我们改变传进库函数的参数，就可以实现不同的功能。并且丝毫不需要修改库函数的实现，这就是解耦。

​        再仔细看看，主函数和回调函数是在同一层的，而库函数在另外一层，想一想，如果库函数对我们不可见，我们修改不了库函数的实现，也就是说不能通过修改库函数让库函数调用普通函数那样实现，那我们就只能通过传入不同的回调函数了，这也就是在日常工作中常见的情况。现在再把main()、Library()和Callback()函数套回前面 F1、F2和F3函数里面。

### 例程

```C
#include<stdio.h>

int Callback_1(int x) // Callback Function 1
{
	printf("Hello, this is Callback_1: x = %d \n", x);
	return 0;
}

int Callback_2(int x) // Callback Function 2
{
	printf("Hello, this is Callback_2: x = %d \n", x);
	return 0;
}

int Callback_3(int x) // Callback Function 3
{
	printf("Hello, this is Callback_3: x = %d \n", x);
	return 0;
}

int Handle(int y, int (*Callback)(int))
{
	printf("Entering Handle Function. \n");
	Callback(y);
	printf("Leaving Handle Function. \n");
}

int main()
{
	int a = 2;
	int b = 4;
	int c = 6;
	printf("Entering Main Function. \n");
	Handle(a, Callback_1);
	Handle(b, Callback_2);
	Handle(c, Callback_3);
	printf("Leaving Main Function. \n");
	return 0;
}
```

​        可以看到，并不是直接把`int Handle(int (*Callback)()) `改成` int Handle(int (*Callback)(int)) `就可以的，而是通过另外增加一个参数来保存回调函数的参数值，像这里` int Handle(int y, int (*Callback)(int))` 的参数 y。同理，可以使用多个参数的回调函数。

​        例程修改：加入typedef，修改回调用回调函数时候的形参

```C
#include<stdio.h>
typedef int (*funType)(int); //将funType声明为一个函数指针类型,这个函数指针满足传入int返回int,可以用funType来声明函数指针
int Callback_1(int x) // Callback Function 1
{
	printf("Hello, this is Callback_1: x = %d \n", x);
	return 0;
}

int Callback_2(int x) // Callback Function 2
{
	printf("Hello, this is Callback_2: x = %d \n", x);
	return 0;
}

int Callback_3(int x) // Callback Function 3
{
	printf("Hello, this is Callback_3: x = %d \n", x);
	return 0;
}

int Handle(int y, funType Callback) //显式声明
{
	printf("Entering Handle Function. \n");
	Callback(y);
	printf("Leaving Handle Function. \n");
}

int main()
{
	int a = 2;
	int b = 4;
	int c = 6;
	printf("Entering Main Function. \n");
	Handle(a, Callback_1);
	Handle(b, Callback_2);
	Handle(c, Callback_3);
	printf("Leaving Main Function. \n");
	return 0;
}
```

