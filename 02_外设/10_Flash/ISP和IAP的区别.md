# ISP和IAP两种烧录方式

## 1、ISP

ISP就是in-system programming（在系统烧录，在系统编程）。以前要烧录代码bin文件到单片机内部的flash中是需要借助专用的烧录器的，比较麻烦。后来我们就需要一种不用烧录器也不用把单片机从板子上卸下来的烧录方法，这种方法就是ISP。一般ISP都是PC机通过串口把bin/hex文件直接isp到单片机内部flash中，比如利用J_LINK或者USB转TTL进行烧录。

## 2、IAP

IAP就是in-application programming（在应用编程，在应用烧录，在线升级），IAP的核心是用户需要在自己的应用程序中去操作单片机内部flash的控制器的寄存器，实现对内部flash的烧录功能。然后IAP的时候用户程序通过串口（usb、ethernet）来接收PC发送过来的bin/hex文件，然后将之烧录到内部flash中去完成IAP。完成后再次启动后就会从用户存储器区执行，更新就会起效果。一般实现iAP需要人为的将用户存储器区分为2部分：bootloader+app，很多嵌入式产品的后续升级采用这种方式。
