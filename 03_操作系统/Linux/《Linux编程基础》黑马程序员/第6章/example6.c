#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	pid_t pid, p, w;
	pid = fork();							//������һ���ӽ���
	if (pid == -1){							//��һ���ӽ��̴������ӽ��̵�ִ������
		perror("fork1 error");
		exit(1);
	}
	else if (pid == 0){						//�ӽ��̳�˯
		sleep(5);
		printf("First child process:pid=%d\n", getpid());
	}
	else if (pid > 0){						//�����̼�����������
		int i;
		p = pid;
		for (i = 0; i < 3; i++)					//�ɸ����̴���3���ӽ���
		{
			if ((pid = fork()) == 0)
				break;
		}
		if (pid == -1){						//����
			perror("fork error");
			exit(2);
		}
		else if (pid == 0){					//�ӽ���
			printf("Child process:pid=%d\n", getpid());
			exit(0);
		}
		else if (pid > 0){					//������
			w = waitpid(p, NULL, 0);			//�ȴ���һ���ӽ���ִ��
			if (w == p)
				printf("Catch a child Process:pid=%d\n", w);
			else
				printf("waitpid error\n");
		}
	}
	return 0;
}
