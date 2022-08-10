#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	if (argc < 2)						//判断是否传入文件名
	{
		printf("./a.out fifoname\n");
		exit(1);
	}
	int ret = access(argv[1], F_OK);	//判断fifo文件是否存在
	if (ret == -1)						//若fifo不存在就创建fifo
	{
		int r = mkfifo(argv[1], 0664);
		if (r == -1){					//判断文件是否创建成功
			perror("mkfifo");
			exit(1);
		}
		else{
			printf("fifo creat success!\n");
		}
	}
	int fd = open(argv[1], O_WRONLY);	//以读写的方式打开文件
	while (1){						//循环写入数据
		char *p = "hello,world!";
		write(fd, p, strlen(p) + 1);
		sleep(1);
	}
	close(fd);
	return 0;
}
