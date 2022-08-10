#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void sig_int(int signo)
{
	printf("...........catch you,SIGINT,signo=%d\n", signo);
	sleep(5);							//模拟信号处理函数执行时间
}
int main()
{
	struct sigaction act, oldact;
	act.sa_handler = sig_int;				//修改信号处理函数指针
	sigemptyset(&act.sa_mask);  			//初始化位图，表示不屏蔽任何信号
	sigaddset(&act.sa_mask, SIGINT);		//更改信号SIGINT的信号处理函数
	act.sa_flags = 0;     					//设置flags，屏蔽自身所发信号
	sigaction(SIGINT, &act, &oldact);
	while (1);
	return 0;
}
