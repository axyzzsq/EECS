# 共享内存

## 一、共享内存shm

### 例程

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

int main()
{
    int shmid;
    char *shmaddr;
    char write_buf[100];
    char read_buf[100];

    // 1. 创建共享内存
    shmid = shmget(IPC_PRIVATE, 1024, 0666|IPC_CREAT);
    if(shmid == -1)
    {
        printf("create share memory failed\n");
        return -1;
    }

    // 2. 映射共享内存
    shmaddr = (char*)shmat(shmid, NULL, 0);
    if(shmaddr == (void*)-1)
    {
        printf("share memory map failed\n");
        return -1;
    }

    // 3. 写入共享内存
    printf("write data to share memory:\n");
    scanf("%s", write_buf);
    sprintf(shmaddr, "%s", write_buf);

    // 4. 读取共享内存
    printf("read data from share memory:\n");
    sprintf(read_buf, "%s", shmaddr);
    printf("%s\n", read_buf);

    // 5. 解除映射
    shmdt(shmaddr);

    // 6. 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
```
### API

- ```c
  #include <sys/ipc.h>
  #include <sys/shm.h>
  
  int shmget(key_t key, size_t size, int shmflg);
  ```

  其中，`key`共享内存段的键值，`size`是共享内存段的大小，`shmflg`是标志位，用于指定共享内存段的访问权限和操作方式。`IPC_PRIVATE`表示创建一个新的共享内存段，`0666|IPC_CREAT`表示创建一个新的共享内存段并设置访问权限为0666。

  

- ```c
  #include <sys/types.h>
  #include <sys/shm.h>
  
  void *shmat(int shmid, const void *shmaddr, int shmflg);
  ```

  其中，`shmid`是共享内存段的标识符，`shmaddr`是指定映射的地址，如果为`NULL`则表示让系统自动分配一个地址，`shmflg`是标志位，用于指定共享内存段的访问权限和操作方式。

  

- ```c
  #include <sys/types.h>
  #include <sys/shm.h>
  
  int shmdt(const void *shmaddr);
  ```

  `shmdt()`函数用于解除共享内存段的映射，使得进程不能再访问共享内存段。

  `shmaddr`是共享内存段的地址。

  

- ```c
  #include <sys/types.h>
  #include <sys/ipc.h>
  #include <sys/shm.h>
  
  int shmctl(int shmid, int cmd, struct shmid_ds *buf);
  ```

  `shmctl()`函数用于对共享内存段进行控制操作，例如删除共享内存段。

  其中，`shmid`是共享内存段的标识符，`cmd`是控制命令，`buf`是指向共享内存段的信息结构体指针。`IPC_RMID`表示删除共享内存段 。



## 二、文件内存映射与共享内存

文件内存映射和共享内存的区别如下：

| 区别     | 内存映射                 | 内存共享           |
| -------- | ------------------------ | ------------------ |
| 关联文件 | 是                       | 否                 |
| 继承性   | 可以通过fork继承给子进程 | 不可以             |
| 打开函数 | 由open函数打开           | 由shm_open函数打开 |

[(1) Linux内核编程--内存映射和共享内存 - 腾讯云开发者社区-腾讯云. ](https://cloud.tencent.com/developer/article/1998058)
		[(2) 共享内存和内存映射的区别 - CSDN博客.]( https://blog.csdn.net/weixin_42076938/article/details/117378680)
		[(3) 关于共享内存shm和内存映射mmap的区别是什么？ - 知乎.]( https://www.zhihu.com/question/401612303)
		[(4) 共享内存和文件内存映射的区别 - 知乎 - 知乎专栏. ](https://zhuanlan.zhihu.com/p/149277008)