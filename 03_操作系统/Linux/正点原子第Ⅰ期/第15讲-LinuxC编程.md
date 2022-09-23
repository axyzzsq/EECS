# 第15讲-Linux C编程

## 编写

vi编辑器的TAB键默认是8空格，修改成4空格:

用vi打开/etc/vim/vimrc,在文件的最后面输入

`set ts=4`

显示行号，`set nu`

main.c函数：

```C
#include <stdio.h>
int main(int argc,char* argv[])
{
        printf("hello world\n");
        return 0;
}
```

##  编译

```shell
gcc main.c -o main #编译
./main #执行
```





