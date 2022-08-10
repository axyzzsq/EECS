//未添加mutex
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *tfn(void *arg)
{
	srand(time(NULL));
	while (1) {
		printf("hello ");
		//模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误
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
