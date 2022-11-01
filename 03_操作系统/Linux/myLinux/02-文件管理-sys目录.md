# /sys目录

## /sys/devices：
这是系统中所有设备存放的目录,也就是系统中的所有设备在 sysfs 中的呈现、表达,也是 sysfs 管理设备的最重要的目录结构。

 ![image-20221101095030220](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101095030220.png)

## /sys/block：
块设备的存放目录,这是一个过时的接口,按照 sysfs 的设计理念,系统所有的设备都存放在/sys/devices 目录下,所以/sys/block 目录下的文件通常是链接到/sys/devices 目录下的文件。

![image-20221101095224907](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101095224907.png)

## /sys/bus：
这是系统中的所有设备按照总线类型分类放置的目录结构,/sys/devices 目录下每一种设备都是挂在某种总线下的,譬如 i2c 设备挂在 I2C 总线下。同样,/sys/bus 目录下的文件通常也是链接到了/sys/devices 目录。

 ![image-20221101095358793](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101095358793.png)

## /sys/class：
这是系统中的所有设备按照其功能分类放置的目录结构,同样该目录下的文件也是链接到了/sys/devices 目录。按照设备的功能划分组织在/sys/class 目录下,譬如/sys/class/leds目录中存放了所有的LED设备,/sys/class/input 目录中存放了所有的输入类设备。

 ![image-20221101100724412](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101100724412.png)

## /sys/dev：
这是按照主次设备号的方式放置的目录结构,同样该目录下的文件也是链接到了/sys/devices 目录。该目录下有很多以主设备号:次设备号(major:minor)命名的文件,这些文件都是链接文件,链接到/sys/devices 目录下对应的设备。

![image-20221101094226329](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101094226329.png)

## /sys/firmware：
描述了内核中的固件。

 ![image-20221101100802297](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221101100802297.png)

## /sys/fs：
用于描述系统中所有文件系统,包括文件系统本身和按文件系统分类存放的已挂载点。

## /sys/kernel：
这里是内核中所有可调参数的位置。

## /sys/module：
这里有系统中所有模块的信息。

## /sys/power：
这里是系统中电源选项,有一些属性可以用于控制整个系统的电源状态。