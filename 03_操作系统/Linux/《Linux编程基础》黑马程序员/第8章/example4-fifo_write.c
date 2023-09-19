#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	if (argc < 2)						//�ж��Ƿ����ļ���
	{
		printf("./a.out fifoname\n");
		exit(1);
	}
	int ret = access(argv[1], F_OK);	//�ж�fifo�ļ��Ƿ����
	if (ret == -1)						//��fifo�����ھʹ���fifo
	{
		int r = mkfifo(argv[1], 0664);
		if (r == -1){					//�ж��ļ��Ƿ񴴽��ɹ�
			perror("mkfifo");
			exit(1);
		}
		else{
			printf("fifo creat success!\n");
		}
	}
	int fd = open(argv[1], O_WRONLY);	//�Զ�д�ķ�ʽ���ļ�
	while (1){						//ѭ��д������
		char *p = "hello,world!";
		write(fd, p, strlen(p) + 1);
		sleep(1);
	}
	close(fd);
	return 0;
}
