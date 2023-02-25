# static关键字

static_example.c:

```C
#include<stdio.h>
static int j;
int fun1()
{
        static int i = 0;
        i++;
        return i;
}

int fun2()
{
        j = 0;
        j++;
        return j;
}

int  main()
{
        int k,i,j;
        for(k = 0; k < 10; k++)
        {
                i = fun1();
                j = fun2();
        }
        printf("main:  i = %d,j = %d \n",i,j);
        return 0;
}

```

编译、执行结果如下：

 ![image-20220808102240875](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220808102240875.png)