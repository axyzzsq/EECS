#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
void do_sig_child(int signo)					//信号处理函数
{
	waitpid(0, NULL, WNOHANG);
}
int main(void)
{
	pid_t pid;
	int i;
	for (i = 0; i < 5; i++) {					//子进程创建
		if ((pid = fork()) == 0)
			break;
		else if (pid < 0)						//容错处理
			sys_err("fork");
	}
	if (pid == 0) {								//子进程分支
		int n = 1;
		while (n--) {
			printf("child ID %d\n", getpid());
		}
		exit(i + 1);
	}
	else if (pid > 0) {							//父进程分支
		struct sigaction act;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, NULL);
		while (1) {
			printf("Parent ID %d\n", getpid());
			sleep(1);
		}
	}
	return 0;
}
