#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	pid = fork();							//����fork()���������ӽ���
	if (pid == -1)							//����ʧ��
	{
		perror("fork error");
		exit(1);							//�˳����̣�ָ������ֵ1
	}
	else if (pid > 0)						//������
	{
		printf("parent process,pid=%d,ppid=%d\n", getpid(), getppid());
	}
	else if (pid == 0)						//�ӽ���
	{
		printf("child process,pid=%d,ppid=%d\n", getpid(), getppid());
	}
	printf("........finish..........\n");
	return 0;
}
