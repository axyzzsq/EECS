

# 瑞芯微-RV1126芯片开发指令

## 挂载

```shell
mount -t nfs -o nolock 192.168.31.177:/home/book/nfs_rootfs /mnt/nfs
mount -t nfs -o nolock 192.168.1.106:/home/book/nfs_rootfs /mnt/nfs
```



## 一、编译

### 1、代码仓管理

初次建仓，代码上推:

```shell
git add -A   # 忽略所有关于子模块的报错,直接上推，git会自动包含子模块的信息，然后上推
```

后续代码下拉:

```shell
git clone
git submodule update --init --recursive   # 同步子模块
```

修改代码子仓，git补丁





### 2、SDK编译环境配置

```shell
#Step1:
sudo apt-get update

#Step2:
sudo apt-get upgrade

#Step3:
sudo apt-get install repo gitk git-gui gcc-arm-linux-gnueabihf u-boot-tools device-tree-compiler gcc-aarch64-linux-gnu mtools parted libudev-dev libusb-1.0-0-dev python-linaro-image-tools linaro-image-tools autoconf autotools-dev libsigsegv2 m4 intltool libdrm-dev curl sed make binutils build-essential gcc g++ bash patch gzip gawk bzip2 perl tar cpio python unzip rsync file bc wget libncurses5 libqt4-dev libglib2.0-dev libgtk2.0-dev libglade2-dev cvs git mercurial rsync openssh-client subversion asciidoc w3m dblatex graphviz python-matplotlib libc6:i386 libssl-dev expect fakeroot cmake flex bison liblz4-tool libtool keychain expect-dev

#Step4:在工程的根目录下执行命令
source envsetup.sh

#Step5:输入“ 85” 来选择 rockchip_rv1126_rv1109_spi_nand

#Step6：在工程的根目录下执行
./build.sh lunch

#Step7: 这时会出现很多选项， 输入“2” 来选择BoardConfig-38x38-spinand.mk

```

Note:

- Step1~Step3，只需要安装一次即可；
- Step4~Step7，每在一个新终端上编译都需要执行一次。  

### 3、总体编译

在工程根目录下执行命令

```shell
./build.sh
```

### 4、分步编译

#### (1) U-boot 编译

① U-oot配置说明

使用 menuconfig 配置 U-Boot， 选择需要的模块， 最后保存退出。 
rv1126_defconfig 文件在目录 u-boot/configs 
命令格式： make "RK_UBOOT_DEFCONFIG"_defconfig 
RK_UBOOT_DEFCONFIG 定义在./build.sh 选择的 BoardConfig*.mk  

```shell
cd u-boot
make rv1126_defconfig
make menuconfig
#保存配置到对应的文件 rv1126_defconfig
make savedefconfig
cp defconfig configs/rv1126_defconfig
```

② U-Boot编译

到压缩包根目录执行  

```shell
./build.sh uboot
```

#### (2) Kernel 编译

① Kernel 配置说明  

在工程根目录下执行：

```shell
./build.sh device/rockchip/rv1126_rv1109/BoardConfig-38x38-spi-nand.mk
cd kernel
make ARCH=arm rv1126_defconfig
make ARCH=arm menuconfig #执行完之后进行menuconfig配置
make ARCH=arm savedefconfig
cp defconfig arch/arm/configs/rv1126_defconfig
```

② Kernel编译

在工程目录下执行：

```shell
./build.sh kernel
```

#### (3) Rootfs 编译

① 首先在SDK根目录查看Board Config对应的rootfs是哪个配置

```shell
./build.sh  -h rootfs
```

 ![image-20220803205529316](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220803205529316.png)

② 然后执行反馈的配置指令

```shell
source envsetup.sh rockchip_rv1126_rv1109_spi_nand
```

④ 执行配置表

```shell
make menuconfig 
```

⑤ 保存到rootfs配置文件

```shell
make savedefconfig
```

⑥ Rootfs编译

```shell
./build.sh  rootfs
```



#### Trouble Shooting

- 报错：

![image-20230326154131172](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230326154131172.png)

- 解决办法： 执行

  ```shell
  export  LD_LIBRARY_PATH=
  ```

这条指令的作用是将环境变量`LD_LIBRARY_PATH`设置为空，即清空该环境变量的值。这个环境变量是用来指定动态链接库的搜索路径的，当我们运行一个程序时，系统会根据该环境变量的值去搜索动态链接库。如果该环境变量的值为空，则系统会使用默认的搜索路径去查找动态链接库。然后重新执行`build.sh`。

## 三、应用

### 1、串口设置

波特率设置为 1500000



### 2、设置板端ip

方式1: 临时设置：

```shell
ifconfig eth0 设置网段
```

方式2: 自动分配——编辑/etc/init.d/rcS,添加：

```shell
udhcpc -i eth0
```

### 3、NFS挂载

```shell
mount -t nfs -o nolock,vers=3 192.168.123.39:/home/book/nfs_rootfs /mnt
```





## Trouble Shooting

### 1、进入烧录模式

设备烧录需要进入MASKROM，但是插上USB之后，上位机发现的是ADB工具。

![image-20221216205426351](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221216205426351.png)

处理方法：

点击切换；会切到LOADER模式,再进入高级功能，点击切换进入MASKROM。

