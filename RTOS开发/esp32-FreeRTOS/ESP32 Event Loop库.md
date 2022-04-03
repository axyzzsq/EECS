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



