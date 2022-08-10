#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
void sig_alrm(int signo)
{
	//do something...   
}

unsigned int mysleep(unsigned int seconds)
{
	struct sigaction newact, oldact;
	unsigned int unslept;
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);		//屏蔽信号SIGALRM
	alarm(seconds);							//倒计时
	sigaction(SIGALRM, &oldact, NULL);		//解除信号屏蔽
	pause();									//挂起等待信号
	return alarm(0);							//返回
}
int main()
{
	while (1){
		mysleep(2);
		printf("two seconds passed.\n");
	}
	return 0;
}
