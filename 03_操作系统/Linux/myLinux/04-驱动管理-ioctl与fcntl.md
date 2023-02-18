# ioctl与fcntl

## 一、ioctl

`ioctl`是设备驱动程序中对设备的I/O通道进行管理的函数，它是一个系统调用。

对I/O通道进行管理，就是对设备的一些特性进行控制，例如串口的传输波特率、马达的转速等等。

### 1、Application Usage

```C
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int main(int argc, char *argv[]) {
    int fd;
    struct v4l2_capability cap;
    int ret;

    fd = open("/dev/video0", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        perror("Failed to query capabilities");
        return -1;
    }

    printf("Driver: %s\n", cap.driver);
    printf("Card: %s\n", cap.card);
    printf("Bus info: %s\n", cap.bus_info);
    printf("Capabilities: %08x\n", cap.capabilities);

    close(fd);
    return 0;
}

```

此应用程序代码打开视频设备 `/dev/video0`，使用 `VIDIOC_QUERYCAP` 查询其功能，并将设备的一些属性打印到控制台。

### 2、Kernel Usage

```c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/videodev2.h>

#define MY_DEVICE_NAME "mydevice"
#define MY_DEVICE_CLASS "mydevice_class"

static dev_t my_dev;
static struct cdev my_cdev;
static struct class *my_class;

static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct v4l2_capability cap;
    int ret;

    switch (cmd) {
        case VIDIOC_QUERYCAP:
            memset(&cap, 0, sizeof(struct v4l2_capability));
            strcpy(cap.driver, "mydriver");
            strcpy(cap.card, "mycard");
            strcpy(cap.bus_info, "mybusinfo");
            cap.capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING;
            ret = copy_to_user((void __user *)arg, &cap, sizeof(struct v4l2_capability));
            if (ret) {
                return -EFAULT;
            }
            return 0;
        default:
            return -ENOTTY;
    }
}

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl,
};

static int __init my_init(void) {
    int ret;

    ret = alloc_chrdev_region(&my_dev, 0, 1, MY_DEVICE_NAME);
    if (ret < 0) {
        return ret;
    }

    cdev_init(&my_cdev, &my_fops);
    my_cdev.owner = THIS_MODULE;
    my_cdev.ops = &my_fops;

    ret = cdev_add(&my_cdev, my_dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(my_dev, 1);
        return ret;
    }

    my_class = class_create(THIS_MODULE, MY_DEVICE_CLASS);
    if (IS_ERR(my_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(my_dev, 1);
        return PTR_ERR(my_class);
    }

    device_create(my_class, NULL, my_dev, NULL, MY_DEVICE_NAME);
    return 0;
}

	static void __exit my_exit(void) {
        device_destroy(my_class, my_dev);
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(my_dev, 1);

}

    module_init(my_init);
    module_exit(my_exit);

    MODULE_LICENSE("Dual MIT/GPL");
```

此内核驱动程序代码实现了一个字符设备`/dev/mydevice`，并使用预定义的功能结构响应`VIDIOC_QUERYCAP` `ioctl `命令。 当应用程序使用 `ioctl` 向该驱动程序发送`VIDIOC_QUERYCAP` 命令时，该驱动程序将其自己的功能结构复制到用户空间并返回成功。

在实践中，您需要实施更多的 `ioctl` 命令来处理视频捕获和流式传输操作。



## 二、fcntl

