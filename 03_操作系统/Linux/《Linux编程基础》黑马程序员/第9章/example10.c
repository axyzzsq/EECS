#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
struct msg {
	struct msg *next;
	int num;
};
struct msg *head;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;	//初始化条件变量
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;		//初始化互斥锁
//消费者
void *consumer(void *p)
{
	struct msg *mp;
	for (;;) {
		pthread_mutex_lock(&lock);						//加锁
		//若头结点为空，表明产品数量为0，消费者无法消费产品
		while (head == NULL) {
			pthread_cond_wait(&has_product, &lock);		//阻塞等待并解锁
		}
		mp = head;
		head = mp->next;    					//模拟消费一个产品
		pthread_mutex_unlock(&lock);
		printf("-Consume ---%d\n", mp->num);
		free(mp);
		sleep(rand() % 5);
	}
}
//生产者
void *producer(void *p)
{
	struct msg *mp;
	while (1) {
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000 + 1;        	//模拟生产一个产品
		printf("-Produce ---%d\n", mp->num);
		pthread_mutex_lock(&lock);			//加锁
		mp->next = head;						//插入结点（添加产品）
		head = mp;
		pthread_mutex_unlock(&lock);			//解锁
		pthread_cond_signal(&has_product);  	//唤醒等待在该条件变量上的一个线程
		sleep(rand() % 5);
	}
}
int main(int argc, char *argv[])
{
	pthread_t pid, cid;
	srand(time(NULL));
	//创建生产者、消费者线程
	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	//回收线程
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);
	return 0;
}
