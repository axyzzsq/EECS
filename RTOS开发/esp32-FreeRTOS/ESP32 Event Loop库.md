# ESP32-IDF Event_Loop库

## 整体框架图

整体框架图：

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/21f58cef108c49a58a6894ca255246fa.png)

示例代码框架：

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/f6f6d9faa7654315a52970df55304f60.png)

## Event Loop库

 Event_Loop 包含两个，一个是用户可以自定义的Event_Loop ，还有一个是Event_Loop_default，是供系统设置好的事件的使用的，Event_Loop 库本质上是为了实现当某些事情发生时（比如wifi完成了链接、定时器中断到了时间），系统可以自动调用回调函数来处理针对当前事件。

## 什么是Event?

Event 就是一种事件的发生标记，对于 WiFi，成功连接到接入点就是一个事件。对于每一个Event 都有两部分识别符，分别是 Event base 和 Event ID，Event base声明了这个事件属于哪个 事件族 而 Event ID 表明了是事件族的哪个事件。就是像一个人的姓和名一样，姓就是 Event base，名就是 Event ID。



那么怎么创建一个 **Event** ？

如何定义**Event base**？

**IDF** 提供了两个宏定义用来声明、定义 **Event base** 分别为**ESP_EVENT_DECLARE_BASE**、**ESP_EVENT_DEFINE_BASE**

```C
#define ESP_EVENT_DECLARE_BASE(id) extern esp_event_base_t id

#define ESP_EVENT_DEFINE_BASE(id) esp_event_base_t id = #id

typedef const char*  esp_event_base_t; /**< unique pointer to a subsystem that exposes events */
```

这里 **#** 是“字符串化”的意思，将出现在宏定义中的 **#** 是把跟在后面的参数转换成一个字符串，这里就是把传入的id转化成字符串。你没看错就这么简单，其实就是定义一个常量字符串。

你一定会有疑问，那对于 **Event_Loop_default** 来说系统的事件是怎么声明的呢？声明在哪里？定义在哪里？我们跳转到 **wifi_init.c** 这个文件，可以看到他的第39行就是用使用了宏定义来定义了WIFI_EVENT **事件族** 。

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/4cf4b34d9a164252a403590e6fe77f89.png)

如何定义**Event ID**？根据官方的说法：通常放在对应的头文件里使用**enmu**来定义具体的事件。

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/247ba848b3484d89b14f51d63bfe0935.png)

- 再拿WIFI举例子，我在 **esp_wifi_types.h** 中找到了wifi事件的具体定义。

```C
/** WiFi event declarations */
typedef enum {
    WIFI_EVENT_WIFI_READY = 0,           /**< ESP32 WiFi ready */
    WIFI_EVENT_SCAN_DONE,                /**< ESP32 finish scanning AP */
    WIFI_EVENT_STA_START,                /**< ESP32 station start */
    WIFI_EVENT_STA_STOP,                 /**< ESP32 station stop */
    WIFI_EVENT_STA_CONNECTED,            /**< ESP32 station connected to AP */
    WIFI_EVENT_STA_DISCONNECTED,         /**< ESP32 station disconnected from AP */
    WIFI_EVENT_STA_AUTHMODE_CHANGE,      /**< the auth mode of AP connected by ESP32 station changed */

    WIFI_EVENT_STA_WPS_ER_SUCCESS,       /**< ESP32 station wps succeeds in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_FAILED,        /**< ESP32 station wps fails in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_TIMEOUT,       /**< ESP32 station wps timeout in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_PIN,           /**< ESP32 station wps pin code in enrollee mode */
    WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP,   /**< ESP32 station wps overlap in enrollee mode */

    WIFI_EVENT_AP_START,                 /**< ESP32 soft-AP start */
    WIFI_EVENT_AP_STOP,                  /**< ESP32 soft-AP stop */
    WIFI_EVENT_AP_STACONNECTED,          /**< a station connected to ESP32 soft-AP */
    WIFI_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from ESP32 soft-AP */
    WIFI_EVENT_AP_PROBEREQRECVED,        /**< Receive probe request packet in soft-AP interface */

    WIFI_EVENT_FTM_REPORT,               /**< Receive report of FTM procedure */

    /* Add next events after this only */
    WIFI_EVENT_STA_BSS_RSSI_LOW,         /**< AP's RSSI crossed configured threshold */
    WIFI_EVENT_ACTION_TX_STATUS,         /**< Status indication of Action Tx operation */
    WIFI_EVENT_ROC_DONE,                 /**< Remain-on-Channel operation complete */

    WIFI_EVENT_STA_BEACON_TIMEOUT,       /**< ESP32 station beacon timeout */

    WIFI_EVENT_MAX,                      /**< Invalid WiFi event ID */
} wifi_event_t;

/** @cond **/
/** @brief WiFi event base declaration */
ESP_EVENT_DECLARE_BASE(WIFI_EVENT);
/** @endcond **/
```

这当中定义了所有wifi使用中可能产生的具体事件。总的来说如果你想创建自定义事件就要首先定义**事件族**+**事件ID**。而事件就是由这两个东西构成。说完了定义，那么如何使用呢？

## 什么是Event Loop？

**Event_loop** 是事件循环的真正执行者，每当一个 **Event** POST 到事件中，**Event_loop** 就会根据**Event**的情况调用回调函数，来处理事件发生。

如何创建**Event_loop**？

使用API：**esp_event_loop_create** 这里我们看一下他的声明。

```C
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
esp_err_t esp_event_loop_create(const esp_event_loop_args_t *event_loop_args, esp_event_loop_handle_t *event_loop);

```

一共有两个参数，分别为**esp_event_loop_args_t** 的指针**event_loop_args**，还有一个就是这个**loop**的句柄，这里是传入进去并不是赋值等于。
  这里的 **esp_event_loop_args_t** 就是用来设置LOOP的参数使用的结构体。

```C
typedef struct {
    int32_t queue_size;   /**< size of the event loop queue */
    const char *task_name;      /**< name of the event loop task; if NULL,a dedicated task is not created for event loop*/
    UBaseType_t task_priority;  /**< priority of the event loop task, ignored if task name is NULL */
    uint32_t task_stack_size;     /**< stack size of the event loop task, ignored if task name is NULL */
    BaseType_t task_core_id;      /**< core to which the event loop task is pinned to,
                                                        ignored if task name is NULL */
} esp_event_loop_args_t;
```

分别指定了 **queue_size** 的大小，就是待处理队列的长度，还需要指定任务的名字、优先级、栈空间大小、运行到哪个核心。

下面进入函数详细看一下。（**分析写在了注释**）

```C
esp_err_t esp_event_loop_create(const esp_event_loop_args_t* event_loop_args, esp_event_loop_handle_t* event_loop)
{
    assert(event_loop_args);//检查一下参数入口

    esp_event_loop_instance_t* loop;
    esp_err_t err = ESP_ERR_NO_MEM; // most likely error

    loop = calloc(1, sizeof(*loop));//为指针分配具体内存空间
    if (loop == NULL) {
        ESP_LOGE(TAG, "alloc for event loop failed");
        return err;
    }

    loop->queue = xQueueCreate(event_loop_args->queue_size , sizeof(esp_event_post_instance_t));//创建一个消息队列，大小是queue_size。每个消息长度是esp_event_post_instance_t。
    if (loop->queue == NULL) {
        ESP_LOGE(TAG, "create event loop queue failed");
        goto on_err;
    }

    loop->mutex = xSemaphoreCreateRecursiveMutex();//创建递归信号量
    if (loop->mutex == NULL) {
        ESP_LOGE(TAG, "create event loop mutex failed");
        goto on_err;
    }

#ifdef CONFIG_ESP_EVENT_LOOP_PROFILING//此宏未定义
    loop->profiling_mutex = xSemaphoreCreateMutex();
    if (loop->profiling_mutex == NULL) {
        ESP_LOGE(TAG, "create event loop profiling mutex failed");
        goto on_err;
    }
#endif

    SLIST_INIT(&(loop->loop_nodes));//创建一个单向列表

    // 创建一个任务作为eventloop的处理线程，任务的处理函数esp_event_loop_run_task，传入了loop作为参数
    if (event_loop_args->task_name != NULL) {
        BaseType_t task_created = xTaskCreatePinnedToCore(esp_event_loop_run_task, event_loop_args->task_name,
                    event_loop_args->task_stack_size, (void*) loop,
                    event_loop_args->task_priority, &(loop->task), event_loop_args->task_core_id);

        if (task_created != pdPASS) {
            ESP_LOGE(TAG, "create task for loop failed");
            err = ESP_FAIL;
            goto on_err;
        }
		
        loop->name = event_loop_args->task_name;

        ESP_LOGD(TAG, "created task for loop %p", loop);
    } else {
        loop->name = "";
        loop->task = NULL;
    }

    loop->running_task = NULL;

#ifdef CONFIG_ESP_EVENT_LOOP_PROFILING//此宏未定义
    portENTER_CRITICAL(&s_event_loops_spinlock);
    SLIST_INSERT_HEAD(&s_event_loops, loop, next);
    portEXIT_CRITICAL(&s_event_loops_spinlock);
#endif

    *event_loop = (esp_event_loop_handle_t) loop;//这句有点不明所以，请指教

    ESP_LOGD(TAG, "created event loop %p", loop);

    return ESP_OK;//创建成功，返回

on_err:
    if (loop->queue != NULL) {
        vQueueDelete(loop->queue);
    }

    if (loop->mutex != NULL) {
        vSemaphoreDelete(loop->mutex);
    }

#ifdef CONFIG_ESP_EVENT_LOOP_PROFILING
    if (loop->profiling_mutex != NULL) {
        vSemaphoreDelete(loop->profiling_mutex);
    }
#endif

    free(loop);

    return err;
}
```

创建过程分析完毕，本质上就是一个消息队列和一个task组成的线程安全的结构。

如何结合 Event 来使用呢？

下面介绍注册API：

```C
esp_err_t esp_event_handler_instance_register_with(esp_event_loop_handle_t event_loop, esp_event_base_t event_base,
                                          int32_t event_id, esp_event_handler_t event_handler, void* event_handler_arg,
                                          esp_event_handler_instance_t* handler_ctx_arg);
```

 在创建完Event_loop后这个API 就可以向指定Event_loop 注册 Event ，一共6个参数，首先是被注册的 Event_loop 的句柄，注册的Event 的Event base 和 Event ID，最后是事件发生后的回调服务函数，传入回调服务函数的参数。
   这里举一个下面代码中的例子：

```C
esp_event_handler_instance_register_with(loop_with_task, TASK_EVENTS, TASK_ITERATION_EVENT, task_iteration_handler, loop_with_task, NULL)
```

这里是向 loop_with_task 这个 Event_loop 注册了一个Event ，Event base 是 TASK_EVENTS，Event ID 是 TASK_ITERATION_EVENT，回调函数是 task_iteration_handler，传入参数是 loop_with_task。
   其中回调函数 task_iteration_handler代码如下：

```C
static void task_iteration_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    char* loop;
    if (handler_args == loop_with_task) //检查一下传入的参数是不是loop_with_task
    {
        loop = "loop_with_task";
    } 
    ESP_LOGI(TAG, "handling %s:%s from %s", base, "TASK_ITERATION_EVENT", loop);//提示事件回调正在运行。
}
```

 到这一步就完成了一个Event和Event_loop的全部初始化，那怎么向Event_loop 说明事件发生了呢？
这里就涉及到了最后一个API ：esp_event_post_to（）

一共6个参数，首先是被通知的 Event_loop 的句柄，注册的Event 的Event base 和 Event ID，最后是事件发生后的需要传入回调的参数，参数大小，等待时间。

下面我们进入分析一下。

```C
esp_err_t esp_event_post_to(esp_event_loop_handle_t event_loop, esp_event_base_t event_base, int32_t event_id,
                            void* event_data, size_t event_data_size, TickType_t ticks_to_wait)
{
    assert(event_loop);

    if (event_base == ESP_EVENT_ANY_BASE || event_id == ESP_EVENT_ANY_ID)
     {
        return ESP_ERR_INVALID_ARG;
    }//入口参数检查，不能为任意事件的任意ID

    esp_event_loop_instance_t* loop = (esp_event_loop_instance_t*) event_loop;//指针复印转移

    esp_event_post_instance_t post;
    memset((void*)(&post), 0, sizeof(post));
		/***********************post时的参数传递处理********************/
    if (event_data != NULL && event_data_size != 0) {
        // Make persistent copy of event data on heap.
        void* event_data_copy = calloc(1, event_data_size);

        if (event_data_copy == NULL) {
            return ESP_ERR_NO_MEM;
        }

        memcpy(event_data_copy, event_data, event_data_size);
    }
    
    post.base = event_base;
    post.id = event_id;
    
    BaseType_t result = pdFALSE;
        // The loop has a dedicated task.
        if (loop->task != xTaskGetCurrentTaskHandle()) {
            result = xQueueSendToBack(loop->queue, &post, ticks_to_wait);//加入到loop的事件发生队列中
        } else {//如果是当前正在运行的Task，则不等待直接加入队列。这里有一点问题就是满员的时候这个位置就会报错因为没有等待时间，但是并不会运行到这个分支，因为不会在loop->task中使用post。
            result = xQueueSendToBack(loop->queue, &post, 0);
        }
    }
    if (result != pdTRUE) {
        post_instance_delete(&post);
    return ESP_OK;
}

```

那么如何使用这个函数呢？
  这里我们建立一个线程，模拟事件发生，而API ：**esp_event_post_to** 就可以对 **Event_loop** 中的事件提示发生，进而进行回调处理。代码如下：

```C
static void task_event_source(void* args)
{
    while (1)
    {
        ESP_ERROR_CHECK(esp_event_post_to(loop_with_task, TASK_EVENTS, TASK_ITERATION_EVENT, NULL, NULL, portMAX_DELAY));
        //调用esp_event_post_to,向特定Event_loop提示有特定事件发生，这里的Event与上文创建的一致。
        vTaskDelay(pdMS_TO_TICKS(1000));//每隔1s通知一次事件发生.
    }
}
```

最后运行起来就可以看到，每一秒回调处理函数都会运行一次，并说明是哪个事件导致的运行。

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/71d104d4cb7347c78a504350e4779c78.png)