#  return a >= b ? a : b;

## 1.找出两个数的最大值
```C
inline int iMax(int a, int b) { return a >= b ? a : b; }
```
等价于
```C
inline int iMax(int a, int b)
{ 
    if ( a >= b ) 
        return a; 
    else 
        return b; 
}
```