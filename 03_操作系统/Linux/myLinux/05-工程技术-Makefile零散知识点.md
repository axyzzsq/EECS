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





## 5-1、 make distclean

make distclean类似make clean，但同时也将configure生成的文件全部删除掉，包括Makefile。

## 5-2、make clean

清除上次的make命令所产生的object文件（后缀为“.o”的文件）及可执行文件。

## 6、make distcheck

生成发布软件包并对其进行测试检查，以确定发布包的正确性。这个操作将自动把压缩包文件解开，然后执行configure命令，并且执行make，来确认编译不出现错误，最后提示你软件包已经准备好，可以发布了。



## 7、make install

将编译成功的可执行文件安装到系统目录中，一般为/usr/local/bin目录。



## 8、make dist

产生发布软件包文件（即distribution package）。这个命令将会将可执行文件及相关文件打包成一个tar.gz压缩的文件用来作为发布软件的软件包。

它会在当前目录下生成一个名字类似“PACKAGE-VERSION.tar.gz”的文件。PACKAGE和VERSION，是我们在configure.in中定义的AM_INIT_AUTOMAKE(PACKAGE, VERSION)。
