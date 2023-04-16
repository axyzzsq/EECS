# Android软件分层

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/android-stack_2x.png)



## 系统内核层

实现各种外设通讯协议，留出一个接口用来发起对外设的`open`、`read`、`write`、`close`。



## 硬件抽象层

这个层的出现是为了保护各个硬件厂商的权益，避免受到Kernel的开源协议限制。

HAL层不受开源协议限制，可以实现硬件驱动逻辑代码的闭源。闭源的开发思路参考[Android HAL层与Linux Kernel层驱动开发简介](https://blog.csdn.net/qq_28029345/article/details/65442587)。

HAL可以实现闭源，硬件厂商可以根据自己的需求，重新编写HAL模块，并将其编译成静态库或者动态库文件。上层应用框架必须通过HAL接口来间接地访问硬件功能，而不能直接访问硬件驱动。

HAL的两种类型：

- Binderized HAL：通过Binder机制来实现进程间通信的HAL，可以在用户空间运行，也可以在系统服务中运行。
- Passthrough HAL：直接与硬件驱动进行通信的HAL，必须在用户空间运行。
- 

## 系统运行库层

这一层包含`Native C/C++ libraries`和`Android Runtime`。

开发人员可以基于`Native C/C++ libraries`库使用C/C++语言编写Android应用程序。但是这玩意儿更适合用来写一些算法库的实现或者是中间层处理算法，如何OpenGL ES和Media Framework等。

Android Runtime管理Android应用程序跑起来之后的内存、线程、异常等。

C/C++编写出来的程序即使是应用程序，执行也受到Android Runtime管理。

Android应用程序转变成机器码是发生在安装程序的时候。



## 应用程序框架层

Android 的 Java API Framework 是一组 Java 类和接口，用于访问 Android 系统的所有功能。如

- 丰富、可扩展的[视图系统](https://developer.android.google.cn/guide/topics/ui/overview?hl=zh-cn)，可用以构建应用的 UI，包括列表、网格、文本框、按钮甚至可嵌入的网络浏览器
- [资源管理器](https://developer.android.google.cn/guide/topics/resources/overview?hl=zh-cn)，用于访问非代码资源，例如本地化的字符串、图形和布局文件
- [通知管理器](https://developer.android.google.cn/guide/topics/ui/notifiers/notifications?hl=zh-cn)，可让所有应用在状态栏中显示自定义提醒
- [Activity 管理器](https://developer.android.google.cn/guide/components/activities?hl=zh-cn)，用于管理应用的生命周期，提供常见的[导航返回栈](https://developer.android.google.cn/guide/components/tasks-and-back-stack?hl=zh-cn)
- [内容提供程序](https://developer.android.google.cn/guide/topics/providers/content-providers?hl=zh-cn)，可让应用访问其他应用（例如“联系人”应用）中的数据或者共享其自己的数据

开发者可以完全访问 Android 系统应用使用的[框架 API](https://developer.android.google.cn/reference/packages?hl=zh-cn)。



## 应用程序层

.APK



## 参考文献

- [Android开发者-平台架构](https://developer.android.com/guide/platform?hl=zh-cn#linux-kernel)
- [Android系统五层架构](https://blog.csdn.net/weixin_45099376/article/details/126263379?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-126263379-blog-123525297.235%5Ev29%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-126263379-blog-123525297.235%5Ev29%5Epc_relevant_default_base3&utm_relevant_index=1)