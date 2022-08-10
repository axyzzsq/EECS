#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
//�Զ��干����
union semu{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
	struct seminfo* _buf;
};
static int sem_id;
//�����ź���ֵ
static int set_semvalue()
{
	union semu sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
		return 0;
	return 1;
}
//p��������ȡ�ź���
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
//V�������ͷ��ź���
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
//ɾ���ź���
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
	sem_id = semget((key_t)1000, 1, 0664 | IPC_CREAT);//�����ź���
	if (sem_id == -1){
		perror("sem_c err");
		exit(-1);
	}
	if (!set_semvalue()){					//�����ź���ֵ
		perror("init err");
		exit(-1);
	}
	pid = fork();								//�����ӽ���
	if (pid == -1){								//������ʧ��
		del_semvalue();						//ɾ���ź���
		exit(-1);
	}
	else if (pid == 0)     						//�����ӽ��̴�ӡ���ַ�
		ch = 'Z';
	else										//���ø����̴�ӡ���ַ�
		ch = 'C';
	srand((unsigned int)getpid());			//�������������
	for (i = 0; i < 8; i++)							//ѭ����ӡ�ַ�
	{
		semaphore_p();						//��ȡ�ź���
		printf("%c", ch);
		fflush(stdout);						//���ַ���ӡ����Ļ
		sleep(rand() % 4);						//��˯
		printf("%c", ch);
		fflush(stdout);						//�ٴδ�ӡ����Ļ
		sleep(1);
		semaphore_v();						//�ͷ��ź���
	}
	if (pid > 0){
		wait(NULL);							//�����ӽ���
		del_semvalue();						//ɾ���ź���
	}
	printf("\nprocess %d finished.\n", getpid());
	return 0;
}
