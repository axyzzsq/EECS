# 通用指针

使用`void*`可以屏蔽内部细节，使得函数对于不同类型的数据都能够通用。这是因为`void*`是一种通用指针类型，可以接受任意类型的指针作为参数。

例程，展示了如何使用`void*`来屏蔽内部细节并实现通用的函数。

```c
#include <stdio.h>

void printValue(void *ptr, int dataType) 
{
    if (dataType == 1) 
    {
        //强制类型转换
        int *p = (int *)ptr;
        printf("Value: %d\n", *p);
    } 
    else if (dataType == 2) 
    {
        float *p = (float *)ptr;
        printf("Value: %f\n", *p);
    } 
    else if (dataType == 3) 
    {
        char *p = (char *)ptr;
        printf("Value: %c\n", *p);
    }
}

int main() 
{
    int intValue = 10;
    float floatValue = 3.14;
    char charValue = 'A';

    printValue(&intValue, 1);   // 输出：Value: 10
    printValue(&floatValue, 2); // 输出：Value: 3.140000
    printValue(&charValue, 3);  // 输出：Value: A

    return 0;
}
```

在上面的示例中，`printValue`函数接受一个`void*`类型的指针作为参数，以及一个用于指示数据类型的整数值。根据`dataType`的值，使用适当的类型转换将`ptr`指针转换为相应的指针类型。然后，通过解引用指针并打印对应的值来输出结果。

通过使用`void*`作为函数参数类型，我们可以将不同类型的指针传递给`printValue`函数，并通过`dataType`参数来指示具体的数据类型。这样，我们就能够屏蔽内部细节，使得函数可以处理不同类型的数据，实现了通用性和灵活性。

需要注意的是，在实际应用中，需要根据具体的需求和情况来设计和实现通用函数，并确保在进行类型转换和解引用时遵循类型安全的规则，以避免潜在的错误和未定义行为。