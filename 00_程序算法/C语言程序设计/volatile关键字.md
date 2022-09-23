# volatile key word

## 1. Brief Introduction

Indicates that a variable can be changed by a background routine.

Keyword volatile is an extreme opposite of const.It indicates that a variable may be changed in a way which is absolutely unpredictable by analysing the normal program flow (for example, a variable which may be changed by an interrupt handler). This keyword uses the following syntax:

```c
volatile data-definition;
```

Every reference to the variable will reload the contents from memory rather than take advantage of situations where a copy can be in a register.

变量如果加了volatile修饰，会从内存中重新加载，不会从寄存器中直接读取；

应用的场景主要是在单片机中断和CPU相关的寄存器配置。

example:

```c
//编译led.c文件
 
#define GPC1CON *((volatile unsigned int*)0xE0200080)
#define GPC1DAT *((volatile unsigned int*)0xE0200084)
#define GPC1PUD *((volatile unsigned int*)0xE0200088)
//隐式声明
void delay (unsigned int);
void led_test (void) {
    //配置相应管脚为输出功能 GPC1_3
    GPC1CON &= ~(0x0f << 12);
    GPC1CON |= (1 << 12);
    //GPC1_4为输出功能
    GPC1CON |= (1 << 16);
    //禁止内部上拉下拉功能
    GPC1PUD &= ~(0x03 << 6);
    GPC1PUD &= ~(0x03 << 8);
    while (1) {
        //灯亮
        GPC1DAT |= (1 << 3);
        GPC1DAT |= (1 << 4);
        delay (0x100000);
        //灯灭
        GPC1DAT &= ~(1 << 3);
        GPC1DAT &= ~(1 << 4);
        delay (0x100000);
    }
}
 
void delay (unsigned int n) {
    unsigned int i = 0;
    for (i = n; i != 0; i--);
}
```

编译：
`arm-linux-gcc -c led.c -o led.o –nostdlib` 
不使用标准库，生成led.o文件

## 2. Why use volatile

volatile的作用是作为指令关键字，确保本指令不会因为编译器的优化而省略，且要求每次直接读值。

现在考虑一个问题，编译器如何对代码进行优化？

C程序：

```cpp
//示例一
#include <stdio.h>
int main (void)
{
	int i = 10;
	int a = i; //优化
	int b = i;
 
	printf ("i = %d\n", b);
	return 0;
}
```

反汇编：
`gc -O2 -S test.c` 

```assembly
cat test.s 
 
        .file   "test1.c"
        .text
        .section        .rodata.str1.1,"aMS",@progbits,1
.LC0:
        .string "i = %d\n"
        .section        .text.startup,"ax",@progbits
        .p2align 4
        .globl  main
        .type   main, @function
main:
.LFB23:
        .cfi_startproc
        endbr64
        subq    $8, %rsp
        .cfi_def_cfa_offset 16
        movl    $10, %edx
        movl    $1, %edi
        xorl    %eax, %eax
        leaq    .LC0(%rip), %rsi
        call    __printf_chk@PLT
        xorl    %eax, %eax
        addq    $8, %rsp
        .cfi_def_cfa_offset 8
        ret
        .cfi_endproc
.LFE23:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
        .section        .note.GNU-stack,"",@progbits
        .section        .note.gnu.property,"a"
        .align 8
        .long    1f - 0f
        .long    4f - 1f
        .long    5
0:
        .string  "GNU"
1:
        .align 8
        .long    0xc0000002
        .long    3f - 2f
2:
        .long    0x3
3:
        .align 8
4:

```

对C程序i的定义进行修改，`volatile int i = 10;`

```C
//示例一
#include <stdio.h>
int main (void)
{
	volatile int i = 10;
	int a = i; //优化
	int b = i;
 
	printf ("i = %d\n", b);
	return 0;
}
```



```assembly
        .file   "test1.c"
        .text
        .section        .rodata.str1.1,"aMS",@progbits,1
.LC0:
        .string "i = %d\n"
        .section        .text.startup,"ax",@progbits
        .p2align 4
        .globl  main
        .type   main, @function
main:
.LFB23:
        .cfi_startproc
        endbr64
        subq    $24, %rsp
        .cfi_def_cfa_offset 32
        leaq    .LC0(%rip), %rsi
        movl    $1, %edi
        movl    $10, 12(%rsp)
        movl    12(%rsp), %eax
        xorl    %eax, %eax
        movl    12(%rsp), %edx
        call    __printf_chk@PLT
        xorl    %eax, %eax
        addq    $24, %rsp
        .cfi_def_cfa_offset 8
        ret
        .cfi_endproc
.LFE23:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
        .section        .note.GNU-stack,"",@progbits
        .section        .note.gnu.property,"a"
        .align 8
        .long    1f - 0f
        .long    4f - 1f
        .long    5
0:
        .string  "GNU"
1:
        .align 8
        .long    0xc0000002
        .long    3f - 2f
2:
        .long    0x3
3:
        .align 8
4:

```

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190012.png)

可以清楚的看到：使用 volatile 的代码编译未优化。

- volatile 指出 i 是随时可能发生变化的，每次使用它的时候必须从 i的地址中读取，因而编译器生成的汇编代码会重新从i的地址读取数据放在 b 中。而优化做法是，由于编译器发现两次从 i读数据的代码之间的代码没有对 i 进行过操作，它会自动把上次读的数据放在 b 中。而不是重新从 i 里面读。这样以来，如果 i是一个寄存器变量或者表示一个端口数据就容易出错，所以说 volatile 可以保证对特殊地址的稳定访问。
  

第二个例程test2.c：

```C

#include <stdio.h>  
#include <sys/timeb.h>  
long long getSystemTime() {  
    struct timeb t;  
    ftime(&t);  
    return 1000 * t.time + t.millitm;  
}  
  
#define TIME 1000000000  
  
int main(void)  
{     
      
    volatile int a, b = TIME; /* volatile修饰变量 */  
    int x, y = TIME;          /* 一般变量   */  
    long long start = 0, end = 0;  
      
    start=getSystemTime();  
    for (a = 0; a < b; a++);  
    end=getSystemTime();  
    printf("vloatile修饰变量用时: %lld ms\n", end - start);  
      
    start=getSystemTime();  
    for (x = 0; x < y; x++);  
    end=getSystemTime();  
    printf("一般变量用时: %lld ms\n", end - start);  
  
    return 0;  
}
```

`gcc test2.c`

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190051.png)

`gcc -O2 test2.c`

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190112.png)

`for(int i=0; i<100000; i++);`

这个语句用来测试空循环的速度的，但是编译器肯定要把它优化掉，根本就不执行。如果你写成，

`for(volatile int i=0; i<100000; i++);`,

它就会执行了。

## 3. applications

### 3.1 并行设备的硬件寄存器（e.g. 状态寄存器）

存储器映射的硬件寄存器通常也要加 voliate，因为每次对它的读写都可能有不同意义。

例如：
假设要对一个设备进行初始化，此设备的某一个寄存器为0xff800000。

```C
int  *output = (unsigned  int *)0xff800000;//定义一个IO端口；
int   init(void)
{
      int i;
      for(i=0;i< 10;i++)
      {
         *output = i;
      }
}
```

经过编译器优化后，编译器认为前面循环半天都是废话，对最后的结果毫无影响，因为最终只是将output这个指针赋值为 9，所以编译器最后给你编译编译的代码结果相当于：

```C
int  init(void)
{
      *output = 9;
}
```

如果你对此外部设备进行初始化的过程是必须是像上面代码一样顺序的对其赋值，显然优化过程并不能达到目的。反之如果你不是对此端口反复写操作，而是反复读操作，其结果是一样的，编译器在优化后，也许你的代码对此地址的读操作只做了一次。然而从代码角度看是没有任何问题的。这时候就该使用volatile通知编译器这个变量是一个不稳定的，在遇到此变量时候不要优化。
再例如上面提到的 volatile 用于相关寄存器定义

```C
//编译led.c文件
 
#define GPC1CON *((volatile unsigned int*)0xE0200080)
#define GPC1DAT *((volatile unsigned int*)0xE0200084)
#define GPC1PUD *((volatile unsigned int*)0xE0200088)
//隐式声明
void delay (unsigned int);
void led_test (void) {
    //配置相应管脚为输出功能 GPC1_3
    GPC1CON &= ~(0x0f << 12);
    GPC1CON |= (1 << 12);
    //GPC1_4为输出功能
    GPC1CON |= (1 << 16);
    //禁止内部上拉下拉功能
    GPC1PUD &= ~(0x03 << 6);
    GPC1PUD &= ~(0x03 << 8);
    while (1) {
        //灯亮
        GPC1DAT |= (1 << 3);
        GPC1DAT |= (1 << 4);
        delay (0x100000);
        //灯灭
        GPC1DAT &= ~(1 << 3);
        GPC1DAT &= ~(1 << 4);
        delay (0x100000);
    }
}
 
void delay (unsigned int n) {
    unsigned int i = 0;
    for (i = n; i != 0; i--);
}
```

- 编译：
  `arm-linux-gcc -c led.c -o led.o –nostdlib` 
  不使用标准库，生成led.o文件

> - `#define GPC1CON *((volatile unsigned int*)0xE0200080) `  怎么理解？
>   这里其实就是定义了一个指针变量。
>   - GPC1CON 为寄存器名称、0xE0200080 为寄存器地址、(volatile unsigned int*) 为强制类型转换。
>
> - 我们知道 volatile 和 const 一样为类型修饰符，不改变变量类型。寄存器地址为什么要加 volatile 修饰呢？
>   - 这些寄存器里面的值是随时变化的。如果我们没有将这个地址强制类型转换成 volatile，那么我们在使用GPC1CON 这个寄存器的时候， 会直接从 CPU 的寄存器中取值。因为之前GPC1CON  被访问过，也就是之前就从内存中取出 GPC1CON 的值保存到某个寄存器中。之所以直接从寄存器中取值，而不去内存中取值，是因为编译器优化代码的结果（访问 CPU寄存器比访问 RAM 快的多）。用 volatile 关键字对 0xE0200080  进行强制转换，使得每一次访问 GPC1CON 时，执行部件都会从 0xE0200080  这个内存单元中取出值来赋值给 GPC1CON  。
>     

### 3.2 一个中断服务子程序中会访问到的非自动变量(Non-automatic variable)

由于访问寄存器的速度要快过RAM，所以编译器一般会做减少存取外部RAM的优化，例如：

```C
static int i=0; //i 为非自动变量
int main(void)
{
     ...
     while (1)
     {
         if (i)
         { 
             dosomething();
         }
     }
}
    
/* Interrupt service routine. */
void ISR_2(void)
{
      i=1;
}
```

> 程序的本意是希望 ISR_2 中断产生时，在main函数中调用 dosomething 函数。
>
> 但是，由于编译器判断在 main 函数里面没有修改过 i，因此可能只执行一次对从i到某寄存器的读操作，然后每次if判断都只使用这个寄存器里面的“i副本”，导致 dosomething 永远也不会被调用。
>
> 如果将变量加上 volatile 修饰，则编译器保证对此变量的读写操作都不会被优化（肯定执行）。此例中i也应该如此说明。

### 3.3 多线程应用中被几个任务共享的变量

当两个线程都要用到某一个变量且该变量的值会被改变时，应该用 volatile 声明，该关键字的作用是防止优化编译器把变量从内存装入CPU寄存器中。如果变量被装入寄存器，那么两个线程有可能一个使用内存中的变量，一个使用寄存器中的变量，这会造成程序的错误执行。volatile的意思是让编译器每次操作该变量时一定要从内存中真正取出，而不是使用已经存在寄存器中的值，如下：

```C

volatile  BOOL  bStop  =  FALSE;  //bStop  为共享全局变量
//(1) 在一个线程中：  
  while(  !bStop  )  {  ...  }  
  bStop  =  FALSE;  
  return;    
 
//(2) 在另外一个线程中，要终止上面的线程循环：  
  bStop  =  TRUE;  
  while(  bStop  );

```

> 等待上面的线程终止，如果bStop不使用volatile申明，那么这个循环将是一个死循环，因为bStop已经读取到了寄存器中，寄存器中bStop的值永远不会变成FALSE，加上volatile，程序在执行时，每次均从内存中读出bStop的值，就不会死循环了。

## 4. volatile issues and summary

### 4.1 volatile issues

- **一个参数既可以是const还可以是volatile吗？**

  - 可以，例如**只读的状态寄存器**。它是 volatile 因为它可能被意想不到地改变。它是 const 因为程序不应该试图去修改它。

     ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190144.png)

- **一个指针可以是volatile吗？**

  - 可以，当一个中服务子程序修改一个指向一个 buffer 的指针时。

- **下面的函数有什么错误？**

  ```C
  int square(volatile int *ptr) 
  { 
      return *ptr * *ptr; 
  }
  ```

  - 这段代码的目的是用来返指针*ptr指向值的平方，但是，由于*ptr指向一个volatile型参数，编译器将产生类似下面的代码：

    ```C
    int square(volatile int *ptr)  
    { 
        int a,b; 
        a = *ptr; 
        b = *ptr; 
        return a * b; 
    } 
    ```

  - 由于*ptr的值可能被意想不到地改变，因此a和b可能是不同的。结果，这段代码可能返不是你所期望的平方值！正确的代码如下：

    ```C
    long square(volatile int *ptr)  
    { 
        int a; 
        a = *ptr; 
        return a * a; 
    }
    ```

### 4.2 volatile summary

volatile 关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素更改。volatile 提醒编译器它后面所定义的变量随时都有可能改变，因此编译后的程序每次需要存储或读取这个变量的时候，都会直接从变量地址中读取数据。如 果没有 volatile 关键字，则编译器可能优化读取和存储，可能暂时使用寄存器中的值，如果这个变量由别的程序更新了的话，将出现不一致的现象。所以遇到这个关键字声明的变量，编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问。

[volatile的一种解释](https://blog.csdn.net/ma57457/article/details/79953381)