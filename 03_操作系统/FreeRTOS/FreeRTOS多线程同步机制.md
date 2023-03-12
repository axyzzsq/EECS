# FreeRTOS的线程同步机制

## 一、同步机制

为了实现线程间的同步与通信，FreeRTOS提供了以下机制。

| 机制     | 区别                                                         | 应用场景                                                     |
| :------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 信号量   | [二值信号量只有两种状态，0或1，用来通知事件的发生或者保护共享资源](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。计数信号量有一个计数值，用来表示可用资源的数量或者等待事件的任务数量[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。 | [二值信号量可以用来实现互斥锁或者二元同步](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。计数信号量可以用来实现资源管理或者多元同步[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。 |
| 互斥量   | 一种特殊的二值信号量，用来实现资源管理和优先级继承[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。当一个线程获取互斥量时，它会继承互斥量所属的最高优先级，以避免优先级反转问题[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。 | 互斥量可以用来保护共享资源的访问，例如打印机、文件、内存等[1](https://www.freertos.org/zh-cn-cmn-s/Inter-Task-Communication.html)。 |
| 事件组   | 一种位操作的同步机制，可以用来通知线程多个事件的发生或者等待多个事件的条件[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。每个事件组有8个位，每个位代表一个事件或者一个条件[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。 | 事件组可以用来实现复杂的同步逻辑，例如等待任意事件、等待所有事件、设置或清除事件等[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。 |
| 消息队列 | 一种数据结构，可以用来传递消息或者数据包[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。每个消息队列有一个缓冲区，用来存储消息或者数据包，并且有一个长度限制[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。 | 消息队列可以用来实现线程之间的数据交换，例如传感器数据、控制命令、状态信息等[2](https://blog.csdn.net/laifengyuan1/article/details/107539793)。 |

### 信号量



### 互斥量



### 事件组



### 消息队列



## 二、相关问题点

### 消息和事件的区别

| 机制       | 消息                                                         | 事件                                                      |
| :--------- | :----------------------------------------------------------- | :-------------------------------------------------------- |
| 定义       | 一种数据结构，可以用来传递信息                               | 一种位操作，可以用来通知事件的发生或者等待事件的条件      |
| 存储和传输 | 需要使用消息队列，有缓冲区和长度限制                         | 需要使用事件组，有8个位，每个位代表一个事件或者一个条件。 |
| 功能       | 可以实现数据交换，但不能实现复杂的同步逻辑                   | 可以实现复杂的同步逻辑，但不能实现数据交换                |
| 应用场景   | 可以用在任何任务或中断之间，但发送消息的任务可能需要等待消息队列有空间可用 | 可以用在任何任务或中断之间，且不需要等待事件组可用        |

### 如何改变事件组的优先级

