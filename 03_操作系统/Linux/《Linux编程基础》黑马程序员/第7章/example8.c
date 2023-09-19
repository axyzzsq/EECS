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
	//��ΪSIGALRM���ò�׽����
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);
	//�����������źż�������SIGALRM�ź�
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	//�����ü�ʱ��
	alarm(seconds);
	//�ܹ�����ʱ�����źż�
	suspmask = oldmask;
	sigdelset(&suspmask, SIGALRM);
	//�ݲ�����ʱ�����źż�suspmask�滻ԭ�������źż���������SIGALRM�źţ�
	sigsuspend(&suspmask);			//������̣��ȴ��źŵݴ�
	unslept = alarm(0);
	//�޻ָ�SIGALRMԭ�еĴ���������Ӧע�͢�
	sigaction(SIGALRM, &oldact, NULL);
	//�߽����SIGALRM�����Σ���Ӧע�͢�
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
