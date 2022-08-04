# SigmaStar-SSD222

### (1)编译工程

```shell
/project$   make image  # 连同kernel一起编译
/project$   make image-fast # 不编译kernel，只编译工程文件
/project$   ./make_usb_factory_sigmastar.sh # 打包烧录文件
```

### (2) 烧录

每次烧录都需要进入uboot模式执行指令

```shell
 setenv ota_upgrade_status 1
 saveenv
 reset
```

### （3）Kernel menuconfig

- step1:进入kernel/目录,执行 build_elsa_lcd.sh脚本中编译kernel部分

	```shell
	declare -x ARCH="arm"
	declare -x CROSS_COMPILE="arm-linux-gnueabihf-"
	make pioneer3_ssc020a_s01a_spinand_demo_camera_defconfig
	```

- step2:

	```shell
	make menuconfig
	```

- step3:修改menuconfig中的配置项，保存后退出

- step4:对比kernel目录下的.config文件把修改项配置到kernel\arch\arm\configs\pioneer3_ssc020a_s01a_spinand_demo_camera_defconfig中去

 ![image-20220521110919174](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220521110919174.png)

- step5:在/project目录下执行编译指令

	```shell
	make image -j15
	```

	

