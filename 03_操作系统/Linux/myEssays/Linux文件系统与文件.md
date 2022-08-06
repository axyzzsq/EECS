# Linux文件系统和文件

## 1、/etc目录

这个目录用来存放系统管理所需要的配置文件和子目录。

## （1）/etc/init.d

RockChip RV1126板端/init.d中的文件如下图

 ![image-20220806141357693](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220806141357693.png)

- ① rcS文件

	> rcS是一个脚本文件，在inittab文件中本解析调用，用于配置Linux系统。**所以，可以在这个文件配置开机启动项。**
	>
	> 用法：在rcS脚本中执行指令，在开机的时候完成客制化配置
	>
	> - 例子
	>
	> 	- RockChip RV1126板端 在rcS末行加入：`udhcpc -i eth0`，配置开机进行dhcp自动化分配ip
	>
	> 	-  SigmaStar SSD222板端，在rcS中执行
	>
	> 		```shell
	> 		if [ -e /customer/demo.sh ]; then
	> 		    /customer/demo.sh
	> 		fi;
	> 		```
	>
	> 		文件`/customer/demo.sh`如果存在，在开机的时候将执行；

-   

- 

- 









## 1、/proc目录

