# FreeRTOS的线程同步机制

## 一、同步机制

为了实现线程间的同步与通信，FreeRTOS提供了以下机制。

==什么是同步？==

处理竞争的过程，安排进程的先后执行顺序就是同步，每个进程都有一定的先后执行顺序。

| 机制     | 区别                                                         | 应用场景                                                     |
| :------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 信号量   | 二值信号量只有两种状态，0或1，用来通知事件的发生或者保护共享资源。<br />计数信号量有一个计数值，用来表示可用资源的数量或者等待事件的任务数量。 | 二值信号量可以用来实现互斥锁或者二元同步。<br />计数信号量可以用来实现资源管理或者多元同步。 |
| 互斥量   | 一种特殊的二值信号量，用来实现资源管理和优先级继承。<br />当一个线程获取互斥量时，它会继承互斥量所属的最高优先级，以避免优先级反转问题。 | 互斥量可以用来保护共享资源的访问，例如打印机、文件、内存等。 |
| 事件组   | 一种位操作的同步机制，可以用来通知线程多个事件的发生或者等待多个事件的条件。<br />每个事件组有8个位，每个位代表一个事件或者一个条件。 | 事件组可以用来实现复杂的同步逻辑，例如等待任意事件、等待所有事件、设置或清除事件等。 |
| 消息队列 | 一种数据结构，可以用来传递消息或者数据包。<br />每个消息队列有一个缓冲区，用来存储消息或者数据包，并且有一个长度限制。 | 消息队列可以用来实现线程之间的数据交换，例如传感器数据、控制命令、状态信息等。 |

### 信号量

#### 1.API

`xSemaphoreTake()`和`xSemaphoreGive()`是FreeRTOS中用于访问信号量的API函数。这两个函数都是宏定义，可以用于访问二值信号量、计数信号量、互斥信号量和递归互斥信号量。

`xSemaphoreTake()`函数用于获取一个信号量。如果信号量当前不可用，则该函数会==阻塞==当前任务，直到信号量变为可用为止。该函数的原型如下：

```c
BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );
```

其中，`xSemaphore`参数是一个指向要获取的信号量的句柄；`xTicksToWait`参数是等待信号量变为可用的时间，以系统时钟节拍为单位。如果设置为0，则该函数会立即返回；如果设置为`portMAX_DELAY`，则该函数会一直等待，直到信号量变为可用。

`xSemaphoreGive()`函数用于释放一个信号量。如果有任务正在等待该信号量，则该函数会唤醒其中一个任务。该函数的原型如下：

```c
BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );
```

其中，`xSemaphore`参数是一个指向要释放的信号量的句柄。

#### 2.二值信号量同步

```C
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xSemaphore;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 等待二值信号量
        xSemaphoreTake( xSemaphore, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送二值信号量
        xSemaphoreGive( xSemaphore );
    }
}

int main( void )
{
    // 创建二值信号量
    xSemaphore = xSemaphoreCreateBinary();
    if( xSemaphore == NULL )
    {
        // 二值信号量创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，使用了一个二值信号量来实现任务间的同步。其中，`vTask1`和`vTask2`是两个不同的任务，它们需要相互协作完成某项任务。

在`vTask2`中，通过调用`xSemaphoreGive()`函数来发送一个二值信号量；而在`vTask1`中，通过调用`xSemaphoreTake()`函数来等待这个二值信号量。当`vTask2`发送了这个二值信号量之后，`vTask1`就会被唤醒并开始执行相应的任务。

需要注意的是，在等待二值信号量时，使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当这个二值信号量被发送之后，才会唤醒等待它的任务。



#### 3. 计数信号量同步

```C
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xSemaphore;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 等待计数型信号量
        xSemaphoreTake( xSemaphore, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送计数型信号量
        xSemaphoreGive( xSemaphore );
    }
}

void vTask3( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送计数型信号量
        xSemaphoreGive( xSemaphore );
    }
}

int main( void )
{
    // 创建计数型信号量，最大值为10，初始值为0
    xSemaphore = xSemaphoreCreateCounting( 10, 0 );
    if( xSemaphore == NULL )
    {
        // 计数型信号量创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，==使用了一个计数型信号量来实现多个线程等待==。

其中，`vTask1`、`vTask2`和`vTask3`是三个不同的任务，它们需要相互协作完成某项任务。

在每个任务中，都通过调用`xSemaphoreTake()`函数来等待这个计数型信号量；而在`vTask2`和`vTask3`中，我们通过调用`xSemaphoreGive()`函数来发送这个计数型信号量。当所有的线程都等待了这个计数型信号量之后，它就会被唤醒并开始执行相应的任务。

`xSemaphoreTake()`函数用于获取一个信号量。如果信号量当前不可用，则该函数会阻塞当前任务，直到信号量变为可用为止。在使用计数型信号量时，==如果计数型信号量的值大于0，则该函数不会被阻塞，否则会一直等待，直到计数型信号量的值大于0为止==。因此，只有当计数型信号量的值大于0时，`xSemaphoreTake()`函数才不会被阻塞。

需要注意的是，在等待计数型信号量时，我们使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当这个计数型信号量被发送之后，才会唤醒等待它的线程。

#### 4.对比

下面是一个表格，它展示了二值信号量和计数信号量之间的区别：

| **类型**   | **描述**                        | **值**    |
| :--------- | :------------------------------ | :-------- |
| 二值信号量 | 只有两种状态：0和1              | 0或1      |
| 计数信号量 | 可以有多个状态，从0到某个最大值 | 0到最大值 |

在FreeRTOS中，二值信号量和计数信号量都是用于线程同步和资源共享的机制。

二值信号量只有两种状态：0和1，而计数信号量可以有多个状态，从0到某个最大值。

==这意味着，当使用二值信号量时，只能有一个任务等待该信号量；而当使用计数信号量时，可以有多个任务等待该信号量==。



### 互斥量

```C
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xMutex;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 获取互斥型信号量
        xSemaphoreTake( xMutex, portMAX_DELAY );
        // 执行任务
        // 释放互斥型信号量
        xSemaphoreGive( xMutex );
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 获取互斥型信号量
        xSemaphoreTake( xMutex, portMAX_DELAY );
        // 执行任务
        // 释放互斥型信号量
        xSemaphoreGive( xMutex );
    }
}

int main( void )
{
    // 创建互斥型信号量
    xMutex = xSemaphoreCreateMutex();
    if( xMutex == NULL )
    {
        // 互斥型信号量创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，我们使用了一个互斥型信号量来实现任务间的同步。

其中，`vTask1`和`vTask2`是两个不同的任务，它们需要相互协作完成某项任务。

在每个任务中，都通过调用`xSemaphoreTake()`函数来获取这个互斥型信号量；而在每个任务中执行完相应的任务后，我们通过调用`xSemaphoreGive()`函数来释放这个互斥型信号量。当一个任务获取了这个互斥型信号量之后，另一个任务就无法获取它，直到第一个任务释放了这个互斥型信号量。

需要注意的是，在获取和释放互斥型信号量时，我们都使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当这个互斥型信号量被释放之后，才会唤醒等待它的任务。

#### 二值信号和互斥信号

二值信号量和互斥信号量都是FreeRTOS中用于线程同步和资源共享的机制。二值信号量只有两种状态：0和1，而互斥信号量也只有两种状态，但它具有优先级继承机制，用于线程的互斥访问。在FreeRTOS中，二值信号量和互斥信号量都可以用于线程同步和资源共享，但是在以下情况下，二值信号量无法替代互斥信号量：

- 当多个任务需要访问同一个共享资源时，使用互斥信号量可以保证同一时间只有一个任务能够访问该资源，而使用二值信号量则无法保证这一点。
- 当多个任务需要访问多个共享资源时，使用互斥信号量可以保证对每个共享资源的访问是互斥的，而使用二值信号量则无法实现这一点。

在FreeRTOS中，互斥型信号量是一种用于线程同步和资源共享的机制。

==它与二值型信号量类似，但具有优先级继承机制。这意味着，当一个任务获取了互斥型信号量之后，如果有其他任务试图获取该信号量，那么这些任务的优先级会被提升到当前任务的优先级，以确保它们能够及时获取该信号量。==

### 事件组

#### Trigger Source

事件组中的事件可以由==任务==或==中断服务程序==发送，也可以由其他事件组发送。当一个任务或中断服务程序发送了一个事件时，等待该事件的任务就会被唤醒并开始执行相应的任务。

#### Example Code

使用事件组同步5个线程的示例代码。

```C
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

EventGroupHandle_t xEventGroup;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 等待事件组中的所有事件
        xEventGroupWaitBits( xEventGroup, ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ), pdTRUE, pdTRUE, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送事件组中的某个事件
        xEventGroupSetBits( xEventGroup, ( 1 << 0 ) );
        
        /* 1 << 0是一个位运算符，表示将数字1左移0位，即不移动。这个操作的结果是1。使用了1 << 0来表示事件组中的第0个事件。*/


    }
}

void vTask3( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送事件组中的某个事件
        xEventGroupSetBits( xEventGroup, ( 1 << 1 ) );
    }
}

void vTask4( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送事件组中的某个事件
        xEventGroupSetBits( xEventGroup, ( 1 << 2 ) );
    }
}

void vTask5( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 发送事件组中的某个事件
        xEventGroupSetBits( xEventGroup, ( 1 << 3 ) );
    }
}

int main( void )
{
    // 创建事件组
    xEventGroup = xEventGroupCreate();
    if( xEventGroup == NULL )
    {
        // 事件组创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask4, "Task 4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask5, "Task 5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，我们使用了一个事件组来实现多个线程之间的同步。

其中，`vTask2`、`vTask3`、`vTask4`和`vTask5`是四个不同的任务，它们需要相互协作完成某项任务。

在每个任务中，都通过调用`xEventGroupSetBits()`函数来发送事件组中的某个事件；而在`vTask1`中，通过调用`xEventGroupWaitBits()`函数来等待事件组中的所有事件。当所有的线程都发送了它们需要发送的事件之后，`vTask1`就会被唤醒并开始执行相应的任务。

需要注意的是，在等待事件组中的所有事件时，我们使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当所有的事件都被发送之后，才会唤醒等待它们的线程。

### 消息队列

#### 线程间同步

```C
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t xQueue;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 从消息队列中接收数据
        int data;
        xQueueReceive( xQueue, &data, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 向消息队列中发送数据
        int data = 123;
        xQueueSend( xQueue, &data, portMAX_DELAY );
    }
}

int main( void )
{
    // 创建消息队列
    xQueue = xQueueCreate( 10, sizeof( int ) );
    if( xQueue == NULL )
    {
        // 消息队列创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，使用了一个消息队列来实现线程间的同步。

其中，`vTask1`和`vTask2`是两个不同的任务，它们需要相互协作完成某项任务。

在`vTask1`中，通过调用`xQueueReceive()`函数来接收从消息队列中发送过来的数据；而在`vTask2`中，通过调用`xQueueSend()`函数来向消息队列中发送数据。

当`vTask2`发送了数据之后，`vTask1`就会被唤醒并开始执行相应的任务。

需要注意的是，在接收和发送数据时，我们都使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当数据被接收或发送之后，才会唤醒等待它们的线程。

#### 线程-中断间同步

要在中断和线程之间使用消息队列进行同步和通信，可以使用`xQueueSendFromISR()`函数和`xQueueReceiveFromISR()`函数。这两个函数与`xQueueSend()`函数和`xQueueReceive()`函数类似，但它们是专门用于在中断服务程序中发送和接收数据的。

```C
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t xQueue;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 从消息队列中接收数据
        int data;
        xQueueReceive( xQueue, &data, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 执行任务
        // 向消息队列中发送数据
        int data = 123;
        xQueueSend( xQueue, &data, portMAX_DELAY );
    }
}

void vInterruptHandler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int data = 456;
    // 向消息队列中发送数据
    xQueueSendFromISR( xQueue, &data, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

int main( void )
{
    // 创建消息队列
    xQueue = xQueueCreate( 10, sizeof( int ) );
    if( xQueue == NULL )
    {
        // 消息队列创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 注册中断服务程序
    vPortSetInterruptHandler( vInterruptHandler );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这个示例代码中，使用了一个消息队列来实现线程间的同步。

其中，`vTask1`和`vTask2`是两个不同的任务，它们需要相互协作完成某项任务。

在`vTask1`中，通过调用`xQueueReceive()`函数来接收从消息队列中发送过来的数据；而在`vTask2`中，通过调用`xQueueSend()`函数来向消息队列中发送数据。当`vInterruptHandler`被触发时，通过调用`xQueueSendFromISR()`函数来向消息队列中发送数据。

需要注意的是，在接收和发送数据时，都使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当数据被接收或发送之后，才会唤醒等待它们的线程。

## 二、相关问题点

### 消息和事件的区别

| 机制       | 消息                                                         | 事件                                                      |
| :--------- | :----------------------------------------------------------- | :-------------------------------------------------------- |
| 定义       | 一种数据结构，可以用来传递信息                               | 一种位操作，可以用来通知事件的发生或者等待事件的条件      |
| 存储和传输 | 需要使用消息队列，有缓冲区和长度限制                         | 需要使用事件组，有8个位，每个位代表一个事件或者一个条件。 |
| 功能       | 可以实现数据交换，但不能实现复杂的同步逻辑                   | 可以实现复杂的同步逻辑，但不能实现数据交换                |
| 应用场景   | 可以用在任何任务或中断之间，但发送消息的任务可能需要等待消息队列有空间可用 | 可以用在任何任务或中断之间，且不需要等待事件组可用        |

### 如何改变事件组的优先级

在FreeRTOS中，==事件组的优先级是由等待事件组的任务的优先级决定的==。

当一个任务等待事件组中的某个事件时，如果该任务的优先级高于其他等待该事件的任务的优先级，则该任务会被优先唤醒。因此，要改变事件组的优先级，只需要改变等待事件组的任务的优先级即可。

```C
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

EventGroupHandle_t xEventGroup;
TaskHandle_t xTaskHandle;

void vTask1( void *pvParameters )
{
    while( 1 )
    {
        // 等待事件组中的某个事件
        xEventGroupWaitBits( xEventGroup, ( 1 << 0 ), pdTRUE, pdTRUE, portMAX_DELAY );
        // 执行任务
    }
}

void vTask2( void *pvParameters )
{
    while( 1 )
    {
        // 改变等待事件组的任务的优先级
        vTaskPrioritySet( xTaskHandle, tskIDLE_PRIORITY + 2 );
        // 发送事件组中的某个事件
        xEventGroupSetBits( xEventGroup, ( 1 << 0 ) );
    }
}

int main( void )
{
    // 创建事件组
    xEventGroup = xEventGroupCreate();
    if( xEventGroup == NULL )
    {
        // 事件组创建失败
    }

    // 创建任务
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTaskHandle );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // 启动调度器
    vTaskStartScheduler();

    return 0;
}

```

在这段代码中，使用了一个事件组来实现任务间的同步。

其中，`vTask1`和`vTask2`是两个不同的任务，它们需要相互协作完成某项任务。

在`vTask1`中，通过调用`xEventGroupWaitBits()`函数来等待事件组中的某个事件；而在`vTask2`中，通过调用`vTaskPrioritySet()`函数来改变等待事件组的任务（即`vTask1`）的优先级，并通过调用`xEventGroupSetBits()`函数来发送事件组中的某个事件。当`vTask2`发送了这个事件之后，`vTask1`就会被唤醒并开始执行相应的任务。

需要注意的是，在等待事件组中的某个事件时，使用了`portMAX_DELAY`参数。这个参数表示等待时间无限长，也就是说，只有当这个事件被发送之后，才会唤醒等待它的任务。

### API解释

- `xEventGroupCreate()`是一个FreeRTOS API，用于创建一个事件组。它的原型如下：

```c
EventGroupHandle_t xEventGroupCreate( void );
```

其中，`xEventGroupCreate()`函数返回一个事件组的句柄。

- `vTaskPrioritySet()`是一个FreeRTOS API，用于设置任务的优先级。它的原型如下：

```c
void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority );
```

其中，`xTask`参数是一个指向要设置优先级的任务的句柄；`uxNewPriority`参数是任务的新优先级。

- `xEventGroupSetBits()`是一个FreeRTOS API，用于向事件组中发送一个或多个事件。它的原型如下：

```c
EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet );
```

其中，`xEventGroup`参数是一个指向要发送事件的事件组的句柄；`uxBitsToSet`参数是要发送的事件的位掩码。

- `xEventGroupWaitBits()`是一个FreeRTOS API，用于等待事件组中的一个或多个事件。它的原型如下：

```c
EventBits_t xEventGroupWaitBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, const TickType_t xTicksToWait );
```

其中，`xEventGroup`参数是一个指向要等待事件的事件组的句柄；`uxBitsToWaitFor`参数是要等待的事件的位掩码；`xClearOnExit`参数表示当任务退出时是否清除已经等待到的事件；`xWaitForAllBits`参数表示是否等待所有指定的事件；`xTicksToWait`参数表示等待时间。



