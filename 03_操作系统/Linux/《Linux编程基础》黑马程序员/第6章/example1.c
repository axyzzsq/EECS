#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	pid = fork();							//调用fork()函数创建子进程
	if (pid == -1)							//创建失败
	{
		perror("fork error");
		exit(1);							//退出进程，指定返回值1
	}
	else if (pid > 0)						//父进程
	{
		printf("parent process,pid=%d,ppid=%d\n", getpid(), getppid());
	}
	else if (pid == 0)						//子进程
	{
		printf("child process,pid=%d,ppid=%d\n", getpid(), getppid());
	}
	printf("........finish..........\n");
	return 0;
}
