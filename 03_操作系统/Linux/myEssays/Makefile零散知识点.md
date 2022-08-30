# Makefile 知识点

## 1、make  -C M

当make的目标为all时，

```shell
-C $(KDIR) 
```

指明跳转到源码目录下读取那里的Makefile；

```shell
M=$(PWD)
```

 表明然后返回到当前目录继续读入、执行当前的Makefile。

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



## 4、LDLIBRARY_PATH环境变量

`LD_LIBRARY_PATH`是Linux环境变量名，该环境变量主要用于指定查找共享库（动态链接库）时除了默认路径之外的其他路径。

当执行函数动态链接`.so`时，如果此文件不在缺省目录下 /lib和/usr/lib，那么就需要指定环境变量`LD_LIBRARY_PATH` 假如现在需要在已有的环境变量上添加新的路径名，则采用如下方式： `LD_LIBRARY_PATH=NEWDIRS:$LD_LIBRARY_PATH` （newdirs是新的路径串）, 实例如下:

```shell
export LD_LIBRARY_PATH=/export/apps/anaconda2/2.4.1/lib/:$LD_LIBRARY_PATH
```

