# 指针常量和常量指针

在C/C++中，常量指针和指针常量是指针的两种不同类型。

常量指针**(const pointer)**,指针本身是一个常量，它指向那个地址，里面的值是可以变化的。也就是说，一旦指针被初始化指向某个内存地址，就不能再指向其他地址，但是可以通过指针来修改所指向地址的值。

指针常量**(pointer to constant)**,指针指向的值是一个常量，它指向的值不能被修改，但是可以指向其他地址。

## 1、常量指针

**只要修改只能传址，不能传值。**

```C
#include <stdio.h>

int main() 
{
    int num = 10;
    int newNum = 20;

    const int* ptr = &num;
    printf("Before: *ptr = %d\n", *ptr);

    ptr = &newNum;  // 合法:指针可以指向不同的地址
    printf("After: *ptr = %d\n", *ptr);

    *ptr = 30;  // 错误:不能通过常量指针修改所指向地址的值

    return 0;
}

```

在这个示例中，`ptr`是一个常量指针，它被初始化指向`num`的地址。然后，它被重新赋值为`newNum`的地址，指针的值可以改变。但当我们尝试通过指针修改所指向地址的值时，会导致编译错误。

![image-20230627134001500](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230627134001500.png)



## 2、指针常量

**要修改只能传值，不能传址。**

```C
#include <stdio.h>

int main() 
{
    int num = 10;
    int newNum = 20;

    int* const ptr = &num;
    printf("Before: *ptr = %d\n", *ptr);

    *ptr = 30;  // 合法：可以通过指针修改所指向地址的值
    printf("After: *ptr = %d\n", *ptr);

    ptr = &newNum;  // 错误：指针是常量，不能指向其他地址

    return 0;
}

```

在这个示例中，ptr是一个指针常量，它被初始化指向num的地址。然后，我们可以通过指针修改所指向地址的值，所以在第一个printf语句中，输出的是10。但当我们尝试将指针重新赋值为newNum的地址时，会导致编译错误。

![image-20230627134320401](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230627134320401.png)

**这两个示例代码展示了常量指针和指针常量之间的差异，常量指针允许更改所指向地址的值，但不能更改指针本身的值；指针常量允许更改指针本身的值，但不能更改所指向地址的值。**

## 3、使用场景

- 常量指针：
  - 常量指针可以用于函数参数中，防止函数内部修改指针所指向的内存空间的值。
  - 另外，常量指针也可以用于保护数据结构中的数据不被修改。
- 指针常量：
  - 指针常量可以用于定义只读变量，例如字符串常量。
  - 另外，指针常量也可以用于定义只读的全局变量。

例程：

```C
#include <stdio.h>

// 常量指针作为函数参数
void print(const int *p) 
{
    // 不能修改指针所指向的内存空间的值
    printf("%d\n", *p);
}

int main() 
{
    // 常量指针
    const int a = 10;
    const int *p1 = &a;
    // 不能修改指针所指向的内存空间的值
    // *p1 = 20; // error

    // 指针常量
    int b = 20;
    int *const p2 = &b;
    // 不能修改指针本身的值
    // p2 = &a; // error

    return 0;
}

```

