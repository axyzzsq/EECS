## my_strcat()

## 方法1：使用指针

```c
#include <stdio.h>

void my_strcat(char* dest, const char* src) 
{
    //先找到dest的末尾
    while (*dest)
        dest++;
    
    //拼接,直至src遇到'\0'
    while (*src) 
    {
        *dest = *src;
        dest++;
        src++;
    }
    
    *dest = '\0'; // 添加字符串结尾的空字符
}

int main() 
{
    char str1[20] = "Hello";
    char str2[] = "World";

    my_strcat(str1, str2);
    printf("%s\n", str1);

    return 0;
}
```

![image-20230625212437260](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625212437260.png)

## 方法2：使用数组索引

```c
#include <stdio.h>

void my_strcat(char dest[], const char src[]) 
{
    int i = 0;
    int j = 0;

    // 定位到dest字符串的结尾
    while (dest[i])
        i++;

    // 拷贝src字符串到dest字符串的结尾
    while (src[j]) {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0'; // 添加字符串结尾的空字符
}

int main() 
{
    char str1[20] = "Hello";
    char str2[] = "World";

    my_strcat(str1, str2);
    printf("%s\n", str1);

    return 0;
}
```

![image-20230625212910944](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625212910944.png)

## 方法3：使用递归

```c
#include <stdio.h>

void my_strcat(char dest[], const char src[])
{
    if (*dest)
    {
        my_strcat(dest + 1, src); // 递归调用，移动dest指针到字符串结尾
    }
    else
    {
        while ((*dest++ = *src++)); // 拷贝src字符串到dest字符串的结尾
       
        /*通俗写法:
            while (*src) 
            {
                dest = *src;
                dest++;
                src++;
            } 
        */
    }
}

int main() 
{
    char str1[20] = "Hello";
    char str2[] = "World";

    my_strcat(str1, str2);
    printf("%s\n", str1);

    return 0;
}
```

![image-20230625213547732](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625213547732.png)

**这些示例都假设目标字符串`dest`具有足够的空间来容纳源字符串`src`的内容。**