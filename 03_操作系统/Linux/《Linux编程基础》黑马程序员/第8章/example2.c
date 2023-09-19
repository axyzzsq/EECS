#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	int fd[2];
	int ret = pipe(fd);
	if (ret == -1){
		perror("pipe err");
		exit(1);
	}
	int i;
	pid_t pid, wpid;
	for (i = 0; i < 2; i++){					//���������ӽ���
		if ((pid = fork()) == 0)
			break;
	}
	if (2 == i){               				//������
		close(fd[0]);
		close(fd[1]);
		wpid = wait(NULL);
		printf("wait child 1 success,pid=%d\n", wpid);
		pid = wait(NULL);
		printf("wait child 2 success,pid=%d\n", pid);
	}
	else if (i == 0){          				//�ӽ��̡�д
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);		//��fd[1]��ָ�ļ����ݶ��򵽱�׼���
		execlp("ls", "ls", NULL);
	}
	else if (i == 1){          				//�ӽ��̡���
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		execlp("wc", "wc", "-l", NULL);
	}
	return 0;
}
