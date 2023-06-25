# 字符拷贝

```C
char *string1 = "helloworld";
char *string2 = NULL;
```

**有几种方法把字符串“helloworld”从string1赋值给string2;**

在C语言中，有几种方法可以将字符串 "helloworld" 从 `string1` 赋值给 `string2`。请注意，在C语言中，字符串是以空字符(`'\0'`)结尾的字符数组。下面是几种实现的方法：

### 方法1: 使用strcpy函数

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
    char string1[] = "helloworld";
    char *string2;

    // 分配足够的内存来存储字符串
    string2 = (char *)malloc((strlen(string1) + 1) * sizeof(char));

    // 使用strcpy函数进行赋值
    strcpy(string2, string1);

    printf("string2: %s\n", string2);

    // 释放动态分配的内存
    free(string2);

    return 0;
}

```

### 方法2: 使用循环逐个字符赋值

```C
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    char string1[] = "helloworld";
    char *string2;
    int length = 0;
    int i;

    // 计算字符串的长度
    while (string1[length] != '\0') 
    {
        length++;
    }

    // 分配足够的内存来存储字符串
    string2 = (char *)malloc((length + 1) * sizeof(char));

    // 逐个字符赋值
    for (i = 0; i < length; i++) 
    {
        string2[i] = string1[i];
    }
    string2[length] = '\0';  // 添加字符串结尾的空字符

    printf("string2: %s\n", string2);

    // 释放动态分配的内存
    free(string2);

    return 0;
}


```

### 方法3: 使用指针操作

```C
#include <stdio.h>

int main() 
{
    char string1[] = "helloworld";
    char string2[20];
    char *ptr1 = string1;
    char *ptr2 = string2;

    while (*ptr1 != '\0') 
    {
        *ptr2 = *ptr1;
        ptr1++;
        ptr2++;
    }
    *ptr2 = '\0';
    printf("string2: %s\n", string2);
    return 0;
}
```

代码简化：

```C
#include <stdio.h>

int main() 
{
    char string1[] = "helloworld";
    char string2[20];
    char *ptr1 = string1;
    char *ptr2 = string2;

    while ((*ptr2 = *ptr1) != '\0') 
    {
        ptr1++;
        ptr2++;
    }

    printf("string2: %s\n", string2);

    return 0;
}
```

- 不使用数组初始化的结果

![image-20230625172807248](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625172807248.png)

在C语言中，字符串字面量（例如 "helloworld"）是常量，存储在只读内存区域，因此不能通过指针进行修改。

如果尝试将字符串字面量的地址赋给`char*`指针，例如 `char* ptr1 = "helloworld";`，这样的操作是非法的。

```c
char *ptr1 = "helloworld"; // 非法操作
```

这是因为编译器将字符串字面量存储在只读的内存段，尝试通过指针来修改只读内存会导致未定义的行为。

在某些情况下，编译器可能会警告这样的赋值，甚至拒绝编译。

如果需要修改字符串内容或对其进行操作，应该使用字符数组来存储字符串，如下所示：

```c
char string1[] = "helloworld"; // 使用字符数组存储字符串
char *ptr1 = string1;          // 使用指针指向字符数组
```

这样，`string1` 是一个字符数组，存储了字符串 "helloworld" 的副本，并且可以通过指针 `ptr1` 来访问和操作该字符数组的内容。

总结起来，字符串字面量是常量，存储在只读内存区域，因此不能通过指针进行修改。为了能够修改字符串内容，应该使用字符数组来存储字符串。



## 空字符

在C语言中，使用反斜杠（`\`）来表示转义字符，用于表示一些特殊的字符或控制序列。空字符的ASCII码值为0，但直接写作0会与数字0混淆，为了明确表示空字符，需要使用转义字符来表示。

因此，为了将空字符（ASCII码值为0）区分于数字0（ASCII码值为48），C语言使用`\0`来表示空字符。其中，`\`是转义字符的起始，而`0`表示ASCII码值为0的空字符。

`char array[10] = {0};`为什么可以填入数字0？

在C语言中，对于字符数组的初始化，特别是在使用花括号 `{}` 列出初始值时，存在一种隐式类型转换的情况。

对于语句 `char array[10] = {0};`，在进行初始化时发生了一次隐式类型转换。这是因为数组 `array` 的元素类型是 `char`，而花括号中的初始值为整型常量 `0`。

根据C语言的初始化规则，在这种情况下，整型常量 `0` 会被自动转换为 `char` 类型的值。这个转换是符合规范的，因为 `char` 类型可以表示的范围包含整型常量 `0`。

因此，在 `char array[10] = {0};` 这个语句中，发生了一次从整型常量 `0` 到 `char` 类型的隐式类型转换，将整型常量 `0` 转换为 `char` 类型的 `0`，也就是 ASCII 值为 `NUL`（空字符）的字符。

![image-20230625185636594](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625185636594.png)
