# context 

在 Linux 中，术语“上下文”通常是指处理器寄存器的状态以及进程或线程的执行环境。 进程或线程的上下文包括程序计数器 (PC)、堆栈指针 (SP) 和其他寄存器的值等信息，以及进程或线程可能正在使用的有关系统调用表、文件描述符和其他资源的状态的信息。

在 C 代码中，术语“上下文”可能指的是函数调用的上下文，其中包括函数参数的值、局部变量和程序计数器。 调用函数时，处理器在执行函数之前将当前上下文（包括 PC 和其他寄存器）保存到堆栈中。 当函数返回时，处理器从堆栈中恢复先前的上下文并在函数调用之后的指令处恢复执行。

在 Linux 编程中，上下文切换是保存正在运行的进程或线程的当前上下文并恢复不同进程或线程的上下文的过程。 这是 Linux 内核使用的一种基本机制，允许多个进程或线程在同一系统上并发运行。 上下文切换由内核调度程序执行，它会定期中断正在运行的进程或线程，并根据其优先级和其他标准选择一个新进程或线程运行。

C 代码可以使用 fork()、exec()、wait()、pthread_create() 和 pthread_join() 等系统调用与 Linux 调度程序交互并管理进程或线程上下文。 这些系统调用允许 C 程序创建新进程或线程、管理它们的执行并同步它们对共享资源的访问。 此外，C 代码可以使用信号和信号处理程序来管理异步事件，这些事件可以中断进程或线程的执行并更改其上下文。

## fork() and context

```C
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Parent process (PID=%d) is starting...\n", getpid());

    pid = fork(); // create a new child process

    if (pid == -1) {
        // error handling
        fprintf(stderr, "fork() failed\n");
        return 1;
    }
    else if (pid == 0) {
        // child process
        printf("Child process (PID=%d) is running...\n", getpid());
        sleep(3); // simulate some work
        printf("Child process (PID=%d) is exiting...\n", getpid());
        return 0;
    }
    else {
        // parent process
        printf("Parent process (PID=%d) is waiting for child process (PID=%d)...\n", getpid(), pid);
        waitpid(pid, NULL, 0); // wait for child to exit
        printf("Parent process (PID=%d) has finished.\n", getpid());
        return 0;
    }
}

```

This program creates a new child process using the `fork()` system call. The child process executes some code and then exits, while the parent process waits for the child to exit using the `waitpid()` system call.

Here's a brief explanation of the program's context-related features:

- 父进程使用 `fork()` 的返回值初始化 `pid` 变量。 该值是标识子进程的进程` ID (PID)`。 父进程在` fork()` 调用后继续执行，而子进程从与父进程相同的点开始执行，但在单独的执行上下文中。

- 在子进程的代码块中，程序打印包含子进程 `PID `的消息，使用` sleep() `函数模拟一些工作，然后通过返回状态代码 0 退出。当子进程退出时，其执行上下文 被销毁，其资源被操作系统回收。

- I在父进程的代码块中，程序打印一条消息，其中包含父进程的` PID `和子进程的 `PID`，表明父进程正在等待子进程退出。 然后父进程使用 `waitpid() `系统调用等待子进程退出。 在此期间，父进程的执行上下文被挂起，操作系统的调度程序可以切换到不同的进程或线程。 当子进程退出时，`waitpid()` 调用返回，父进程继续执行。

- 该程序使用标准输入/输出函数将消息打印到控制台，并将错误消息打印到`“stderr”`。 这些函数使用文件描述符与操作系统的输入/输出系统进行交互，该系统由内核使用单独的执行上下文进行管理。
