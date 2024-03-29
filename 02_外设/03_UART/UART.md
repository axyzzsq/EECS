# UART

## 通信协议

![image-20220814172035762](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220814172035762.png)

## 通信格式与编码

![image-20220814185808350](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220814185808350.png)

- start bit：发送位，拉低一个单位时间，表示单片机要开始发送信息，发送长度是一个字节，
- stop big：停止位，拉高一个单位时间，表示单片机已经完成一个字节的信息发送。
- 那这个单位时间是多长？

  - 波特率取1200bps—— bit per second

    > $$
    > 传输一个bit的时间 time = \frac{1}{1200}(s) ≈ 833(μs)
    > $$
    >
    > ![image-20220814190953811](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220814190953811.png)
    >
    > - ```c
    >   // 拉低+延时833微秒即进入发送状态
    >   uartTXD = 1;
    >   uartTXD = 0;
    >   Delay833μs();
    >   ```
    > - 最低位在右边，向右移动一位；
    > - 软件延时的方法在实际工程中不允许使用，不准确，且造成线程的死延时；
    > - 快速计算：
    >
    >   - 当选择波特率为9600bps的时候，发送一个bit耗时约为104微秒，发送一个字节需要1ms；
    >   - 当选择115200bps时候，1ms能发送12个字节；
    >
  - 例程：

    - uart_init

      ```c
      /******************************************************************************/
      // 函数名称：UART_Init 
      // 输入参数：无
      // 输出参数：无 
      // 函数功能：串口初始化（波特率1200，8-N-1）
      /******************************************************************************/
      void UART_Init(void){
      	SM0 = 0;
      	SM1 = 1;     //确定串口工作方式8位UART，波特率可变
      	PCON &= 0x7f;//设置SMOD
      	TMOD &= 0x0f;//清除TMOD高四位
      	TMOD |= 0x20;//定时器1工作模式8位自动重装
      	TL1 = 0xfd;//0xe8;  //设置TL1的初始值
      	TH1 = 0xfd;//0xe8;  //重装值存储在TH1
      	TR1 = 1;
      	REN = 1;
      	ES = 1;
      	EA = 1;
      }
      ```

      初始化完成串口模式和定时器模式、初值的设定，初值决定了上拉或者下拉一个字节的时间，影响到波特率。所以在开发中不允许使用模拟计时。

## 奇偶校验

UART会设置校验位(最后一位)，用一个值确保传输数据有偶数个或者奇数个逻辑高位。

- 例如：

  数据是011，对于偶校验，校验位为0；对于奇校验，校验位则为1；

  - 就是说，偶校验的时候，要凑出偶数个逻辑高，奇校验的时候要凑出奇数个逻辑高；
- 如何使用奇偶校验发现错误？

  - 通信双方定义数据格式带有校验位，发送方会对数据做一次奇偶校验，填充在校验位；当接收方拿到数据之后也对数据做一次奇偶校验，和发送发的校验位进行对比，如果奇偶校验位对不上就抛出异常；
  - 不能保证绝对可以发现错误；
