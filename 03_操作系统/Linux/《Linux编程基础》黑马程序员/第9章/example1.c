#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
void *tfn(void *arg)
{
	printf("tfn--pid=%d,tid=%lu\n", getpid(), pthread_self());
	return (void*)0;
}
int main()
{
	pthread_t tid;
	printf("main--pid=%d,tid=%lu\n", getpid(), pthread_self());
	int ret = pthread_create(&tid, NULL, tfn, NULL);
	if (ret != 0){
		fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
		exit(1);
	}
	sleep(1);
	return 0;
}
