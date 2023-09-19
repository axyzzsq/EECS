# wireshark抓包教程

【导读】

目的：

WiFi的抓包是为了捕获设备的网络通讯的细节，提取特定信道中的所有设备的数据，再根据设备的ip或者mac地址过滤处特定的设备的数据。

环境：
- ubuntu需要采用桥接模式接入物理主机，和物理主机共享物理网卡；
- 需要抓包的设备需要和电脑接入同一个路由器，这样才能确保处于同一个信道。

## Step1: 安装WireShark

```shell
sudo apt-get install wireshark
```

## Step2:查看Wireshark版本，确认安装成功

```shell
wireshark --version
```

 ![image-20220913091752372](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209130917699.png)

## Step3:确认当前网卡所在信道

去小路由的后台查看，或者在物理主机用Linux子系统用iwlist命令查看，我去路由器后台看

 ![image-20220913092027818](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209130920881.png)

## Step4:在ubuntu中保存脚本`wiresharkCapture.sh`

```shell
ifconfig $1 down
iwconfig $1 mode monitor
iwconfig $1 channel $2
ifconfig $1 up
wireshark&
```

## Step5:查看当前桥接到物理主机的网卡名称

 ![image-20220913092503221](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209130925337.png)

## Step6:执行脚本启动wireshark

```shell
sudo ./wiresharkCapture.sh ens33 6 # 执行脚本通过网卡ens33在Wi-Fi信道6抓空中包
```

## Step7:在wireshark界面双击对应的网卡进入抓包界面

 ![image-20220913093116870](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209130931991.png)

![image-20220913093231127](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209130932268.png)

## Step8:保存抓包数据

点击功能按键第三个(Restart current capture)，在弹出的对话框中选择Save before continue，保存现有的抓包数据。

![image-20220913100833925](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209131008052.png)

![image-20220913101022109](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209131010231.png)

 ![image-20220913101112631](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209131011667.png)

## Step9:数据过滤

eth.addr==24:0A:C4:2D:44:74

![image-20220913111908400](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209131119555.png)
