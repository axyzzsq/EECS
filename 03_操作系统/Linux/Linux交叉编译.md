# Linux交叉编译一般性过程

以NXP公司iMx6ull芯片、RockChip RV1126芯片、SigmaStar SSD222芯片Bootloader、Kernel、Rootfs三大组件交叉编译过程为例，归纳Linux交叉编译的一般性过程。 

## 1.编译工具链配置

### ① NXP-iMx6ull

- 在编译脚本加入:

	```shell
	export ARCH=arm
	export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
	export PATH=$PATH:/home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin
	```

- 解析：

	>  1、配置过程：
	>
	> - 指定arm架构
	> - 指定编译工具链名称
	> - 指定编译工具链路径
	>
	> 2、export: Linux export 命令用于设置或显示环境变量。在 shell 中执行程序时，shell 会提供一组环境变量。export 可新增，修改或删除环境变量，供后续执行的程序使用。**export 的效力仅限于该次登陆操作。** 所以可以在同一台Linux主机上通过在脚本中执行export配置当此所需的编译工具链，只在当此执行有效，而不影响其他SDK的编译。
	>
	> 3、PATH: PAHT环境变量定义了用于进行命令和程序查找的目录；echo $PATH,不同的目录之间以冒号分隔
	>
	>  ![image-20220804140720509](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220804140720509.png)

	

### ② RockChip-RV1126

- 

### ③ SigmaStar-SSD222



### ★ 归纳





## 2. Bootloader编译

### ① NXP-iMx6ull

### ② RockChip-RV1126

### ③ SigmaStar-SSD222

### ★ 归纳





## 3. Kernel编译

### ① NXP-iMx6ull

### ② RockChip-RV1126

### ③ SigmaStar-SSD222

### ★ 归纳





## 4. Rootfs编译

### ① NXP-iMx6ull

### ② RockChip-RV1126

### ③ SigmaStar-SSD222

### ★ 归纳





## 6. 编译脚本集成

### ① NXP-iMx6ull

### ② RockChip-RV1126

### ③ SigmaStar-SSD222

### ★ 归纳



## 6. 可执行档文件属性

### ① NXP-iMx6ull

### ② RockChip-RV1126

### ③ SigmaStar-SSD222

### ★ 归纳



## 7.整体性归纳

