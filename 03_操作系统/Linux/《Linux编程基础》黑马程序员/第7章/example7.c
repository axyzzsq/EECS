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
	sigaction(SIGALRM, &newact, &oldact);		//�����ź�SIGALRM
	alarm(seconds);							//����ʱ
	sigaction(SIGALRM, &oldact, NULL);		//����ź�����
	pause();									//����ȴ��ź�
	return alarm(0);							//����
}
int main()
{
	while (1){
		mysleep(2);
		printf("two seconds passed.\n");
	}
	return 0;
}
