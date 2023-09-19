#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int i;
	for (i = 0; i<5; i++){				//循环创建进程
		if ((pid = fork()) == 0)		//若当前进程为子进程，便跳出循环
			break;
	}
	if (pid == -1){
		perror("fork error");
		exit(1);
	}
	else if (pid>0){					//父进程
		printf("parent process:pid=%d\n", getpid());
	}
	else if (pid == 0){					//子进程
		printf("I am child=%d,pid=%d\n", i + 1, getpid());
	}
	return 0;
}
