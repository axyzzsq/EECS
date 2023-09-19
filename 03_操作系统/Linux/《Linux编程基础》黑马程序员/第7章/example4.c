#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void printset(sigset_t *ped)			//pending打印函数
{
	int i;
	for (i = 1; i < 32; i++){
		if ((sigismember(ped, i) == 1))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}
int main()
{
	sigset_t set, oldset, ped;			//信号集定义
	sigemptyset(&set);					//初始化自定义信号集set
	sigaddset(&set, SIGINT);				//将2号信号SIGINT加入set
	sigprocmask(SIG_BLOCK, &set, &oldset);//位操作
	while (1){
		sigpending(&ped);
		printset(&ped);
		sleep(1);
	}
	return 0;
}
