//δ���mutex
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *tfn(void *arg)
{
	srand(time(NULL));
	while (1) {
		printf("hello ");
		//ģ�ⳤʱ�����������Դ������cpu������������ʱ���йصĴ���
		sleep(rand() % 3);
		printf("world\n");
		sleep(rand() % 3);
	}
	return NULL;
}
int main(void)
{
	pthread_t tid;
	srand(time(NULL));
	pthread_create(&tid, NULL, tfn, NULL);
	while (1) {
		printf("HELLO ");
		sleep(rand() % 3);
		printf("WORLD\n");
		sleep(rand() % 3);
	}
	pthread_join(tid, NULL);
	return 0;
}
