# my_strcmp()

## 方法1：逐字符比较

```C
#include <stdio.h>

int my_strcmp(const char* str1, const char* str2)
{
    /* 是否非空且是否相等 */
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    /* 对字符的ascii值进行相减 */
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

int main()
{
    const char* str1 = "Hello";
    const char* str2 = "Hella";

    int result = my_strcmp(str1, str2);

    if (result == 0)
    {
        printf("Strings are equal.\n");
    }
    else if (result < 0)
    {
        printf("String 1 is less than String 2.\n");
    }
    else
    {
        printf("String 1 is greater than String 2.\n");
    }

    return 0;
}

```

![image-20230625203907765](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625203907765.png)



## 方法2:指针比较

```C
#include <stdio.h>

int my_strcmp(const char *str1, const char *str2) 
{
    while (*str1 && (*str1 == *str2)) 
    {
        str1++;
        str2++;
    }
    /* 直接比较指针解引用 */
    if (*str1 == '\0' && *str2 == '\0') 
    {
        return 0;
    }
    else if (*str1 < *str2) 
    {
        return -1;
    } 
    else 
    {
        return 1;
    }
}

int main() {
    const char *str1 = "Hello";
    const char *str2 = "Hella";
    
    int result = my_strcmp(str1, str2);
    
    if (result == 0) 
    {
        printf("Strings are equal.\n");
    } 
    else if (result < 0) 
    {
        printf("String 1 is less than String 2.\n");
    } 
    else 
    {
        printf("String 1 is greater than String 2.\n");
    }
    
    return 0;
}

```

