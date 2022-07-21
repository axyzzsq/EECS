# FreeRTOS的事件组

[博文路径](https://blog.csdn.net/baidu_19348579/article/details/122192402)

FreeRTOS有一个进程同步的组件，就是event_group事件组。

- 事件是一种实现任务间通信的机制，主要用于实现多任务间的同步，但事件通信只能是事件类型的通信，无数据传输。
- 与信号量不同的是，它可以实现一对多，多对多的同步。即一个任务可以等待多个事件的发生：可以是任意一个事件发生时唤醒任务进行事件处理；也可以是几个事件都发生后才唤醒任务进行事件处理。同样，也可以是多个任务同步多个事件。

就像是一个全局变量，每一位表示一个事件，通过事件位的设置和读取，来同步最多24类消息，允许一个或者多个任务来设置和读取，例如一些按键操作，按下不同的按键，来启动或者控制不同的任务进行。

通过配置宏定义，来决定了同步数据类型的多少

```C
#define configUSE_16_BIT_TICKS		0

#if configUSE_16_BIT_TICKS == 1
	#define eventCLEAR_EVENTS_ON_EXIT_BIT	0x0100U
	#define eventUNBLOCKED_DUE_TO_BIT_SET	0x0200U
	#define eventWAIT_FOR_ALL_BITS			0x0400U
	#define eventEVENT_BITS_CONTROL_BYTES	0xff00U
#else
	#define eventCLEAR_EVENTS_ON_EXIT_BIT	0x01000000UL
	#define eventUNBLOCKED_DUE_TO_BIT_SET	0x02000000UL
	#define eventWAIT_FOR_ALL_BITS			0x04000000UL
	#define eventEVENT_BITS_CONTROL_BYTES	0xff000000UL
#endif
```

当configUSE_16_BIT_TICKS==0的时候，整个数据是4个字节，消息占据了三个字节，一共是24种

当configUSE_16_BIT_TICKS==1的时候，整个数据是2个字节，消息占据了一个字节，一共是8种。
这块是猜测的，不知道对不对——待考证

## demo程序

```C
#define send_1 (0x01 << 0)
#define send_2 (0x01 << 1)
#define send_3 (0x01 << 2)
#define send_4 (0x01 << 3)
 
static EventGroupHandle_t g_event_group;
 
static void setevent_task(void *pvParameter)
{
    while (1)
    {
        xEventGroupSetBits(g_event_group, send_1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xEventGroupSetBits(g_event_group, send_2);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xEventGroupSetBits(g_event_group, send_3);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xEventGroupSetBits(g_event_group, send_4);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
     
}
 
void readevent_task()
{
    EventBits_t uxBits;

    while (1)
    {
        uxBits = xEventGroupWaitBits(g_event_group, send_1 | send_2 | send_3 | send_4, true, false, portMAX_DELAY);
		if (uxBits & send_1)
        {
			 printf("get 1 \n");
        }
        if (uxBits & send_2)
        {
			printf("get 2\n");
        }
        if (uxBits & send_3)
        {
			printf("get 3 \n");
        }
        if (uxBits & send_4)
        {
			printf("get 4 \n");
        }
		vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}
 
void app_main()
{
	g_event_group = xEventGroupCreate();    
	xTaskCreate(&readevent_task, "readevent_task", 2048, NULL, 5, NULL);
	xTaskCreate(&setevent_task, "setevent_task", 2048, NULL, 5, NULL);
}
```

## 重点函数

### 创建事件组

```C
EventGroupHandle_t xEventGroupCreate( void )
```

 ![image-20220401232109848](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220401232109848.png)

### 设置事件组

```C
EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet )
```

 ![image-20220401232201979](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220401232201979.png)

### 清除事件组

```C
EventBits_t xEventGroupClearBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear )
```

 ![image-20220401232238004](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220401232238004.png)

### 读取事件组

```C
EventBits_t xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
								const EventBits_t uxBitsToWaitFor,
								const BaseType_t xClearOnExit,
								const BaseType_t xWaitForAllBits,
								TickType_t xTicksToWait )
```

 ![image-20220401232328752](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220401232328752.png)

### 删除事件组

```C
void vEventGroupDelete( EventGroupHandle_t xEventGroup )
```

 ![image-20220401232427646](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220401232427646.png)
