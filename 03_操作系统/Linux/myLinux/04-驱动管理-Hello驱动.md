# Hello驱动

## 一、关键结构体和API

驱动层：

```C
static int __init hello_init(void)
static void __exit hello_exit(void)
    
//修饰入口函数出口函数
module_init(hello_init);
module_exit(hello_exit);
    
struct file_operations 
struct file   

register_chrdev
unregister_chrde
```

- struct file_operations

  ```C
  struct file_operations {
  	struct module *owner;
  	loff_t (*llseek) (struct file *, loff_t, int);
  	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
  	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
  	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
  	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
  	int (*iterate) (struct file *, struct dir_context *);
  	int (*iterate_shared) (struct file *, struct dir_context *);
  	unsigned int (*poll) (struct file *, struct poll_table_struct *);
  	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
  	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
  	int (*mmap) (struct file *, struct vm_area_struct *);
  	int (*open) (struct inode *, struct file *);
  	int (*flush) (struct file *, fl_owner_t id);
  	int (*release) (struct inode *, struct file *);
  	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
  	int (*fasync) (int, struct file *, int);
  	int (*lock) (struct file *, int, struct file_lock *);
  	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
  	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
  	int (*check_flags)(int);
  	int (*flock) (struct file *, int, struct file_lock *);
  	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
  	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
  	int (*setlease)(struct file *, long, struct file_lock **, void **);
  	long (*fallocate)(struct file *file, int mode, loff_t offset,
  			  loff_t len);
  	void (*show_fdinfo)(struct seq_file *m, struct file *f);
  #ifndef CONFIG_MMU
  	unsigned (*mmap_capabilities)(struct file *);
  #endif
  	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
  			loff_t, size_t, unsigned int);
  	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
  			u64);
  	ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
  			u64);
  };
  ```

  - 确定主设备号`major = register_chrdev(0, "hello", &hello_drv);  /* /dev/hello */`
    - 第一个参数如果传入0，则由内核分配一个主设备号；也可以自己指定；
    
    - `register_chrdev`把带有操作回调函数的`file_operations`结构体注册到内核的`chrdev[]`，完成这个动作之后，注册到内核的驱动程序具有自己的主设备号;
    
    - 驱动程序创建设备
    
      ```C
      device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello"); /* /dev/hello */ 
      ```
    
      如此，应用层打开/dev下面的设备即为驱动程序创建的设备，返回的fd可以在内核找到对应此次open操作生成的file结构体，在这个结构体中由对应的`file_operations`驱动对应的操作函数。**是这样吗？？？？？那主设备号怎么起作用？**
    
      应用层打开/dev下面的设备，这个设备名本身由驱动程序中指定，插入内核之后生成，设备名与设备号绑定，打开设备名，也就找到了设备号，通过设备号在内核中`chrdev[]`查找到对应的驱动程序`file_operations`的操作函数；

应用层：

```C
fd = open("/dev/hello", O_RDWR);
```

- fd返回的是一个整数，这个整数对应内核一个结构体`struct file`

  路径：/Linux-4.9.88/include/linux/fs.h

  ```C
  struct file {
  	union {
  		struct llist_node	fu_llist;
  		struct rcu_head 	fu_rcuhead;
  	} f_u;
  	struct path		f_path;
  	struct inode		*f_inode;	/* cached value */
  	const struct file_operations	*f_op;
  
  	/*
  	 * Protects f_ep_links, f_flags.
  	 * Must not be taken from IRQ context.
  	 */
  	spinlock_t		f_lock;
  	atomic_long_t		f_count;
  	unsigned int 		f_flags;
  	fmode_t			f_mode;
  	struct mutex		f_pos_lock;
  	loff_t			f_pos;
  	struct fown_struct	f_owner;
  	const struct cred	*f_cred;
  	struct file_ra_state	f_ra;
  
  	u64			f_version;
  #ifdef CONFIG_SECURITY
  	void			*f_security;
  #endif
  	/* needed for tty driver, and maybe others */
  	void			*private_data;
  
  #ifdef CONFIG_EPOLL
  	/* Used by fs/eventpoll.c to link all the hooks to this file */
  	struct list_head	f_ep_links;
  	struct list_head	f_tfile_llink;
  #endif /* #ifdef CONFIG_EPOLL */
  	struct address_space	*f_mapping;
  } __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */
  
  struct file_handle {
  	__u32 handle_bytes;
  	int handle_type;
  	/* file identifier */
  	unsigned char f_handle[0];
  };
  ```

  - `struct file`结构体中嵌套了一个`struct file_operations`结构体

## 二、如何写？

- 常规SOP

  ![image-20221009115405263](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009115405263.png)

- 经典例程misc.c驱动程序-CV大法

## 三、例程

### 1、驱动

hello_drv.c

```C
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

/* 1. 确定主设备号                                                                 */
static int major = 0;
static char kernel_buf[1024];
static struct class *hello_class;


#define MIN(a, b) (a < b ? a : b)

/* 3. 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	int err;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_to_user(buf, kernel_buf, MIN(1024, size));
	return MIN(1024, size);
}

static ssize_t hello_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(kernel_buf, buf, MIN(1024, size));
	return MIN(1024, size);
}

static int hello_drv_open (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static int hello_drv_close (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* 2. 定义自己的file_operations结构体                                              */
static struct file_operations hello_drv = {
	.owner	 = THIS_MODULE,
	.open    = hello_drv_open,
	.read    = hello_drv_read,
	.write   = hello_drv_write,
	.release = hello_drv_close,
};

/* 4. 把file_operations结构体告诉内核：注册驱动程序                                */
/* 5. 谁来注册驱动程序啊？得有一个入口函数：安装驱动程序时，就会去调用这个入口函数 */
static int __init hello_init(void)
{
	int err;
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "hello", &hello_drv);  /* /dev/hello */

/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */
	hello_class = class_create(THIS_MODULE, "hello_class");
	err = PTR_ERR(hello_class);
	if (IS_ERR(hello_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "hello");
		return -1;
	}
	
	device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello"); /* /dev/hello */ 
	
	return 0;
}

/* 6. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数           */
static void __exit hello_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(hello_class, MKDEV(major, 0));
	class_destroy(hello_class);
	unregister_chrdev(major, "hello");
}




module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

```

- 在应用层调用glibc的`write`的时候会把数据内存传递给驱动层的`copy_from_user`的`const char __user *buf`，再由`__user`修饰的内存段把数据拷贝给在驱动层定义的数组，这样数据才能从应用层传给驱动层；
- 反向传输数据也是一样；由`copy_from_user`的一个参数提供内存段来转递数据。

### 2、应用

hello_drv_test.c

```C

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * ./hello_drv_test -w abc
 * ./hello_drv_test -r
 */
int main(int argc, char **argv)
{
	int fd;
	char buf[1024];
	int len;
	
	/* 1. 判断参数 */
	if (argc < 2) 
	{
		printf("Usage: %s -w <string>\n", argv[0]);
		printf("       %s -r\n", argv[0]);
		return -1;
	}

	/* 2. 打开文件 */
	fd = open("/dev/hello", O_RDWR);
	if (fd == -1)
	{
		printf("can not open file /dev/hello\n");
		return -1;
	}

	/* 3. 写文件或读文件 */
	if ((0 == strcmp(argv[1], "-w")) && (argc == 3))
	{
		len = strlen(argv[2]) + 1;
		len = len < 1024 ? len : 1024;
		write(fd, argv[2], len);
	}
	else
	{
		len = read(fd, buf, 1024);		
		buf[1023] = '\0';
		printf("APP read : %s\n", buf);
	}
	close(fd);
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

KERN_DIR = /home/book/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules  
	$(CROSS_COMPILE)gcc -o hello_drv_test hello_drv_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f hello_drv_test

obj-m	+= hello_drv.o  # 指定了hello_drv.c最重要生成出一个hello_drv.ko
```

驱动编译参考：

- [【Kernel】驱动编译的两种方式：编译成模块、编译进内核（使用杂项设备驱动模板）](https://blog.csdn.net/qq_43581670/article/details/125163371?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166530165416782391858367%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166530165416782391858367&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~pc_rank_34-2-125163371-null-null.142^v52^pc_rank_34_queryrelevant25,201^v3^add_ask&utm_term=%E7%BC%96%E8%AF%91%E8%BF%9B%E5%86%85%E6%A0%B8%E7%9A%84%E9%A9%B1%E5%8A%A8%E5%A6%82%E4%BD%95%E5%8D%B8%E8%BD%BD&spm=1018.2226.3001.4187)
- [.c文件如何编译为ko的MAKEFILE文件编写](https://blog.csdn.net/yangbingzhou/article/details/51177066?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166529772916800182783749%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166529772916800182783749&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~pc_rank_34-2-51177066-null-null.142^v52^pc_rank_34_queryrelevant25,201^v3^add_ask&utm_term=.ko%E6%96%87%E4%BB%B6%E7%9A%84%E7%BC%96%E8%AF%91%E8%BF%87%E7%A8%8B&spm=1018.2226.3001.4187)

## 四、编译

![image-20221009143805770](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009143805770.png)





## 五、运行

拷贝到单板后

执行`insmod hello_drv.ko`安装驱动程序，`cat /proc/devices`查看内核中设备节点

 ![image-20221009145821596](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009145821596.png)

`ls  /dev/hello -l` 

 ![image-20221009150232575](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009150232575.png)

`lsmod`

 ![image-20221009150123780](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009150123780.png)

卸载驱动程序,`rmmod hello_drv`，不需要加.ko;下图对比，卸载掉了

 ![image-20221009150434529](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009150434529.png)

应用层测试：

 ![image-20221009150606470](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20221009150606470.png)
