#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main()
{
	pid_t pid;
	pid = fork();
	if (pid == 0){			//子进程
		sleep(1);
		printf("child pid=%d,ppid=%d\n", getpid(), getppid());
		kill(getppid(), SIGKILL);//发送信号SIGKILL给父进程
	}
	else if (pid > 0){		//父进程
		while (1){
			printf("parent pid=%d,ppid=%d\n", getpid(), getppid());
		}
	}
	return 0;
}
