# http OTA

## 一、ESP32 Flash空间分区配置

目前使用的ESP32-WROOM-32集成4MB SPI Flash。在编译esp32程序时，通过make menuconfig -> Partition Table可以有三种分区选择，工厂程序(无OTA分区)/工厂程序(双OTA分区)/用户自定义分区。如下图：

 ![image-20220409144530992](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220409144530992.png)

menuconfig中的配置只是修改配置文件中的宏，实际上ESP32 SDK对应Flash分区配置的源码路径是\esp-idf-v3.0\components\partition_table该路径下有以下.csv文件都是用来对Flash分区进行配置的。

- 无OTA分区：partitions_singleapp.csv、partitions_singleapp_coredump.csv
- 双OTA分区：partitions_two_ota.csv、partitions_two_ota_coredump.csv

## 二、OTA升级策略(Http)

ESP32 连接HTTP服务器，发送请求Get升级固件；每次读取1KB固件数据，写入Flash。

ESP32 SPI Flash 内有与升级相关的（至少）四个分区：

> 1、OTA data区: 决定运行哪个区的App
>
> 2、Factory App区: 有出厂设置时默认的App
>
> 3、OTA_0区: OTA_0 App
>
> 4、OTA_1区: OTA_1App

首次进行OTA升级的时候，OTA demo向OTA_0分区烧录目标App，并在烧录完成后，更新OTA分区数据并重启；

系统重启时获取 OTA data 分区数据进行计算，决定此后加载OTA_0分区的App执行（而不是默认的 Factory App 分区内的App），从而实现升级。

同理，若某次升级后 ESP32 已经在执行 OTA_0 内的App，此时再升级时，OTA Demo 就会向 OTA_1 分区写入目标App。再次启动后，执行 OTA_1 分区实现升级。以此类推，升级的目标App始终在 OTA_0、OTA_1 两个分区之间交互烧录，不会影响到出厂时的 Factory App 固件。

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/6793005-68b34f943cf297fa.png)





[OTA方式教程–1](https://blog.csdn.net/Marchtwentytwo/article/details/117790043?)

[OTA方式教程–2](https://blog.csdn.net/Marchtwentytwo/article/details/119353289?)
