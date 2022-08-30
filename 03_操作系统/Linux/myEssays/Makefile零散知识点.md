# Makefile 知识点

## 1、make  -c M

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