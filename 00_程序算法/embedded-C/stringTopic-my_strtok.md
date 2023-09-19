# strtok

## 一、Usage

`strtok()`函数是C语言中的一个字符串处理函数，用于将字符串分割成一个个子字符串。该函数的原型为：

```c
char *strtok(char *str, const char *delim);
```

其中，`str`为要分割的字符串，`delim`为分隔符字符串。该函数会返回从`str`开头开始的一个个被分割的子串，当没有被分割的子串时则返回`NULL`。使用该函数时需要注意，每次调用该函数时，传入的第一个参数必须是要分割的字符串，而在后续调用时，传入的第一个参数应该是`NULL`。

例程：

```C
#include <stdio.h>
#include <string.h>

int main() 
{
    char s[] = "hello world";
    char *p = strtok(s, " ");  // 以空格为分隔符分割字符串
    while (p != NULL) 
    {
        // 当没有被分割的子串时则返回NULL
        printf("%s\n", p);  // 输出被分割的子串
        p = strtok(NULL, " ");  // 继续分割字符串
    }
    return 0;
}
```

- `strtok`返回的分隔符前的子串，要获取几个子串，就要调用几次`strtok`;
- 每次调用`strtok()`函数时，原来的字符串就会少掉一部分，完整性会被破坏。

## 二、my_strtok()

`my_strtok`函数可以根据指定的分隔符将一个字符串切分成多个子字符串。

```c
#include <stdio.h>

char* my_strtok(char* str, const char* delim) 
{
    static char* ptr;  // 保留切分位置的指针
    if (str != NULL)
        ptr = str;

    if (ptr == NULL || *ptr == '\0')
        return NULL;

    char* token_start = ptr;  // 子字符串的起始位置
    char* token_end = NULL;   // 子字符串的结束位置

    // 查找分隔符或字符串结束符
    while (*ptr != '\0') 
    {
        const char* d = delim;
        while (*d != '\0') 
        {
            if (*ptr == *d) 
            {
                token_end = ptr;
                *token_end = '\0';  // 在分隔符处添加字符串结束符
                ptr++;
                return token_start;
            }
            d++;
        }
        ptr++;
    }

    // 返回最后一个子字符串（不包含分隔符）
    if (token_start != NULL)
        return token_start;

    return NULL;
}

int main()
{
    char str[] = "This,is,a,test";
    char delim[] = ",";

    char* token = my_strtok(str, delim);
    while (token != NULL) 
    {
        printf("%s\n", token);
        token = my_strtok(NULL, delim);
    }

    return 0;
}
```

在上述示例中，`my_strtok`函数通过使用一个静态指针`ptr`来保持切分位置，它接收一个字符串`str`和一个分隔符`delim`作为输入。每次调用`my_strtok`函数时，它会在字符串中查找下一个分隔符，并将该位置的字符修改为字符串结束符`\0`，然后返回当前子字符串的起始位置。通过反复调用`my_strtok`函数，可以获取所有的子字符串。

请注意，这只是`my_strtok`函数的一种实现方式，可能不具有与标准库函数`strtok`完全相同的行为。在实际应用中，请确保对边界情况和特殊输入进行适当的处理和测试。
