## UART应用编程

## 第一讲 UART硬件

### 1. 串口的硬件介绍

UART的全称是Universal Asynchronous Receiver and Transmitter，即异步发送和接收。
串口在嵌入式中用途非常的广泛，主要的用途有：

* 打印调试信息；
* 外接各种模块：GPS、蓝牙；

串口因为结构简单、稳定可靠，广受欢迎。

通过三根线即可，发送、接收、地线。
<img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_001.jpg">
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

  <img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_002.jpg">

前面图中提及到了逻辑电平，也就是说代表信号1的引脚电平是人为规定的。
如图是TTL/CMOS逻辑电平下，传输‘A’时的波形：
<img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_003.jpg">
在xV至5V之间，就认为是逻辑1，在0V至yV之间就为逻辑0。

如图是RS-232逻辑电平下，传输‘A’时的波形：
<img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_004.jpg">
在-12V至-3V之间，就认为是逻辑1，在+3V至+12V之间就为逻辑0。

RS-232的电平比TTL/CMOS高，能传输更远的距离，在工业上用得比较多。

市面上大多数ARM芯片都不止一个串口，一般使用串口0来调试，其它串口来外接模块。

### 3. 串口电平

ARM芯片上得串口都是TTL电平的，通过板子上或者外接的电平转换芯片，转成RS232接口，连接到电脑的RS232串口上，实现两者的数据传输。
<img src="F:\韦东山系列\韦东山\doc_and_source_for_drivers\IMX6ULL\doc_pic\09_UART\pic\lesson1\lesson1_005.jpg">
现在的电脑越来越少有RS232串口的接口，当USB是几乎都有的。因此使用USB串口芯片将ARM芯片上的TTL电平转换成USB串口协议，即可通过USB与电脑数据传输。
<img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_006.jpg">
上面的两种方式，对ARM芯片的编程操作都是一样的。

### 4. 串口内部结构

ARM芯片是如何发送/接收数据？
如图所示串口结构图：
<img src="https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/lesson1_007.bmp">
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



