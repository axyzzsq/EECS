# FreeRTOS定时器

[博文路径](https://blog.csdn.net/baidu_19348579/article/details/122196101)

FreeRTOS提供了一种软件定时器，用来快速实现一些周期性的操作，并且节约了硬件定时器。不过尽量还是不要做过多的操作，以免影响其他定时器的准度。

## demo

演示了定时器定义，回调函数会每一秒进入一次，等到10次之后，定时器被销毁。

```C
TimerHandle_t xTimerUser; // 定义句柄

// 定时器回调函数格式
void vTimerCallback( TimerHandle_t xTimer )
{
	// do something no block
	// 获取溢出次数
	static unsigned int ulCount =0;
	
	ulCount= ( unsigned int ) pvTimerGetTimerID( xTimer );
	// 累积溢出次数
	++ulCount; 
	// 更新溢出次数
	vTimerSetTimerID( xTimer, ( void * ) ulCount );
	printf("get timer ulCount:%d\n",ulCount);
	if(ulCount == 10) 
	{
		// 停止定时器
		xTimerStop( xTimer, 0 );
	}
}

void app_main()
{
	// 申请定时器， 配置
	xTimerUser = xTimerCreate("Timer's name",/*调试用， 系统不用*/
								1000 / portTICK_PERIOD_MS, /*定时溢出周期， 单位是任务节拍数*/  
								pdTRUE,/*是否自动重载， 此处设置周期性执行*/
								( void * ) 0,/*记录定时器溢出次数， 初始化零, 用户自己设置*/
								vTimerCallback);/*回调函数*/

	if( xTimerUser != NULL ) 
	{
		// 启动定时器， 0 表示不阻塞
		xTimerStart( xTimerUser, 0 );
	}
}

```

## 主要函数

### 定时器创建

```C
TimerHandle_t xTimerCreate(const char * const pcTimerName,
                           const TickType_t xTimerPeriodInTicks,
                           const UBaseType_t uxAutoReload,
                           void * const pvTimerID,
                           TimerCallbackFunction_t pxCallbackFunction )

```

![image-20220330203405671](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203405671.png)

### 定时器启动

```C
BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait )
```

![image-20220330203444427](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203444427.png)

### 定时器停止

```C
BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait )
```

![image-20220330203511279](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203511279.png)

### 定时器删除

```C
BaseType_t xTimerDelete( TimerHandle_t xTimer, TickType_t xTicksToWait )
```

![image-20220330203536966](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203536966.png)

### 定时器周期修改

```C
BaseType_t xTimerChangePeriod( 	TimerHandle_t xTimer, TickType_t xNewPeriod, TickType_t xTicksToWait )

```

![image-20220330203608488](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203608488.png)

### 定时器重启

```C
BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait )
```

![image-20220330203641337](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330203641337.png)

还有一系列的中断中使用的函数

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/ac6f20bdf7e84483a9aae400adb4ec93.png)