#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void sig_int(int signo)
{
	printf("...........catch you,SIGINT,signo=%d\n", signo);
	sleep(5);							//ģ���źŴ�����ִ��ʱ��
}
int main()
{
	struct sigaction act, oldact;
	act.sa_handler = sig_int;				//�޸��źŴ�����ָ��
	sigemptyset(&act.sa_mask);  			//��ʼ��λͼ����ʾ�������κ��ź�
	sigaddset(&act.sa_mask, SIGINT);		//�����ź�SIGINT���źŴ�����
	act.sa_flags = 0;     					//����flags���������������ź�
	sigaction(SIGINT, &act, &oldact);
	while (1);
	return 0;
}
