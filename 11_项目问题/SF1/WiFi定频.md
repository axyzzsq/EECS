# WIFI 配置台灯定频方法

## 开启笔记本电脑热点

 ![image-20220812105603883](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812105603883.png)

## 台灯连接电脑热点后，查看IP

![image-20220812105622131](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812105622131.png)



## 使用MobaXterm的Telnet协议连接台灯

![image-20220812105634222](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812105634222.png)

## 输入账号密码登陆台灯后可以对台灯进行指令控制

- login: admin 
- Password:admin

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/clip_image002.png)

 

## 使用wifi控制指令

定频测试b模 channel=1 rate=11（注意以下指令是一条）

```shell
ifconfig wlan0 up;
rtwpriv wlan0 mp_ctx stop;
rtwpriv wlan0 mp_start;rtwpriv wlan0 mp_channel 1;
rtwpriv wlan0 mp_bandwidth 40M=0,shortGI=0;
rtwpriv wlan0 mp_ant_tx a;
rtwpriv wlan0;
mp_rate 22;
rtwpriv wlan0 mp_get_txpower;
rtwpriv wlan0 mp_ctx background,pkt;
```

定频测试g模 channel=1 rate=54（注意以下指令是一条）

```shell
ifconfig wlan0 up;
rtwpriv wlan0 mp_ctx stop;rtwpriv wlan0 mp_start;
rtwpriv wlan0 mp_channel 1;rtwpriv wlan0 mp_bandwidth 40M=0,shortGI=0;
rtwpriv wlan0 mp_ant_tx a;
rtwpriv wlan0;mp_rate 108;
rtwpriv wlan0 mp_get_txpower;
rtwpriv wlan0 mp_ctx background,pkt;
```

定频测试n模 channel=1 rate=72.2（注意以下指令是一条）

```shell
ifconfig wlan0 up;
rtwpriv wlan0 mp_ctx stop;
rtwpriv wlan0 mp_start;
rtwpriv wlan0 mp_channel 1;
rtwpriv wlan0 mp_bandwidth 40M=0,shortGI=1;
rtwpriv wlan0 mp_ant_tx a;
rtwpriv wlan0;mp_rate 135;
rtwpriv wlan0 mp_get_txpower;
rtwpriv wlan0 mp_ctx background,pkt;
```

**因为wifi控制下输入wifi指令会断开连接，所以测试一项后给台灯重启重复以上步骤**