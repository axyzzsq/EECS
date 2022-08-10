#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	int fd[2];								//定义文件描述符数组
	int ret = pipe(fd);						//创建管道
	if (ret == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid_t pid = fork();
	if (pid > 0)
	{
		//父进程—写
		close(fd[0]);						//关闭读端
		char *p = "hello,pipe\n";
		write(fd[1], p, strlen(p) + 1);			//写数据
		close(fd[1]);
		wait(NULL);
	}
	else if (pid == 0)
	{
		//子进程—读
		close(fd[1]);						//关闭写端
		char buf[64] = { 0 };
		ret = read(fd[0], buf, sizeof(buf));		//读数据
		close(fd[0]);
		write(STDOUT_FILENO, buf, ret);		//将读到的数据写到标准输出
	}
	return 0;
}
