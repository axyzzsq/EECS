#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM 5 
int queue[NUM];                           //ȫ������ʵ�ֻ��ζ���
sem_t blank_number, product_number;   //�ո����ź���, ��Ʒ�ź���
void *producer(void *arg)
{
	int i = 0;
	while (1) {
		sem_wait(&blank_number);       //�����߽��ո�����--,Ϊ0�������ȴ�
		queue[i] = rand() % 1000 + 1;          	//����һ����Ʒ
		printf("----Produce---%d\n", queue[i]);
		sem_post(&product_number);             	//����Ʒ��++
		i = (i + 1) % NUM;                         	//�����±�ʵ�ֻ���
		sleep(rand() % 1);
	}
}
void *consumer(void *arg)
{
	int i = 0;
	while (1) {
		sem_wait(&product_number); //�����߽���Ʒ��--,Ϊ0�������ȴ�
		printf("-Consume---%d\t%lu\n", queue[i], pthread_self());
		queue[i] = 0;                               	//����һ����Ʒ 
		sem_post(&blank_number);                 	//���ѵ��Ժ�,���ո�����++
		i = (i + 1) % NUM;
		sleep(rand() % 1);
	}
}
int main(int argc, char *argv[])
{
	pthread_t pid, cid;
	sem_init(&blank_number, 0, NUM);           	//��ʼ���ո����ź���Ϊ5
	sem_init(&product_number, 0, 0);           	//��ʼ����Ʒ���ź���Ϊ0
	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);
	sem_destroy(&blank_number);
	sem_destroy(&product_number);
	return 0;
}
