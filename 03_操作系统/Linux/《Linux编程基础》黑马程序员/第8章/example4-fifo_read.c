#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	if (argc < 2)							//判断是否传入文件名
	{
		printf("./a.out fifoname\n");
		exit(1);
	}
	int ret = access(argv[1], F_OK);		//判断文件是否存在
	if (ret == -1)							//若文件不存在则创建文件
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
	int fd = open(argv[1], O_RDONLY);	//打开文件
	if (fd == -1){
		perror("open");
		exit(1);
	}
	while (1){							//不断读取fifo中的数据并打印
		char buf[1024] = { 0 };
		read(fd, buf, sizeof(buf));
		printf("buf=%s\n", buf);
	}
	close(fd);							//关闭文件
	return 0;
}
