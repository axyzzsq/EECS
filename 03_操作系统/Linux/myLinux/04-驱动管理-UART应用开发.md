# UART应用编程

## 第一讲 UART硬件

### 1. 串口的硬件介绍

UART的全称是Universal Asynchronous Receiver and Transmitter，即异步发送和接收。
串口在嵌入式中用途非常的广泛，主要的用途有：

* 打印调试信息；
* 外接各种模块：GPS、蓝牙；

串口因为结构简单、稳定可靠，广受欢迎。

通过三根线即可，发送、接收、地线。
![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_001.jpg)
通过TxD->RxD把ARM开发板要发送的信息发送给PC机。
通过RxD->TxD线把PC机要发送的信息发送给ARM开发板。
最下面的地线统一参考地。

### 2. 串口的参数

- 波特率：一般选波特率都会有9600,19200,115200等选项。其实意思就是每秒传输这么多个比特位数(bit)。
- 起始位:先发出一个逻辑”0”的信号，表示传输数据的开始。
- 数据位：可以是5~8位逻辑”0”或”1”。如ASCII码（7位），扩展BCD码（8位）。小端传输。
- 校验位：数据位加上这一位后，使得“1”的位数应为偶数(偶校验)或奇数(奇校验)，以此来校验数据传送的正确性。
- 停止位：它是一个字符数据的结束标志。

怎么发送一字节数据，比如‘A‘?
‘A’的ASCII值是0x41,二进制就是01000001，怎样把这8位数据发送给PC机呢？

* 双方约定好波特率（每一位占据的时间）；

* 规定传输协议

  *  原来是高电平，ARM拉低电平，保持1bit时间；
  *  PC在低电平开始处计时；
  *  ARM根据数据依次驱动TxD的电平，同时PC依次读取RxD引脚电平，获得数据；

  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_002.jpg)

前面图中提及到了逻辑电平，也就是说代表信号1的引脚电平是人为规定的。
如图是TTL/CMOS逻辑电平下，传输‘A’时的波形：
![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_003.jpg)
在xV至5V之间，就认为是逻辑1，在0V至yV之间就为逻辑0。

如图是RS-232逻辑电平下，传输‘A’时的波形：
![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_004.jpg)
在-12V至-3V之间，就认为是逻辑1，在+3V至+12V之间就为逻辑0。

RS-232的电平比TTL/CMOS高，能传输更远的距离，在工业上用得比较多。

市面上大多数ARM芯片都不止一个串口，一般使用串口0来调试，其它串口来外接模块。

### 3. 串口电平

ARM芯片上得串口都是TTL电平的，通过板子上或者外接的电平转换芯片，转成RS232接口，连接到电脑的RS232串口上，实现两者的数据传输。
![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_005.jpg)
现在的电脑越来越少有RS232串口的接口，当USB是几乎都有的。因此使用USB串口芯片将ARM芯片上的TTL电平转换成USB串口协议，即可通过USB与电脑数据传输。
![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_006.jpg)
上面的两种方式，对ARM芯片的编程操作都是一样的。

### 4. 串口内部结构

ARM芯片是如何发送/接收数据？
如图所示串口结构图：
![str](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_007.bmp)
要发送数据时，CPU控制内存要发送的数据通过FIFO传给UART单位，UART里面的移位器，依次将数据发送出去，在发送完成后产生中断提醒CPU传输完成。
接收数据时，获取接收引脚的电平，逐位放进接收移位器，再放入FIFO，写入内存。在接收完成后产生中断提醒CPU传输完成。

## 第二讲 TTY体系中设备节点的差别

参考资料

* [解密TTY](https://www.cnblogs.com/liqiuhao/p/9031803.html)
* [彻底理解Linux的各种终端类型以及概念](https://blog.csdn.net/dog250/article/details/78766716)
* [Linux终端和Line discipline图解](https://blog.csdn.net/dog250/article/details/78818612)
* [What Are Teletypes, and Why Were They Used with Computers?](https://www.howtogeek.com/727213/what-are-teletypes-and-why-were-they-used-with-computers/)


### 1. 傻傻分不清

/dev/ttyS0、/dev/ttySAC0、/dev/tty、/dev/tty0、/dev/tty1、/dev/console，

它们有什么差别？

| 设备节点                            | 含义                                         |
| ----------------------------------- | -------------------------------------------- |
| /dev/ttyS0、/dev/ttySAC0            | 串口                                         |
| /dev/tty1、/dev/tty2、/dev/tty3、…… | 虚拟终端设备节点                             |
| /dev/tty0                           | 前台终端                                     |
| /dev/tty                            | 程序自己的终端，可能是串口、也可能是虚拟终端 |
| /dev/console                        | 控制台，又内核的cmdline参数确定              |



TTY/Terminal/Console/UART，

它们有什么差别？

| 术语     | 含义                                                         |
| -------- | ------------------------------------------------------------ |
| TTY      | 来自teletype，最古老的输入输出设备，现在用来表示内核的一套驱动系统 |
| Terminal | 终端，暗含远端之意，也是一个输入输出设备，可能是真实设备，也可能是虚拟设备 |
| Console  | 控制台，含控制之意，也是一种Terminal，权限更大，可以查看内核打印信息 |
| UART     | 串口，它的驱动程序包含在TTY驱动体系之内                      |



### 2. 要讲历史了

#### 2.1 电传机teletype 

teletype，更准确地说是teleprinter，是一种通信设备，可以用来发送、接收文本信息。

teletype是一家公司的名字，它生产的teleprinter实在太有名，结果公司名变成了这类产品的名字：teleprinter都被称为teletype了。

![image-20210714103221659](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/03_teletype.png)

teletype被用来传输商业电报，想像一下：

* 把两台teletype的线缆接在一起，或者使用无线技术连接两台teletype
* 这边打字，另一边就可以接收到信息并通过纸张打印出来
* 注意：这两台teletype可以离得很远

#### 2.2 计算机需要控制

##### 2.2.1 使用teletype

teletype的简称就是TTY。



最古老的计算机支持的是批处理模型：

* 怎么编程？卡片打孔，然后喂给计算机。
* 怎么得到输出信息？计算机根据结果在卡片上打孔，需要专人翻译这些卡片。



如果把两台teletype的其中一台，替换为计算机，不就更方便了吗？可以即时输入指令、即时看到结果。

于是teletype变成了计算机的终端、Terminal，远端之意。

teletype和计算机可以放在一个房间里，也可以放在很远很远的地方。

![image-20210714104254779](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/04_tty_and_computer.png)



teletype是通过串口(UART)跟计算机相连的：

![image-20210714112051427](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/07_teletype_uart.png)



那时计算机很贵，发展出多用户系统，多个用户可以分时使用计算机，降低成本：

![image-20210714105233555](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/05_multi_user.png)





##### 2.2.2 teletype被淘汰了

1960年代，CRT显示器+键盘，替代了teletype：

* 显示器替代了纸张
* 速度更快
* 成本更低
* 它仍然只是一个终端(terminal)，通过线缆连接到计算机
* 虽然不再是teletype，但是它的驱动程序仍然叫做TTY

![image-20210714110125179](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/06_crt.png)



##### 2.2.3 个人电脑和虚拟终端

上图长得像电脑，但是它只是一个终端，它要连接到计算机才能工作。

这才是电脑：计算单元(CPU、硬盘、内存等等)、终端(键盘、显示器)都齐全了：

![image-20210714113112636](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/08_personal_computer.png)

硬件上只有一套键盘、显示器。

但是在Linux系统中，我们可以打开多个命令行程序(也叫terminal、shell)，每个程序都对应一个"虚拟终端"。



### 3. 在Ubuntu上演示

按住键盘：Ctrl+Alt+F3启动一个虚拟终端，Ctrl+Alt+F4再启动一个虚拟终端。

在里面切换为root用户：

```shell
sudo passwd root // 如果su root不成功，就先设置root密码
su root
```



#### 3.1 各类设备节点的差别

由于历史原因，下图中两条红线之内的代码被称为TTY子系统。

它既支持UART，也支持键盘、显示器，还支持更复杂的功能(比如伪终端)。

![image-20210714151502368](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/09_tty_console.png)

#### 3.2 /dev/ttyN(N=1,2,3,...)

/dev/tty3、/dev/tty4：表示某个程序使用的虚拟终端

```shell
// 在tty3、tty4终端来回切换，执行命令

echo hello > /dev/tty3
echo hi    > /dev/tty4
```



#### 3.3 /dev/tty0

/dev/tty0：表示前台程序的虚拟终端

* 你正在操作的界面，就是前台程序

* 其他后台程序访问/dev/tty0的话，就是访问前台程序的终端，切换前台程序时，/dev/tty0是变化的

  ```shell
  // 1. 在tty3终端执行如下命令
  // 2. 然后在tty3、tty4来回切换
  
  while [ 1 ]; do echo msg_from_tty3 > /dev/tty0; sleep 5; done
  ```

#### 3.4  /dev/tty

/dev/tty表示本程序的终端，可能是虚拟终端，也可能是真实的中断。

程序A在前台、后台间切换，它自己的/dev/tty都不会变。

```shell
// 1. 在tty3终端执行如下命令
// 2. 然后在tty3、tty4来回切换

while [ 1 ]; do echo msg_from_tty3 > /dev/tty; sleep 5; done
```



#### 3.5 Terminal和Console的差别

Terminal含有远端的意思，中文为：终端。Console翻译为控制台，可以理解为权限更大、能查看更多信息。

比如我们可以在Console上看到内核的打印信息，从这个角度上看：

* Console是某一个Terminal
* Terminal并不都是Console。
* 我们可以从多个Terminal中选择某一个作为Console
* 很多时候，两个概念混用，并无明确的、官方的定义

#### 3.6 /dev/console

选哪个？内核的打印信息从哪个设备上显示出来？
可以通过内核的cmdline来指定，
比如: console=ttyS0 console=tty
我不想去分辨这个设备是串口还是虚拟终端，
有没有办法得到这个设备？
有！通过/dev/console！
console=ttyS0时：/dev/console就是ttyS0
console=tty时：/dev/console就是前台程序的虚拟终端
console=tty0时：/dev/console就是前台程序的虚拟终端
console=ttyN时：/dev/console就是/dev/ttyN
console有多个取值时，使用最后一个取值来判断



## 第三讲 TTY驱动程序框架

参考资料

* [解密TTY](https://www.cnblogs.com/liqiuhao/p/9031803.html)
* [彻底理解Linux的各种终端类型以及概念](https://blog.csdn.net/dog250/article/details/78766716)
* [Linux终端和Line discipline图解](https://blog.csdn.net/dog250/article/details/78818612)
* [What Are Teletypes, and Why Were They Used with Computers?](https://www.howtogeek.com/727213/what-are-teletypes-and-why-were-they-used-with-computers/)

### 1. 行规程的引入

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/10_tty_drivers.png)

以下文字引用自参考资料**解密TTY**：

大多数用户都会在输入时犯错，所以退格键会很有用。这当然可以由应用程序本身来实现，但是根据UNIX设计“哲学”，应用程序应尽可能保持简单。为了方便起见，操作系统提供了一个编辑缓冲区和一些基本的编辑命令（退格，清除单个单词，清除行，重新打印），这些命令在行规范（line discipline）内默认启用。高级应用程序可以通过将行规范设置为原始模式（raw mode）而不是默认的成熟或准则模式（cooked and canonical）来禁用这些功能。大多数交互程序（编辑器，邮件客户端，shell，及所有依赖curses或readline的程序）均以原始模式运行，并自行处理所有的行编辑命令。行规范还包含字符回显和回车换行（译者注：\r\n 和 \n）间自动转换的选项。如果你喜欢，可以把它看作是一个原始的内核级sed(1)。

另外，内核提供了几种不同的行规范。一次只能将其中一个连接到给定的串行设备。行规范的默认规则称为N_TTY（drivers/char/n_tty.c，如果你想继续探索的话）。其他的规则被用于其他目的，例如管理数据包交换（ppp，IrDA，串行鼠标），但这不在本文的讨论范围之内。



### 2. TTY驱动程序框架

![image-20210715190445223](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/11_tty_driver_level.png)

![image-20221003183722896](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221003183722896.png)

 ![image-20221003183811000](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221003183811000.png)

## 第四讲 串口应用编程-回环

参考资料

* [Serial Programming Guide for POSIX Operating Systems](https://digilander.libero.it/robang/rubrica/serial.htm)
* [Linux串口编程](https://www.cnblogs.com/feisky/archive/2010/05/21/1740893.html)：有参考代码
* [Linux串口—struct termios结构体](https://blog.csdn.net/yemingzhu163/article/details/5897156)
  * 这个是转载，排版更好看: https://www.cnblogs.com/sky-heaven/p/9675253.html

### 1. 串口API

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/10_tty_drivers.png)

在Linux系统中，操作设备的统一接口就是：open/ioctl/read/write。

对于UART，又在ioctl之上封装了很多函数，主要是用来设置行规程。

所以对于UART，编程的套路就是：

* open
* 设置行规程，比如波特率、数据位、停止位、检验位、RAW模式、一有数据就返回
* read/write



怎么设置行规程？行规程的用结构体termios来表示，可以参考[Linux串口—struct termios结构体](https://blog.csdn.net/yemingzhu163/article/details/5897156)

![image-20210716152256884](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/12_termios.png)

这些函数在名称上有一些惯例：

* tc：terminal contorl
* cf: control flag

下面列出一些函数：

| 函数名      | 作用                                      |
| ----------- | ----------------------------------------- |
| tcgetattr   | get terminal attributes，获得终端的属性   |
| tcsetattr   | set terminal attributes，修改终端参数     |
| tcflush     | 清空终端未完成的输入/输出请求及数据       |
| cfsetispeed | sets the input baud rate，设置输入波特率  |
| cfsetospeed | sets the output baud rate，设置输出波特率 |
| cfsetspeed  | 同时设置输入、输出波特率                  |

函数不多，主要是需要设置好termios中的参数，这些参数很复杂，可以参考[Linux串口—struct termios结构体](https://blog.csdn.net/yemingzhu163/article/details/5897156)。

![image-20221003213353610](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221003213353610.png)



### 2. 串口收发实验

本实验用过把串口的发送、接收引脚短接，实现自发自收：使用write函数发出字符，使用read函数读取字符。

serial_send_recv.c

```C
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

/* set_opt(fd,115200,8,'N',1) */
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	
	if ( tcgetattr( fd,&oldtio) != 0) { 
		perror("SetupSerial 1");
		return -1;
	}
	
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
	break;
	case 8:
		newtio.c_cflag |= CS8;
	break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
	break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
	break;
	case 'N': 
		newtio.c_cflag &= ~PARENB;
	break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
	break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
	break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
	break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	}
	
	if( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;
	
	newtio.c_cc[VMIN]  = 1;  /* 读数据时的最小字节数: 没读到这些数据我就不返回! */
	newtio.c_cc[VTIME] = 0; /* 等待第1个数据的时间: 
	                         * 比如VMIN设为10表示至少读到10个数据才返回,
	                         * 但是没有数据总不能一直等吧? 可以设置VTIME(单位是10秒)
	                         * 假设VTIME=1，表示: 
	                         *    10秒内一个数据都没有的话就返回
	                         *    如果10秒内至少读到了1个字节，那就继续等待，完全读到VMIN个数据再返回
	                         */

	tcflush(fd,TCIFLUSH);
	
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	//printf("set done!\n");
	return 0;
}

int open_port(char *com)
{
	int fd;
	//fd = open(com, O_RDWR|O_NOCTTY|O_NDELAY);
	fd = open(com, O_RDWR|O_NOCTTY);
    if (-1 == fd){
		return(-1);
    }
	
	  if(fcntl(fd, F_SETFL, 0)<0) /* 设置串口为阻塞状态*/
	  {
			printf("fcntl failed!\n");
			return -1;
	  }
  
	  return fd;
}


/*
 * ./serial_send_recv <dev>
 */
int main(int argc, char **argv)
{
	int fd;
	int iRet;
	char c;

	/* 1. open */

	/* 2. setup 
	 * 115200,8N1
	 * RAW mode
	 * return data immediately
	 */

	/* 3. write and read */
	
	if (argc != 2)
	{
		printf("Usage: \n");
		printf("%s </dev/ttySAC1 or other>\n", argv[0]);
		return -1;
	}

	fd = open_port(argv[1]);
	if (fd < 0)
	{
		printf("open %s err!\n", argv[1]);
		return -1;
	}

	iRet = set_opt(fd, 115200, 8, 'N', 1);
	if (iRet)
	{
		printf("set port err!\n");
		return -1;
	}

	printf("Enter a char: ");
	while (1)
	{
		scanf("%c", &c);
		iRet = write(fd, &c, 1);
		iRet = read(fd, &c, 1);
		if (iRet == 1)
			printf("get: %02x %c\n", c, c);
		else
			printf("can not get data\n");
	}

	return 0;
}


```

- `newtio.c_cc[VMIN]  = 1`; 如果设置为0，`iRet = read(fd, &c, 1);`则马上进行读取，速度快于UART的传输速度，则会出现`printf("can not get data\n");`

![image-20221003211938219](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221003211938219.png)

- 勘误: read如果没有读到数据，返回的是0

```shell
# 设置编译工具链
export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
export PATH=$PATH:/home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin
# 编译
arm-buildroot-linux-gnueabihf-gcc -o serial_send_recv serial_send_recv.c
```

![image-20221003211604453](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221003211604453.png)

## 第五讲 串口应用编程-GPS

### 1 GPS简介

全球定位系统(Global Positioning System，GPS)是一种以空中卫星为基础的高精度无线电导航的定位系统，它在全球任何地方以及近地空间都能够提供准确的地理位置、车行速度及精确的时间信息。GPS主要由三大组成部分：空间部分、地面监控部分和用户设备部分。GPS系统具有高精度、全天候、用广泛等特点。

太空卫星部分由多颗卫星组成，分成多个轨道，绕行地球一周约12小时。每个卫星均持续发射载有卫星轨道数据及时间的无线电波，提供地球上的各种接收机来应用。

地面管制部分，这是为了追踪及控制太空卫星运行所设置的地面管制站，主要工作为负责修正与维护每个卫星能够正常运转的各项参数数据，以确保每个卫星都能够提供正确的讯息给使用者接收机来接收

使用者接收机（即用户设备），追踪所有的GPS卫星，并实时的计算出接收机所在位置的坐标、移动速度及时间。我们日常接触到的是用户设备部分，这里使用到的GPS模块即为用户设备接收机部分。

 

### 2 GPS模块硬件

GPS模块与外部控制器的通讯接口有多种方式，这里我们使用串口进行通讯，波特率为9600bps,1bit停止位，无校验位，无流控，默认每秒输出一次标准格式数据。

GPS模块外观如下图所示，通过排线与控制器进行供电和通讯。该模块为集成模块，没有相关原理图。

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/13_gps.png)

### 3 GPS模块数据格式

GPS使用多种标准数据格式，目前最通用的GNSS格式是NMEA0183格式。NMEA0183是最终定位格式，即将二进制定位格式转为统一标准定位格式，与卫星类型无关。这是一套定义接收机输出的标准信息，有几种不同的格式，每种都是独立相关的ASCII格式，逗点隔开数据流，数据流长度从30-100字符不等，通常以每秒间隔持续输出。

NVMEA0183格式主要针对民用定位导航，与专业RTCM2.3/3.0和CMR+的GNSS数据格式不同。通过NMEA0183格式，可以实现GNSS接收机与PC或PDA之间的数据交换，可以通过USB和COM口等通用数据接口进行数据传输，其兼容性高，数据传输稳定。这里我们使用串口进行是通讯，通信框图如下图所示。

 ![image-20210716154938718](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/14_gps_communication.png)

我们使用串口接收数据，收到的数据包含：$GPGGA（GPS定位数据）、$GPGLL（地理定位信息）、$GPGSA（当前卫星信息）、$GPGSV（可见卫星状态信息）、$GPRMC（推荐最小定位信息）、$GPVTG（地面速度信息）。

这里我们只分析$GPGGA (Global Positioning System Fix Data)即可，它包含了GPS定位经纬度、质量因子、HDOP、高程、参考站号等字段。其标准格式如下：

$GPGGA，<1>，<2>，<3>，<4>，<5>，<6>，<7>，<8>，<9>，M，<10>，M，<11>，<12>*hh<CR><LF>　

$XXGGA语句各字段的含义和取值范围各字段的含义和取值范围见下表所示，XX取值有：

* GPGGA：单GPS
* BDGGA：单北斗
* GLGGA：单GLONASS 
* GNGGA：多星联合定位 

| 字段 | 含义                                         | 取值范围                                                     |
| ---- | -------------------------------------------- | ------------------------------------------------------------ |
| <1>  | UTC时间hhmmss.ss                             | 000000.00~235959.99                                          |
| <2>  | 纬度，格式：ddmm.mmmm                        | 000.00000~8959.9999                                          |
| <3>  | 南北半球                                     | N北纬  S南纬                                                 |
| <4>  | 经度格式dddmm.mmmm                           | 00000.0000~17959.9999                                        |
| <5>  | 东西半球                                     | E表示东经  W表示西经                                         |
| <6>  | GPS状态                                      | 0=未定位  1=GPS单点定位固定解  2=差分定位  3=PPS解  4=RTK固定解  5=RTK浮点解  6=估计值  7=手工输入模式  8=模拟模式 |
| <7>  | 应用解算位置的卫星数                         | 00~12                                                        |
| <8>  | HDOP  水平图形强度因子                       | 0.500~99.000（大于6不可用)                                   |
| <9>  | 海拔高度                                     | -9999.9~99999.9                                              |
| <l0> | 地球椭球面相对大地水准面的高度  （高程异常） | -9999.9~99999.9                                              |
| <11> | 差分时间                                     | 从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空   |
| <12> | 参考站号                                     | 0000~1023；不使用DGPS时为空                                  |

 

例子：$GPGGA，074529.82，2429.6717，N，11804.6973，E，1，8，1.098，42.110，，，M，，*76。

```C
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

/* set_opt(fd,115200,8,'N',1) */
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	
	if ( tcgetattr( fd,&oldtio) != 0) { 
		perror("SetupSerial 1");
		return -1;
	}
	
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
	break;
	case 8:
		newtio.c_cflag |= CS8;
	break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
	break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
	break;
	case 'N': 
		newtio.c_cflag &= ~PARENB;
	break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
	break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
	break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
	break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	}
	
	if( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;
	
	newtio.c_cc[VMIN]  = 1;  /* 读数据时的最小字节数: 没读到这些数据我就不返回! */
	newtio.c_cc[VTIME] = 0; /* 等待第1个数据的时间: 
	                         * 比如VMIN设为10表示至少读到10个数据才返回,
	                         * 但是没有数据总不能一直等吧? 可以设置VTIME(单位是10秒)
	                         * 假设VTIME=1，表示: 
	                         *    10秒内一个数据都没有的话就返回
	                         *    如果10秒内至少读到了1个字节，那就继续等待，完全读到VMIN个数据再返回
	                         */

	tcflush(fd,TCIFLUSH);
	
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	//printf("set done!\n");
	return 0;
}

int open_port(char *com)
{
	int fd;
	//fd = open(com, O_RDWR|O_NOCTTY|O_NDELAY);
	fd = open(com, O_RDWR|O_NOCTTY);
    if (-1 == fd){
		return(-1);
    }
	
	  if(fcntl(fd, F_SETFL, 0)<0) /* 设置串口为阻塞状态*/
	  {
			printf("fcntl failed!\n");
			return -1;
	  }
  
	  return fd;
}


int read_gps_raw_data(int fd, char *buf)
{
	int i = 0;
	int iRet;
	char c;
	int start = 0;
	
	while (1)
	{
		iRet = read(fd, &c, 1);
		if (iRet == 1)
		{
			if (c == '$')
				start = 1;
			if (start)
			{
				buf[i++] = c;
			}
			if (c == '\n' || c == '\r')
				return 0;
		}
		else
		{
			return -1;
		}
	}
}

/* eg. $GPGGA,082559.00,4005.22599,N,11632.58234,E,1,04,3.08,14.6,M,-5.6,M,,*76"<CR><LF> */
int parse_gps_raw_data(char *buf, char *time, char *lat, char *ns, char *lng, char *ew)
{
	char tmp[10];
	
	if (buf[0] != '$')
		return -1;
	else if (strncmp(buf+3, "GGA", 3) != 0)
		return -1;
	else if (strstr(buf, ",,,,,"))
	{
		printf("Place the GPS to open area\n");
		return -1;
	}
	else {
		//printf("raw data: %s\n", buf);
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", tmp, time, lat, ns, lng, ew);
		return 0;
	}
}


/*
 * ./serial_send_recv <dev>
 */
int main(int argc, char **argv)
{
	int fd;
	int iRet;
	char c;
	char buf[1000];
	char time[100];
	char Lat[100]; 
	char ns[100]; 
	char Lng[100]; 
	char ew[100];

	float fLat, fLng;

	/* 1. open */

	/* 2. setup 
	 * 115200,8N1
	 * RAW mode
	 * return data immediately
	 */

	/* 3. write and read */
	
	if (argc != 2)
	{
		printf("Usage: \n");
		printf("%s </dev/ttySAC1 or other>\n", argv[0]);
		return -1;
	}

	fd = open_port(argv[1]);
	if (fd < 0)
	{
		printf("open %s err!\n", argv[1]);
		return -1;
	}

	iRet = set_opt(fd, 9600, 8, 'N', 1);
	if (iRet)
	{
		printf("set port err!\n");
		return -1;
	}

	while (1)
	{
		/* eg. $GPGGA,082559.00,4005.22599,N,11632.58234,E,1,04,3.08,14.6,M,-5.6,M,,*76"<CR><LF>*/
		/* read line */
		iRet = read_gps_raw_data(fd, buf);
		
		/* parse line */
		if (iRet == 0)
		{
			iRet = parse_gps_raw_data(buf, time, Lat, ns, Lng, ew);
		}
		
		/* printf */
		if (iRet == 0)
		{
			printf("Time : %s\n", time);
			printf("ns   : %s\n", ns);
			printf("ew   : %s\n", ew);
			printf("Lat  : %s\n", Lat);
			printf("Lng  : %s\n", Lng);

			/* 纬度格式: ddmm.mmmm */
			sscanf(Lat+2, "%f", &fLat);
			fLat = fLat / 60;
			fLat += (Lat[0] - '0')*10 + (Lat[1] - '0');

			/* 经度格式: dddmm.mmmm */
			sscanf(Lng+3, "%f", &fLng);
			fLng = fLng / 60;
			fLng += (Lng[0] - '0')*100 + (Lng[1] - '0')*10 + (Lng[2] - '0');
			printf("Lng,Lat: %.06f,%.06f\n", fLng, fLat);
		}
	}

	return 0;
}


```

