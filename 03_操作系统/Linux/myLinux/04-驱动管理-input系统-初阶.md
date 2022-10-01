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
  	__u16 type; // 表示哪一类事件,EV_KEY表示按键,EV_REL表示相对位移
  	__u16 code; // 表示该类事件中得哪个事件，如数字按键的1、2、3
  	__s32 value; //事件状态码，0、1表示松开或者按下，根据具体外设决定
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

  - H：List of input handles associated with the devices

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

### 2、查询-休眠唤醒方式

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
        if (len > 0 && len <= sizeof(evbit)) //获取字节数
        {
                printf("support ev type: ");
                for (i = 0; i < len; i++)  //遍历字节数
                {
                        byte = ((unsigned char *)evbit)[i];
                        for (bit = 0; bit < 8; bit++)  //遍历字节数内bit位
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

### 3、POLL和SELECT方式

#### （1）POLL

例程`03_input_read_poll.c`:

```C
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>


/* ./01_get_input_info /dev/input/event0 */
int main(int argc, char **argv)
{
        int fd;
        int err;
        int len;
        int ret;
        int i;
        unsigned char byte;
        int bit;
        struct input_id id;
        unsigned int evbit[2];
        struct input_event event;
        struct pollfd fds[1];
        nfds_t nfds = 1;

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

        fd = open(argv[1], O_RDWR | O_NONBLOCK);
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
                fds[0].fd = fd;  //指定了文件描述符
                fds[0].events  = POLLIN;
                fds[0].revents = 0;
                ret = poll(fds, nfds, 5000);
                if (ret > 0)
                {
                        if (fds[0].revents == POLLIN)
                        {
                                while (read(fd, &event, sizeof(event)) == sizeof(event))
                                {
                                        printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
                                }
                        }
                }
                else if (ret == 0)
                {
                        printf("time out\n");
                }
                else
                {
                        printf("poll err\n");
                }
        }

        return 0;
}
```

说明：

- `fds[]`一组描述一个文件，有多个文件的时候，数组成员为多个,`fds[0]、fds[1]、fds[22]  `。

- `poll`机制最关键的数据结构

  ```C
  struct pollfd {
                 int   fd;         /* file descriptor */
                 short events;     /* requested events */
                 short revents;    /* returned events */
             };
  ```

编译执行：

 ![image-20220925160759377](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220925160759377.png)

#### （2）SELECT

例程`04_input_read_select.c`:

```C
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>


/* ./01_get_input_info /dev/input/event0 */
int main(int argc, char **argv)
{
        int fd;
        int err;
        int len;
        int ret;
        int i;
        unsigned char byte;
        int bit;
        struct input_id id;
        unsigned int evbit[2];
        struct input_event event;
        int nfds;
        struct timeval tv;
        fd_set readfds;

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

        fd = open(argv[1], O_RDWR | O_NONBLOCK);
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
                /* 设置超时时间 */
                tv.tv_sec  = 5;
                tv.tv_usec = 0;

                /* 想监测哪些文件? */
                FD_ZERO(&readfds);    /* 先全部清零 */
                FD_SET(fd, &readfds); /* 想监测fd */

                /* 函数原型为:
                        int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
         * 我们为了"read"而监测, 所以只需要提供readfds
                 */
                nfds = fd + 1; /* nfds 是最大的文件句柄+1, 注意: 不是文件个数, 这与poll不一样 */
                ret = select(nfds, &readfds, NULL, NULL, &tv);
                if (ret > 0)  /* 有文件可以提供数据了 */
                {
                        /* 再次确认fd有数据 */
                        if (FD_ISSET(fd, &readfds))
                        {
                                while (read(fd, &event, sizeof(event)) == sizeof(event))
                                {
                                        printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
                                }
                        }
                }
                else if (ret == 0)  /* 超时 */
                {
                        printf("time out\n");
                }
                else   /* -1: error */
                {
                        printf("select err\n");
                }
        }

        return 0;
}

```

编译执行：

 ![image-20220925161929544](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220925161929544.png)

### 4、异步通知方式

在例程`03_input_read_poll.c`基础上编写`05_input_read_fasync.c`：

```C
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int fd;

void my_sig_handler(int sig)
{
        struct input_event event;
        while (read(fd, &event, sizeof(event)) == sizeof(event))
        {
                printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
        }
}

/* ./05_input_read_fasync /dev/input/event0 */
int main(int argc, char **argv)
{
        int err;
        int len;
        int ret;
        int i;
        unsigned char byte;
        int bit;
        struct input_id id;
        unsigned int evbit[2];
        unsigned int flags;
        int count = 0;

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

        /* 注册信号处理函数 */
        signal(SIGIO, my_sig_handler);

        /* 打开驱动程序 */
        fd = open(argv[1], O_RDWR | O_NONBLOCK);
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

        /* 把APP的进程号告诉驱动程序 */
        fcntl(fd, F_SETOWN, getpid());

        /* 使能"异步通知" */
        flags = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, flags | FASYNC);

        while (1)
        {
                printf("main loop count = %d\n", count++);
                sleep(2);
        }

        return 0;
}

```

说明：

- 异步通知的编程步骤：

  - Step1：编写信号处理函数

    ```C
    static void sig_func(int sig)
    {
        int val;
        read(fd, &val, 4);
        printf("get button : 0x%x\n", val);
    }
    ```

  - Step2：注册信号处理函数  

    ```C
    signal(SIGIO, sig_func);
    ```

  - Step3：打开驱动：  

    ```C
    fd = open(argv[1], O_RDWR);
    ```

  - Step4：把进程 ID 告诉驱动

    ```C
    fcntl(fd, F_SETOWN, getpid());
    ```

  - Step5：使能驱动的 FASYNC 功能

    ```C
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | FASYNC);
    ```

编译执行:

 ![image-20220925165115645](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220925165115645.png)

## 三、电阻屏和电容屏



## 四、tslib框架分析



## 五、tslib测试

### Step1:配置交叉编译工具链

```shell
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=$PATH:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin
 
```

- 疑问点：为什么不是buildroot的编译链了？

### Step2:交叉编译tslib

```shell
tslib-1.21$ ./configure --host=arm-linux-gnueabihf --prefix=/
make
make install DESTDIR=$PWD/tmp
 
```

### step3:确定工具链中头文件库文件目录

```shell
echo 'main(){}'| arm-linux-gnueabihf-gcc -E -v -
 
```

返回日志：

```shell
Using built-in specs.
COLLECT_GCC=arm-linux-gnueabihf-gcc
Target: arm-linux-gnueabihf
Configured with: /home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/snapshots/gcc-linaro-6.2-2016.11/configure SHELL=/bin/bash --with-mpc=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/builds/destdir/x86_64-unknown-linux-gnu --with-mpfr=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/builds/destdir/x86_64-unknown-linux-gnu --with-gmp=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/builds/destdir/x86_64-unknown-linux-gnu --with-gnu-as --with-gnu-ld --disable-libstdcxx-pch --disable-libmudflap --with-cloog=no --with-ppl=no --with-isl=no --disable-nls --enable-c99 --enable-gnu-indirect-function --disable-multilib --with-tune=cortex-a9 --with-arch=armv7-a --with-fpu=vfpv3-d16 --with-float=hard --with-mode=thumb --enable-multiarch --with-build-sysroot=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf --enable-lto --enable-linker-build-id --enable-long-long --enable-shared --with-sysroot=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/builds/destdir/x86_64-unknown-linux-gnu/arm-linux-gnueabihf/libc --enable-languages=c,c++,fortran,lto --enable-checking=release --disable-bootstrap --build=x86_64-unknown-linux-gnu --host=x86_64-unknown-linux-gnu --target=arm-linux-gnueabihf --prefix=/home/tcwg-buildslave/workspace/tcwg-make-release/label/docker-trusty-amd64-tcwg-build/target/arm-linux-gnueabihf/_build/builds/destdir/x86_64-unknown-linux-gnu
Thread model: posix
gcc version 6.2.1 20161016 (Linaro GCC 6.2-2016.11)
COLLECT_GCC_OPTIONS='-E' '-v' '-march=armv7-a' '-mtune=cortex-a9' '-mfloat-abi=hard' '-mfpu=vfpv3-d16' '-mthumb' '-mtls-dialect=gnu'
 /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../libexec/gcc/arm-linux-gnueabihf/6.2.1/cc1 -E -quiet -v -imultilib . -imultiarch arm-linux-gnueabihf -iprefix /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/ -isysroot /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc - -march=armv7-a -mtune=cortex-a9 -mfloat-abi=hard -mfpu=vfpv3-d16 -mthumb -mtls-dialect=gnu
ignoring duplicate directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/../../lib/gcc/arm-linux-gnueabihf/6.2.1/include"
ignoring nonexistent directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/local/include/arm-linux-gnueabihf"
ignoring nonexistent directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/local/include"
ignoring duplicate directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/../../lib/gcc/arm-linux-gnueabihf/6.2.1/include-fixed"
ignoring duplicate directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/../../lib/gcc/arm-linux-gnueabihf/6.2.1/../../../../arm-linux-gnueabihf/include"
ignoring nonexistent directory "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include/arm-linux-gnueabihf"
#include "..." search starts here:
#include <...> search starts here:
 /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/include
 /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/include-fixed
 /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/../../../../arm-linux-gnueabihf/include
 /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/include
End of search list.
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "<stdin>"
main(){}
COMPILER_PATH=/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../libexec/gcc/arm-linux-gnueabihf/6.2.1/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../libexec/gcc/arm-linux-gnueabihf/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../libexec/gcc/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/../../../../arm-linux-gnueabihf/bin/
LIBRARY_PATH=/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/6.2.1/../../../../arm-linux-gnueabihf/lib/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/lib/:/home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc/usr/lib/
COLLECT_GCC_OPTIONS='-E' '-v' '-march=armv7-a' '-mtune=cortex-a9' '-mfloat-abi=hard' '-mfpu=vfpv3-d16' '-mthumb' '-mtls-dialect=gnu'
```

### step4:把头文件库文件放到工具链目录下

```shell
cp include/*  /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/
cp -d lib/*so* /home/book/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/
```

### step5:测试tslib

