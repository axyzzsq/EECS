例程:
```C
#include <stdio.h>
unsigned char mess2[2]={0x01,0x02};
unsigned char *getmess2(unsigned char* mess)
{
        mess2[0] = *mess;
        return mess2;
}
int main()
{
        unsigned char *p =NULL;
        unsigned char temp = 30;
        p = &temp;
        unsigned char *result=NULL;
        result = getmess2(p);
        printf("%d\n",*result);
        return 0;
}
```
指针的内容是一个地址，指针作为形参，要把地址传出去。
