#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void sig_int(int signo)				//自定义信号处理函数
{
	printf(".........catch you,SIGINT\n");
	signal(SIGINT, SIG_DFL);			//信号处理函数执行
}
int main()
{
	signal(SIGINT, sig_int);			//捕获信号SIGINT，修改信号处理函数
	while (1);						//等待信号递达
	return 0;
}
