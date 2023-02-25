# 函数指针数组

## 1、函数指针数组用法

```c
#include <stdio.h>
 
int add(int a,int b)
{
    return a+b;
}
  
int sub(int a,int b)
{
    return a-b;
}
 
 
int mul(int a,int b)
{
    return a*b;
}
 
 
int div(int a,int b)
{
    return a/b;
}
 
 
void make_menu(){
    printf("****************************\n");
    printf("请选择菜单：\n");
    printf("1：加 2：减 3：乘 4：除 0：退出 \n");
}
 
/*定义函数指针数组变量
(int,int) 对应于函数指针数组 指向的函数列表
//方式1*/
//int (*fun_array[4]) (int,int) = {add, sub, mul, div};
 
//方式2： 先定义函数指针类型；在通过指针类型定义函数指针数组 
 
typedef int (*func) (int,int);
 
func fun_array[4] = 
{
    add, 
	sub, 
 	mul, 
 	div
};
 
 
int main()
{
    int i,j;
    int cmd;
    
    while(1)
    {
        make_menu();
        scanf("%d",&cmd);
        if(cmd==0)
        {
            break;
        }
        if(cmd >=1 && cmd<=4 )
        {
            printf("请输入2个数字:");
            scanf("%d%d",&i,&j);
        
            //通过函数指针数组去调用对应的函数
            int result = fun_array[cmd-1](i,j); 
 
            printf("result:%d\n",result);
        }
    }
     
    return 0;
}
```

运行结果如下：

 ![image-20220817092351449](C:\Users\59521\AppData\Roaming\Typora\typora-user-images\image-20220817092351449.png)

