# Linux 输入子系统

[原文出处](https://www.cnblogs.com/lifexy/p/7542989.html)

## 1、简介

> 输入子系统需要输入驱动的框架,好来辨认应用程序要打开的是哪个输入驱动
>
> - 比如: 鼠标、键盘、游戏手柄等等这些都属于输入设备；这些输入设备的驱动都是通过输入子系统来实现的（当然，这些设备也依赖于usb子系统）
>
> 这些输入设备都各有不同，那么输入子系统也就只能实现他们的共性，差异性则由设备驱动来实现。差异性又体现在哪里？
>
> 最直观的就表现在这些设备功能上的不同了。**对于我们写驱动的人来说在设备驱动中就只要使用输入子系统提供的工具（也就是函数）来完成这些“差异”就行了**，其他的则是输入子系统的工作。这个思想不仅存在于输入子系统，其他子系统也是一样(比如:usb子系统、video子系统等)
>
> 所以我们先来分析下输入子系统**input.c**的代码,然后怎么来使用输入子系统(在内核中以input来形容输入子系统)

## 2、input_init()

打开input.c,位于内核deivers/input

> 有以下这么两句：
>
> ```c
> subsys_initcall(input_init);   //修饰入口函数
> 
> module_exit(input_exit);     //修饰出口函数
> ```
>
> **显然输入子系统是作为一个模块存在,我们先来分析下input_init()入口函数**
>
> ```C
> static int __init input_init(void)
> {
>        int err;
>        err = class_register(&input_class);   //(1)注册类,放在/sys/class
>        if (err) {
>               printk(KERN_ERR "input: unable to register input_dev class\n");
>               return err;
>        }
> 
>        err = input_proc_init();    //在/proc下面建立相关的文件
>        if (err)
>               goto fail1;
> 
>        err = register_chrdev(INPUT_MAJOR, "input", &input_fops); //(2)注册驱动
>        if (err) {
>               printk(KERN_ERR "input: unable to register char major %d", INPUT_MAJOR);
>               goto fail2;
>        }
>        return 0;
>     
> 
>  fail2:     input_proc_exit();
> 
>  fail1:     class_unregister(&input_class);
> 
>        return err;
> }
> ```
>
> - **(1)”err = class_register(&input_class);”** 是在/sys/class 里创建一个 input类, input_class变量如下图:
>
>   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190251.png)
>
>   - 如下图,我们启动内核,再启动一个input子系统的驱动后，也可以看到创建了个"input"类 :
>
>    ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190317.png)
>
>   - 为什么这里代码只创建类，没有使用class_device_create()函数在类下面创建驱动设备？
>
>     简单描述:当注册input子系统的驱动后，才会有驱动设备，此时这里的代码是没有驱动的。
>
> - **(2)" err = register_chrdev(INPUT_MAJOR, "input", &input_fops); //(2)注册驱动"**  这一行通过register_chrdev创建驱动设备,
>
>   其中变量INPUT_MAJOR =13,所以创建了一个主设备为13的"input"设备。
>
>   然后我们来看看它的操作结构体input_fops,如下图:
>
>   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190335.png)
>
>   只有一个.open函数,比如当我们挂载一个新的input驱动,则内核便会调用该.open函数,接下来分析该.open函数

## 3、input_open_file()

然后进入input_open_file函数(drivers/input/input.c)

```C
static int input_open_file(struct inode *inode, struct file *file)
 {
     struct input_handler *handler = input_table[iminor(inode) >> 5]; // (1)
     const struct file_operations *old_fops, *new_fops = NULL;
     int err;

     if (!handler || !(new_fops = fops_get(handler->fops)))  //(2)
          return -ENODEV;

    if (!new_fops->open) {
           fops_put(new_fops);
           return -ENODEV;
    }

    old_fops = file->f_op;
    file->f_op = new_fops;     //(3)

    err = new_fops->open(inode, file);   //(4)
    if (err) {
          fops_put(file->f_op);
           file->f_op = fops_get(old_fops);
   }

   fops_put(old_fops);

    return err;
}
```

> - ```c
>   struct input_handler *handler = input_table[iminor(inode) >> 5]; //(1)
>   ```
>
>   -  其中iminor (inode)函数调用了MINOR(inode->i_rdev);读取子设备号,然后将子设备除以32,找到新挂载的input驱动的数组号,然后放在input_handler 驱动处理函数handler中 
>
> - ```c
>   if (!handler || !(new_fops = fops_get(handler->fops)))  //(2)
>   ```
>
>   - 若handler有值,说明挂载有这个驱动,就将handler结构体里的成员file_operations * fops赋到新的file_operations *new_fops里面
>
> - ```c
>   file->f_op = new_fops;     //(3)
>   ```
>
>   - 再将新的file_operations *new_fops赋到file-> file_operations  *f_op里, 此时input子系统的file_operations就等于新挂载的input驱动的file_operations结构体,实现一个偷天换日的效果.
>
> - ```c
>   err = new_fops->open(inode, file);   //(4)
>   ```
>
>   - 然后调用新挂载的input驱动的*new_fops里面的成员.open函数

## 4、input_table[] 赋值

上面代码的input_table[]数组在初始时是没有值的,所以我们来看看input_table数组里面的数据又是在哪个函数里被赋值

在input.c函数(drivers/input/input.c)中搜索input_table,找到它在input_register_handler()函数中被赋值,代码如下:

```C
int input_register_handler(struct input_handler *handler)
{
... ...
input_table[handler->minor >> 5] = handler;   //input_table[]被赋值
... ...
list_add_tail(&handler->node, &input_handler_list); //然后将这个input_handler放到input_handler_list链表中
... ...
}
```

就是将驱动处理程序input_handler注册到input_table[]中，然后放在input_handler_list链表中,后面会讲这个链表

## 5、input_register_handler

如下图所示,有evdev.c(事件设备)，tsdev.c(触摸屏设备)，joydev.c(joystick操作杆设备)，keyboard.c(键盘设备)，mousedev.c(鼠标设备) 这5个内核自带的设备处理函数注册到input子系统中

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190352.png)

我们以evdev.c为例,它在evdev_ini()函数中注册:

```C
static int __init evdev_init(void)
{
       return input_register_handler(&evdev_handler);  //注册
}
```

## 6、evdev_handler

```C
static struct input_handler evdev_handler = {
       .event =  evdev_event,
       .connect =      evdev_connect,  //(4)
       .disconnect = evdev_disconnect,
       .fops =           &evdev_fops,    //(1)
       .minor =  EVDEV_MINOR_BASE, //(2)
       .name =         "evdev",
       .id_table =      evdev_ids, //(3)
};
```

就是我们之前看的input_handler驱动处理结构体

> - **.fops:** 文件操作结构体,其中evdev_fops函数就是自己的写的操作函数,然后赋到.fops中
>
> - **.minor:** 用来存放次设备号
>
>   - 其中EVDEV_MINOR_BASE=64, 然后调用input_register_handler(&evdev_handler)后,由于EVDEV_MINOR_BASE/32=2,所以存到input_table[2]中
>
>     所以当open打开这个input设备,就会进入 input_open_file()函数,执行evdev_handler-> evdev_fops -> .open函数,如下图所示:
>
>     ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190415.png)
>
> - .id_table : 表示能支持哪些输入设备，比如某个驱动设备的input_dev->的id和某个input_handler的id_table相匹配，就会调用.connect连接函数,如下图
>
> - **.connect:** 连接函数，将设备input_dev和某个input_handler建立连接,如下图
>
>   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190451.png)
>
>   

## 7、input_register_device()创建驱动设备

搜索input_register_device,发现内核自己就已经注册了很多驱动设备

> ### 7.1 然后进入input_register_device()函数,代码如下:
>
> ```c
> int input_register_device(struct input_dev *dev)   //*dev:要注册的驱动设备
> {
>  ... ...
>        list_add_tail(&dev->node, &input_dev_list);   //(1)放入链表中
>  ... ...
>        list_for_each_entry(handler, &input_handler_list, node)  //(2)
>        input_attach_handler(dev, handler);
>  ... ...
> }
> ```
>
> - ```c
>   list_add_tail(&dev->node, &input_dev_list);   //(1)放入链表中
>   ```
>
>   - 将要注册的input_dev驱动设备放在input_dev_list链表中 
>
> - ```c
>   list_for_each_entry(handler, &input_handler_list, node)  //(2)
>   ```
>
>   - 其中input_handler_list在前面讲过,就是存放每个input_handle驱动处理结构体,
>   - 然后list_for_each_entry()函数会将每个input_handle从链表中取出,放到handler中
>   - 最后会调用input_attach_handler()函数,将每个input_handle的id_table进行判断,若两者支持便进行连接。
>
> ### 7.2 然后我们在回过头来看注册input_handler的input_register_handler()函数,如下图所示
>
>  ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190507.png)
>
> 所以,不管新添加input_dev还是input_handler,都会进入input_attach_handler()判断两者id是否有支持, 若两者支持便进行连接。
>
> ### 7.3 我们来看看input_attach_handler()如何实现匹配两者id的:
>
> ```c
> static int input_attach_handler(struct input_dev *dev, struct input_handler *handler)
> {
> ... ...
> id = input_match_device(handler->id_table, dev);  //匹配两者
> 
> if (!id)                                     //若不匹配,return退出
> return -ENODEV; 
> 
> error = handler->connect(handler, dev, id);  //调用input_handler ->connect函数建立连接
> ... ...
> 
> }
> ```
>
> 若两者匹配成功，就会自动进入input_handler 的connect函数建立连接

## 8、evdev_handler->connect建立连接

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190529.png)

> ### 8.1 evdev_handler的.connect函数是evdev_connect(),代码如下:
>
> ```C
> static int evdev_connect(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id)
> {
> ... ...
> for (minor = 0; minor < EVDEV_MINORS && evdev_table[minor]; minor++); //查找驱动设备的子设备号
>     if (minor == EVDEV_MINORS) {  // EVDEV_MINORS=32,所以该事件下的驱动设备最多存32个,
>         printk(KERN_ERR "evdev: no more free evdev devices\n");
>         return -ENFILE;                //没找到驱动设备
>     }
>  ... ...
>  evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);   //分配一个input_handle全局结构体(没有r)
>  ... ...
>  evdev->handle.dev = dev;              //指向参数input_dev驱动设备
> evdev->handle.name = evdev->name;
> evdev->handle.handler = handler;    //指向参数 input_handler驱动处理结构体
> evdev->handle.private = evdev;
> sprintf(evdev->name, "event%d", minor);    //(1)保存驱动设备名字, event%d
> ... ...
> devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor),  //(2) 将主设备号和次设备号转换成dev_t类型
> cdev = class_device_create(&input_class, &dev->cdev, devt,dev->cdev.dev, evdev->name);                                                            // (3)在input类下创建驱动设备
> 
> ... ...
> error = input_register_handle(&evdev->handle); //(4)注册这个input_handle结构体
> 
> ... ...
> }
> ```
>
> - ```C
>   sprintf(evdev->name, "event%d", minor);    //(1)保存驱动设备名字, event%d
>   ```
>
>   - 在保存驱动设备名字,名为event%d, 比如下图(键盘驱动)event1: 因为没有设置子设备号，默认从小到大排列,其中event0是表示这个input子系统,所以这个键盘驱动名字就是event1
>
> - ```C
>   devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor),  //(2) 将主设备号和次设备号转换成dev_t类型
>   ```
>
>   - 在保存驱动设备的主次设备号,其中主设备号INPUT_MAJOR=13,因为EVDEV_MINOR_BASE=64,所以此设备号=64+驱动程序本事子设备号, 比如下图(键盘驱动)event1:  主次设备号就是13,65
>
> - 在之前在2小结里就分析了input_class类结构，所以第19行中,会在/sys/class/input类下创建驱动设备event%d，比如下图(键盘驱动)event1:
>
> ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190549.png)
>
> (4)最终会进入input_register_handle()函数来注册,代码在下面
>
> ### 8.2  input_register_handle()函数如下:
>
> ```C
> int input_register_handle(struct input_handle *handle)
> {
>       struct input_handler *handler = handle->handler; //handler= input_handler驱动处理结构体
> 
>       list_add_tail(&handle->d_node, &handle->dev->h_list); //(1)
>       list_add_tail(&handle->h_node, &handler->h_list);    // (2)
> 
>       if (handler->start)
>              handler->start(handle);
>       return 0;
> }
> ```
>
> - ```C
>   list_add_tail(&handle->d_node, &handle->dev->h_list); //(1)
>   ```
>
>   - 因为handle->dev指向input_dev驱动设备,所以就是将handle->d_node放入到input_dev驱动设备的h_list链表中,
>
>     即input_dev驱动设备的h_list链表就指向handle->d_node
>
> - ```c
>   list_add_tail(&handle->h_node, &handler->h_list);    // (2)
>   ```
>
>   - 同样, input_handler驱动处理结构体的h_list也指向了handle->h_node
>
>   最终如下图所示:
>
>   ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190753.png)
>
>   两者的.h_list都指向了同一个handle结构体，然后通过.h_list 来找到handle的成员.dev和handler,便能找到对方,便建立了连接

## 9、如何读取evdev_handler->.fops->.read

**建立了连接后，又如何读取evdev.c(事件驱动) 的evdev_handler->.fops->.read函数？**

事件驱动的.read函数是evdev_read()函数,我们来分析下:

```C
static ssize_t evdev_read(struct file *file, char __user *      buffer, size_t count, loff_t *ppos)
{
 ... ...
/*判断应用层要读取的数据是否正确*/
if (count < evdev_event_size())
return -EINVAL;

/*在非阻塞操作情况下,若client->head == client->tail|| evdev->exist时(没有数据),则return返回*/
 if (client->head == client->tail && evdev->exist && (file->f_flags & O_NONBLOCK))
return -EAGAIN;
 
/*若client->head == client->tail|| evdev->exist时(没有数据),等待中断进入睡眠状态  */
  retval = wait_event_interruptible(evdev->wait,client->head != client->tail || !evdev->exist);

  ... ...           //上传数据

}
```

## 10、若read函数进入了休眠状态，又是谁来唤醒？

我们搜索这个evdev->wait这个等待队列变量,找到evdev_event函数里唤醒:

```C
static void evdev_event(struct input_handle *handle, unsigned int type, unsigned int code, int value)
{
... ...
 wake_up_interruptible(&evdev->wait);   //有事件触发，便唤醒等待中断
}
```

其中evdev_event()是evdev.c(事件驱动) 的evdev_handler->.event成员,如下图所示:

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190608.png)

当有事件发生了,比如对于按键驱动,当有按键按下时,就会进入.event函数中处理事件

## 11、是谁调用evdev_event()这个.event事件驱动函数

应该就是之前分析的input_dev那层调用的

我们来看看内核 gpio_keys_isr()函数代码例子就知道了 (driver/input/keyboard/gpio_key.c)

```C
static irqreturn_t gpio_keys_isr(int irq, void *dev_id)
{
 /*获取按键值,赋到state里*/
 ... ...

/*上报事件*/
input_event(input, type, button->code, !!state);  
input_sync(input);                        //同步信号通知,表示事件发送完毕
}
```

显然就是通过input_event()来调用.event事件函数,我们来看看:

```C
void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
struct input_handle *handle;
... ...

/* 通过input_dev ->h_list链表找到input_handle驱动处理结构体*/
list_for_each_entry(handle, &dev->h_list, d_node)    
if (handle->open)  //如果input_handle之前open 过,那么这个就是我们的驱动处理结构体
    handle->handler->event(handle, type, code, value); //调用evdev_event()的.event事件函数 

}
```

若之前驱动input_dev和处理input_handler已经通过input_handler 的.connect函数建立起了连接,那么就调用evdev_event()的.event事件函数，如下图所示:

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326190642.png)

## 12、本节总结分析：

> - 1、注册输入子系统,进入put_init():
>   - 1)创建主设备号为13的"input"字符设备
>
> ```C
> err = register_chrdev(INPUT_MAJOR, "input", &input_fops);
> ```
>
> - 2、open打开驱动,进入input_open_file():
>
>   - 1)更新设备的file_oprations
>
>     ```C
>     file->f_op=fops_get(handler->fops);
>     ```
>
>   - 2)执行file_oprations->open函数
>
>     ```C
>     err = new_fops->open(inode, file);
>     ```
>
> - 3、注册input_handler,进入input_register_handler():
>
>   - 1)添加到input_table[]处理数组中
>
>     ```C
>     input_table[handler->minor >> 5] = handler;
>     ```
>
>   - 2）添加到input_handler_list链表中
>
>     ```C
>     list_add_tail(&handler->node, &input_handler_list);
>     ```
>
>   - 3)判断input_dev的id,是否有支持这个驱动的设备
>
>     ```C
>      list_for_each_entry(dev, &input_dev_list, node)   //遍历查找input_dev_list链表里所有input_dev
>       
>      input_attach_handler(dev, handler);             //判断两者id,若两者支持便进行连接。
>     ```
>
> - 4、注册input_dev，进入input_register_device():
>
>   - 1)放在input_dev_list链表中
>
>     ```c
>     list_add_tail(&dev->node, &input_dev_list);
>     ```
>
>   - 2)判断input_handler的id，是否有支持这个设备的驱动
>
>     ```c
>     list_for_each_entry(handler, &input_handler_list, node)  //遍历查找input_handler_list链表里所有input_handler
>     input_attach_handler(dev, handler);                      //判断两者id,若两者支持便进行连接。
>     ```
>
> - 5、判断input_handler和input_dev的id,进入input_attach_handler():
>
>   - 1）匹配两者id,
>
>     ```C
>     input_match_device(handler->id_table, dev);        //匹配input_handler和dev的id,不成功退出函数
>     ```
>
>   - 2)匹配成功调用input_handler ->connect
>
>     ```C
>     handler->connect(handler, dev, id);              //建立连接
>     ```
>
> - 6、建立input_handler和input_dev的连接，进入input_handler->connect():
>
>   - 1)创建全局结构体,通过input_handle结构体连接双方
>
>     ```C
>     evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);    //创建两者连接的input_handle全局结构体
>     list_add_tail(&handle->d_node, &handle->dev->h_list); //连接input_dev->h_list
>     list_add_tail(&handle->h_node, &handler->h_list);    // 连接input_handle->h_list
>     ```
>
> - 7、有事件发生时,比如按键中断,在中断函数中需要进入input_event()上报事件:
>
>   - 1)找到驱动处理结构体，然后执行input_handler->event()
>
>     ```C
>     list_for_each_entry(handle, &dev->h_list, d_node)     // 通过input_dev ->h_list链表找到input_handle驱动处理结构体
>     if (handle->open)  //如果input_handle之前open 过,那么这个就是我们的驱动处理结构体(有可能一个驱动设备在不同情况下有不同的驱动处理方式)
>         handle->handler->event(handle, type, code, value); //调用evdev_event()的.event事件函数 
>     ```
>
>     

