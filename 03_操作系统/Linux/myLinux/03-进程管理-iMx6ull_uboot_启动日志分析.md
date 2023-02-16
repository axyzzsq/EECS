# iMx6ull u-boot启动日志分析

## 1、Raw log

![image-20230216235805479](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230216235805479.png)

```shell
U-Boot 2017.03-g8ba4c5bb19 (Jul 03 2022 - 05:34:19 -0400) # 此行显示 U-Boot 的版本号，以及构建日期和时间。

CPU:   Freescale i.MX6ULL rev1.1 696 MHz (running at 396 MHz) # 此行标识系统中的 CPU 及其时钟速度。
CPU:   Industrial temperature grade (-40C to 105C) at 26C     # 此行表示 CPU 的额定温度范围和当前温度。
Reset cause: POR # 此行显示上次系统复位的原因，在本例中为上电复位。
Model: Freescale i.MX6 ULL 14x14 EVK Board  # 此行标识电路板型号。
Board: MX6ULL 14x14 EVK  # 此行显示电路板名称和尺寸
DRAM:  512 MiB  # 此行表示系统中 DRAM（动态随机存取存储器）的数量。
MMC:   FSL_SDHC: 0, FSL_SDHC: 1 # 此行显示系统中可用的 MMC（多媒体卡）设备。
Display: TFT7016 (1024x600)     # 此行标识连接到系统的显示器类型及其分辨率。
Video: 1024x600x24 # 此行显示视频输出的颜色深度。
In:    serial      # 此行表示引导加载程序的输入设备，在本例中为串行端口。
Out:   serial      # 这一行表示引导加载程序的输出设备，在本例中也是一个串行端口。
Err:   serial      # 这一行表示bootloader的错误输出设备，也是串口。
Net:   No ethernet found.  # 此行表示系统中未检测到以太网接口。
Normal Boot # 这一行表示系统正常启动。
Hit any key to stop autoboot:  0 # 此行表示引导加载程序将继续执行默认引导过程，而无需等待用户输入。
## Error: "findtee" not defined  # 此行表示引导加载程序出错，特别是未定义名为“findtee”的命令。
switch to partitions #0, OK      # 此行表明引导加载程序已毫无问题地切换到 MMC 设备上的分区 0。
mmc1(part 0) is current device  # 这一行表明当前的MMC设备是第二个MMC设备上的partition 0。
switch to partitions #0, OK  
mmc1(part 0) is current device
** Unrecognized filesystem type **         # 此行表示引导加载程序无法识别引导设备上的文件系统。
8846320 bytes read in 488 ms (17.3 MiB/s)  # 这一行表明引导加载程序在 488 毫秒内成功地从引导设备读取了 8,846,320 字节，传输速率为 17.3 MiB/s。
Booting from mmc ...  # 此行表示引导加载程序正在从 MMC 设备引导。
38598 bytes read in 387 ms (96.7 KiB/s)  # 这一行显示引导加载程序在 387 毫秒内成功地从引导设备读取了 38,598 字节，传输速率为 96.7 KiB/s。
Kernel image @ 0x80800000 [ 0x000000 - 0x86fbf0 ]  # 这一行显示内核映像在内存中的位置和大小。
## Flattened Device Tree blob at 83000000  # 此行指示扁平化设备树 (FDT) blob 在内存中的位置。
   Booting using the fdt blob at 0x83000000  # 这一行表明引导加载程序正在使用内存地址 0x83000000 处的 FDT blob。
   Using Device Tree in place at 83000000, end 8300c6c5 # 这一行表示系统正在使用的Device Tree的起始和结束内存地址。
Modify /soc/aips-bus@02200000/epdc@0228c000:status disabled #此行表明引导加载程序已禁用系统中的设备（嵌入式显示控制器）。
ft_system_setup for mx6 # 这一行表示引导加载程序已经完成了系统设置，现在正在将控制权交给操作系统。
```

此日志显示 U-Boot 引导加载程序识别嵌入式系统的 CPU、主板和内存配置。引导加载程序检测可用的 MMC 设备，尝试从它们引导，并在引导设备上遇到文件系统错误。引导加载程序然后将内核映像和 FDT blob 读取到内存中，修改系统中的设备，并将控制权移交给操作系统。

具体如下：

1. 第一行显示 U-Boot 版本和构建时间戳。
2. 显示 CPU 和主板信息，包括时钟速度、工作温度范围和主板型号。
3. 显示上次复位的原因（在这种情况下为上电复位）。
4. 显示系统上的 DRAM 容量。
5. 列出系统检测到的 MMC 设备（存储卡）。
6. 显示和视频分辨率已设置。
7. 控制台输入和输出设备设置为串行。
8. 未找到网络接口（未检测到以太网设备）。
9. U-Boot 尝试正常启动，但用户可以选择按任意键停止启动过程。
10. U-Boot 尝试寻找可信执行环境 (TEE)，但未能成功，因为未定义“findtee”命令。
11. MMC 设备被选为引导设备，U-Boot 尝试从中读取。
12. 引导加载程序遇到引导设备上的文件系统错误。
13. U-Boot 成功将内核映像和 FDT blob 加载到内存中。
14. 引导加载程序禁用系统中的嵌入式显示控制器设备。
15. U-Boot 完成系统设置并将控制权交给操作系统。

总之，U-Boot 执行一系列初始化和配置步骤来设置系统硬件和选择引导设备，尝试从引导设备读取内核映像和 FDT blob，最后将控制权交给操作系统完成引导过程。