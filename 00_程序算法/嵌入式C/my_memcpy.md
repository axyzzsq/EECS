# my memcpy

- memcpy能拷贝可不止是字符，也可以是整数

## 一、区分memcpy与strcpy

- '\0'对拷贝的影响
- `memcpy`函数是按字节拷贝，无论源内存区域中是否存在字符串终止符`\0`，都会拷贝指定数量的字节。
- `strcpy`函数是按字符串拷贝，只拷贝从源字符串开始直到遇到终止符`\0`的字符，并在目标字符串末尾添加终止符`\0`。
- 如果需要拷贝字节而不关心是否为字符串，可以使用`memcpy`函数；如果需要拷贝以`\0`结尾的字符串，应使用`strcpy`函数。
- `strcpy`函数会将从`src`开始直到遇到字符串终止符`\0`的所有字符拷贝到`dest`所指向的内存区域，并在目标字符串的末尾添加一个终止符`\0`。

## 二、区分memcpy与strncpy

- `strncpy`函数会将从`src`开始直到遇到字符串终止符`\0`或拷贝了`n`个字符为止的内容拷贝到`dest`所指向的内存区域。如果`src`的长度小于`n`，则剩余的空间将用`\0`填充，以确保目标字符串总长度为`n`。

  ![image-20230625191906016](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625191906016.png)

  **strncpy()的拷贝，一碰到'\0'就会终止；**

- memcpy不会考虑'\0'

  ![image-20230625192045821](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625192045821.png)

## 二、my_memcpy

### 方法1：使用循环逐字节拷贝

```C
#include <stdio.h>

void my_memcpy(void *dest, const void *src, size_t n) 
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (size_t i = 0; i < n; i++) 
    {
        d[i] = s[i];
    }
}

int main() 
{
    int source[] = {1, 2, 3, 4, 5};
    int destination[5];

    my_memcpy(destination, source, sizeof(source));

    for (int i = 0; i < sizeof(destination) / sizeof(destination[0]); i++)
    {
        printf("%d ", destination[i]);
    }
    printf("\n");

    return 0;
}

```

![image-20230625194127750](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230625194127750.png)

- 疑问点1：`const char *s = (const char *)src; `代码中`char* s`是`const`，为什么可以被赋值?

  对于声明为`const char *s`的指针变量，`const`关键字修饰的是指针所指向的数据，而不是指针本身。

  在上述代码中，`(const char *)src`进行了类型转换，将`src`指针的类型转换为`const char *`类型，然后将其赋值给了`const char *s`指针变量。这里的`const`表示`src`指针所指向的数据是只读的，即不允许通过`*s`来修改它们。

  虽然`const char *s`意味着不能通过`*s`修改所指向的数据，但它并不限制指针`s`本身的修改。也就是说，可以通过改变指针`s`指向的地址，使其指向其他地方，但不能通过`s`来修改所指向的数据。

  总结起来，`const char *s`表示指向`const char`类型的指针，指针本身可以修改，但不能通过指针来修改所指向的数据。在给`const char *s`赋值时，编译器会进行类型转换以满足赋值的需求，但它并不违反`const`修饰的数据只读的规定。

  

- `void my_memcpy(void *dest, const void *src, size_t n) `为什么使用的是`void*`?

  在函数声明中，使用`void*`类型作为参数类型，表示指针是一个通用的指针类型，可以指向任意类型的数据。

  在`my_memcpy`函数中，使用`void*`类型的参数`dest`和`src`，允许传入任意类型的指针，无论是指向字符型数据、整型数据还是其他类型的数据。这样设计的目的是为了实现通用的内存拷贝函数，不限制特定类型的数据拷贝，使函数更具灵活性和通用性。

  使用`void*`类型的指针可以接受任意类型的指针，但需要注意的是，在实际使用中，需要根据需要进行适当的类型转换，以确保在操作指针所指向的数据时不会导致类型错误或未定义行为。

  因此，使用`void*`作为参数类型，使得`my_memcpy`函数可以接受不同类型的指针参数，实现通用的内存拷贝功能。