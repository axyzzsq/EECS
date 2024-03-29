# 队列

## 一、循环队列

**只针对顺序存储结构开展循环队列的研究。**

### 1、概念和公式

- 定义：头尾相接的数据结构称为循环队列；

- 循环队列满的定义：

  - 顺序存储结构的队列满时，数组中还有一个空闲单位。

- 队列为空的条件：

  
  $$
  rear == front
  $$



- 循环队列满的条件：

  ![image-20220818193047227](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220818193047227.png)

- 循环队列当前长度计算公式:

  

  ![image-20220818193055943](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220818193055943.png)



### 2、



## 二、链式队列

### 1、链式队列的结构

```C
typedef int QElemType;
/*
 * 结点结构
 */
typedef struct QNode
{
    QElemType data;
    struct QNode *next;
}QNode,*QueuePtr;

/*
 * 链表结构
 */
typedef struct
{
    QueuePtr front,reat;
}LinkQueue;
```

### 2、链式队列-入队操作

```C
/*
 * 插入元素e为Q的新的队尾元素
 */
Status EnQueue(LinkQueue *Q, QElemType e)
{
    QueuePtr s = (QueuePtr)malloc(Sizeof(QNode));
    if(!s)
    {
        exit(OVERFLOW);
    }
    s->data = e;
    s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
    return OK;
}
```

### 3、链式队列-出队操作

