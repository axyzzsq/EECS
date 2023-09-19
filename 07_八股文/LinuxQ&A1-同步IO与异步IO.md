# Q&A1: 同步IO与异步IO

在Linux开发中，I/O（输入/输出）操作是非常常见的任务之一。

同步I/O和异步I/O是两种不同的I/O模型，它们在处理I/O操作时有着不同的工作方式和用途。

## 同步IO:阻塞

同步I/O是指在进行I/O操作时，程序会**等待**该操作完成后再继续执行后续的代码。

在同步I/O模型中，当一个I/O请求被发起时，程序会阻塞（即停止执行）直到I/O操作完成，然后继续执行后续代码。

这种方式下，程序的执行按照顺序进行，每个I/O操作都会导致程序的阻塞。

例程：

```C
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() 
{
    int fd;
    char buffer[1024];

    // 打开文件
    fd = open("input.txt", O_RDONLY);
    if (fd == -1) 
    {
        perror("无法打开文件");
        return 1;
    }

    // 读取文件内容
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) 
    {
        perror("读取文件失败");
        return 1;
    }

    // 关闭文件
    close(fd);

    // 打印文件内容
    printf("文件内容：%.*s\n", bytesRead, buffer);

    return 0;
}

```

上述代码中，`open`、`read`和`close`是Linux系统调用函数，用于打开文件、读取文件内容和关闭文件。

在`read`函数调用期间，程序会阻塞等待I/O操作完成，然后才会继续执行后续代码。



## 异步IO

异步I/O是指在进行I/O操作时，**程序可以继续执行后续的代码而无需等待该I/O操作的完成**。

在异步I/O模型中，当一个I/O请求被发起时，程序会立即返回并继续执行后续代码，而不会阻塞等待I/O操作完成。

当I/O操作完成后，程序会得到通知，然后可以处理已完成的I/O数据。

例程：

```C
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() 
{
    int fd;
    char buffer[1024];
    int epollfd;
    struct epoll_event event;

    // 打开文件
    fd = open("input.txt", O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        perror("无法打开文件");
        return 1;
    }

    // 创建 epoll 实例
    epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        perror("创建 epoll 实例失败");
        return 1;
    }

    // 将文件描述符添加到 epoll 实例中
    event.events = EPOLLIN | EPOLLET;  // 监听可读事件，使用边缘触发模式
    event.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1) 
    {
        perror("添加文件描述符到 epoll 实例失败");
        return 1;
    }

    // 异步读取文件内容
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) 
    {
        if (errno == EAGAIN) 
        {
            printf("I/O 操作仍在进行中，稍后再试\n");
        } 
        else 
        {
            perror("读取文件失败");
            return 1;
        }
    }

    // 关闭文件
    close(fd);

    // 打印文件内容
    printf("文件内容：%.*s\n", bytesRead, buffer);

    return 0;
}

```

上述代码中，使用了Linux的`epoll`机制来实现异步I/O。

首先，将文件描述符添加到`epoll`实例中，并设置对可读事件感兴趣。

然后，通过非阻塞（`O_NONBLOCK`）方式打开文件，并尝试读取文件内容。如果文件尚未准备好（即I/O仍在进行中），则`read`函数会立即返回，我们可以稍后再次尝试。

最后，关闭文件并处理已读取的内容。

### epoll机制

当使用`epoll`机制时，通常需要按照以下步骤进行操作：

1. 创建epoll实例：使用`epoll_create1`函数创建一个epoll实例，该函数返回一个与该实例相关的文件描述符。例如：
   ```c
   int epollfd = epoll_create1(0);
   if (epollfd == -1) {
       perror("创建epoll实例失败");
       return 1;
   }
   ```

2. 设置感兴趣的事件：创建一个`struct epoll_event`结构，并设置相关的事件类型和其他参数。`struct epoll_event`结构包含两个字段：`events`和`data`。
   - `events`字段用于指定感兴趣的事件类型，常用的事件类型包括：
     - `EPOLLIN`：可读事件，表示文件描述符有数据可读。
     - `EPOLLOUT`：可写事件，表示文件描述符可以写入数据。
     - `EPOLLERR`：错误事件，表示文件描述符发生错误。
     - `EPOLLHUP`：挂起事件，表示文件描述符挂起。
   - `data`字段用于存储与文件描述符相关的用户数据。

3. 将文件描述符添加到epoll实例：使用`epoll_ctl`函数将文件描述符添加到epoll实例中。此时，指定操作标志为`EPOLL_CTL_ADD`，并传递文件描述符、感兴趣的事件和相关参数。例如：
   ```c
   struct epoll_event event;
   event.events = EPOLLIN;  // 设置感兴趣的事件类型
   event.data.fd = fd;  // 设置文件描述符
   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1) {
       perror("将文件描述符添加到epoll实例失败");
       return 1;
   }
   ```

4. 等待事件发生：使用`epoll_wait`函数等待事件的发生。该函数会阻塞，直到有事件发生或超时。`epoll_wait`函数会填充一个存放事件的数组，我们可以对数组中的事件进行处理。例如：
   ```c
   #define MAX_EVENTS 10
   struct epoll_event events[MAX_EVENTS];
   int numEvents = epoll_wait(epollfd, events, MAX_EVENTS, -1);
   if (numEvents == -1) {
       perror("等待事件发生失败");
       return 1;
   }
   ```

5. 处理事件：根据`epoll_wait`返回的事件数量，对每个事件进行处理。通过访问事件的`data.fd`字段，可以获取与事件相关联的文件描述符。根据事件类型进行相应的操作。例如：
   ```c
   for (int i = 0; i < numEvents; i++) {
       int fd = events[i].data.fd;
       if (events[i].events & EPOLLIN) {
           // 可读事件发生，读取数据
           // ...
       }
       if (events[i].events & EPOLLOUT) {
           // 可写事件发生，写入数据
           // ...
       }
       if (events[i].events & EPOLLERR) {
            // 错误事件发生，处理错误
              // ...
          }
          // ...
      }
          

6. 从epoll实例中移除文件描述符（可选）：在不再需要监听某个文件描述符的事件时，可以使用`epoll_ctl`函数将其从epoll实例中移除。此时，指定操作标志为`EPOLL_CTL_DEL`。例如：
   ```c
   
      if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
          perror("从epoll实例中移除文件描述符失败");
          return 1;
      }

需要注意的是，以上步骤仅为基本的使用方法示例，实际使用中可能会根据具体需求和场景进行调整和扩展。

使用epoll机制时，关键的函数包括`epoll_create1`、`epoll_ctl`和`epoll_wait`。这些函数提供了创建epoll实例、添加/移除文件描述符和等待事件发生的功能，通过合理配置和处理，可以实现高效的事件驱动编程。



