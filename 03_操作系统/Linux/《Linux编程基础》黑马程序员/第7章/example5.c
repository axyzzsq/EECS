#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void sig_int(int signo)				//�Զ����źŴ�����
{
	printf(".........catch you,SIGINT\n");
	signal(SIGINT, SIG_DFL);			//�źŴ�����ִ��
}
int main()
{
	signal(SIGINT, sig_int);			//�����ź�SIGINT���޸��źŴ�����
	while (1);						//�ȴ��źŵݴ�
	return 0;
}
