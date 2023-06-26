# my_strcpy()

## 方法一：使用循环实现

```C
#include <stdio.h>

void my_strcpy(char *dest, const char *src) 
{
    int length = 0;
    while (src[length] != '\0') 
    {
        dest[length] = src[length];
        length++;
    }
    dest[length] = '\0';
}

int main() 
{
    char source[] = "Hello, World!";
    char destination[20];

    my_strcpy(destination, source);

    printf("Copied string: %s\n", destination);

    return 0;
}

```

 ![image-20230625190319220](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625190319220.png)

## 方法二：使用指针和递归实现

```C
#include <stdio.h>

void my_strcpy(char *dest, const char *src) 
{
    if (*src == '\0') 
    {
        *dest = '\0';
    } 
    else 
    {
        *dest = *src;
        my_strcpy(dest + 1, src + 1);
    }
}

int main() 
{
    char source[] = "Hello, World!";
    char destination[20];

    my_strcpy(destination, source);

    printf("Copied string: %s\n", destination);

    return 0;
}

```

![image-20230625190342407](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625190342407.png)