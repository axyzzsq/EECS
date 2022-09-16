# Makefile 知识点

## 1、make  -C M

当make的目标为all时，

```shell
-C $(KDIR) 
```

其中 -C 切换到编译所用的内核源码目录;

```shell
M=$(PWD)
```

 M=选项让该Makefile在构造modules目标之前返回到模块源代码所在目录；

![image-20220830152753577](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202208301528519.png)



## 2、CFLAGS选项

CFLAGS 表示用于 C 编译器的选项。

CFLAGS： 指定头文件（.h文件）的路径，如：

```makefile
CFLAGS=-I/usr/include -I/path/include
```

同样地，安装一个包时会在安装路径下建立一个include目录，当安装过程中出现问题时，试着把以前安装的包的include目录加入到该变量中来。



## 3、LDFLAGS选项

LDFLAGS：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置。用法：

```makefile
LDFLAGS=-L/usr/lib -L/path/to/your/lib
```

每安装一个包都几乎一定的会在安装目录里建立一个lib目录。如果明明安装了某个包，而安装另一个包时，它愣是说找不到，可以把那个包的lib路径加入的LDFALGS中试一下。



## 4、是否把模块编译进内核

```makefile
obj-m += test1.o
obj-y += test2.o
```

obj-m表示把文件test1.o作为"模块"进行编译，不会编译到内核，但是会生成一个独立的 "test1.ko" 文件；

obj-y表示把test2.o文件编译进内核;

