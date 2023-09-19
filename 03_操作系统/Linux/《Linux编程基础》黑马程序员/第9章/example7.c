#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
void *tfn(void *arg)
{
	int n = 5;
	while (n--) {
		printf("pthread tfn n = %d\n", n);
		sleep(1);
	}
	return (void *)7;
}
int main(void)
{
	pthread_t tid;
	void *ret;
	pthread_create(&tid, NULL, tfn, NULL);
	pthread_detach(tid);				//分离新线程
	int retvar = pthread_join(tid, (void **)&ret);
	if (retvar != 0) {
		fprintf(stderr, "pthread_join error %s\n", strerror(retvar));
	}
	else
	{
		printf("pthread exit with %ld\n", (long int)ret);
	}
	return 0;
}
