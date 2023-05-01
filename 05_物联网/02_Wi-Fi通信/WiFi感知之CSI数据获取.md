# Wi-Fi 感知之CSI数据获取

## 一、原理-CSD

[视频版  -  波束成形无线电灯塔-WiFi CSI 感知在路由器什么方位](https://www.bilibili.com/video/BV1Y3411a7jv/?spm_id_from=333.337.search-card.all.click&vd_source=bed7d2f6394486587a42febc934054a6)

[文字版  -  时间、空间、频率、灯塔、波束成型与航空无线电导航](https://mp.weixin.qq.com/s/7yyD4egvhvGBCq-2-ycZNQ)

[代码版  -  sdrfun](https://github.com/JiaoXianjun/sdrfun)

- 依据：802.11n CSD（Cyclic Shift Diversity）

  - 具有多子载波的OFDM信号经过天线向外辐射，在一个天线支路上进行信号延迟，如下图： 

    ![image-20230129015720626](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129015720626.png)

  - 不同频率的载波,如下图：

    ![image-20230129013037256](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129013037256.png)

  - OFDM信号的不同子载波经过信号延迟，形成了指向不同方向的波束(CSI (Channel Status Information)数据用来表示每个子载波的相位和幅度)

    ![image-20230129012210617](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129012210617.png)

  - 不同方向的接收机检测到的不同频率的信号的强度不同

    ![image-20230129014114711](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129014114711.png)

    - 接收机1朝向红色子载波，红色子载波的信号强度就高
    - 接收机2朝向蓝色子载波，蓝色子载波的信号强度就高



## 二、获取CSI相关工程

[CSDN博客 - IEEE 802.11n协议中CSI报告来源](https://blog.csdn.net/sinat_22991367/article/details/62418654)

### 工程1：STA - ESP CSI Data

- [ESP32 STA  Github工程路径](https://github.com/StevenMHernandez/ESP32-CSI-Tool/tree/master/active_sta)

- CSI使能配置

  ![image-20230129141017937](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129141017937.png)

- 编译烧录，运行结果:

  ![image-20230129140950907](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129140950907.png)

### 工程2：AP - ESP CSI Data

- [ESP32 AP  Github工程路径](https://github.com/StevenMHernandez/ESP32-CSI-Tool/tree/master/active_ap)

- 使能配置

  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129141602536.png)

- 编译烧录，运行结果:

  ![image-20230129142523954](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129142523954.png)

  ESP作为一个AP(路由器or发射机),当有设备接入这个AP之后，信道建立，此时才有CSI数据帧产生。

### 参考工程：Intel - Atheros CSI tool PC端运行

[搭建5300网卡获取csi数据](https://blog.csdn.net/qq_44118744/article/details/121928111)

- 这个教程执行完内核置换之后就会消耗将近100G的硬盘空间

![image-20230129155024050](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129155024050.png)

- 建议使用文中所推荐14.04版本的ubuntu以避免内核版本问题造成的编译错误。

### 参考工程：Atheros SDK的板端运行

[Atheros SDK研究与使用](https://www.witimes.com/atheros-sdk-study1/)

## 三、相关连接

### 1.文献

- [Wi-Fi雷达：从RSSI到CSI[J]. 中国计算机学会通讯](http://tns.thss.tsinghua.edu.cn/~yangzheng/files/Yang-WiFi-Radar-CCCF.pdf)

- [无线感知论文阅读笔记（一）——Wi-Fi雷达：从RSSI到CSI](https://www.cnblogs.com/G-Y-C/p/16034313.html)

- [WiFi无线感知技术原理与应用](https://www.h3c.com/cn/d_202201/1526841_233453_0.htm)

- [WiPasLoc:基于 WiFi 的被动式室内人员定位新方法[J]. 计算机科学](https://www.jsjkx.com/CN/article/openArticlePDF.jsp?id=21105)

- [bilibili视频 - 无线感知手势识别-Ubicomp2022论文分享](https://www.bilibili.com/video/BV1kt4y1A7mo/?spm_id_from=333.788.recommend_more_video.2&vd_source=bed7d2f6394486587a42febc934054a6)

### 2.先验

- [bilibili视频  -  乐鑫 ESP-CSI 智能人体感知检测方案](https://www.bilibili.com/video/BV1ui4y1o7fz/?spm_id_from=333.337.search-card.all.click&vd_source=bed7d2f6394486587a42febc934054a6)

