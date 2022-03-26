# platform机制实现驱动层分离

[原文出处](https://www.cnblogs.com/lifexy/p/7569371.html)

## 1、介绍：

分离就是在驱动层中使用platform机制把硬件相关的代码(固定的,如板子的网卡、中断地址)和驱动(会根据程序作变动,如点哪一个灯)分离开来，即要编写两个文件:dev.c和drv.c(platform设备和platform驱动)

## 2、platform机制

- 基本内容：

  platform会存在/sys/bus/里面

  如下图所示, platform目录下会有两个文件,分别就是platform设备和platform驱动。

   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190845.png)

  - **1) device设备**

    挂接在platform总线下的设备, platform_device结构体类型

  - **2) driver驱动**

    挂接在platform总线下,是个与某种设备相对于的驱动, platform_driver结构体类型

  - **3) platform总线**

    是个全局变量,为platform_bus_type,属于虚拟设备总线,通过这个总线将设备和驱动联系起来,属于Linux中bus的一种

    该platform_bus_type的结构体定义如下所示(位于drivers/base):

    ```c
    struct bus_type platform_bus_type = {              
    .name              = "platform",             //设备名称
    .dev_attrs        = platform_dev_attrs,                 //设备属性、含获取sys文件名,该总线会放在/sys/bus下
    .match              = platform_match,   //匹配设备和驱动,匹配成功就调用driver的.probe函数
    .uevent             = platform_uevent,  //消息传递,比如热插拔操作
    .suspend         = platform_suspend,     //电源管理的低功耗挂起
    .suspend_late          = platform_suspend_late,
    .resume_early         = platform_resume_early,
    .resume           = platform_resume, //恢复
    };
    ```

    **驱动、设备注册匹配图如下所示**

    ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190913.png)

    只要有一方注册,就会调用platform_bus_type的.match匹配函数,来找对方,成功就调用driver驱动结构体里的.probe函数来使总线将设备和驱动联系起来。

## 3、实例分析driver驱动

我们以/drivers/input/keybard/gpio_keys.c内核自带的示例程序为例,

它的代码中只有**driver**驱动,因为是个示例程序,所以没有**device**硬件设备代码。

### 3.1 发现在gpio_keys.c中有1个全局变量driver驱动:

```C
struct platform_driver gpio_keys_device_driver = {  //定义一个platform_driver类型驱动

    .probe      = gpio_keys_probe,                //设备的检测,当匹配成功就会调用这个函数(需要自己编写)   
    .remove     = __devexit_p(gpio_keys_remove), //删除设备(需要自己编写)
    .driver     = {
            .name  = "gpio-keys",               //驱动名称,用来与设备名称匹配用的
         }
};
```

###  3.2 然后来找找这个gpio_keys_device_driver被谁用到

发现在驱动层init入口函数中通过platform_driver_register()来注册driver驱动

在驱动层exit出口函数中通过platform_driver_unregister()函数来注销driver驱动

代码如下:

```C
static int __init gpio_keys_init(void)    //init出口函数
{
   return platform_driver_register(&gpio_keys_device_driver);   //注册driver驱动
}

static void __exit gpio_keys_exit(void)   //exit出口函数
{
   platform_driver_unregister(&gpio_keys_device_driver);   //注销driver驱动
}
```

###  3.3 我们进来platform_driver_register(),看它是如何注册diver的,注册到哪里？

platform_driver_register()函数如下:

```C
int platform_driver_register(struct platform_driver *drv)
{
   drv->driver.bus = &platform_bus_type;                //(1)挂接到虚拟总线platform_bus_type上
   if (drv->probe)
         drv->driver.probe = platform_drv_probe;
   if (drv->remove)
        drv->driver.remove = platform_drv_remove;
   if (drv->shutdown)
        drv->driver.shutdown = platform_drv_shutdown;
   if (drv->suspend)
        drv->driver.suspend = platform_drv_suspend;
    if (drv->resume)
        drv->driver.resume = platform_drv_resume;

   return driver_register(&drv->driver);              //(2) 注册到driver目录下
}
```

> (1) 挂接到虚拟总线platform_bus_type上,然后会调用platform_bus_type下的platform_match匹配函数,来匹配device和driver的名字,其中driver的名字如下图所示:
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326191144.png)
>
> platform_match()匹配函数如下所示:
>
> ```C
> static int platform_match(struct device * dev, struct device_driver * drv)
> {
> /*找到所有的device设备*/
> struct platform_device *pdev = container_of(dev, struct platform_device, dev);
> 
> return (strncmp(pdev->name, drv->name, BUS_ID_SIZE) == 0); //找BUS_ID_SIZE次
> }
> ```
>
> 若名字匹配成功,则调用device的.probe成员函数
>
> (2)然后放到/sys/bus/platform/driver目录下,其中driver_register()函数就是用来创建dirver目录的

## 4、使用platform机制,编写LED驱动层

首先创建设备代码和驱动代码:led_dev.c 、led_drv.c

led_dev.c用来指定灯的引脚地址,当更换平台时,只需要修改这个就行

led_drv.c用来初始化灯以及如何控制灯的逻辑,当更换控制逻辑时,只需要修改这个就行   

## 5、编写led_dev.c

### 5.1 编写led_dev.c之前先来看看platform_device结构体和要使用的函数:

platform_device结构体如下:

```C
struct platform_device {
  const char       * name;           //设备名称，要与platform_driver的name一样,这样总线才能匹配成功
  u32          id;                   //id号,插入总线下相同name的设备编号(一个驱动可以有多个设备),如果只有一个设备填-1
  struct  device  dev;               //内嵌的具体的device结构体,其中成员platform_data,是个void *类型,可以给平台driver提供各种数据(比如:GPIO引脚等等)
  u32 num_resources;                 //资源数量，
  struct resource         * resource;    //资源结构体,保存设备的信息
};
```

其中resource资源结构体,如下:

```C
struct resource {
         resource_size_t start;                    //起始资源,如果是地址的话,必须是物理地址
         resource_size_t end;                      //结束资源,如果是地址的话,必须是物理地址
         const char *name;                         //资源名
         unsigned long flags;                      //资源的标志
         //比如IORESOURCE_MEM,表示地址资源, IORESOURCE_IRQ表示中断引脚... ...

         struct resource *parent, *sibling, *child;   //资源拓扑指针父、兄、子,可以构成链表
};
```

要用的函数如下,在dev设备的入口出口函数中用到

```C
int platform_device_register(struct platform_device * pdev);       //注册dev设备
int platform_device_register(struct platform_device * pdev);       //注销dev设备
```

### 5.2 接下来开始写代码

> 1)先写要注册的led设备:platform_device结构体
>
> ```C
> #include <linux/module.h>
> #include <linux/version.h>
> #include <linux/init.h>
> #include <linux/kernel.h>
> #include <linux/types.h>
> #include <linux/interrupt.h>
> #include <linux/list.h>
> #include <linux/timer.h>
> #include <linux/init.h>
> #include <linux/serial_core.h>
> #include <linux/platform_device.h>
> 
> static struct resource led_resource[] = {               //资源数组
>     [0] = {
>         .start = 0x56000050,                     //led的寄存器GPFCON起始地址
>         .end   = 0x56000050 + 8 - 1,     // led的寄存器GPFDAT结束地址
>         .flags = IORESOURCE_MEM,      //表示地址资源
>     },
>     [1] = {
>         .start =  5,                                   //表示GPF第几个引脚开始
>         .end   = 5,                            //结束引脚
>         .flags = IORESOURCE_IRQ,     //表示中断资源        } 
> };
> 
> static void led_release(struct device * dev)       //释放函数
> {}
> 
> static struct platform_device led_dev = {
>     .name         = "myled",                    //对应的platform_driver驱动的名字
>     .id       = -1,                                    //表示只有一个设备
>     .num_resources    = ARRAY_SIZE(led_resource),        //资源数量,ARRAY_SIZE()函数:获取数量
>     .resource     = led_resource,      //资源数组led_resource
>     .dev = {
>     .release = led_release,                 //释放函数，必须向内核提供一个release函数, 、
>                                        //否则卸载时,内核找不到该函数会报错
>        },
> };
> ```
>
> 2)最后写出口入口函数:
>
> ```C
> static int led_dev_init(void)    //入口函数,注册dev设备
> {
>   platform_device_register(&led_dev);
>   return 0;
> }
> 
> static void led_dev_exit(void) //出口函数,注销dev设备
> {
>   platform_device_unregister(&led_dev); 
> }
> module_init(led_dev_init);   //修饰入口函数
> module_exit(led_dev_exit);  //修饰出口函数
> MODULE_LICENSE("GPL");   //声明函数
> ```

## 6、编写led_drv.c

### 6.1编写led_drv.c之前先来看看platform_driver结构体和要使用的函数:

```C
struct platform_driver {
       int (*probe)(struct platform_device *);       //查询设备的存在
       int (*remove)(struct platform_device *);             //删除
       void (*shutdown)(struct platform_device *);         //断电
       int (*suspend)(struct platform_device *, pm_message_t state);  //休眠
       int (*suspend_late)(struct platform_device *, pm_message_t state);
       int (*resume_early)(struct platform_device *);
       int (*resume)(struct platform_device *);           //唤醒
       struct device_driver driver;       //内嵌的driver,其中的name成员要等于设备的名称才能匹配

};

int platform_driver_register(struct platform_driver *drv);     //注册驱动
platform_driver_unregister(struct platform_driver *drv);    //卸载驱动

struct resource * platform_get_resource(struct platform_device *dev, unsigned int type,unsigned int num);
//获取设备的某个资源,获取成功,则返回一个resource资源结构体
//参数:
// *dev :指向某个platform device设备
// type:获取的资源类型
// num: type资源下的第几个数组
```

### 6.2接下来开始写代码

> 1)先写要注册的led驱动:platform_driver结构体
>
> ```C
> /*函数声明*/
> static  int  led_remove(struct platform_device *led_dev);
> static  int led_probe(struct platform_device *led_dev);
> 
> struct platform_driver led_drv = {
>        .probe           = led_probe,        //当与设备匹配,则调用该函数
>        .remove         = led_remove,             //删除设备
> 
>        .driver            = {
>               .name     = "myled",           //与设备名称一样
>        }
> };
> ```
>
> 2)写file_operations 结构体、以及成员函数(.open、.write)、.probe函数、
>
> 当驱动和设备都insmod加载后,然后bus总线会匹配成功,就进入.probe函数,
>
> 在.probe函数中便使用platform_get_resource()函数获取LED的地址和引脚,然后初始化LED,并注册字符设备和设备节点"led"
>
> ```C
> static struct class *cls;                                      //类,用来注册,和注销
> static volatile unsigned long *gpio_con;         //被file_operations的.open函数用
> static volatile unsigned long *gpio_dat;          //被file_operations的.write函数用
> static int pin;                                                 //LED位于的引脚值
> 
> static int led_open(struct inode *inode, struct file  *file)
>  {
>        *GPFcon&=~(0x03<<(LED_PIN*2));
>        *GPFcon|=(0x01<<(LED_PIN*2));   
>        return 0;
>  } 
> 
> static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
> {
>        int val=0;
>        if(count!=1)
>               return -EINAL;
>        copy_from_user(&val,buf,count);      //从用户(应用层)拷贝数据       
> 
>        if(val)      //开灯
>        {
>        *GPFdat&=~(0x1<<LED_PIN);
>        }
>        else
>        {
>        *GPFdat |= (0x1<<LED_PIN);
>        }   
>        return 0 ;
> }
> 
> 
> static struct  file_operations led_fops= {
>     .owner  =   THIS_MODULE,     //被使用时阻止模块被卸载
>     .open   =   led_open,     
>     .write   =   led_write,   
>   };
> 
>  
> static int led_probe(struct platform_device *pdev)
> {
>        struct resource      *res;
>        printk("enter probe\n");
> 
>        /* 根据platform_device的资源进行ioremap */
>        res = platform_get_resource(pdev, IORESOURCE_MEM, 0); //获取寄存器地址
>        gpio_con = ioremap(res->start, res->end - res->start + 1); //获取虚拟地址
>        gpio_dat = gpio_con + 1;
> 
>        res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);   //获取引脚值
>        pin = res->start;
> 
>        /* 注册字符设备驱动程序 */
>        major = register_chrdev(0, "myled", &led_fops);   //赋入file_operations结构体
>        cls = class_create(THIS_MODULE, "myled");
>        class_device_create(cls, NULL, MKDEV(major, 0), NULL, "led"); /* /dev/led */
>        return 0;
> }
> ```
>
> 3)写.remove函数
>
> 如果驱动与设备已联系起来,当卸载驱动时,就会调用.remove函数卸载设备和.probe函数一样,注册了什么就卸载什么便可
>
> ```
> static int led_remove(struct platform_device *pdev)
> {
>        /* 卸载字符设备驱动程序 */
>        printk("enter remove\n");
>        class_device_destroy(cls, MKDEV(major, 0));
>        class_destroy(cls);
>        unregister_chrdev(major, "myled");
> 
>        iounmap(gpio_con);       //注销虚拟地址
>        return 0;
> }
> ```
>
> 4)最后写drv的入口出口函数
>
> ```C
> static int led_drv_init(void)           //入口函数,注册驱动
> {
>        platform_driver_register(&led_drv);
>        return 0;
> } 
> static void led_drv_exit(void)       //出口函数,卸载驱动
> {
>        platform_driver_unregister(&led_drv);
> }
> 
> module_init(led_drv_init);     
> module_exit(led_drv_exit);
> MODULE_LICENSE("GPL");
> ```

## 7、测试运行

1)如下图,我们先挂载dev设备模块,和我们之前分析的一样,它在platform/devices目录下生成一个"myled"设备

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190952.png)

2)如下图,我们再来挂载drv驱动模块,同样的在platform/drivers目录下生成一个"myled"驱动,devices目录下的"myled"设备匹配成功,进入.probe函数创建设备,接下来就可以使用应用程序来控制led灯了

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326191008.png)

3)如下图,卸载驱动时,也会进入.remove函数卸载设备

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326191023.png)
