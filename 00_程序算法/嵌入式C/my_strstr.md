# my_strstr()

## 一、Usage

在C语言中，`strstr`是一个非常有用的字符串处理函数。它用于在一个字符串中查找子字符串的出现位置。

函数原型如下：

```c
char *strstr(const char *haystack, const char *needle);
```

`strstr`函数接受两个参数：`haystack`和`needle`。`haystack`是要搜索的字符串，`needle`是要查找的子字符串。函数返回一个指向**第一次**出现子字符串的位置的指针，如果未找到子字符串，则返回`NULL`。

下面是一个使用`strstr`函数的示例：

```c
#include <stdio.h>
#include <string.h>

int main() 
{
    char str[] = "Hello, world!";
    char *result = strstr(str, "world");

    if (result != NULL) 
    {
        printf("Substring found at index %ld\n", result - str);
    } 
    else 
    {
        printf("Substring not found\n");
    }

    return 0;
}
```



在上面的示例中，声明了一个字符串`str`，并使用`strstr`函数查找子字符串"world"的位置。如果找到了子字符串，则打印它在原始字符串中的索引；否则，打印"Substring not found"。

![image-20230625215420024](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625215420024.png)

这意味着子字符串"world"在原始字符串中的索引为7。

`strstr`函数的实现通常使用了一种称为"Knuth-Morris-Pratt"算法的高效算法，它可以在较短的时间内找到子字符串的位置。这使得`strstr`函数在C语言中广泛应用于字符串搜索和处理的各种场景。

## 二、Knuth-Morris-Pratt

Knuth-Morris-Pratt（简称KMP）算法是一种用于字符串匹配的高效算法，它可以在线性时间内找到一个字符串在另一个字符串中的出现位置。该算法的关键思想是利用已经匹配过的部分信息，避免不必要的比较，从而提高匹配效率。

KMP算法的核心是构建一个部分匹配表（Partial Match Table），也称为next数组。该表存储了在模式串中每个位置上，当前位置之前的子串中的最长公共前缀和最长公共后缀的长度。利用这个部分匹配表，可以根据匹配失败时的信息，快速移动模式串。

以下是KMP算法的基本步骤：

1. 构建部分匹配表（next数组）：
   - 初始化两个指针i和j，分别指向模式串的第一个字符和第二个字符。
   - 从模式串的第二个字符开始，逐个计算每个位置上的最长公共前缀和最长公共后缀的长度。
   - 如果当前字符和前一个字符匹配，则最长公共前缀的长度为前一个位置的最长公共前缀长度加1；否则，将j回溯到前一个位置，并继续比较。
   - 重复上述步骤，直到计算完所有位置上的最长公共前缀和最长公共后缀的长度。

2. 执行匹配：
   - 初始化两个指针i和j，分别指向原始串和模式串的第一个字符。
   - 逐个比较原始串和模式串中的字符。
   - 如果当前字符匹配，则将两个指针同时后移一位。
   - 如果当前字符不匹配：
     - 根据部分匹配表，将模式串的指针j移动到合适的位置。
     - 继续比较原始串和模式串中的下一个字符。
   - 重复上述步骤，直到找到匹配或遍历完原始串。

KMP算法的优势在于它避免了在匹配过程中的回溯，而是利用已经匹配过的信息进行移动，从而减少了不必要的比较次数，提高了匹配效率。这使得KMP算法在字符串匹配、子串查找等问题中具有广泛的应用。



## 二、my_strstr

下面是使用KMP算法实现的自定义`my_strstr`函数：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void computeLPSArray(const char *pattern, int patternLen, int *lps) {
    int len = 0;
    int i = 1;

    lps[0] = 0;

    while (i < patternLen) 
    {
        if (pattern[i] == pattern[len]) 
        {
            len++;
            lps[i] = len;
            i++;
        } 
        else 
        {
            if (len != 0) 
            {
                len = lps[len - 1];
            } 
            else 
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

char *my_strstr(const char *haystack, const char *needle) 
{
    int haystackLen = strlen(haystack);
    int needleLen = strlen(needle);

    if (needleLen == 0) 
    {
        return (char *)haystack;
    }

    int *lps = (int *)malloc(sizeof(int) * needleLen);
    computeLPSArray(needle, needleLen, lps);

    int i = 0;
    int j = 0;

    while (i < haystackLen) 
    {
        if (haystack[i] == needle[j]) 
        {
            i++;
            j++;
        }

        if (j == needleLen) 
        {
            free(lps);
            return (char *)(haystack + i - j);
        }

        if (i < haystackLen && haystack[i] != needle[j]) 
        {
            if (j != 0) 
            {
                j = lps[j - 1];
            }
            else 
            {
                i++;
            }
        }
    }
    free(lps);
    return NULL;
}

int main() {
    char haystack[] = "Hello, world!";
    char needle[] = "world";

    char *result = my_strstr(haystack, needle);

    if (result != NULL) {
        printf("Substring found at index %ld\n", result - haystack);
    } else {
        printf("Substring not found\n");
    }

    return 0;
}

```

在上述代码中，`computeLPSArray`函数用于计算部分匹配表（LPS数组），`my_strstr`函数使用KMP算法进行字符串匹配。

使用自定义的`my_strstr`函数，我们可以在原始字符串中查找子字符串的出现位置。如果找到了子字符串，将返回指向其第一次出现的位置的指针；否则，返回`NULL`。

上述示例中，我们在字符串"Hello, world!"中查找子字符串"world"，并输出其在原始字符串中的索引。

输出结果为：![image-20230625220025900](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625220025900.png)
