# ESP32网络通信与数据处理

 ![image-20220718091030562](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220718091030562.png)

网络通信事件是指接入服务器过程中产生的状态事件，如ESP32 WiFi状态就绪、ESP32完成路由器AP的扫描、ESP32连接路由器成功、ESP32和路由器断开等事件发生。

## 一、通信事件处理（Event Loop）

### 1、什么是Event

Event 就是一种事件的发生标记，对于 WiFi，成功连接到接入点就是一个事件。对于每一个Event 都有两部分识别符，分别是 Event base 和 Event ID。Event base表示属于哪一类事件，Event ID 表示属于哪个具体事件。

### 2、Event Loop

**Event_loop** 是事件循环的真正执行者，每当一个 **Event** POST 到事件中，**Event_loop** 就会根据**Event**的情况调用回调函数，来处理事件发生。

#### **Step1:创建循环事件**

使用**esp_event_loop_create**来创建Event_loop.

```C
/*
   参数①：esp_event_loop_args_t 的指针event_loop_args，
   参数②：loop的句柄，这里是传入进去并不是赋值等于。这里的esp_event_loop_args_t 就是用来设置LOOP的参数使用的结构体。
*/
esp_err_t esp_event_loop_create(const esp_event_loop_args_t *event_loop_args, 
                                esp_event_loop_handle_t *event_loop);
```

这里的 **esp_event_loop_args_t** 就是用来设置LOOP的参数使用的结构体。

```C
typedef struct {
    int32_t queue_size;   /* 待处理队列的长度 */
    const char *task_name;      /**名字，必不可少的参数，没有名字不创建循环事件*/
    UBaseType_t task_priority;  /* 优先级 */
    uint32_t task_stack_size;     /* 栈空间大小 */
    BaseType_t task_core_id;      /* 运行到哪个核心*/
} esp_event_loop_args_t;
```

#### **Step2:注册**

```c
esp_err_t esp_event_handler_instance_register_with(
    				esp_event_loop_handle_t event_loop,  //首先是被注册的 Event_loop 的句柄
    				esp_event_base_t event_base,  //注册事件类
    				int32_t event_id,   //注册事件ID
    				esp_event_handler_t event_handler,  // 回调 
    				void* event_handler_arg,  //参数
    				esp_event_handler_instance_t* handler_ctx_arg);  //NULL
```

在创建完Event_loop后这个API 就可以向指定Event_loop 注册 Event ，一共6个参数，首先是被注册的 Event_loop 的句柄，注册的Event 的Event base 和 Event ID，最后是事件发生后的回调服务函数，传入回调服务函数的参数。   

 这里举一个下面代码中的例子：

```c
esp_event_handler_instance_register_with(loop_with_task, 
										 TASK_EVENTS, 
										 TASK_ITERATION_EVENT, 
										 task_iteration_handler, 
										 loop_with_task, 
										 NULL)
```

这里是向 loop_with_task 这个 Event_loop 注册了一个Event ，Event base 是 TASK_EVENTS，Event ID是TASK_ITERATION_EVENT，回调函数是 task_iteration_handler，传入参数是 loop_with_task。其中回调函数 task_iteration_handler代码如下：

```c
static void task_iteration_handler(void* handler_args, 
                                   esp_event_base_t base, 
                                   int32_t id, 
                                   void* event_data)
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

#### **Step3: Post**

这里就涉及到了最后一个API ：esp_event_post_to(),一共6个参数，首先是被通知的 Event_loop 的句柄，注册的Event 的Event base 和 Event ID，最后是事件发生后的需要传入回调的参数，参数大小，等待时间。

```C
esp_err_t esp_event_post_to(esp_event_loop_handle_t event_loop, 
                            esp_event_base_t event_base, 
                            int32_t event_id,
                            void* event_data, 
                            size_t event_data_size, 
                            TickType_t ticks_to_wait)
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
    if (event_data != NULL && event_data_size != 0) 
    {
        // Make persistent copy of event data on heap.
        void* event_data_copy = calloc(1, event_data_size);
        if (event_data_copy == NULL) 
        {
            return ESP_ERR_NO_MEM;
        }
        memcpy(event_data_copy, event_data, event_data_size);
    }
    
    post.base = event_base;
    post.id = event_id;
    
    BaseType_t result = pdFALSE;
    // The loop has a dedicated task.
    if (loop->task != xTaskGetCurrentTaskHandle()) 
    {
        result = xQueueSendToBack(loop->queue, &post, ticks_to_wait);//加入到loop的事件发生队列中
    } 
    else 
    {   /*如果是当前正在运行的Task，则不等待直接加入队列。
              这里有一点问题就是满员的时候这个位置就会报错因为没有等待时间，
              但是并不会运行到这个分支，因为不会在loop->task中使用post。
    	*/
        result = xQueueSendToBack(loop->queue, &post, 0);
    }
    if (result != pdTRUE) 
    {
        post_instance_delete(&post);
    }
    return ESP_OK;
}
```

那么如何使用这个函数呢？

这里我们建立一个线程，模拟事件发生，而API ：esp_event_post_to 就可以对 Event_loop 中的事件提示发生，进而进行回调处理。代码如下：

```C
static void task_event_source(void* args)
{
    while (1)
    {
        ESP_ERROR_CHECK(esp_event_post_to(loop_with_task, 
                              			  TASK_EVENTS, 
                             			  TASK_ITERATION_EVENT, 
                             			  NULL, 
                            			  NULL, 
                         			      portMAX_DELAY));
        //调用esp_event_post_to,向特定Event_loop提示有特定事件发生，这里的Event与上文创建的一致。
        vTaskDelay(pdMS_TO_TICKS(1000));//每隔1s通知一次事件发生.
    }
}
```

测试运行结果：

![image-20220718084057927](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220718084057927.png)

在开发验证中尤其要注意一场操作处理，比如故意输入错误的Wi-Fi密码等。

## 二、通信数据处理

### 1、通信模型

![image-20220718090230141](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220718090230141.png)

### 2、下行数据

![image-20220718093252860](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220718093252860.png![image-20220719093118814](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220719093118814.png)



### 3、上行数据

![image-20220718095453232](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220718095453232.png)



