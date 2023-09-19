#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	pid_t pid, p, w;
	pid = fork();							//创建第一个子进程
	if (pid == -1){							//第一个子进程创建后父子进程的执行内容
		perror("fork1 error");
		exit(1);
	}
	else if (pid == 0){						//子进程沉睡
		sleep(5);
		printf("First child process:pid=%d\n", getpid());
	}
	else if (pid > 0){						//父进程继续创建进程
		int i;
		p = pid;
		for (i = 0; i < 3; i++)					//由父进程创建3个子进程
		{
			if ((pid = fork()) == 0)
				break;
		}
		if (pid == -1){						//出错
			perror("fork error");
			exit(2);
		}
		else if (pid == 0){					//子进程
			printf("Child process:pid=%d\n", getpid());
			exit(0);
		}
		else if (pid > 0){					//父进程
			w = waitpid(p, NULL, 0);			//等待第一个子进程执行
			if (w == p)
				printf("Catch a child Process:pid=%d\n", w);
			else
				printf("waitpid error\n");
		}
	}
	return 0;
}
