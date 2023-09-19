#include <stdio.h>
#include <signal.h>
#include <stdio.h>
void sig_alrm(int signo)
{
	//do something...
}
unsigned int mysleep(unsigned int seconds)
{
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslept;
	//①为SIGALRM设置捕捉函数
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);
	//②设置阻塞信号集，屏蔽SIGALRM信号
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	//③设置计时器
	alarm(seconds);
	//④构造临时阻塞信号集
	suspmask = oldmask;
	sigdelset(&suspmask, SIGALRM);
	//⑤采用临时阻塞信号集suspmask替换原有阻塞信号集（不包含SIGALRM信号）
	sigsuspend(&suspmask);			//挂起进程，等待信号递达
	unslept = alarm(0);
	//⑥恢复SIGALRM原有的处理动作，呼应注释①
	sigaction(SIGALRM, &oldact, NULL);
	//⑦解除对SIGALRM的屏蔽，呼应注释②
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	return unslept;
}
int main()
{
	while (1){
		mysleep(2);
		printf("two seconds passed\n");
	}
	return 0;
}
