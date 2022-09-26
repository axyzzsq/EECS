# Linux板端文件系统与文件

通过RockChip RV1126与SigmaStar SSD222板端文件系统的交叉项归纳嵌入式Linux的常用文件。

- 按照以下几个方面展开对文件系统的复习:

	- 大家对这个文件的定义是什么?


	- 在什么场合下开发者会用到这个文件，配置什么内容？


	- 这个文件里面是什么？执行了哪些命令或者系统调用？清楚了内容和调用关系之后对这个文件的普遍定义是否有疑问？是否可以更加通俗/具体地描述？


	- 这个文件是被哪个文件调用，在系统的启动、初始化中处于什么环节?

## 1、/etc目录

这个目录用来存放系统管理所需要的配置文件和子目录。

目录对比：

![image-20220806153501308](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220806153501308.png)

### （1）/etc/init.d目录

RockChip RV1126板端/init.d中的文件如下图

 ![image-20220806141357693](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220806141357693.png)

SigmaStar SSD222板端/init.d的文件如下图所示：

 ![image-20220806143554054](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220806143554054.png)

**共同项只有一个：rcS脚本,所以这个文件在系统中是一个必要文件，不可裁剪**

- rcS文件

	> rcS是一个脚本文件，在inittab文件中本解析调用，用于配置Linux系统。**所以，可以在这个文件配置开机启动项。**
	>
	> 在RockChip RV1126和SigmaStar SSD222的开发板端，/etc/inittab脚本中执行
	>
	> ```shell
	> ::sysinit:/etc/init.d/rcS
	> ```
	>
	> 调用rcS脚本。
	>
	> 用法：在rcS脚本中执行指令，在开机的时候完成客制化配置
	>
	> - 例子
	>
	> 	- RV1126板端 在rcS末行加入：`udhcpc -i eth0`，配置开机进行dhcp自动化分配ip
	>
	> 	- SSD222板端，在rcS中执行
	>
	> 		```shell
	> 		if [ -e /customer/demo.sh ]; then
	> 		    /customer/demo.sh
	> 		fi;
	> 		```
	>
	> 		文件`/customer/demo.sh`如果存在，在开机的时候将被执行；




### （2）/etc/profile文件

SigmStar SSD222板端 /etc/profile文件：

```shell
#!/bin/sh
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
export LD_LIBRARY_PATH=/lib
mkdir -p /dev/pts
ulimit -c unlimited
export PATH=$PATH:/config
export TERMINFO=/config/terminfo
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/config/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/customer/ssh/lib
```

RochChip RV1126板端 /etc/profile文件：

```shell
export PATH=/bin:/sbin:/usr/bin:/usr/sbin

if [ "$PS1" ]; then
        if [ "`id -u`" -eq 0 ]; then
                export PS1='[\u@\h:\w]# '
        else
                export PS1='[\u@\h:\w]$ '
        fi
fi

export PAGER='/bin/more '
export EDITOR='/bin/vi'

# Source configuration files from /etc/profile.d
for i in /etc/profile.d/*.sh ; do
        if [ -r "$i" ]; then
                . $i
        fi
        unset i
done
```

- 共同点：

	- 通过export命令配置针对当次启动的环境变量，所以profile文件的作用是设置全局的环境变量，改完之后对所有的用户都会起作用。

	- ```shell
		export PATH=/bin:/sbin
		```

		预设可执行文件或命令的搜索路径,各个文件夹之间通过冒号分隔。如果shell指令在这些目录下找不到可执行文件则报命令未找到的错误。

- 不同点：

	- RockChip RV1126板端的profile文件中

		```shell
		for i in /etc/profile.d/*.sh ; do
		        if [ -r "$i" ]; then
		                . $i
		        fi
		        unset i
		done
		```

		对profile.d文件夹中的脚本全部执行。

		![image-20220806151957808](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220806151957808.png)

		

	- SigmaStar的/etc/目录下没有profile.d目录，所以profile.d不是一个必要项。

- 问题点: /etc/profile.d的存在有什么意义？

	> - /etc/profile文件设置的变量对所有用户都生效，/etc/profile.d可以设置针对不同用户的特定环境变量，profile文件调用profile.d文件，会对环境变量重新定义，实现环境变量的私人定制；
	> - /etc/profile.d对系统来说解耦强度高，封装性好，不需要的时候直接把某个profile.d文件删除，不会影响系统的运行；



### （3）/etc/inittab文件

/etc/inittab文件的作用是设定Linux的运行等级，Linux常见运行级别如下

- 0：关机模式

- 1：单用户模式

- 2：无网络支持的多用户模式

- 3：字符界面多用户模式

- 4：保留，未使用模式

- 5：图像界面多用户模式

- 6：重新引导系统，重启模式

  

RochChip RV1126板端 /etc/inittab:

```shell
# /etc/inittab
#
# Copyright (C) 2001 Erik Andersen <andersen@codepoet.org>
#
# Note: BusyBox init doesn't support runlevels.  The runlevels field is
# completely ignored by BusyBox init. If you want runlevels, use
# sysvinit.
#
# Format for each entry: <id>:<runlevels>:<action>:<process>
#
# id        == tty to run on, or empty for /dev/console
# runlevels == ignored
# action    == one of sysinit, respawn, askfirst, wait, and once
# process   == program to run

# Startup the system
::sysinit:/bin/mount -t proc proc /proc
::sysinit:/bin/mount -o remount,rw /
::sysinit:/bin/mkdir -p /dev/pts
::sysinit:/bin/mkdir -p /dev/shm
::sysinit:/bin/mount -a 2>/dev/null
::sysinit:/bin/hostname -F /etc/hostname
# now run any rc scripts
::respawn:-/bin/sh
::sysinit:/etc/init.d/rcS

# Put a getty on the serial port
#ttyFIQ0::respawn:/sbin/getty -L  ttyFIQ0 0 vt100 # GENERIC_SERIAL

# Stuff to do for the 3-finger salute
#::ctrlaltdel:/sbin/reboot

# Stuff to do before rebooting
::shutdown:/etc/init.d/rcK
::shutdown:/sbin/swapoff -a
::shutdown:/bin/umount -a -r
```

SigmaStar SSD222板端 /etc/inittab:

```shell
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
```

- 在两段代码中，都缺省了唯一标识符id、runlevels运行等级；

- 第三个参数

## 2、/proc目录

Linux 内核提供了一种通过 /proc 文件系统，在运行时访问内核内部数据结构、改变内核设置的机制。proc文件系统是一个伪文件系统，它只存在内存当中，而不占用外存空间。它以文件系统的方式为访问系统内核数据的操作提供接口。

```shell
/proc/bus  #系统总线(Bus)信息，例如pci/usb/input等
/proc/driver  #驱动信息
/proc/irq  #中断请求设备信息
/proc/net   #网卡设备信息
```

eg：查看iMx6ull开发板端,input系统事件对应的硬件信息：

 ![image-20220926095729971](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220926095729971.png)
