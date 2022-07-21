# FreeRTOS信号量机制

[博文路径](https://blog.csdn.net/baidu_19348579/article/details/122174230?spm=1001.2014.3001.5506)

FreeRTOS中任务见的消息传递及同步，都是通过消息队列，也可以通过全局变量来同步，但是全局变量需要在轮询中判断，会造成响应不及时的问题。

FreeRTOS中信号量的实现，也是通过消息队列做到的

## 二进制信号量

二值信号量比较直观，类似于红绿灯，创建的时候，默认是红灯的，其他任务都得不到这个信号量的，只有give之后才能变成绿灯，其他任务才能得到信号量，得以运行。（本质上，信号量是用队列实现的）上面是类比，在代码中，创建的时候，初始值是0，give是相当于将计数值设置为1，take后相当于将计数值清0。

下面的程序创建了一个二值信号量，例如我们去操作某些硬件，读取I2C之类的，某一时刻，应该是只允许一个任务去操作，无论是读还是写，所以需要用这个信号量去控制一下，并且操作完之后，要及时让出CPU，使得其他任务能够得到信号量。

### demo

```C
SemaphoreHandle_t xSemaphore = NULL;

// A task that uses the semaphore.
void WriteSomethingTask( void * pvParameters )
{
	long lValueToSend; 

	/* 该任务会被创建两个实例，所以写入队列的值通过任务入口参数传递 – 这种方式使得每个实例使用不同的值。
	   队列创建时指定其数据单元为long型，所以把入口参数强制转换为数据单元要求的类型 */ 
	lValueToSend = ( long ) pvParameters; 
	for(;;)
	{
		if( xSemaphore != NULL )
		{
			if( xSemaphoreTake(xSemaphore,( TickType_t ) 10 ) == pdTRUE )
			{
				printf("tast:%ld get signal ,keep it 2 seconds\n",lValueToSend);
				{
					//my work
					vTaskDelay(2000 / portTICK_PERIOD_MS); 
				}
				xSemaphoreGive(xSemaphore);
				taskYIELD(); 
			}
			else
			{
                
			}
		}
	}
}

void app_main(void) 
{ 
	// Semaphore cannot be used before a call to xSemaphoreCreateBinary().
	// This is a macro so pass the variable in directly.
	xSemaphore = xSemaphoreCreateBinary();
	
	if( xSemaphore != NULL )
	{
		xSemaphoreGive(xSemaphore);
		xTaskCreate(WriteSomethingTask, "write1", 1000, ( void * ) 1, 1, NULL ); 
		xTaskCreate(WriteSomethingTask, "write2", 1000, ( void * ) 2, 1, NULL ); 
		xTaskCreate(WriteSomethingTask, "write3", 1000, ( void * ) 3, 1, NULL ); 
	}
}

```

### 创建二值信号

```C
SemaphoreHandle_t xSemaphoreCreateBinary( void )
```

![image-20220330191728570](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330191728570.png)

### 二值信号减一

```C
portBASE_TYPE xSemaphoreTake( xSemaphoreHandle xSemaphore, portTickType xTicksToWait );
```

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330191811190.png)

下面函数专门用于中断下的信号量获取

```C
portBASE_TYPE xSemaphoreGiveFromISR( xSemaphoreHandle xSemaphore, portBASE_TYPE *pxHigherPriorityTaskWoken );
```

![image-20220330191902844](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330191902844.png)

### 二值信号加一

```C
void xSemaphoreGive( SemaphoreHandle_t xSemaphore )
```

![image-20220330191944608](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330191944608.png)

## 多值信号量

在中断不频繁的系统中，使用二值信号量没有问题，但是中断频繁发生时，则会有中断丢失的问题。因为中断发生时延迟任务执行，延迟任务执行的过程中，如果又来了两次中断，则只会处理第一次，第二次将会丢失。为此引入多值信号量来处理这个问题。

### demo

```C
SemaphoreHandle_t xSemaphore = NULL;

// A task that uses the semaphore.
void WriteSomethingTask1( void * pvParameters )
{
	int countme=0;
	for(;;)
	{
		printf("write1 %d\n",countme++);
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
		taskYIELD(); 
	}
}
// A task that uses the semaphore.
void WriteSomethingTask2( void * pvParameters )
{
	int countme=0;
	for(;;)
	{
		printf("write2 %d\n",countme++);
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
		taskYIELD(); 
	}
}

// A task that uses the semaphore.
void ReadSomethingTask( void * pvParameters )
{
	int countme=0;
	for(;;)
	{
		if(xSemaphoreTake(xSemaphore,( TickType_t ) 0 ) == pdTRUE )
		{
			printf("read %d\n",countme++);
		}
		else
		{
		}
		vTaskDelay(10 / portTICK_PERIOD_MS); 
		taskYIELD(); 
	}
}


void app_main(void) 
{ 

    xSemaphore = xSemaphoreCreateCounting( 10, 0 );
	
	if( xSemaphore != NULL )
	{
		xTaskCreate(WriteSomethingTask1, "write1", 1000, NULL, 1, NULL ); 
		xTaskCreate(WriteSomethingTask2, "write2", 1000, NULL, 1, NULL ); 
		xTaskCreate(ReadSomethingTask, "read", 1000, NULL, 1, NULL ); 
	}
}

```

### 多值信号量的创建

```
SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount )
```

![image-20220330190501536](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330190501536.png)

其他操作与二值信号量函数一样。

## 互斥信号量

互斥量是一种特殊的二值信号量，用于控制在两个或多个任务间访问共享资源。单词MUTEX(互斥量)源于”MUTual EXclusion”。
在用于互斥的场合，互斥量从概念上可看作是与共享资源关联的令牌。一个任务想要合法地访问资源，其必须先成功地得到(Take)该资源对应的令牌(成为令牌持有者)。当令牌持有者完成资源使用，其必须马上归还(Give)令牌。只有归还了令牌，其它任务才可能成功持有，也才可能安全地访问该共享资源。一个任务除非持有了令牌，否则不允许访问共享资源。

如果某资源同时只准一个任务访问，可以用互斥量保护这个资源。这个资源一定是存在的，所以创建互斥量时会先释放一个互斥量，表示这个资源可以使用。任务想访问资源时，先获取互斥量，等使用完资源后，再释放它。也就是说互斥量一旦创建好后，要先获取，后释放，要在同一个任务中获取和释放。这也是互斥量和二进制信号量的一个重要区别，二进制信号量可以在随便一个任务中获取或释放，然后也可以在任意一个任务中释放或获取。互斥量不同于二进制信号量的还有：互斥量具有优先级继承机制，二进制信号量没有，互斥量不可以用于中断服务程序，二进制信号量可以。

简单说就是谁用谁释放。

### demo

两个写任务，只有一个能写临界资源，所以用互斥信号量来控制。自己抢自己释放，这就是互斥。

```C
SemaphoreHandle_t xSemaphore = NULL;

void WriteSomethingTask1( void * pvParameters )
{
	for(;;)
	{
		xSemaphoreTake(xSemaphore,portMAX_DELAY);
		printf("WriteSomethingTask1 work\n");
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
	}

}
void WriteSomethingTask2( void * pvParameters )
{
	for(;;)
	{
		xSemaphoreTake(xSemaphore,portMAX_DELAY);
		printf("WriteSomethingTask2 work\n");
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000 / portTICK_PERIOD_MS); 
	}

}


void app_main(void) 
{ 

    xSemaphore = xSemaphoreCreateMutex();
	
	if( xSemaphore != NULL )
	{
		xSemaphoreGive(xSemaphore);
		xTaskCreate(WriteSomethingTask1, "write1", 1000, NULL, 1, NULL ); 
		xTaskCreate(WriteSomethingTask2, "write2", 1000, NULL, 1, NULL ); 
	}
}

```

### 互斥信号量创建

```C
SemaphoreHandle_t xSemaphoreCreateMutex( void )
```

![image-20220330190634868](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220330190634868.png)

## 异常错误

在某次测试中，发现这个看门狗的报错，随机出现了程序重启，这个是因为有任务一直在运行，导致看门狗没有喂养，所以要保证每个任务，都有适当的休息时间。

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/abb387b46bc348908ba886a660569bc4.png)