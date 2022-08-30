# Linux环境变量

## 1.PATH

PATH环境变量是用于记住程序路径，方便在命令行窗口cmd中的任意目录启动程序。

```shell
echo $PATH
```

![image-20220822115331608](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220822115331608.png)

不同目录之间用分号间隔

## 2、LDLIBRARY_PATH环境变量

`LD_LIBRARY_PATH`是Linux环境变量名，该环境变量主要用于指定查找共享库（动态链接库）时除了默认路径之外的其他路径。

当执行函数动态链接`.so`时，如果此文件不在缺省目录下 /lib和/usr/lib，那么就需要指定环境变量`LD_LIBRARY_PATH` 假如现在需要在已有的环境变量上添加新的路径名，则采用如下方式： `LD_LIBRARY_PATH=NEWDIRS:$LD_LIBRARY_PATH` （newdirs是新的路径串）, 实例如下:

```shell
export LD_LIBRARY_PATH=/export/apps/anaconda2/2.4.1/lib/:$LD_LIBRARY_PATH
```

