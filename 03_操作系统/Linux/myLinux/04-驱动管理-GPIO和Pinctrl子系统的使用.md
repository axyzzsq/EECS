# 基于设备树的GPIO/Pinctrl子系统的使用

[源码位置](https://github1s.com/axyzzsq/LinuxDriver/blob/main/05_gpio_and_pinctrl/01_led/leddrv.c)

 ![gpio子系统.drawio.png](https://github.com/axyzzsq/MyPicture/blob/main/pinctrl/gpio%E5%AD%90%E7%B3%BB%E7%BB%9F.drawio.png?raw=true)

## Step1 使用配置工具生成节点代码

![image-20221110151807766](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221110151807766.png)



## Step2 拷贝

在设备树文件中，找到同名的iomux_snvs，把工具中生成的节点代码拷贝过来

 ![image-20221110151850036](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221110151850036.png)

## Step3 管脚冲突节点失能

 通过`MX6UL_PAD_GPIO1_IO05__GPIO1_IO05 0x000010B0`在设备树中找到复用的节点，把那个节点，在设备树中的根目录下失能掉![image-20221110152016428](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221110152016428.png)

 ![image-20221110152442055](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221110152442055.png)

如果这个动作没有做，将会在编译的时候报错，如下图：

![image-20221110152515823](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221110152515823.png)

## Step4: 重新编译设备树

编译服务器：

```shell
cd Linux-4.9.88
make dtbs
```

板端：

```shell
cp  /mnt/*.dtb   /boot
sync  #同步文件
reboot #重启之后开发板就是使用新的 dtb/
```

插入驱动，测试控制灯效。