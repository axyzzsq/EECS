# wc

[wc](http://www.linuxso.com/command/wc.html)[命令](http://www.linuxso.com/command/)的功能为统计指定文件中的字节数、字数、行数, 并将统计结果显示输出。 

\- c 统计字节数。 

\- l 统计行数。 

\- w 统计字数。 

```shell
find /dev/ -name "mmcblk1*" | wc -l
```

![image-20220726165735549](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220726165735549.png)

**shell脚本应用：用来通过特殊字符检索设备**

```shell
cnt=$(find /dev/ -name "mmcblk1*" | wc -l)
if [ $cnt == "0" ]; then
	HW_SD=0
	USBFILE="/dev/mmcblk0p3"
else
	HW_SD=1
	USBFILE="/dev/mmcblk1"
fi
```

