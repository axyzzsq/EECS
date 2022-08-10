#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	if (argc < 2)							//�ж��Ƿ����ļ���
	{
		printf("./a.out fifoname\n");
		exit(1);
	}
	int ret = access(argv[1], F_OK);		//�ж��ļ��Ƿ����
	if (ret == -1)							//���ļ��������򴴽��ļ�
	{
		int r = mkfifo(argv[1], 0664);
		if (r == -1){
			perror("mkfifo");
			exit(1);
		}
		else{
			printf("fifo creat success!\n");
		}
	}
	int fd = open(argv[1], O_RDONLY);	//���ļ�
	if (fd == -1){
		perror("open");
		exit(1);
	}
	while (1){							//���϶�ȡfifo�е����ݲ���ӡ
		char buf[1024] = { 0 };
		read(fd, buf, sizeof(buf));
		printf("buf=%s\n", buf);
	}
	close(fd);							//�ر��ļ�
	return 0;
}
