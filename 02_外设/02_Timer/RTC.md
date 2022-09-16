# RTC实时时钟

## 一、RTC实时时钟使用说明

"RTC"是Real Time Clock 的简称，意为实时时钟。[stm32](https://www.zhihu.com/search?q=stm32&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"article"%2C"sourceId"%3A"157535721"})提供了一个秒中断源和一个闹钟中断源，修改计数器的值可以重新设置系统当前的时间和日期。
　　RTC模块之所以具有实时时钟功能，是因为它内部维持了一个独立的定时器，通过配置，可以让它准确地每秒钟中断一次。但实际上，RTC就只是一个定时器而已，掉电之后所有信息都会丢失，因此我们需要找一个地方来存储这些信息，于是就找到了备份寄存器。其在掉电后仍然可以通过纽扣电池供电，所以能时刻保存这些数据。
 **配置RTC前须知：**

- BKP：
  　　RTC模块和时钟配置系统的寄存器是在后备区域的（即BKP），通过BKP后备区域来存储RTC配置的数据可以让其在系统复位或待机模式下唤醒后，RTC里面配置的数据维持不变。
- PWR：
  　　PWR为电源的寄存器，我们需要用到的是电源控制寄存器（PWR_CR），通过使能PWR_CR的DBP位来取消后备区域BKP的写保护。
- RTC：
  　　由一组可编程计数器组成，分成两个模块。第一个模块是RTC的预分频模块，它可编程产生最长为1秒的RTC时间基准TR_CLK。RTC的预分频模块包含了一个20位的可编程分频器（RTC）TR_CLK 周期中RTC产生一个中断（秒中断）。第二个模块是一个32位的可编程计数器，可被初始化为当前的系统时间。系统时间按TR_CLK周期累加并与存储在RTC_ALR寄存器中的可编程时间相比较，如果RTC_CR控制寄存器中设置了相应允许位，比较匹配时，将产生一个闹钟中断。

## 二、配置整体过程

-  第一步： 通过设置寄存器 RCC_APB1ENR 的 PWREN 和 BKPEN 位来打开电源和后备接口的时钟

  调用库函数： 

  ```C
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE );
  ```

- 第二步：电源控制寄存器(PWR_CR) 的 DBP 位来使能对后备寄存器和 RTC 的访问

  调用库函数：

  ```C
  PWR_BackupAccessCmd(ENABLE );
  ```

- 第三步：初始化复位 BKP 寄存器 

  调用库函数：

  ```C
  BKP_DeInit ();
  ```

- 第四步：设置 RTCCLK，如下图：

   ![preview](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209152116148.jpeg)

  我们需要将 RTCCLK 设置为 LSE OSC 这个 32.768KHZ 的晶振。

  调用的库函数:

  ```C
  RCC_LSEConfig (RCC_LSE_ON);
  While(!RCC_GetFlagStatus (RCC_FLAG_HSERDY));//设置后需要等待启动  
  ```

- 第五步：将 RTC 输入时钟 选择为 LSE 时钟输入并使能 RTC，等待 RTC 和 APB 时钟同步
  调用库函数：

  ```C
  RCC_RTCCLKConfig (RCC_RTCCLKSource_LSE);//选择 LSE 为 RTC 设备的时钟
  RCC_RTCCLKCmd (ENABLE );//使能
  RTC RTC_WaitForSynchro();//等待同步
  ```

- 第六步：配置 RTC 时钟参数。

  > 查询 RTOFF 位，直到 RTOFF 的值变为’1’
  >
  > 置 CNF 值为 1 ，进入配置模式
  >
  > 对一个或多个 RTC 寄存器进行写操作
  >
  > 清除 CNF 标志位，退出配置模式
  >
  > 查询 RTOFF，直至 RTOFF 位变为’1’ 以确认写操作已经完成。仅当 CNF 标志位被清除时，写操作才能进行，这个过程至少需要 3 个 RTCCLK 周期。

  

## 三、S3C2410的实时时钟RTC

 ![image-20210325115735625](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209152133546.png)

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209152134438.png)