# 89C51模拟I2C

## 例程

```c
#include <reg51.h>
#include <intrins.h>

#define SDA P1_0
#define SCL P1_1

void delay_us(unsigned int us)
{
    while (us--)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void I2C_Start()
{
    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 0;
}

void I2C_Stop()
{
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
    SDA = 1;
}

unsigned char I2C_SendByte(unsigned char dat)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            SDA = 1;
        else
            SDA = 0;

        dat <<= 1;
        delay_us(5);
        SCL = 1;
        delay_us(5);
        SCL = 0;
        delay_us(5);
    }

    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);

    if (SDA == 1)
        return (0);

    else
        return (1);
}

unsigned char I2C_ReadByte()
{
    unsigned char i, dat;

    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        delay_us(5);

        if (SDA == 1)
            dat |= 0x01;

        else
            dat &= ~0x01;

        SCL = 0;
        delay_us(5);
    }

    return (dat);
}

unsigned char I2C_WaitAck()
{
    unsigned char re;

    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);

    if (SDA == 1)
        re = 0;

    else
        re = 1;

    SCL = 0;
    delay_us(5);
    return re;
}

void I2C_Ack()
{
    SDA = 0;
    delay_us(5);
    SCL = 1;
    delay_us(5);
}

void I2C_NoAck()
{
    SDA = 1;
    delay_us(5);
    SCL = 1;
    delay_us(5);
}

void Write_I2C(unsigned char addr, unsigned char dat)
{
   I2C_Start();
   I2C_SendByte(addr<<1|0x00); //写命令
   I2C_WaitAck();
   I2C_SendByte(dat); //写数据
   I2C_WaitAck();
   I2C_Stop();
}

unsigned char Read_I2C(unsigned char addr)
{
   unsigned char dat;

   I2C_Start();
   I2C_SendByte(addr<<1|0x00); //写命令
   I2C_WaitAck();
   I2C_SendByte(addr<<1|0x01); //读命令
   I2C_WaitAck();
   dat=I2C_ReadByte(); //读数据
   I2C_NoAck();
   I2C_Stop();

   return dat;
}
```


[(1) 51单片机软件模拟I2C通信 - 知乎.](https://zhuanlan.zhihu.com/p/90929240) 

[(2) 51单片机模拟I2C程序_AX_liujiucheng的博客-CSDN博客.]( https://blog.csdn.net/Mr_liu_JC/article/details/119428448 )

[(3) 51单片机 普通IO口模拟IIC(I2C)接口通讯的程序代码 - 21ic ....]( https://www.21ic.com/app/mcu/201701/701380.htm)



## ACK和NACK

[I2C中关于ACK和NACK的几点东西](https://www.cnblogs.com/aspirs/p/13088283.html)



## I2C死锁

以下是I2C时序表格，包括空闲、开始、结束、ACK、NACK、死锁等状态下SCL和SDA的电平状态和电平变化。

| 状态 | SCL  |  SDA  |
| :--: | :--: | :---: |
| 空闲 |  高  |  高   |
| 开始 |  高  | 高→低 |
| ACK  |  高  |  低   |
| NACK |  高  |  高   |
| 结束 |  高  | 低→高 |
| 死锁 |  高  |  低   |

- 空闲状态：SCL保持高电平，SDA保持高电平

  - 所以只有当主机检测到SCL和SDA均为高电平的时候才会认为I2C总线available，才会发起通讯。
  - 主机发起I2C通讯之后，SCL才呈现时钟脉冲波形，I2C通讯Stop之后，恢复到空闲拉高模式。(所以读写操作的只写地址不写数据等异常操作不会导致死锁)

- ACK/NACK：

  - 主机读数据：SDA线由从机控制，当第8个时钟的数据发送结束之后，从机会释放SDA的控制权。如果从机是标准硬件I2C,主机需要做出ACK(第9个时钟内拉低SDA)/NACK(第9个时钟内拉高SDA)响应。

  - 主机写数据：SDA线由主机控制，当第8个时钟的数据发送结束之后，主机释放SDA的控制权，从机在第9个时钟掌握SDA控制权，对主机做出ACK(遭得住，继续写)/NACK(遭不住，别写了)响应。

    - ACK信号波形：

      ![image-20230403111935172](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230403111935172.png)

- 死锁:

  - 出现在主机写数据的第9个时钟

    正常情况下，第9个时钟结束之中，SCL拉低，从机释放SDA，SDA翻转为高电平。

    ![image-20230403112449364](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230403112449364.png)

    当第9个时钟进行到中间的时候，复位了。

    ![image-20230403112904632](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230403112904632.png)

    - 此时，从机先做ACK响应，会把SDA线拉低。然后，主机复位了。
    - 主机复位之后保持空闲状态，SCL始终为高电平，而从机还在等待SCL拉低，来释放SDA，让其恢复到高电平状态。
    - 当主机要操作这条I2C总线的时候发现，SDA保持在低电平状态，就认为此时的I2C总线是不可用的，而从机一直在等待SCL被拉低以翻转电平。
    - 就这样，死锁了。。

  - 出现死锁的核心是SCL始终为高，SDA被从机占据始终为低。

    

- 死锁的处理办法：

  - 主机检测SDA占用超时，通过拉低从机reset脚等操作把从机复位掉，释放从机对SDA线的占用。

