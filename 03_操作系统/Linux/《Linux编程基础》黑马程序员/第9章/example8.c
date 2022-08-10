#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 0x90000000
void *th_fun(void *arg)
{
	while (1)
		sleep(1);
}
int main()
{
	pthread_t tid;					//�߳�id
	int err, detachstate;
	int i = 1;
	pthread_attr_t attr;				//�߳�����
	size_t stacksize;				//ջ����
	void *stackaddr;					//ջ��ַ
	pthread_attr_init(&attr);		//��ʼ���߳����Խṹ��
	//��ȡ��ջ��ַ��ջ����
	pthread_attr_getstack(&attr, &stackaddr, &stacksize);
	//��ȡ�̷߳���״̬
	pthread_attr_getdetachstate(&attr, &detachstate);
	//�ж��̷߳���״̬
	if (detachstate == PTHREAD_CREATE_DETACHED)
		printf("thread detached\n");
	else if (detachstate == PTHREAD_CREATE_JOINABLE)
		printf("thread join\n");
	else
		printf("thread un known\n");
	//�����̷߳���״̬��ʹ�̷߳���
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	while (1) {
		//�ڶ��������ڴ�,ָ���߳�ջ����ʼ��ַ�ʹ�С
		stackaddr = malloc(SIZE);
		if (stackaddr == NULL) {
			perror("malloc");
			exit(1);
		}
		stacksize = SIZE;
		//�����߳�ջ��ַ��ջ����
		pthread_attr_setstack(&attr, stackaddr, stacksize);
		//ʹ���Զ������Դ����߳�
		err = pthread_create(&tid, &attr, th_fun, NULL);
		if (err != 0) {
			printf("%s\n", strerror(err));
			exit(1);
		}
		i++;
		printf("%d\n", i);				//��ӡ�̱߳��
	}
	pthread_attr_destroy(&attr);		//����attr��Դ
	return 0;
}
