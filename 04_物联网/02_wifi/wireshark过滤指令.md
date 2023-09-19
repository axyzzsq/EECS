# wireshark过滤指令

## 过滤源ip

```shell
ip.src==192.168.123.173
```

![image-20221129113847536](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129113847536.png)

## 过滤目的ip

```shell
ip.dst==47.117.78.230
```

![image-20221129113756005](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129113756005.png)

## 过滤端口

```shell
tcp.srcport==80    #只过滤源端口为80的包
tcp.dstport==80    #只过滤目的端口为80的包
```

##### ![image-20221129113954160](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129113954160.png)

## 协议过滤

- 在框中直接输入协议的名称,如：http

![image-20221129114251115](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129114251115.png)

- 剔除某个协议的命令

```shell
not http
```

![image-20221129132253133](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129132253133.png)

## Http过滤模式

```shell
http.request.method=="GET"   #筛选出get包
http.request.method=="POST"  #筛选出post包
```

![image-20221129114505921](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129114505921.png)

## 连接符

过滤两种条件时，使用and连接，如过滤ip为192.168.101.8并且为http协议的

```shell
ip.dst==222.79.77.161 and http
```

![image-20221129114733902](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221129114733902.png)





参考：

[wireshark 端口号过滤_Wireshark网络分析入门篇](https://blog.csdn.net/weixin_39715290/article/details/111226571?spm=1001.2101.3001.6650.5&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-5-111226571-blog-125846593.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-5-111226571-blog-125846593.pc_relevant_3mothn_strategy_recovery&utm_relevant_index=10)