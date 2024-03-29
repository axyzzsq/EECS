# 冒泡排序(bubble sorting)

## 算法简介:

1. 比较相邻的元素，前一个比后一个大（或者前一个比后一个小）调换位置
2. 每一对相邻的元素进行重复的工作，从开始对一直到结尾对，这步完成后，结尾为做大或最小的数
3. 针对除了最后一个元素重复进行上面的步骤
4. 重复1-3步骤直到完成排序

## 动画演示：

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/1391679-20180618163321525-1936669878.gif)

## 代码实现：

### 方法1:迭代法

### 	（循环实现）

- 使用两层循环：外层循环控制比较的轮数，内层循环执行相邻元素的比较和交换操作。
- 每轮比较结束后，最大的元素会被冒泡到末尾。
- 需要通过迭代进行多次比较和交换，直到整个数组有序。
- 代码中使用了两个循环来实现冒泡排序。

```C
#include <stdio.h>

// 冒泡排序函数
void bubbleSort(int arr[], int n) 
{
    int i, j;
    
    // 外层循环控制比较的轮数
    for (i = 0; i < n - 1; i++) 
    {
        
        // 内层循环执行相邻元素的比较和交换
        // 每轮比较结束后，最大的元素将会被冒泡到末尾
        for (j = 0; j < n - i - 1; j++) 
        {
            
            // 如果当前元素大于下一个元素，则交换它们的位置
            if (arr[j] > arr[j + 1]) 
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() 
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("初始数组: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    
    bubbleSort(arr, n); // 调用冒泡排序函数
    
    printf("\n排序后的数组: ");
    for (int i = 0; i < n; i++) 
    {
        printf("%d ", arr[i]);
    }
    
    return 0;
}
```

这段代码使用了两层循环来实现冒泡排序。外层循环控制比较的轮数，内层循环执行相邻元素的比较和交换操作。

每轮比较结束后，最大的元素会被冒泡到末尾。在主函数中，我们声明一个整型数组 `arr`，并初始化了一组数据。

然后，计算数组的长度，并输出初始数组。接下来，我们调用 `bubbleSort` 函数进行排序，最后输出排序后的数组。



### 方法2:递归法

- 使用递归的方式实现冒泡排序，将排序过程分解为多个小问题。
- 递归函数接收一个数组和数组长度作为参数。
- 基本情况是当数组长度为1时，无需排序，直接返回。
- 内层循环执行相邻元素的比较和交换操作。
- 递归调用自身，对除最后一个元素外的子数组进行排序。
- 递归函数的调用会反复执行子问题的排序，直到整个数组有序。

```C
#include <stdio.h>

// 递归实现冒泡排序函数
void recursiveBubbleSort(int arr[], int n) 
{
    // 基本情况：当数组长度为1时，无需排序
    if (n == 1) 
    {
        return;
    }
    
    // 内层循环执行相邻元素的比较和交换
    for (int i = 0; i < n - 1; i++) 
    {
        // 如果当前元素大于下一个元素，则交换它们的位置
        if (arr[i] > arr[i + 1])
        {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
        }
    }
    
    // 递归调用，对除最后一个元素外的子数组进行排序
    recursiveBubbleSort(arr, n - 1);
}

int main() 
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("初始数组: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    
    recursiveBubbleSort(arr, n); // 调用递归冒泡排序函数
    
    printf("\n排序后的数组: ");
    for (int i = 0; i < n; i++) 
    {
        printf("%d ", arr[i]);
    }
    
    return 0;
}

```

这段代码使用了递归的方式来实现冒泡排序。递归函数 `recursiveBubbleSort` 接收一个数组和数组的长度作为参数。

首先，函数会判断数组长度是否为1，如果是，则说明数组已经有序，直接返回。否则，内层循环会执行相邻元素的比较和交换操作。

然后，函数通过递归调用自身，对除最后一个元素外的子数组进行排序。在主函数中，我们声明一个整型数组 `arr`，并初始化了一组数据。然后，我们计算数组的长度，并输出初始数组。

接下来，我们调用 `recursiveBubbleSort` 函数进行排序，最后输出排序后的数组。



### 总结

- 迭代法使用循环来控制排序过程，每次通过比较和交换将最大元素冒泡到末尾，直到整个数组有序。
- 递归法使用递归函数来分解排序问题，通过递归调用自身对子数组进行排序，最终实现整个数组的排序。
- 迭代法和递归法都可以实现冒泡排序，只是实现的方式不同，但原理相同。
- 在实际应用中，迭代法通常比递归法更常用，因为迭代法通常效率更高，而递归法可能会导致额外的函数调用开销。