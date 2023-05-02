# [数组-二分查找(No.704)](https://leetcode.cn/problems/binary-search/)

## 思路



## 公司

![image-20230312170624989](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230312170624989.png)

## 解题

### C

#### 1、左闭右闭

```c
int search(int* nums, int numsSize, int target){
    int left = 0;
    int right = numsSize - 1;
    int middle  = 0 ;
    while(left <= right) //右边是闭合的，应该被包含,left = right是合法的
    {
        middle = left + (right - left)/2;
        if(target < nums[middle] )
        {
            right = middle-1; //条件已经排除了num[middle],middle不应该再放入比较的范围内
        }
        else if(target > nums[middle]) 
        {
            left = middle+1;
        }
        else if(target == nums[middle])
        {
            return middle;
        }
    }
    return -1;
}
```

#### 2、左闭右开

```C
int search(int* nums, int numsSize, int target){
    int left = 0;
    int right = numsSize; //右开区间
    int middle  = 0 ;
    while(left < right)
    {
        middle = left + (right - left)/2;
        if(target < nums[middle] )
        {
            right = middle;
        }
        else if(target > nums[middle])
        {
            left = middle+1;
        }
        else if(target == nums[middle])
        {
            return middle;
        }
    }
    return -1;
}
```

### C++