# 绘本台灯无线信道切换

## Step1:查看信道

执行命令：

```shell
iwlist wlan0 channel
```

 ![image-20220812091942850](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812091942850.png)

如上图，支持2.412GHZ至2.472GHZ，当前处于Channel 13



## Step2: 切换路由器后台信道

如下图，切换到信道6并且生效

![image-20220812100231102](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812100231102.png)

## Step3: 确认验证

指令命令：

```shell
iwlist wlan0 channel
```

![image-20220812100455849](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220812100455849.png)

