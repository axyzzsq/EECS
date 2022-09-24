# input系统框架

对应的学习内容是韦东山-[输入系统应用编程](https://www.100ask.net/detail/p_5f338bede4b0b4059c4bbb5b/6)

## 一、输入系统框架

### 1、类比框架图

 ![image-20220924160400298](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924160400298.png)

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/2019050610425019.PNG)

### 2、关键数据结构

- 输入设备`input_dev`结构体

  location: `/inlcude/linux/input.h`

  ```C
  struct input_dev {
  	const char *name;
  	const char *phys;
  	const char *uniq;
  	struct input_id id;
  
  	unsigned long propbit[BITS_TO_LONGS(INPUT_PROP_CNT)];
      
  	//支持的事件类型，对应的位会被置1，使用的方法类似于STM32的寄存器的使用，把对应的开关打开或者关闭
  	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
  	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
  	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
  	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
  	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
  	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
  	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
  	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
  	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];
  	。。。。//省略一堆结构体成员
  };
  ```

  `input_dev`是输入系统中最重要的数据结构，是驱动的主体，每个`struct input_dev`代表一个输入设备。

- 输入事件`input_event`结构体

  ```C
  // location: /linux/uapi/linux/input.h
  struct input_event {
  	struct timeval time;
  	__u16 type;
  	__u16 code;
  	__s32 value;
  };
  
  // location: /linux/uapi/linux/time.h
  struct timeval {
  	__kernel_time_t		tv_sec;		/* seconds */
  	__kernel_suseconds_t	tv_usec;	/* microseconds */
  };
  ```

### 3、调试信息

- 确定设备信息

  ```shell
  ls /dev/input/* -l
  ```

  或者

  ```shell
  ls /dev/event* -l
  ```

- 查看设备节点对应的硬件信息

  ```shell
  cat /proc/bus/input/devices
  ```

   ![image-20220924190612222](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924190612222.png)

  - I:id of the device，用`input_id`结构体表示

    ```C
    //location: /Linux-4.9.88/include/uapi/linux/input.h
    struct input_id {
    	__u16 bustype;
    	__u16 vendor;
    	__u16 product;
    	__u16 version;
    };
    ```

  - N:name of the devices

  -  P: Physical path to the device in the system hierarchy

    系统层次结构中物理设备的地址

  - U:Unique indentification code for the device(if the devices has it)

  - H：List of input handles associated with with the devices

  - B：Bitmaps

    位图中事件的定义在`/Linux-4.9.88/include/uapi/linux/input-event-codes.h`

     ![image-20220924202906240](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924202906240.png)

## 二、不使用库的应用编程

### 1、获取设备信息

例程：

```C

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>


/* ./01_get_input_info /dev/input/event0 */
int main(int argc, char **argv)
{
        int fd;
        int err;
        int len;
        int i;
        unsigned char byte;
        int bit;
        struct input_id id;
        unsigned int evbit[2];
        char *ev_names[] = {
                "EV_SYN ",
                "EV_KEY ",
                "EV_REL ",
                "EV_ABS ",
                "EV_MSC ",
                "EV_SW  ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "EV_LED ",
                "EV_SND ",
                "NULL ",
                "EV_REP ",
                "EV_FF  ",
                "EV_PWR ",
                };

        if (argc != 2)
        {
                printf("Usage: %s <dev>\n", argv[0]);
                return -1;
        }

        fd = open(argv[1], O_RDWR);
        if (fd < 0)
        {
                printf("open %s err\n", argv[1]);
                return -1;
        }

        err = ioctl(fd, EVIOCGID, &id);  //通过ioctl函数获取input device的input_id
        if (err == 0)
        {
                printf("bustype = 0x%x\n", id.bustype );
                printf("vendor  = 0x%x\n", id.vendor  );
                printf("product = 0x%x\n", id.product );
                printf("version = 0x%x\n", id.version );
        }

        len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);   //获取evbit,但是这里的evbit为什么要加个&呢
        if (len > 0 && len <= sizeof(evbit)) //获取字节数
        {
                printf("support ev type: ");
                for (i = 0; i < len; i++) //获取bit位
                {
                        byte = ((unsigned char *)evbit)[i];
                        for (bit = 0; bit < 8; bit++)
                        {
                                if (byte & (1<<bit)) {
                                        printf("%s ", ev_names[i*8 + bit]);
                                }
                        }
                }
                printf("\n");
        }

        return 0;
}


```

说明：

- `input.h`不是内核程序中的`input.h`，而是工具链中的`input.h`

  - 为什么是这样？这是什么机制？

- 韦东山`ioctl`函数的编程逻辑：

  参考`/drivers/input/evdev.c`文件中的`evdev_do_ioctl()`函数；

  **需要分析`evdev_do_ioctl()`函数**.

编译执行：

 ![image-20220924225825997](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924225825997.png)

同步事件：会通知已经同步完数据了。

### 2、查询-休眠唤醒方式读数据

例程：

```C
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* ./01_get_input_info /dev/input/event0 noblock */
int main(int argc, char **argv)
{
        int fd;
        int err;
        int len;
        int i;
        unsigned char byte;
        int bit;
        struct input_id id;
        unsigned int evbit[2];
        struct input_event event;

        char *ev_names[] = {
                "EV_SYN ",
                "EV_KEY ",
                "EV_REL ",
                "EV_ABS ",
                "EV_MSC ",
                "EV_SW  ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "NULL ",
                "EV_LED ",
                "EV_SND ",
                "NULL ",
                "EV_REP ",
                "EV_FF  ",
                "EV_PWR ",
                };

        if (argc < 2)
        {
                printf("Usage: %s <dev> [noblock]\n", argv[0]);
                return -1;
        }

        if (argc == 3 && !strcmp(argv[2], "noblock"))
        {
                fd = open(argv[1], O_RDWR | O_NONBLOCK);
        }
        else
        {
                fd = open(argv[1], O_RDWR);
        }
        if (fd < 0)
        {
                printf("open %s err\n", argv[1]);
                return -1;
        }

        err = ioctl(fd, EVIOCGID, &id);
        if (err == 0)
        {
                printf("bustype = 0x%x\n", id.bustype );
                printf("vendor  = 0x%x\n", id.vendor  );
                printf("product = 0x%x\n", id.product );
                printf("version = 0x%x\n", id.version );
        }

        len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
        if (len > 0 && len <= sizeof(evbit))
        {
                printf("support ev type: ");
                for (i = 0; i < len; i++)
                {
                        byte = ((unsigned char *)evbit)[i];
                        for (bit = 0; bit < 8; bit++)
                        {
                                if (byte & (1<<bit)) {
                                        printf("%s ", ev_names[i*8 + bit]);
                                }
                        }
                }
                printf("\n");
        }

        while (1)
        {
                len = read(fd, &event, sizeof(event));
                if (len == sizeof(event))
                {
                        printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
                }
                else
                {
                        printf("read err %d\n", len);
                }
        }

        return 0;
}

```



说明：

- 韦东山在编写读取按键数据的时候参考的是 `/drivers/input/evdev.c`文件中的`static ssize_t evdev_read(struct file *file, char __user *buffer,size_t count, loff_t *ppos)`

编译执行：

![image-20220924233424542](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924233424542.png)

![image-20220924233327734](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924233327734.png)

