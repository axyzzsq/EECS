# ESP32事件库

## 事件库

FreeRTOS的事件可以理解为多个二值信号量的组合；

事件只与任务相关联，事件之间相互独立；事件仅用于同步，不能提供数据传输；

事件无排队性，多次向任务设置同一个事件，等效于只设置了一次；允许多个任务对同一个事件进行读写；

事件通常可以用来替代裸机编程中的if/switch语句配合枚举/全局变量标志；

```C
EventGroupHandle_t xEventGroupCreate(void); //创建事件标志组,返回事件标志组的句柄

//设置事件标志位
EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,//事件标志组句柄
                              const EventBits_t uxBitsToSet //事件标志位
                              ); //注意使用前一定要创建对应的事件标志

//从中断服务程序中设置事件标志位
BaseType_t EventGroupSetBitsFromISR(EventGroupHandle_t xEventGroup, //事件标志组句柄
                                    const EventBits_t uxBitsToSet, //事件标志位设置
                                    BaseType_t *pxHigherPriorityTaskWoken //高优先级任务是否被唤醒的状态保存
);
```

## ESP-IDF中的事件循环库

为了处理wifi、蓝牙、网络接口等外设中大量的状态变化，一般会使用状态机(FSM)，而指示状态就需要用到事件集。ESP-IDF中提供了可用的**事件循环**。向默认**事件循环**发送事件相当于事件的handler依次执行队列中的命令。

事件循环被囊括在事件循环库中，事件循环库允许组件将事件发布到事件循环，而当其他组件被注册到事件循环且设置了对应的处理函数时，程序会自动地在事件发生时执行处理程序。

在ESP32魔改版FreeRTOS中很少使用正经的事件集，而是使用ESP-IDF提供的更方便的事件循环。

使用`#include "esp_event.h"`即可开启事件循环库功能。

流程如下：

1、用户定义一个事件处理函数，该函数必须与esp_event_handler_t具有相同的结构(也就是说函数是esp_event_handler_t类型的函数指针)

```C
typedef void (*esp_event_handler_t)(void *event_handler_arg, //事件处理函数的参数
                                   esp_event_base_t event_base //只想引发事件子程序的特殊指针
                                   int32_t event_id,  //事件的ID
                                   void *event_data)  //事件数据
    
```

2、使用`esp_event_loop_create()`函数创建一个事件循环，该API会回传一个esp_event_loop_handle_t类型的指针用于指向事件循环。每个用该API创建的事件循环都被成为**用户事件循环**；除此之外，还可以使用一种称为**默认事件循环**的特殊事件循环(默认事件循环是系统自带的事件循环，实际上只要使用默认事件循环就足够了)。

3、使用`esp_event_handler_register_with()`函数将事件处理函数注册到事件循环中(注意：一个处理函数可以被注册到多个不同的事件循环中)

4、开始运行程序

5、使用`esp_event_post_to`发送一个事件到目标事件循环

6、事件处理函数收取该事件并进行处理；

7、使用`esp_event_handler_unregister_with`来取消注册某个事件处理函数

8、使用`esp_event_loop_delete`删除不在需要循环的事件

```C
//1.定义事件处理函数
void run_on_event(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{}

void app_main()
{
    //2.配置esp_event_loop_args_t结构体来配置事件循环
    esp_event_loop_args_t loop_args = {
        .queue_size = ...,
        .task_name = ...
        .task_priority = ...,
        .task_stack_size = ...,
        .task_core_id = ...
    };

    //创建一个用户事件循环
    esp_event_loop_handle_t loop_handle;
    esp_event_loop_create(&loop_args, &loop_handle);
    //3.注册事件处理函数
    esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event, ...);
    ...
    //4.事件源使用以下API将事件发送到事件循环，随后事件处理函数会根据其中的逻辑进行处理
    //这一系列操作可以跨任务使用
    esp_event_post_to(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, ...)
    ...
    //5.解除注册一个事件处理函数
    esp_event_handler_unregister_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event);
    ...
    //6.删除一个不需要的事件循环
    esp_event_loop_delete(loop_handle);
}
```

使用如下函数来声明和定义事件

一个事件由两部分标识组成：事件类型和事件ID

事件类型标识了一个独立的事件组；事件ID区分在该组内的事件

可以将事件类型视为人的姓，事件ID是人的名

使用以下两个宏函数来声明、定义事件类型。一般地，在程序中使用XXX_EVENT的形式来定义一个事件类型

```C
ESP_EVENT_DECLARE_BASE(EVENT_BASE)//声明事件类型
ESP_EVENT_DEFINE_BASE(EVENT_BASE)//定义事件类型
    
//事件类型举例：WIFI_EVENT
```

一般使用枚举变量来定义事件ID，如下所示

```C
enum {
    EVENT_ID_1,
    EVENT_ID_2,
    EVENT_ID_3,
    ...
}
```

当注册一个事件处理函数到不同事件循环后，**事件循环可以根据不同的事件类型和事件ID来区分应该执行哪一个事件处理函数**

可以使用ESP_EVENT_ANY_BASE和ESP_EVENT_ANY_ID作为注册事件处理函数的参数，这样事件处理函数就可以处理发到当前注册事件循环上的任何事件

## 默认事件循环

默认事件循环是一种系统事件（如wifi、蓝牙事件等）使用的特殊事件循环。特殊的一点是它的句柄被隐藏起来，用户无法直接使用。用户只能通过一系列固定的API来操作这个事件循环

API如下表所示：

![image-20220404230003213](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220404230003213.png)

除了API区别和系统事件会自动发送到默认事件循环外，**两者并没有更多差别**，所以说用户可以将自定义的事件直接发送到默认事件循环，这比用户定义的事件循环更节约内存且**更方便**！

任务、队列和事件循环是ESP32中最常用也是最特殊的SMP FreeRTOS API。

## 事件循环库API简介

使用以下API控制事件循环

```c
esp_err_t esp_event_loop_create_default(void);//创建默认事件循环
esp_err_t esp_event_loop_delete_default(void);//删除默认事件循环

//创建用户事件循环
esp_err_t esp_event_loop_create(const esp_event_loop_args_t *event_loop_args,//事件循环参数
                                esp_event_loop_handle_t *event_loop);//事件循环句柄
    
//删除用户事件循环
esp_err_t esp_event_loop_delete(esp_event_loop_handle_t event_loop);//事件循环

esp_err_t esp_event_loop_run(esp_event_loop_handle_t event_loop, TickType_t ticks_to_run);
//将时间分配到一个事件循环，不常用，注意事项一大堆懒得看了——总之详细用法请参考官网API简介    
```

使用以下API来注册/注销事件处理函数

```c
//将事件处理程序注册到系统事件循环
esp_err_t esp_event_handler_register(esp_event_base_t event_base,//事件类型
                                     int32_t event_id,//事件ID
                                     esp_event_handler_t event_handler,//事件处理函数
                                     void *event_handler_arg);//事件处理函数的参数

//将事件处理程序注册到用户事件循环
esp_err_t esp_event_handler_register_with(esp_event_loop_handle_t event_loop,//要注册到的事件循环
                                          esp_event_base_t event_base,//事件类型
                                          int32_t event_id,//事件ID
                                          esp_event_handler_t event_handler,//事件处理函数
                                          void *event_handler_arg);//事件处理函数的参数
//取消注册（系统事件循环）
esp_err_t esp_event_handler_unregister(esp_event_base_t event_base,//事件类型
                                       int32_t event_id,//事件ID
                                       esp_event_handler_t event_handler);//事件处理函数
//取消注册（用户事件循环）
esp_err_t esp_event_handler_unregister_with(esp_event_loop_handle_t event_loop,//要取消注册的事件循环
                                            esp_event_base_t event_base,//事件类型
                                            int32_t event_id,//事件ID
                                            esp_event_handler_t event_handler);//事件处理函数

```

可以使用ESP_EVENT_ANY_BASE 和ESP_EVENT_ANY_ID来取消注册所有事件循环上的事件处理函数

使用以下API来发送事件到事件循环

```C
//发送事件到系统事件循环
esp_err_t esp_event_post(esp_event_base_t event_base,//事件类型
                         int32_t event_id,//事件ID
                         void *event_data,//事件数据
                         size_t event_data_size,//事件数据的大小
                         TickType_t ticks_to_wait);//等待时间

//发送事件到用户事件循环
esp_err_t esp_event_post_to(esp_event_loop_handle_t event_loop,//要发送到的用户事件循环的句柄
                            esp_event_base_t event_base,//事件类型
                            int32_t event_id,//事件ID
                            void *event_data,//事件数据
                            size_t event_data_size,//事件数据的大小
                            TickType_t ticks_to_wait)//等待时间
```

事件循环库函数会保留事件数据的副本并自动控制副本的存活时间。