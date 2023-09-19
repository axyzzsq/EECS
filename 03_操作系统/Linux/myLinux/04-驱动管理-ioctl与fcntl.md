# ioctl与fcntl

## 一、ioctl

`ioctl`是设备驱动程序中对设备的I/O通道进行管理的函数，它是一个系统调用。

对I/O通道进行管理，就是对设备的一些特性进行控制，例如串口的传输波特率、马达的转速等等。

### 1、Application Usage

[github-**ioctl_app.c**](https://github1s.com/axyzzsq/Code_For_EECS/blob/main/02_%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F/Linux/myLinux/ioctl_usage/ioctl_app.c)

```C
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int main(int argc, char *argv[]) 
{
    int fd;
    struct v4l2_capability cap;
    int ret;

    fd = open("/dev/mydevice", O_RDWR);
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

static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) 
{
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

在实践中，需要实施更多的 `ioctl` 命令来处理视频捕获和流式传输操作。

#### 知识点

##### alloc_chrdev_region()

`alloc_chrdev_region()` is a kernel function that is used to dynamically allocate a range of character device numbers. The function prototype is defined in the `linux/fs.h` header file as follows:

```C
int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count,const char *name);
```

The function takes four arguments:

- `dev`: a pointer to a variable of type `dev_t` that will be used to return the first allocated device number in the range.
- `firstminor`: the first minor number to allocate within the range.
- `count`: the number of device numbers to allocate within the range.
- `name`: a name string to associate with the device range.

The function returns 0 on success, or a negative error code on failure.

In the given code, `alloc_chrdev_region()` is called with the following arguments:

- `&my_dev`: a pointer to a `dev_t` variable that will store the first allocated device number in the range.
- `0`: the first minor number to allocate within the range. In this case, the minor number is set to 0.
- `1`: the number of device numbers to allocate within the range. In this case, only one device number is allocated.
- `MY_DEVICE_NAME`: a name string to associate with the device range. In this case, the name is set to "mydevice".

The return value of `alloc_chrdev_region()` is stored in the `ret` variable. If the function returns a negative value, it indicates that the allocation failed and an appropriate error code is returned. If the function returns 0, it indicates that the allocation succeeded and the allocated device number is stored in `my_dev`.

##### cdev_init()

`cdev_init()` is a function provided by the Linux kernel that initializes a character device structure with the specified file operations.

In the code you provided, `&my_fops` is a pointer to a `struct file_operations` instance, which defines the file operations that will be performed on the device. `cdev_init()` takes this pointer along with a pointer to a `struct cdev` instance (`&my_cdev` in this case), and initializes the `cdev` structure with the specified file operations.

The `cdev` structure represents the character device driver, and is used to manage the device's file operations and internal state. Once initialized with `cdev_init()`, the `cdev` structure can be registered with the kernel using `cdev_add()`. This makes the driver's functionality available to user-space programs.

##### cdev_add()

`cdev_add()` is a function provided by the Linux kernel that adds a character device to the system.

In the code you provided, `&my_cdev` is a pointer to a `struct cdev` instance, which represents the character device driver. `my_dev` is the device number obtained from the kernel with `alloc_chrdev_region()`, and `1` is the number of consecutive minor numbers that are requested.

`cdev_add()` takes these parameters and adds the character device represented by `my_cdev` to the kernel, making it available to user-space programs. The return value of `cdev_add()` indicates whether the operation was successful or not. If the return value is negative, then an error occurred and the device should be unregistered with `cdev_del()` and `unregister_chrdev_region()`.



## 二、fcntl

