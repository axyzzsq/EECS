# my_memcmp()

## 一、区分memcmp和strcmp

- `memcmp`比较的是两个存储空间的前n个字节，即使字符串已经结束，仍然要比较剩余的空间，直到比较完n个字节。
- `strcmp`比较的是两个字符串，任一字符串结束，则比较结束。

如果想比较两个字符串是否相等，应该使用`strcmp`函数。如果你想比较两个内存区域是否相等，应该使用`memcmp`函数。

## 二、my_memcmp

### 方法1：逐字节比较

```C
#include <stdio.h>

int my_memcmp(const void *ptr1, const void *ptr2, size_t num) 
{
    const unsigned char *p1 = (const unsigned char *)ptr1;
    const unsigned char *p2 = (const unsigned char *)ptr2;

    for (size_t i = 0; i < num; i++) 
    {
        if (p1[i] != p2[i]) 
        {
            return p1[i] - p2[i];
        }
    }

    return 0;
}

int main() 
{
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 6};

    int result = my_memcmp(arr1, arr2, sizeof(arr1));

    if (result == 0) 
    {
        printf("Arrays are equal.\n");
    } 
    else if (result < 0) 
    {
        printf("Array 1 is less than Array 2.\n");
    }
    else 
    {
        printf("Array 1 is greater than Array 2.\n");
    }
    
    return 0;
}

```

![image-20230625205749222](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625205749222.png)



### 方法2：使用指针递增

```C
#include <stdio.h>

int my_memcmp(const void *ptr1, const void *ptr2, size_t num) 
{
    const unsigned char *p1 = (const unsigned char *)ptr1;
    const unsigned char *p2 = (const unsigned char *)ptr2;
	
    while (num-- > 0 && *p1 == *p2) 
    {
        p1++, p2++;
    }
    return (num >= 0) ? (*p1 - *p2) : 0;
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 6};

    int result = my_memcmp(arr1, arr2, sizeof(arr1));

    if (result == 0) 
    {
        printf("Arrays are equal.\n");
    } 
    else if (result < 0) 
    {
        printf("Array 1 is less than Array 2.\n");
    } 
    else 
    {
        printf("Array 1 is greater than Array 2.\n");
    }

    return 0;
}

```

![image-20230625210100082](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625210100082.png)