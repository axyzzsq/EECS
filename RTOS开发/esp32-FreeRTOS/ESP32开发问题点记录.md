# 问题点记录

## 1、编译位置

idf.py的路径如果错了，会报CMake Error at CMakeLists错误，报错的时候先看下编译路径是否正确。

## 2、websocket_wss获取服务器证书的命令

```shell
openssl s_client -showcerts -connect test.aerogarden.com:8443 </dev/null
```

Note:如果命令运行失败，首先检查是否在复制的过程中引入了中文空格符

## 3、添加自己的component

[参考原文路径](https://blog.csdn.net/qq_42856778/article/details/120276638)

> 前提条件：
>
> > 一、hello_world整一个工程都拷贝出来，然后编译执行一下（过程看乐鑫官网）
> > 二、在工程目录建立一个目录（components）也就是和main目录同一个目录
> > 三、在components目录下面建立各个子项目录
> > 整体目录结构如下：
> >
> >    ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193420.png)
> >
> > > ### (1) 调用库
> > >
> > > 首先要想使用到系统里面的默认提供的工具库函数（json / spi_flash /LWIP ）等待API
> > >
> > > CmakeLists.txt
> > >
> > > ```
> > > /* test.c 是你自己实现的代码文件 */
> > > /* 第一种连接外部API头文件方法   */
> > > idf_component_register(SRCS "test.c"
> > >                     INCLUDE_DIRS "$ENV{IDF_PATH}/components/lwip/include"
> > >                     INCLUDE_DIRS "$ENV{IDF_PATH}/components/spi_flash/include"
> > >                     INCLUDE_DIRS "$ENV{IDF_PATH}/components/nvs_flash/include"
> > >                     INCLUDE_DIRS ".")
> > > 
> > > /* 第二种连接外部API头文件方法 */
> > > idf_component_register(SRCS "test.c"
> > >                     	REQUIRES lwip
> > >                     	REQUIRES json
> > >                     	REQUIRES mqtt
> > >                     	REQUIRES spi_flash
> > >                     	REQUIRES nvs_flash
> > >                     	INCLUDE_DIRS ".")
> > > ```
> > >
> > > component.mk
> > >
> > > ```
> > > /*这个文件基本都是这个配置*/
> > > COMPONENT_ADD_INCLUDEDIRS := .
> > > ```
> > >
> > > ### (2) 连接自己各个目录的代码.
> > >
> > >   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193420.png)
> > >
> > > 自身实现的代码进行跨目录调用：
> > > CmakeLists.txt
> > >
> > > ```
> > > /* REQUIRES 的对象是你 对应的目录名字 */
> > > idf_component_register(SRCS "key.c"
> > >                     	REQUIRES gpio
> > >                     	REQUIRES led
> > >                     	REQUIRES lcd
> > >                     	INCLUDE_DIRS ".")
> > > ```
> > >
> > > component.mk
> > >
> > > ```
> > > /*这个文件基本都是这个配置*/
> > > COMPONENT_ADD_INCLUDEDIRS := .
> > > ```
> > >
> > > 在main目录下面的hello_world_main.c文件进行调用头文件
> > >
> > > ```C
> > > /*基本用到什么头文件  就添加 */
> > > #include "led.h"
> > > #include "gpio.h"
> > > #include "key.h"
> > > #include "lcd.h"
> > > ```
> > >
> > > CmakeLists.txt
> > >
> > > ```
> > > idf_component_register(SRCS "key.c"
> > >                     	REQUIRES gpio
> > >                     	REQUIRES led
> > >                     	REQUIRES lcd
> > >                     	INCLUDE_DIRS ".")
> > > ```
> > >
> > > component.mk
> > >
> > > ```
> > > COMPONENT_ADD_INCLUDEDIRS := .
> > > ```

## 4、添加esp-idf中的组件

​		Note:esp-idf文件夹中的components默认是被编译的，可以在工程中直接调用，如果出现①已经包含头文件；②程序中有调用；但是编译的时候，仍旧提示找不到定义引用，考虑用menuconfig来开启对应sdkconfig的使能，切忌直接修改components,任何修改库文件的行为都是不可原谅的。

一个没有调用自定义组件的工程结构示例：

```
.
├── build
├── CMakeLists.txt
├── example_test.py
├── main
│   ├── build
│   ├── CMakeLists.txt
│   ├── inc
│   ├── Kconfig.projbuild
│   ├── src
│   └── test_aerogarden_com.pem
├── old_sdkconfig.old
├── README.md
├── sdkconfig
├── sdkconfig.ci
└── sdkconfig.old

```

工程顶层CMakeLists.txt

```CM
# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# (Not part of the boilerplate)
# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(aeroGarden_esp)
```

main组件的CMakeLists.txt

```cmake
idf_component_register(SRCS "./src/main.c"
                            "./src/mainwindow/aeroSerial/serialMain.c"
                            "./src/mainwindow/aeroWebsocket/websocketMain.c"
                            "./src/mainwindow/aeroSerial/serialQueueRecv.c"
                            "./src/protocolLayer/serialParse.c"
                            "./src/mainwindow/aeroSerial/serialDataHandle.c"
                        INCLUDE_DIRS "./inc/mainwindow/"
                        INCLUDE_DIRS "./inc/mainwindow/aeroSerial/"
                        INCLUDE_DIRS "./inc/mainwindow/aeroWebsocket/"
                        INCLUDE_DIRS "./inc/protocolLayer/"
                        EMBED_TXTFILES test_aerogarden_com.pem)
```

## 5、esp32中断回调函数的修饰问题

```C
/*********************************************************************
 @fn        uartIntrCallBack

 @brief     uart interrupt call back function 

 @param	   event: event type
 		   user_data: user data
 **********************************************************************/
static void IRAM_ATTR uartIntrCallBack(void *param)
{
	ESP_LOGI(TAG,"触发串口中断");
	switch(event.type) 
	{
		case UART_DATA:
            ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
			stUart1Com.unRcvlen = event.size;
			stUart1Com.bRcv = true;
			break;
		case UART_DATA_BREAK:
			ESP_LOGI(TAG, "Ready to write data to uart");
			stUart1Com.bSnd = true;
			break;
		default:
			ESP_LOGI(TAG, "uart evet default case,break");
			break;
	}
}

	ESP_ERROR_CHECK(uart_isr_register(ECHO_UART_PORT_NUM, uartIntrCallBack, NULL,  ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, &handle )); //回调函数

```

需要用`IRAM_ATTR`来修饰回调函数，否则在程序运行过程中会报参数错误问题如下所示

```
ESP_ERR_INVALID_ARG (0x102): Invalid argument
```

