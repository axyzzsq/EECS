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
	pthread_t tid;					//线程id
	int err, detachstate;
	int i = 1;
	pthread_attr_t attr;				//线程属性
	size_t stacksize;				//栈容量
	void *stackaddr;					//栈地址
	pthread_attr_init(&attr);		//初始化线程属性结构体
	//获取程栈地址、栈容量
	pthread_attr_getstack(&attr, &stackaddr, &stacksize);
	//获取线程分离状态
	pthread_attr_getdetachstate(&attr, &detachstate);
	//判断线程分离状态
	if (detachstate == PTHREAD_CREATE_DETACHED)
		printf("thread detached\n");
	else if (detachstate == PTHREAD_CREATE_JOINABLE)
		printf("thread join\n");
	else
		printf("thread un known\n");
	//设置线程分离状态，使线程分离
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	while (1) {
		//在堆上申请内存,指定线程栈的起始地址和大小
		stackaddr = malloc(SIZE);
		if (stackaddr == NULL) {
			perror("malloc");
			exit(1);
		}
		stacksize = SIZE;
		//设置线程栈地址和栈容量
		pthread_attr_setstack(&attr, stackaddr, stacksize);
		//使用自定义属性创建线程
		err = pthread_create(&tid, &attr, th_fun, NULL);
		if (err != 0) {
			printf("%s\n", strerror(err));
			exit(1);
		}
		i++;
		printf("%d\n", i);				//打印线程编号
	}
	pthread_attr_destroy(&attr);		//销毁attr资源
	return 0;
}
