# 本地websocket测试服务器

## 服务端

`websocket_server.py`

在当前Linux环境中运行一个`websocket`服务器用来监听连接请求，并把客户端发来的消息反推回去。

## 客户端

`wscat`

命令:

```shell
wscat -c ws://Host:Port
```

## 测试

### 1、连接

![image-20230709173901222](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230709173901222.png)

### 2、消息回显

![image-20230709173950681](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230709173950681.png)

