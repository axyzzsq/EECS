# LED驱动框架演进

## 第1节 LED Drv Simple

### led_drv.c

```C
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>

static int major;
static struct class *led_class;

/* registers */
// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

// GPIO5_GDIR 地址：0x020AC004
static volatile unsigned int *GPIO5_GDIR;

//GPIO5_DR 地址：0x020AC000
static volatile unsigned int *GPIO5_DR;

static ssize_t led_write(struct file *filp, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	char val;
	int ret;
	
	/* copy_from_user : get data from app */
	ret = copy_from_user(&val, buf, 1);

	/* to set gpio register: out 1/0 */
	if (val)
	{
		/* set gpio to let led on */
		*GPIO5_DR &= ~(1<<3);
	}
	else
	{

		/* set gpio to let led off */
		*GPIO5_DR |= (1<<3);
	}
	return 1;
}

static int led_open(struct inode *inode, struct file *filp)
{
	/* enable gpio5
	 * configure gpio5_io3 as gpio
	 * configure gpio5_io3 as output 
	 */
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;

	*GPIO5_GDIR |= (1<<3);
	
	return 0;
}

static struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.write		= led_write,
	.open		= led_open,
};

/* 入口函数 */
static int __init led_init(void)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	major = register_chrdev(0, "100ask_led", &led_fops);

	/* ioremap */
	// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 地址：0x02290000 + 0x14
	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290000 + 0x14, 4);
	
	// GPIO5_GDIR 地址：0x020AC004
	GPIO5_GDIR = ioremap(0x020AC004, 4);
	
	//GPIO5_DR 地址：0x020AC000
	GPIO5_DR  = ioremap(0x020AC000, 4);
	
    /*
     * 宏class_create()用于动态创建设备的逻辑类，并完成部分字段的初始化，然后将其添加进Linux内核系统中。此函数的执行效	 * 果就是在/sys/class/目录下创建一个新的文件夹，此文件夹的名字为此函数的第二个输入参数，但此文件夹是空的。
     * 宏class_create()在实现时，调用了函数__class_create()。
     */
	led_class = class_create(THIS_MODULE, "myled");
    
    /*
     *  在逻辑类下创建创建设备/dev/myled
     */
	device_create(led_class, NULL, MKDEV(major, 0), NULL, "myled"); /* /dev/myled */
	
	return 0;
}

static void __exit led_exit(void)
{
	iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	iounmap(GPIO5_GDIR);
	iounmap(GPIO5_DR);
	
	device_destroy(led_class, MKDEV(major, 0));
    
    /* 函数class_destroy()用于删除设备的逻辑类，即从Linux内核系统中删除设备的逻辑类。此函数执行的效果是删除函数			 * __class_create()或宏class_create()在/sys/class/目录下创建的逻辑类对应的文件夹。
     */
	class_destroy(led_class);
	
	unregister_chrdev(major, "100ask_led");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");



```

[参考文献]

- [Linux驱动device_create创建字符设备文件](https://blog.csdn.net/hwx1546/article/details/123053496?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166584147616782414916866%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166584147616782414916866&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-123053496-null-null.142^v56^pc_search_v3,201^v3^control_1&utm_term=device_create&spm=1018.2226.3001.4187)

- [Linux内核API之class_create与class_destroy](https://blog.csdn.net/phmatthaus/article/details/124473300?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166584155616782388081858%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166584155616782388081858&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-1-124473300-null-null.142^v56^pc_search_v3,201^v3^control_1&utm_term=%E5%87%BD%E6%95%B0class_destroy&spm=1018.2226.3001.4187)

  

 ### ledtest.c

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


// ledtest /dev/myled on
// ledtest /dev/myled off

int main(int argc, char **argv)
{
	int fd;
	char status = 0;
	
	if (argc != 3)
	{
		printf("Usage: %s <dev> <on|off>\n", argv[0]);
		printf("  eg: %s /dev/myled on\n", argv[0]);
		printf("  eg: %s /dev/myled off\n", argv[0]);
		return -1;
	}
	// open
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("can not open %s\n", argv[0]);
		return -1;
	}

	// write
	if (strcmp(argv[2], "on") == 0)
	{
		status = 1;
	}

	write(fd, &status, 1);
	return 0;	
}


```



### Makefile

```makefile

# 1. 使用不同的开发板内核时, 一定要修改KERN_DIR
# 2. KERN_DIR中的内核要事先配置、编译, 为了能编译内核, 要先设置下列环境变量:
# 2.1 ARCH,          比如: export ARCH=arm64
# 2.2 CROSS_COMPILE, 比如: export CROSS_COMPILE=aarch64-linux-gnu-
# 2.3 PATH,          比如: export PATH=$PATH:/home/book/100ask_roc-rk3399-pc/ToolChain-6.3.1/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin 
# 注意: 不同的开发板不同的编译器上述3个环境变量不一定相同,
#       请参考各开发板的高级用户使用手册

KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88/

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o ledtest ledtest.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f ledtest

obj-m	+= led_drv.o


```

### 编译

![image-20221016143249704](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221016143249704.png)

### 板端运行

```shell
# 安装驱动
insmod led_drv.ko

# 查看驱动
lsmod

# 查看设备
ls /dev/myled

# 执行测试
./ledtest /dev/myled on
./ledtest /dev/myled off

```





