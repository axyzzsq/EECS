# 问答

[CSDN-【毕业季|进击的技术er】作为一名职场人，精心总结的嵌入式学习路线图](https://blog.csdn.net/dong__ge/article/details/125365471?spm=1001.2100.3001.7377&utm_medium=distribute.pc_feed_blog_category.none-task-blog-classify_tag-1-125365471-null-null.nonecase&depth_1-utm_source=distribute.pc_feed_blog_category.none-task-blog-classify_tag-1-125365471-null-null.nonecase)

[60道音视频经典面试题](https://blog.csdn.net/River_ly/article/details/126754373?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167523787416800213098978%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167523787416800213098978&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-126754373-null-null.142^v72^insert_down1,201^v4^add_ask&utm_term=%E9%9F%B3%E8%A7%86%E9%A2%91%E9%9D%A2%E8%AF%95%E9%A2%98&spm=1018.2226.3001.4187)

[音视频面试题集锦](https://blog.csdn.net/yinshipin007/article/details/124739668?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167523787416800213098978%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167523787416800213098978&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-124739668-null-null.142^v72^insert_down1,201^v4^add_ask&utm_term=%E9%9F%B3%E8%A7%86%E9%A2%91%E9%9D%A2%E8%AF%95%E9%A2%98&spm=1018.2226.3001.4187)

[音视频常见面试题](https://blog.csdn.net/m0_37824357/article/details/123391934?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167523787416800213098978%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167523787416800213098978&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-4-123391934-null-null.142^v72^insert_down1,201^v4^add_ask&utm_term=%E9%9F%B3%E8%A7%86%E9%A2%91%E9%9D%A2%E8%AF%95%E9%A2%98&spm=1018.2226.3001.4187)

[2022年音视频面试题 C/C++/Linux/FFmpeg/webRTC/rtmp/hls/rtsp/ffplay/srs](https://blog.csdn.net/yinshipin007/article/details/127659815?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167523787416800213070332%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=167523787416800213070332&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-6-127659815-null-null.142^v72^insert_down1,201^v4^add_ask&utm_term=%E9%9F%B3%E8%A7%86%E9%A2%91%E9%9D%A2%E8%AF%95%E9%A2%98&spm=1018.2226.3001.4187)

[嵌入式软件面试指南](https://www.zhihu.com/column/c_1366086412383600640)





## 一、外设

### 1、UART

1. 第1题

  ```
  设置一个串口的通信波特率为9600，起始位与停止位各占1位，偶校验，传输300个字节需要的时间是_____
  ```

假设每个字节有 8 个数据位和 1 个奇偶校验位，传输 300 个偶校验字节所需的总位数为：

300 字节 * 9 位/字节（8 数据位 + 1 奇偶校验位）= 2700 位

以每秒9600位的波特率，每个字节1个起始位和1个停止位，传输2700位所需的时间可以计算为：

（2700 位 * 1 个起始位 * 1 个停止位）/（9600 位/秒）= 0.28125 秒

因此，使用指定的串行端口设置传输 300 个字节所需的时间约为 0.28125 秒。 .





2.   ​       
3.   ​      
4.   ​       

### 2、定时器



## 二、程序与数据结构



## 三、Linux

1. 第1题

  ```
  什么是系统调用?
  ```

  > 系统调用（英语：system call），指运行在用户空间的程序向操作系统内核请求需要更高权限运行的服务。系统调用提供用户程序与操作系统之间的接口。大多数系统交互式操作需求在内核态执行。如设备IO操作或者进程间通信。

  

2. 第2题

  ```
  什么是MMU,MMU的作用?
  ```

  > MMU是Memory Management Unit的缩写。
  >
  > 针对各种CPU, MMU是个可选的配件. MMU负责的是虚拟地址与物理地址的转换，提供硬件机制的内存访问授权。
  > 现在的多用户多进程操作系统, 需要MMU, 才能达到每个用户进程都拥有自己的独立的[地址空间](https://so.csdn.net/so/search?q=地址空间&spm=1001.2101.3001.7020)的目标。
  >
  > 使用MMU, OS划分出一段地址区域, 在这块地址区域中, 每个进程看到的内容都不一定一样。
  >
  > **例如：**MICROSOFT WINDOWS操作系统, 地址4M-2G处划分为用户地址空间，
  >
  > 进程A在地址 0X400000映射了可执行文件，进程B同样在地址 0X400000映射了可执行文件。
  >
  > 如果A进程读地址0X400000, 读到的是A的可执行文件映射到RAM的内容；而进程B读取地址0X400000时则读到的是B的可执行文件映射到RAM的内容。
  > 这就是MMU在当中进行地址转换所起的作用。
  > II. X86系列的MMU
  > INTEL出品的80386CPU或者更新的CPU中都集成有MMU. 可以提供32BIT共4G的地址空间.
  >
  > III. ARM系列的MMU

  

3.    

4. 

5. 

6. 

  ​	



## 四、计算机网络

1. 第1题

	```
	七层模型，各个层的作用简述，各个层的常见协议(反向问某个协议在哪个层，作用是什么，ICMP?)
	```

	

2. 
