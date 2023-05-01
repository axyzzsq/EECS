# 形参与实参  声明与调用

## 1、一级指针

### char*

`char*`表示一个指向字符串的指针，也就是说它存储了一个字符串的地址。当我们把`char*`作为形参传递给一个函数时，我们实际上是把这个地址复制给了函数内部的一个局部变量，也就是形参本身。因此，函数内部可以通过这个地址访问或修改原来的字符串内容，但是不能改变这个地址本身。

例程：

```C
#include <stdio.h>
#include <string.h>

// 声明一个函数，接受一个char*类型的形参
void reverse(char *str) {
  // 获取字符串长度
  int len = strlen(str);
  // 定义两个指针分别指向字符串首尾
  char *left = str;
  char *right = str + len - 1;
  // 循环交换两个指针所指向的字符
  while (left < right) {
    char temp = *left;
    *left = *right;
    *right = temp;
    left++;
    right--;
  }
}

int main() {
  // 定义一个字符串并初始化
  char s[] = "Hello";
  // 打印原始字符串
  printf("Original: %s\n", s);
  // 调用reverse函数，并把s作为实参传递给形参str
  reverse(s);
  // 打印反转后的字符串
  printf("Reversed: %s\n", s);
}
```

- `char s[] = "Hello"; `和` char* s = "hello"；`有什么区别

  - `char s[] = “Hello”;` 声明了一个字符数组`s`，它有足够的空间来存储字符串`"Hello"`（包括结束符’\0’），并把这个字符串复制到了数组中。这个数组是存储在栈空间中的，所以你可以修改它的内容，比如s[0] = ‘h’。

    `char* s = “Hello”; `声明了一个字符指针`s`，它指向了一个字符串常量`"Hello"`。这个字符串常量是存储在常量区中的，所以你不能修改它的内容，否则会出现段错误。你只能修改指针s本身，让它指向其他地方。

  -  如果函数的形参类型是`const char`*，那么你也可以用`char* s = “Hello”;`作为实参，但是你需要注意编译器可能会给出警告，因为这样做可能会破坏常量性。你可以在调用函数时加上const修饰符来消除警告。

    如果函数的形参类型是`char[]`或者`char[n]`，那么你不能用`char* s = “Hello”;`作为实参，因为这样做会导致类型不匹配。你需要用一个字符数组来存储字符串，并把数组名作为实参。

- 本例程中`char s[] = "Hello"`可以被`char* s = "hello"；`平替。

## 2、二级指针

### 应用场景

[二级指针是指向指针的指针，它可以作为函数的形参，用来接收一级指针的地址](https://blog.csdn.net/u012351051/article/details/96026652)[1](https://bing.com/search?q=二级指针作为形参)[2](https://blog.csdn.net/u012351051/article/details/96026652)[。二级指针作为形参的情景一般是为了获取或修改一个特定的地址](https://blog.csdn.net/u012351051/article/details/96026652)[2](https://blog.csdn.net/u012351051/article/details/96026652)，比如动态分配内存、交换两个变量的值、传递指针数组等[3](https://zhuanlan.zhihu.com/p/123932357)[4](https://zhidao.baidu.com/question/2121192278350839347.html)[。如果使用一级指针作为形参，那么只能获取或修改一级指针所指向的内存内容，而不能改变一级指针本身的值](https://zhuanlan.zhihu.com/p/89481530)[5](https://zhuanlan.zhihu.com/p/89481530)。

### 例程

[二级指针作为形参和实参的用法一般有两种情况：一是传递指针数组，二是修改传入的一级指针的值](https://www.cnblogs.com/slankka/p/9158564.html)。下面是一个例程，用来实现动态分配内存给一个字符数组，并打印出来：

```C
#include <stdio.h>
#include <stdlib.h>

// 用二级指针作为形参，分配内存并赋值
void alloc_and_assign(char **p, int n) {
    *p = (char *)malloc(n * sizeof(char)); // 分配n个字符的空间
    if (*p == NULL) { // 检查是否分配成功
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        (*p)[i] = 'a' + i; // 赋值为字母a,b,c,...
    }
}

// 用二级指针作为形参，打印字符数组
void print_array(char **p, int n) {
    for (int i = 0; i < n; i++) {
        printf("%c ", (*p)[i]); // 打印每个字符
    }
    printf("\n");
}

int main() {
    char *q; // 定义一个一级指针
    int size = 10; // 定义数组大小
    alloc_and_assign(&q, size); // 调用函数，传入一级指针的地址作为实参
    print_array(&q, size); // 调用函数，传入一级指针的地址作为实参
    free(q); // 释放内存空间
    return 0;
}
```



## 3、数组

## 4、回调

