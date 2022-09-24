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
  
  	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
  	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
  	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
  	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
  	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
  	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
  	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
  	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
  	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];
  	。。。。//省略一堆元素
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

     ![image-20220924202906240](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220924202906240.png)

- 
