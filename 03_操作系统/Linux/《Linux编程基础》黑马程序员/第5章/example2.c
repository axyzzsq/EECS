#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main()
{
	int fd = 0;
	//路径中的目录若不存在将导致文件创建失败
	char filename[20] = "/home/itheima/a.txt";
	//打开文件
	fd = open(filename, O_RDWR | O_EXCL | O_TRUNC, S_IRWXG);
	if (fd == -1){		//判断文件是否成功打开
		perror("file open error.\n");
		exit(-1);
	}
	//写数据
	int len = 0;
	char buf[100] = { 0 };
	scanf("%s", buf);
	len = strlen(buf);
	write(fd, buf, len);
	close(fd);							//关闭文件
	printf("---------------------\n");
	//读取文件
	fd = open(filename, O_RDONLY);			//再次打开文件
	if (fd == -1){
		perror("file open error.\n");
		exit(-1);
	}
	off_t f_size = 0;
	f_size = lseek(fd, 0, SEEK_END);    		//获取文件长度
	lseek(fd, 0, SEEK_SET);				//设置文件读写位置
	while (lseek(fd, 0, SEEK_CUR) != f_size)	//读取文件
	{
		read(fd, buf, 1024);
		printf("%s\n", buf);
	}
	close(fd);
	return 0;
}
