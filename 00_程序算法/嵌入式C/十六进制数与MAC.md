# 十六进制数

## 十六进制数打印

在C语言中，`%x`是用来输出无符号十六进制整数的格式化输出字符，`%02x`是用来输出两位无符号十六进制整数的格式化输出字符，`%08x`是用来输出八位无符号十六进制整数的格式化输出字符。其中，`%02x`和`%08x`中的0表示在不足两位或八位时补0。这些格式化输出字符可以用于`printf`函数等函数中。

```c
#include <stdio.h>

int main() {
    unsigned int num = 0x1234abcd;
    printf("%%x: %x\n", num);
    printf("%%02x: %02x\n", num);
    printf("%%08x: %08x\n", num);
    return 0;
}
```

 ![image-20230418103209544](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230418103209544.png)



## Sprintf

```C
uint8_t* read_mac = LwIP_GetMAC(&xnetif[0]);  
char BLE_name[10]; //5+4+1,read_mac[]每个元素被格式化到BLE_name之后，每个十六进制数占据一个字节
sprintf(BLE_name,"Test-%02x%02x",read_mac[4],read_mac[5]);  
```

