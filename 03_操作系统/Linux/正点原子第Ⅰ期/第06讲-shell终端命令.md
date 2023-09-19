# 第6讲  shell终端命令-查漏补缺

## 1、uname

> uname -a

## 2、su

> ① sudo su 切换到超级用户
>
> ② su + [用户名] ：切换账号

## 3、ifconfig

> **1、操作指定网卡**
>
> ```shell
> ifconfig waln0 up     #启动
> ifconfig wlan0 down   #关闭
> ifconfig wlan0 reload #重启
> ```
>
> **2、用ifconfig修改MAC地址**
>
>> ① `ifconfig eth0 down    //关闭网卡`
>>
>> ②` ifconfig eth0 hw ether 00:AA:BB:CC:DD:EE   //修改MAC地址`
>>
>> ③ `ifconfig eth0 up  //开启网卡`
>>
>
> **3、启用和关闭ARP协议**
>
>> ```shell
>> ifconfig eth0:ws arp #启用
>> ifconfig eth0:wa -arp #关闭
>> ```
>>
>
> **4、设置最大传输单元**
>
>> ```shell
>> ifconfig eth0 mtu 1500
>> ```
>>
>
> **5、网卡配置文件**
>
>> ```shell
>> cat /etc/sysconfig/network-scripts/ifcfg-eth2 
>> ```
>>
>
> **6、IP 重设**
>
>> ```shell
>> ifconfig eth0 IP_addr  #指定一个新的IP_addr
>> ```
>>

## 4、poweroff

## 5、sync

> 把数据从缓冲区写入磁盘

## 6、grep

> -i   //忽略大小写

## 7、du

> 文件大小查看命令
>
> du <文件名/文件夹名>  //以字节显示
>
> ```shell
> du ./cs -ch
> ```
