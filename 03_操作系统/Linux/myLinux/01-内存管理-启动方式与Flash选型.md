# 芯片启动方式与Flash选型

- 为什么NorFlash可以支持XIP，而Nand Flash不支持？
- XIP启动方式，程序存在外部NorFlash，芯片与Flash通过SPI/QSPI通讯读取指令
  - 所以，这个SPI/QSPI在进程运行期间要一直执行通讯？
  - 如果不是，什么时候停止了通讯？
- 两颗pin2pin的同一个厂商的Flash，软件/硬件还需要从哪些角度来评估是否可以直接平替？

## 一、XIP启动与BootRom启动

了解这个问题前，首先需要回归本质，“MCU执行代码”的含义是什么？

“MCU执行代码”是指MCU执行存储在其内存中的代码指令的过程。

MCU 从高速指令缓存中取出指令，对其进行解码，然后执行指令指定的操作。MCU一条一条地读取指令，执行指令指定的操作，然后继续执行下一条指令，直到程序结束。

那这和启动方式有啥关系？

不同的启动方式，决定了高速指令缓存里面的指令从哪里来。

### 1、XIP启动

- XIP启动方式：把指令直接从NorFlash拷贝到芯片的高速指令缓存；

  - 从软件的角度来说，XIP是在外部闪存直接执行代码，就像芯片在内部flash 的地址0x0800 0000直接执行一样，称为“就地执行”。

- 软件技术实现

  - 用户应用程序代码应链接到目标执行存储器地址（外部QSPI或FMC-NOR闪存）

    - 用户应用程序代码编译的时候链接地址要改成外部闪存的地址，如STM32 H7系统给QSPI Flash在系统总线分配的地址是0x9000 0000，那么代码的地址就要改成0x9000 0000。

  - 例程：该代码是运行在RT-Thread操作系统上的嵌入式系统的启动代码。此代码负责初始化系统，包括 LED0 引脚、闪存和系统缓存。

    [github-sourceCode地址](https://github1s.com/jasonleecode/host-mate/blob/5aeadaab076583bd67fae940b2db240f8b0d7b94/projects/art_pi_bootloader/applications/main.c)

    ```C
    #include <rtthread.h>
    #include <rtdevice.h>
    #include <board.h>
    #include <drv_common.h>
    #include "w25qxx.h"
    
    #define DBG_TAG "main"
    #define DBG_LVL DBG_LOG
    #include <rtdbg.h>
    
    /* defined the LED0 pin: PB1 */
    #define LED0_PIN    GET_PIN(I, 8)
    
    #define VECT_TAB_OFFSET      0x00000000UL
    #define APPLICATION_ADDRESS  (uint32_t)0x90000000
    
    typedef void (*pFunction)(void);
    pFunction JumpToApplication;
    
    int main(void)
    {
        /* set LED0 pin mode to output */
        rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);  
        //首先使用该rt_pin_mode功能将 LED0 引脚设置为输出模式。
        
        /* initialize the flash memory */
        W25QXX_Init();
       //使用该函数初始化闪存W25QXX_Init。
    
        /* enable the flash memory to be memory-mapped 内存映射 */
        W25Q_Memory_Mapped_Enable();
    
        /* disable instruction cache 禁止指令缓存*/
        SCB_DisableICache();
    
        /* disable data cache 禁止数据缓存 */
        SCB_DisableDCache();
        
        //使用 SCB_DisableICache 和 SCB_DisableDCache 函数禁用指令和数据缓存。 这样做是因为应用程序代码直接从内存映射的闪存中执行，而不是先复制到系统 RAM 中。
    
        /* disable the SysTick timer */
        SysTick->CTRL = 0;
        //通过将寄存器设置为 0 来禁用 SysTick 计时器。SysTick->CTRL这样做是为了防止计时器在系统启动时生成中断请求。
        
    
        /* set the jump-to-application function to the address of the application's reset handler */
        JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
        //然后将 JumpToApplication 函数设置为应用程序重置处理程序的地址，该地址存储在 APPLICATION_ADDRESS + 4。
        
        
        /* set the main stack pointer (MSP) to the address of the application's initial stack pointer */
        __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);
        //使用 __set_MSP 函数将主堆栈指针 (MSP) 设置为应用程序初始堆栈指针的地址。
        
    
        /* jump to the application */
        JumpToApplication();
        //最后，系统使用 JumpToApplication 函数跳转到应用程序代码。 main函数末尾的return RT_EOK语句用于表示main函数已成功完成并返回RT_EOK错误代码。 在这种情况下，它没有被使用，因为系统已经跳转到应用程序代码并且不再执行启动代码。
    
        return RT_EOK;
    }
    
    ```

    总体而言，此启动代码为运行在 RT-Thread 操作系统上的嵌入式系统提供了基本设置。 它初始化系统，设置闪存，禁用缓存，禁用 SysTick 定时器，并跳转到应用程序代码。 该代码使用 rtthread.h、rtdevice.h 和 rtdbg.h 头文件访问 RT-Thread 库中的函数和宏，以及用于特定板和设备的 board.h、drv_common.h 和 w25qxx.h 头文件 驱动程序定义。

    

### 2、BootRom启动

- 从 ROM 引导：这是一种传统方法，其中程序指令存储在只读存储器 (ROM) 芯片中，然后复制到 RAM 中执行。这种方法需要 ROM 和 RAM，但具有只需更换 ROM 芯片即可更新固件的优点。

- 从SD卡启动：这种方法将固件存储在SD卡上，然后将其复制到RAM中执行。这种方法常用于需要大量固件存储的系统，具有只需更换 SD 卡即可更新固件的优点。

  - 华邦也有SD Nand，把SD卡做成板载的形式，传输性能更好，如果MPU处理音视频性能可以得到释放；当然价格也比同容量的Nand更贵.

- 从NAND/NOR闪存启动：这种方法将程序指令存储在NAND或NOR闪存芯片中，并将其复制到RAM中执行。这种方法类似于从 ROM 引导，但具有能够在不更换芯片的情况下在系统内更新固件的优点。

  - Nand Flash启动例程：

    ```C
    #include "stm32f4xx_hal.h"
    
    #define NAND_FLASH_ADDRESS 0x70000000
    
    void NAND_Start(void)
    {
      /* Disable I cache and D cache */
      SCB_DisableICache();
      SCB_DisableDCache();
      
      /* Initialize NAND flash */
      HAL_NAND_Init();
    
      /* Get the address of the boot loader from NAND flash */
      pFunction JumpToApplication = (pFunction)(*(__IO uint32_t *)(NAND_FLASH_ADDRESS + 4));
    
      /* Set the main stack pointer to the address in the vector table of the application */
      __set_MSP(*(__IO uint32_t *)NAND_FLASH_ADDRESS);
    
      /* Jump to the application */
      JumpToApplication();
    }
    
    
    ```

    下面简单解释一下代码：

    - 首先禁用I cache和D cache，保证NAND flash数据被正确读取。

    - `HAL_NAND_Init`然后，使用STM32Cube HAL 中的函数初始化 NAND 闪存。

    - 接下来，引导加载程序的地址从 NAND 闪存的地址处获取`NAND_FLASH_ADDRESS + 4`。该地址应该是引导加载程序向量表的第一个字，其中包含初始堆栈指针 (MSP) 和复位处理程序地址。

    - 然后将主堆栈指针 (MSP) 设置为应用程序向量表的地址。

    - 最后，微控制器通过调用`JumpToApplication`函数跳转到应用程序。

      - 为什么要禁用I-Cache 和D-Cache?

        I-Cache 和 D-Cache 被禁用，以确保处理器可以直接从 NAND 闪存中读取数据和指令。启用缓存后，处理器将在访问 NAND 闪存之前尝试从中读取数据，这可能会导致操作不正确或数据损坏。禁用缓存可确保处理器直接访问 NAND 闪存，并有助于防止因访问可能已过时或与 NAND 闪存中存储的实际数据不一致的缓存数据而导致的任何潜在问题。

      - 解释`__set_MSP`

        __set_MSP 宏是 ARM Cortex-M 处理器系列（包括 STM32 系列微控制器）提供的标准 C 库函数。 用于设置主栈指针（MSP）的值，即主栈在内存中的栈顶地址。 主栈是处理器在程序执行过程中用来存放函数调用参数、返回地址、局部变量等临时数据的地方。

        MSP 是 Cortex-M 处理器架构的重要组成部分，因为它决定处理器在调用函数时从何处开始为新堆栈帧分配内存，以及在函数返回时在何处查找返回地址。

        在此代码中，__set_MSP 函数用于使用应用向量表的起始地址初始化 MSP，该向量表存储在闪存中的 APPLICATION_ADDRESS。 这样做的目的是确保处理器在执行应用程序代码时使用正确的堆栈和向量表，这些代码存储在闪存中的 APPLICATION_ADDRESS。

  ![image-20230214135035827](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230214135035827.png)



-  NorFlash和Nand Flash对XIP的支持差异？

  NorFlash是一种非易失性存储器，支持随机存取，直接从闪存中执行指令。这使其适用于直接从闪存执行指令的 XIP 模式。

  另一方面，NAND闪存是一种针对高密度存储进行了优化的存储器，通常用于存储大量数据，但并未针对直接执行指令进行优化。在NAND flash中，指令必须先拷贝到RAM中才能执行，这使得它不太适合XIP模式。

  

## 二、XIP启动与SPI通讯

- 使用XIP启动方式，SPI通讯在程序启动之后还要一直保持吗？

  

  是的，在系统设计中，建议对启动Flash采用专用通道，不允许复用。

  这是因为当高速指令缓存（I-Cache）和高速数据缓存（D-Cache）不够大，有可能无法存放当前程序中所有的指令和数据。因此，芯片必须在程序执行期间不断地从 NorFlash 读取数据，这会降低系统的整体性能。但是，这种方法无需将指令和数据从NorFlash 复制到RAM 中，从而节省了内存空间并缩短了系统启动时间。

  如果程序很小，那么程序执行完一个周期之后，所有的指令都被拷贝到了MCU的高速缓存内，往后程序运行的时候将不再从Nor Flash中读取指令和数据，也就释放了对SPI接口的占用。

  - 在 XIP 引导模式下使用 NAND Flash 有一些缺点，例如：
    1. 速度较慢：与 NOR 闪存相比，NAND 闪存的读取速度较慢，这会对 XIP 模式下的引导性能产生负面影响。
    2. 更复杂的接口：NAND Flash 需要更复杂的接口，例如 ECC 和纠错，这会增加引导过程的开销并增加实现 XIP 引导模式的难度。
    3. 存储空间有限：与 NOR 闪存相比，NAND 闪存的存储容量有限，这可能导致难以存储更大的应用程序或固件映像。
    4. 更多可靠性问题：与 NOR Flash 相比，NAND Flash 发生位错误和磨损的可能性更高，这在 XIP 启动模式下使用时会导致可靠性问题。
    5. 更高的功耗：与 NOR 闪存相比，NAND 闪存通常消耗更多功率，这可能是电池供电系统的一个问题。

## 三、Flash平替问题

- 系统采用NorFlash以及XIP启动方式，评估MX25L51245GZ2I-08G在某个MPU使用中能否直接替换掉MX66L51235FZ2I-10G？
  - Flash size：替换Flash的大小必须等于或大于原始Flash。
  - Flash接口和协议：替换Flash必须使用与原始Flash相同的接口和协议，例如 SPI、QSPI 或 NOR Flash。
  - 封装和管脚：替换Flash必须与原始闪存具有相同的引出线。
  - 电压要求：更换的Flash必须能够在 MPU 的电压要求范围内运行。
  - 时序要求：替换闪存必须满足与原始闪存相同的时序要求。
  - 可靠性和耐用性：替换闪存必须具有与原始闪存相似的可靠性和耐用性。
  - 制造商数据表：根据制造商数据表，替换Flash必须受 MPU 支持并与 XIP 模式兼容。
- 一般来说，MPU厂商会提供Flash支持列表，但是经常出现pin2pin的新型号不在支持列表上，除了评估常规的封装/管脚/电压，驱动兼容的问题建议直接向MPU厂商发起咨询。

## Flash选型参考维度

- NorFlash和NandFlash在项目评估中如何选择,有哪些维度？

  - 首先，最好是Soc厂商支持列表上的芯片，然后参考以下角度。

  - NorFlash 的优势：

    - 读取速度快：NorFlash 具有快速的随机读取速度，使其适用于 XIP（就地执行）启动模式，在这种模式下，程序直接从闪存中执行。

    - 易于使用：NorFlash 易于与微控制器接口，并具有简单的命令集，使其易于集成到系统中。

    - 高可靠性：NorFlash具有高度的可靠性和稳定性，适合在关键系统中使用。

    - 低功耗：NorFlash 功耗极低，适合用于电池供电设备。

  - NorFlash 的缺点：

    - 写入速度慢：NorFlash 写入速度慢，不适合需要频繁写入的应用程序。

    - 昂贵：与 Nand Flash 相比，NorFlash 更昂贵。

  - Nand闪存的优点：
    - 大容量：Nand Flash存储容量大，适合存储大量数据。
    - 低成本：与 NorFlash 相比，Nand Flash 更便宜，因此对于存储容量很重要的应用来说更具成本效益。
    - 写入速度快：Nand Flash写入速度快，适合需要频繁写入的应用。

  - Nand Flash的缺点：

    - 接口复杂：Nand Flash接口和命令集复杂，难以与微控制器接口。
    - 可靠性有限：Nand 闪存的可靠性和稳定性水平有限，因此不适合在关键系统中使用。
    - 需要额外的组件：Nand 闪存需要额外的组件，例如闪存控制器，以与微控制器接口，增加了系统的成本和复杂性。
    - 读取速度慢：Nand Flash 随机读取速度慢，不适合 XIP 启动模式。

    主要还是看，没有资源冲突不影响性能的情况下，哪个便宜用哪个。
