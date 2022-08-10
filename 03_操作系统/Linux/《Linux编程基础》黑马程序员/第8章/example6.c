#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
//自定义共用体
union semu{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
	struct seminfo* _buf;
};
static int sem_id;
//设置信号量值
static int set_semvalue()
{
	union semu sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
		return 0;
	return 1;
}
//p操作，获取信号量
static int semaphore_p()
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1){
		perror("sem_p err");
		return 0;
	}
	return 1;
}
//V操作，释放信号量
static int semaphore_v()
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1){
		perror("sem_v err");
		return 0;
	}
	return 1;
}
//删除信号量
static void del_semvalue()
{
	union semu sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		perror("del err");
}
int main()
{
	int i;
	pid_t pid;
	char ch = 'C';
	sem_id = semget((key_t)1000, 1, 0664 | IPC_CREAT);//创建信号量
	if (sem_id == -1){
		perror("sem_c err");
		exit(-1);
	}
	if (!set_semvalue()){					//设置信号量值
		perror("init err");
		exit(-1);
	}
	pid = fork();								//创建子进程
	if (pid == -1){								//若创建失败
		del_semvalue();						//删除信号量
		exit(-1);
	}
	else if (pid == 0)     						//设置子进程打印的字符
		ch = 'Z';
	else										//设置父进程打印的字符
		ch = 'C';
	srand((unsigned int)getpid());			//设置随机数种子
	for (i = 0; i < 8; i++)							//循环打印字符
	{
		semaphore_p();						//获取信号量
		printf("%c", ch);
		fflush(stdout);						//将字符打印到屏幕
		sleep(rand() % 4);						//沉睡
		printf("%c", ch);
		fflush(stdout);						//再次打印到屏幕
		sleep(1);
		semaphore_v();						//释放信号量
	}
	if (pid > 0){
		wait(NULL);							//回收子进程
		del_semvalue();						//删除信号量
	}
	printf("\nprocess %d finished.\n", getpid());
	return 0;
}
