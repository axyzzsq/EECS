

# 指针函数与函数指针

### 一、指针函数

> 指针函数本质上是一种返回指针的函数，整形函数返回的是整形，指针函数返回的是指针。
>
> 指针函数的一般形式：类型标识符 * 函数名(参数列表)  ===> `int *f(int a, int b); ` 或者`int* f(int a, int b); ` 

##### 例程：

```C
#include <stdio.h>
int *f(int a, int b); // 声明指针函数  

int *f(int a, int b)
{
    int *p = (int *)malloc(sizeof(int));
    printf("The memeory address of p = 0x%x \n", p);
    memset(p, 0, sizeof(int));
    *p = a + b;
    printf("*p = %d \n", *p);

    return p;
}

int main()
{
    printf("------------------------------ Start\n");

    int *p1 = NULL;
    printf("The memeory address of p1 = 0x%x \n", p1);

    p1 = f(1, 2);

    printf("The memeory address of p1 = 0x%x \n", p1);
    printf("*p1 = %d \n", *p1);

    printf("------------------------------ End\n");
    getchar();
    return 0;
}

```
> 在指针函数内部声明一个指针,给指针申请控制,返回这个指针
> 用在返回字符串



### 二、函数指针

> 函数指针本质上是一个指针，它指向的是一个函数。每个函数都有一个入口地址，函数指针指向的就是函数的入口地址。
>
> 函数指针的一般形式：
>
> -  类型标识符（* 指针变量名) (参数列表）=====> `int (*f)(int a, int b); // 声明函数指针`

##### 例程1：

```C
// 求2个整数的较大值和较小值
#include <stdio.h>
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int (*f)(int, int); // 声明函数指针，该指针指向返回值类型为int，有两个参数类型都是int的函数

int main()
{
    printf("------------------------------ Start\n");

    f = max;    // 函数指针f指向求较大值的函数max
    int c = (*f)(1, 2);  //也可以写成 int c = f(1, 2);
    printf("The max value is %d \n", c);

    f = min;    // 函数指针f指向求最小值的函数min
    c = (*f)(1, 2);  //也可以写成 c = f(1,2);
    printf("The min value is %d \n", c);

    printf("------------------------------ End\n");
    return 0;
}
```

​		一个变量的内存地址可以存储相应的指针变量中，函数的首地址也可以存储在某个函数指针变量中。这样就可以通过这个函数指针变量来调用所指向的函数了。

​		C系列语言中，任何一个变量都要先声明才能使用。函数指针变量也应该先声明。

```C
//函数指针声明范例：
void (*fun)(int)
void (*fun)(int x) //这种声明方法用的少
```

##### 例程2：

```C
#include <stdio.h>
#include <stdlib.h>

void (*funP)(int);   //声明也可写成void(*funP)(int x)，但习惯上一般不这样。
void (*funA)(int);
void myFun(int x);    //声明也可写成：void myFun( int );
int main()
{
    //一般的函数调用
    myFun(100);

    //myFun与funP的类型关系类似于int 与int *的关系。
    funP=&myFun;  //将myFun函数的地址赋给funP变量
    (*funP)(200);  //通过函数指针变量来调用函数
    
    //myFun与funA的类型关系类似于int 与int 的关系。
    funA=myFun; 
    funA(300);
    funP(400);
    (*funA)(500);
    (*myFun)(600);

    return 0;
}

void myFun(int x)
{
    printf("myFun: %d\n",x);
}
```

 例程运行结果：

 ![image-20220818145936060](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220818145936060.png)

例程2总结：

> 1. myFun的函数名和funP、funA函数指针都是一样的，都是函数指针。myFun函数名是一个函数指针常量，而funP、funA是函数指针变量；
>
> 2. 但是函数名调用如果都得（*myFun）(10)就不方便，标准C的设计就把函数调用简化为允许使用myFun(10)这种方式，原始的函数调用方法就是函数指针调用；
>
> 3. 赋值的时候，可以写成funP=&myFun形式，也可以写成funP=myFun;
>
> 4. 但是在声明的时候，void myFun(int)不能写成 void (*myFun)(int),
>
>    void (*funP)（int）不能写成void funP（int）；
>
> 5. 函数指针变量也可以存入到一个数组中，数组的声明方法： int (*fArray[10]) (int)；

##### 例程3：

```C
#include <stdio.h>
#include <stdlib.h>

void (*funP)(int);
void (*funA)(int);
void myFun(int x);
int main()
{
    funP=&myFun;
    printf("sizeof(myFun)=%d\n",sizeof(myFun));
    printf("sizeof(funP)=%d\n",sizeof(funP));
    printf("myFun\t 0x%p---0x%p\n",&myFun,myFun);
    printf("funP\t 0x%p---0x%p---0x%p\n",&funP,funP,*(&funP));  // &funP ==> P自身的地址值
    printf("funA\t 0x%p---0x%p\n",&funA,funA);
    return 0;
}

void myFun(int x)
{
    printf("myFun: %d\n",x);
}
```

 ![image-20220818150645565](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220818150645565.png)

例程3总结：

>1. 函数指针变量跟普通的指针一样在32位系统下大小都为4，64位系统下指针变量大小为8。但是函数指针常量的大小为1;
>2. 函数指针变量和函数指针常量存储在内存的不同位置;
>3. 为负值的函数指针变量（全局）的值为0;

##### 例程4：

```C
//既然函数指针变量是一个变量，当然也可以作为某个函数的参数来使用的。
#include <stdio.h>
#include <stdlib.h>

typedef void(*FunType)(int);
//前加一个typedef关键字，这样就定义一个名为FunType函数指针类型，而不是一个FunType变量。
//形式同 typedef int* PINT;
void myFun(int x);
void hisFun(int x);
void herFun(int x);
void callFun(FunType fp,int x);
int main()
{
    callFun(myFun,100);//传入函数指针常量，作为回调函数
    callFun(hisFun,200);
    callFun(herFun,300);

    return 0;
}

void callFun(FunType fp,int x)
{
    fp(x);//通过fp的指针执行传递进来的函数，注意fp所指的函数有一个参数
}

void myFun(int x)
{
    printf("myFun: %d\n",x);
}
void hisFun(int x)
{
    printf("hisFun: %d\n",x);
}
void herFun(int x)
{
    printf("herFun: %d\n",x);
}
```

### 三、函数指针数组
```C
int (*fun[10])(int *data, int size);
```
使用方法:
```C
int (*sort_fun[5])(int *data, int size) = {
    quick_sort,     /* 快速排序 */
    insert_sort,    /* 插入排序 */
    bubble_sort,    /* 冒泡排序 */
    heap_sort,      /* 堆排序 */
    selection_sort  /* 选择排序 */
};
// 或者
sort_fun[0] = quick_sort;
sort_fun[1] = insert_sort;
```
