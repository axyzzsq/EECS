# 信号量

## 一、二值信号量

### 使用场景

二值信号量一般用于在多任务系统中同步执行两个或多个任务。 它们可用于保护共享资源不被同时访问，或协调相互依赖的任务的执行。

以下是使用二进制信号量的一些常见场景：

- 保护共享资源：当多个任务访问共享资源时，例如内存位置、文件或外围设备，可以使用二进制信号量来确保一次只有一个任务访问该资源。 一个任务在访问资源之前等待信号量变为可用，而另一个任务通过释放信号量来表示它已完成使用资源。

- 协调任务执行：在某些情况下，一个任务的执行取决于另一个任务的完成。 二进制信号量可用于发出第一个任务完成的信号并唤醒一直在等待信号量可用的第二个任务。

使用二进制信号量时要记住一些限制：

- 二进制信号量通常用于同步单个系统或进程中的任务。 它们不适合跨不同系统或进程同步任务。

- 二进制信号量最适合只涉及两个任务的简单同步场景。 如果需要同步多个任务，则可能需要使用其他同步原语，例如互斥体或计数信号量。

- 二进制信号量可能容易发生优先级倒置，即高优先级任务被持有信号量的低优先级任务阻塞的情况。 为避免优先级反转，您可以使用优先级继承或优先级上限协议等技术。

### 例程

```C
#include "FreeRTOS.h"
#include "semphr.h"

// Declare a binary semaphore
SemaphoreHandle_t xSemaphore;

// Task 1
void task1(void *pvParameters)
{
    // Wait for the semaphore to become available
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    
    // Do some work here...
    
    // Give the semaphore to task 2
    xSemaphoreGive(xSemaphore);
}

// Task 2
void task2(void *pvParameters)
{
    // Do some work here...
    
    // Give the semaphore to task 1
    xSemaphoreGive(xSemaphore);
    
    // Wait for the semaphore to become available
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
}

// Initialization code
void init()
{
    // Create the binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
    
    // Check if the semaphore was created successfully
    if (xSemaphore == NULL) {
        // Handle error condition here...
    }
    
    // Create the tasks
    xTaskCreate(task1, "Task 1", 1000, NULL, 1, NULL);
    xTaskCreate(task2, "Task 2", 1000, NULL, 1, NULL);
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
}

```

在此示例中，使用 SemaphoreHandle_t 数据类型声明了一个二进制信号量 xSemaphore。 然后我们创建两个任务，task1 和 task2，它们使用信号量来同步它们的执行。

在 init() 函数中，我们使用 xSemaphoreCreateBinary() 创建二进制信号量。 然后检查信号量是否成功创建并处理任何错误。 最后，使用 xTaskCreate() 创建任务并使用 vTaskStartScheduler() 启动 FreeRTOS 调度程序。

在任务 1 中，我们使用 xSemaphoreTake() 等待信号量可用。 当信号量可用时，我们做一些工作，然后使用 xSemaphoreGive() 将信号量交给 task2。

在任务 2 中，我们做了一些工作，然后使用 xSemaphoreGive() 将信号量交给任务 1。 然后使用 xSemaphoreTake() 等待信号量可用。 当信号量可用时，我们可以继续执行。

#### 问题解析

- 在`task2`中，为什么是先执行`xSemaphoreGive(xSemaphore);`再执行`xSemaphoreTake(xSemaphore, portMAX_DELAY);`,和`task1`的顺序是不一样的
  - 在提供的示例代码中，task2 在调用 xSemaphoreTake 等待信号量可用之前使用 xSemaphoreGive 将信号量提供给 task1,虽然在技术上可以颠倒 task2 中 xSemaphoreGive 和 xSemaphoreTake 调用的顺序，但这样做可能会导致死锁。 这是因为 task2 将首先尝试获取它尚未提供的信号量，然后它会无限期地等待 task1 提供信号量。因此，重要的是要遵循正确的发出和接收信号量的顺序，以避免死锁并确保任务之间的正确同步。 在这种情况下，task2 被设计为在将信号量交给 task1 之前执行一些工作，然后在继续执行之前等待信号量可用。



## 二、计数信号量

### 使用场景

FreeRTOS 中使用计数信号量来同步对共享资源的访问或控制任务之间的执行流。计数信号量可用于多个任务需要访问资源的情况，并且资源可以同时在多个任务之间共享。

计数信号量可用于多个任务需要访问共享资源的场景，但在任何给定时间可以访问该资源的任务数量是有限的。计数信号量维护可用资源的计数，每次任务访问资源时，计数都会递减。当任务释放资源时，计数会增加。（例如停车场的计数牌，每当有一辆车进入使用车位资源，计数值减一，每当有一辆车驶离，计数板上的可用车位就加一）

信号量计数的一个常用场景是资源分配，例如当可用的硬件外围设备数量有限且多个任务需要访问它们时。计数信号量可用于跟踪有多少外设可用，并确保访问外设的任务数不超过可用资源。

计数信号量的使用受到某些限制。

- 主要限制之一是计数信号量只能在多个任务可以同时访问共享资源时使用。如果一次只能由一个任务访问资源，则应改用二进制信号量或互斥量。

- 另一个限制是计数信号量的计数值必须始终大于或等于零。如果计数信号量的计数值为负，则表示等待信号量的任务多于可用资源，这会导致死锁和其他同步问题。

此外，由于计数信号量用于任务间通信和同步，因此应注意确保正确使用它们并且它们的使用不会引入竞争条件或其他同步问题。正确使用计数信号量需要很好地理解并发编程和同步技术。

### 例程

```c
#include "FreeRTOS.h"
#include "semphr.h"

// Define a counting semaphore with an initial count of 1
SemaphoreHandle_t xCountingSemaphore = NULL;
const int COUNTING_SEMAPHORE_COUNT = 1;

// Task 1 function
void vTask1(void *pvParameters)
{
    // Take the counting semaphore
    if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE)
    {
        // Access the shared resource here
        
        // Release the counting semaphore
        xSemaphoreGive(xCountingSemaphore);
    }
}

// Task 2 function
void vTask2(void *pvParameters)
{
    // Take the counting semaphore
    if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE)
    {
        // Access the shared resource here
        
        // Release the counting semaphore
        xSemaphoreGive(xCountingSemaphore);
    }
}

int main(void)
{
    // Create a counting semaphore
    xCountingSemaphore = xSemaphoreCreateCounting(COUNTING_SEMAPHORE_COUNT, COUNTING_SEMAPHORE_COUNT);

    // Create Task 1
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Create Task 2
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    return 0;
}

```

在这段代码中，首先定义了一个初始计数为 1 的计数信号量 xCountingSemaphore。然后创建了两个任务 vTask1 和 vTask2，它们都将访问受计数信号量保护的共享资源。

在任务函数中，使用xSemaphoreTake获取计数信号量，然后访问共享资源。 然后使用 xSemaphoreGive 释放计数信号量。

在主函数中，使用 xSemaphoreCreateCounting 创建计数信号量，并传入初始计数值。 然后，使用 xTaskCreate 创建这两个任务，并使用 vTaskStartScheduler 启动 FreeRTOS 调度程序。

需要注意的是，xSemaphoreCreateCounting 只应用于多个任务需要访问共享资源的情况，并且该资源可以同时被多个任务访问。 此外，计数信号量的计数值必须始终大于或等于零，程序员有责任确保正确使用计数信号量并且不会引入同步问题。

## 三、互斥信号量

### 使用场景

当多个任务需要独占访问共享资源时，FreeRTOS 使用互斥信号量。 互斥信号量类似于二进制信号量，不同之处在于它可用于保护可被多个任务访问的共享资源。 在这种情况下，在任何给定时间，只有一个任务可以拥有互斥信号量，而在信号量被另一个任务拥有时试图获取该信号量的其他任务将被阻塞，直到该信号量被释放。

使用互斥信号量的局限性包括优先级反转的可能性，这是一种高优先级任务被持有互斥信号量的低优先级任务阻塞的情况。 如果在低优先级任务持有互斥信号量时高优先级任务抢占低优先级任务，就会发生这种情况，导致高优先级任务阻塞，直到低优先级任务释放互斥信号量。

为了防止优先级反转，FreeRTOS 提供了几种方法，包括优先级继承和优先级上限协议，这有助于确保高优先级任务不会被持有互斥信号量的低优先级任务阻塞。

使用互斥信号量的另一个限制是，如果不小心使用，它很容易出现死锁。 当多个任务各自持有一个或多个互斥信号量并等待其他任务释放它们继续执行所需的互斥信号量时，就会发生死锁。 为避免死锁，仔细管理获取和释放互斥信号量的顺序非常重要，并确保没有任务一次持有超过一个互斥信号量。

总的来说，互斥信号量是一个强大的工具，用于管理对 FreeRTOS 中共享资源的访问，但它们需要仔细考虑和设计，以确保它们得到有效使用并避免潜在的陷阱。

### 例程

```C
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Declare a mutex semaphore handle
SemaphoreHandle_t xMutex;

void vTask1( void *pvParameters )
{
    // Acquire the mutex semaphore
    if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
    {
        // Access the shared resource
        // ...

        // Release the mutex semaphore
        xSemaphoreGive( xMutex );
    }

    vTaskDelete( NULL );
}

void vTask2( void *pvParameters )
{
    // Acquire the mutex semaphore
    if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
    {
        // Access the shared resource
        // ...

        // Release the mutex semaphore
        xSemaphoreGive( xMutex );
    }

    vTaskDelete( NULL );
}

int main( void )
{
    // Create a mutex semaphore
    xMutex = xSemaphoreCreateMutex();

    // Create two tasks that access the shared resource protected by the mutex semaphore
    xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // The scheduler should never return
    return 0;
}

```

在此示例中，我们使用 xSemaphoreCreateMutex() 函数创建互斥信号量。 然后我们创建两个任务来访问受互斥信号量保护的共享资源。 每个任务都尝试使用 xSemaphoreTake() 获取互斥信号量，并在完成对共享资源的访问后使用 xSemaphoreGive() 释放它。

使用互斥信号量的局限性包括优先级反转的可能性和死锁的风险。 优先级反转发生在高优先级任务被阻塞等待低优先级任务释放互斥信号量时。 当两个或多个任务无限期地等待对方释放它们持有的互斥信号量时，就会发生死锁。 因此，谨慎使用互斥信号量并以最小化这些问题的方式设计系统非常重要。