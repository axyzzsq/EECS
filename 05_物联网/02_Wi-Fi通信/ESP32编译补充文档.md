# ESP32编译补充文档

## 一、首次安装环境:

Step1: 解压

```shell
tar -zxvf esp.tar.gz
```

Step2: 

```shell
cd esp/esp-idf/
./install.sh
. ./export.sh
```

![image-20230129174132920](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230129174132920.png)

## 二、第二次登录

关闭终端之后再次进入，都要重新在esp32/esp-idf/目录下执行 ./install.sh和 . ./export.sh，否则idf.py不能生效

