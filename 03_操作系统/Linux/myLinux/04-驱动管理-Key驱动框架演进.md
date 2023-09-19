# Linux Key驱动框架演进

## 第1节 App读取按键的四种方式

### 1、查询方式

![image-20221102184851945](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221102184851945.png)

```C
/* 在驱动里面定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = my_open,
	.read    = my_read,
	.write   = my_write,
	.release = my_close,
};
```

机制：APP 调用 open 时，导致驱动中对应的 open 函数被调用，在里面配置 GPIO 为输入引脚。 APP 调用 read 时，导致驱动中对应的 read 函数被调用，它读取寄存器，把引脚状态直接返回给APP。



​		**引擎：应用程序一直在不断地读取。**

### 2、休眠唤醒方式

![image-20221102191213195](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221102191213195.png)

```C
/* 在驱动里面定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = my_open,
	.read    = my_read,
	.write   = my_write,
	.release = my_close,
};
```

- APP 调用 open 时，导致驱动中对应的 open 函数被调用，在里面配置 GPIO 为输入引脚；并且注册 GPIO 的中断处理函数。
- APP 调用 read 时，导致驱动中对应的 read 函数被调用，如果有按键数据则直接返回给 APP；否则 APP 在内核态休眠。

当用户按下按键时， GPIO 中断被触发，导致驱动程序之前注册的中断服务程序被执行。它会记录按键数据，并唤醒休眠中的 APP。  

APP 被唤醒后继续在内核态运行，即继续执行驱动代码，把按键数据返回给 APP(的用户空间)  。

**引擎: 按键触发中断，read数据返回给应用层**

### 3、Poll方式

![image-20221102203259990](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221102203259990.png)

```C
/* 在驱动里面定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = my_open,
	.read    = my_read,
	.write   = my_write,
    .poll    = 
	.release = my_close,
};
```

- APP 调用 open 时，导致驱动中对应的 open 函数被调用，在里面配置GPIO 为输入引脚；并且注册 GPIO 的中断处理函数。  
- APP 调用 poll 或 select 函数，意图是“查询”是否有数据，这 2 个函数都可以指定一个超时时间，即在这段时间内没有数据的话就返回错误。这会导致驱动中对应的 poll 函数被调用，如果有按键数据则直接返回给 APP；否则 APP 在内核态休眠一段时间 。

当用户按下按键时， GPIO 中断被触发，导致驱动程序之前注册的中断服务程序被执行。它会记录按键数据，并唤醒休眠中的 APP。  

如果用户没按下按键，但是超时时间到了，内核也会唤醒 APP。  

所以 APP 被唤醒有 2 种原因：用户操作了按键，超时。被唤醒的 APP 在内核态继续运行，即继续执行驱动代码，把“状态”返回给 APP(的用户空间)。  

APP 得到 poll/select 函数的返回结果后，如果确认是有数据的，则再调用 read 函数，这会导致驱动中的 read 函数被调用，这时驱动程序中含有数据，会直接返回数据。  

相比休眠唤醒的阻塞，poll机制相当于设定了一小段时间的阻塞。

### 4、异步通知方式

![image-20221102211158328](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221102211158328.png)

```C
/* 在驱动里面定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = my_open,
	.read    = my_read,
	.write   = my_write,
    .fasync  = 
	.release = my_close,
};
```

- APP 调用 open 时，导致驱动中对应的 open 函数被调用，在里面配置GPIO为输入引脚；并且注册 GPIO 的中断处理函数。
- APP 给信号 SIGIO 注册自己的处理函数：my_signal_fun。  
- APP 调用 fcntl 函数，把驱动程序的 flag 改为 **FASYNC**，这会导致驱动程序的 fasync 函数被调用，它只是简单记录进程 PID。
- 当用户按下按键时， GPIO 中断被触发，导致驱动程序之前注册的中断服务程序被执行。它会记录按键数据，然后给进程 PID 发送 SIGIO 信号。
- APP 收到信号后会被打断，先执行信号处理函数：在信号处理函数中可以去调用 read 函数读取按键值。  
- 信号处理函数返回后， APP 会继续执行原先被打断的代码 。

异步通知例程:

```C
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void my_sig_func(int signo)
{
    printf("get a signal : %d\n", signo);
}

int main(int argc, char **argv)
{
    int i = 0;
    
    signal(SIGIO, my_sig_func);
    
    while (1) 
    {
        printf("Hello, world %d!\n", i++);
        sleep(2);
    }
    return 0;
}
```

```shell
gcc -o signal signal.c  #编译程序
./signal &  #后台运行
ps -A | grep signal  #查看进程,为2633
kill -SIGIO 2633  #给这个进程发信号
```

![image-20221105152804425](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221105152804425.png)

【参考】

- [CSDN-Signal ()函数详细介绍](https://blog.csdn.net/yyyljw/article/details/80741264?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166763363516800182746943%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166763363516800182746943&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-80741264-null-null.142^v63^control,201^v3^control_1,213^v1^control&utm_term=signal%E5%87%BD%E6%95%B0&spm=1018.2226.3001.4187)

## 第2节 查询方式的按键驱动程序-编写框架

套路:

![image-20221106161051358](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221106161051358.png)

button_drv.h

**这个头文件中定义了不同板子向驱动注册的接口，应该被各个demo板子的文件所包含，当板子的驱动插入之后，把板端的设备初始化和设备操作函数传参给button_drv.ko，与此同时,button_drv.ko才创建出/dev目录下对应的设备。**

```C
#ifndef _BUTTON_DRV_H
#define _BUTTON_DRV_H

struct button_operations {
    int count;
    void (*init) (int which);
    int (*read) (int which);
};

void register_button_operations(struct button_operations *opr);
void unregister_button_operations(void);

#endif
```

button_drv.c

```C
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/signal.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/capi.h>
#include <linux/kernelcapi.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/moduleparam.h>

#include "button_drv.h"


static int major = 0;

static struct button_operations *p_button_opr;
static struct class *button_class;

static int button_open (struct inode *inode, struct file *file)
{
    int minor = iminor(inode);
    p_button_opr->init(minor);
    return 0;
}

static ssize_t button_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    unsigned int minor = iminor(file_inode(file));
    char level;
    int err;
    
    level = p_button_opr->read(minor);
    err = copy_to_user(buf, &level, 1);
    return 1;
}


static struct file_operations button_fops = {
    .open = button_open,
    .read = button_read,
};

void register_button_operations(struct button_operations *opr)
{
    int i;

    p_button_opr = opr;
    for (i = 0; i < opr->count; i++)
    {
        device_create(button_class, NULL, MKDEV(major, i), NULL, "100ask_button%d", i);
    }
}

void unregister_button_operations(void)
{
    int i;

    for (i = 0; i < p_button_opr->count; i++)
    {
        device_destroy(button_class, MKDEV(major, i));
    }
}


EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);


int button_init(void)
{
    major = register_chrdev(0, "100ask_button", &button_fops);

    button_class = class_create(THIS_MODULE, "100ask_button");
    if (IS_ERR(button_class))
        return -1;
    
    return 0;
}

void button_exit(void)
{
    class_destroy(button_class);
    unregister_chrdev(major, "100ask_button");
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");
```

button_xxx.c

```C
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/signal.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/capi.h>
#include <linux/kernelcapi.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/moduleparam.h>

#include "button_drv.h"


static void board_xxx_button_init_gpio (int which)
{
    printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
}

static int board_xxx_button_read_gpio (int which)
{
    printk("%s %s %d, read gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
    return 1;
}

static struct button_operations my_buttons_ops ={
    .count = 2,
    .init  = board_xxx_button_init_gpio,
    .read  = board_xxx_button_read_gpio,
};

int board_xxx_button_init(void)
{
    register_button_operations(&my_buttons_ops);
    return 0;
}

void board_xxx_button_exit(void)
{
    unregister_button_operations();
}

module_init(board_xxx_button_init);
module_exit(board_xxx_button_exit);

MODULE_LICENSE("GPL");
```

button_test.c

```C

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * ./button_test /dev/100ask_button0
 *
 */
int main(int argc, char **argv)
{
	int fd;
	char val;
	
	/* 1. 判断参数 */
	if (argc != 2) 
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	/* 2. 打开文件 */
	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	/* 3. 写文件 */
	read(fd, &val, 1);
	printf("get button : %d\n", val);
	
	close(fd);
	
	return 0;
}



```

Makefile

```C

# 1. 使用不同的开发板内核时, 一定要修改KERN_DIR
# 2. KERN_DIR中的内核要事先配置、编译, 为了能编译内核, 要先设置下列环境变量:
# 2.1 ARCH,          比如: export ARCH=arm64
# 2.2 CROSS_COMPILE, 比如: export CROSS_COMPILE=aarch64-linux-gnu-
# 2.3 PATH,          比如: export PATH=$PATH:/home/book/100ask_roc-rk3399-pc/ToolChain-6.3.1/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin 
# 注意: 不同的开发板不同的编译器上述3个环境变量不一定相同,
#       请参考各开发板的高级用户使用手册

KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o button_test button_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f ledtest

# 参考内核源码drivers/char/ipmi/Makefile
# 要想把a.c, b.c编译成ab.ko, 可以这样指定:
# ab-y := a.o b.o
# obj-m += ab.o


obj-m	+= button_drv.o
obj-m	+= board_xxx.o
        
```

测试结果：

![image-20221106164756225](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221106164756225.png)

## 第3节 iMx6ull的按键驱动程序



