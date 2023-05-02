# Linux GDB远程调试

## Step1:获取GDB源码

路径：

```shell
http://ftp.gnu.org/gnu/gdb/
```

 ![image-20221024112535169](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024112535169.png)

- 移动到ubuntu系统下

 ![image-20221024113616286](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024113616286.png)

## Step2:交叉编译/安装GDB

- 解压后进入

![image-20221024113846768](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024113846768.png)

### Step2-1:生成PC端可执行程序

- configure生成makefile

  ```shell
  ./configure -target=arm-none-linux-gnueabi --prefix=/home/book/shareTemp/arm-linux-gdb
  ```

  target为目标平台，prefix为安装路径。

   ![image-20221024133405011](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024133405011.png)

  运行结果如下：

  ![image-20221024114741265](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024114741265.png)

  确认生成Makefile

  ![image-20221024114916928](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024114916928.png)

- 执行make

  ![image-20221024115322745](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024115322745.png)

- 执行make install

  ![image-20221024133950489](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024133950489.png)

  进入bin文件查看，可以发现可执行程序是运行在PC端的文件

  ![image-20221024134641634](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221024134641634.png)

  

### Step2-2:生成板端可执行程序

- 进入源码

  ```shell
  cd gdb-7.8.1/gdb/gdbserver
  ```

- 生成Makefile

  ```shell
  ./configure -target=arm-none-linux-gnueabi --prefix=/home/book/shareTemp/arm-linux-gdbserver
  ```

  生成Makefile，target为目标平台，prefix为安装路径。

- 执行`make CC=交叉编译工具链的绝对路径`

  ```shell
  make CC=/home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-buildroot-linux-gnueabihf-gcc
  ```

  把gdbserver移动到板端。

## Step3：启动远程调试

调试框架：

![image-20221025171449239](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025171449239.png)

### step3-1: 板端操作

- 获取板子ip-192.168.123.38

  ![image-20221025171847933](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025171847933.png)

- 启动gdb端口

  ```shell
  gdbserver :12345 test
  ```

   ![image-20221025182256680](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025182256680.png)

  板端gdbserver进入监听状态

### step3-2: ubuntu端操作

- 测试调试程序：

  ```C
  #include <stdio.h>
  
  int add(int a,int b)
  {
  	return a + b;
  }
  
  int sub(int a,int b)
  {
  	return a - b;
  }
  
  int main()
  {
  	printf("2+3 = %d\n",add(2,3));
  	printf("2-3 = %d\n",sub(2,3));
  	return 1;
  }
  
  ```

- 编译

  ```shell
  arm-buildroot-linux-gnueabihf-gdb test
  ```

  ![image-20221025184219991](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025184219991.png)

- 以gad调试工具远端通过ip接入板子

  ```shell
  target remote 192.168.123.38:12345
  ```

  ![image-20221025184408465](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025184408465.png)

此时，可以在板子的串口输出看到ubuntu已经远程接入了板子

![image-20221025184504808](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025184504808.png)

### step3-3 GDB调试

```shell
b 5  # 在程序的第5行打一个断点
b 10  # 在程序的第10行打一个断点
c  # 程序继续往下运行
```

![image-20221025185200392](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221025185200392.png)

- ubuntu端退出命令： Ctrl + D

  

