#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main()
{
	int fd = 0;
	//·���е�Ŀ¼�������ڽ������ļ�����ʧ��
	char filename[20] = "/home/itheima/a.txt";
	//���ļ�
	fd = open(filename, O_RDWR | O_EXCL | O_TRUNC, S_IRWXG);
	if (fd == -1){		//�ж��ļ��Ƿ�ɹ���
		perror("file open error.\n");
		exit(-1);
	}
	//д����
	int len = 0;
	char buf[100] = { 0 };
	scanf("%s", buf);
	len = strlen(buf);
	write(fd, buf, len);
	close(fd);							//�ر��ļ�
	printf("---------------------\n");
	//��ȡ�ļ�
	fd = open(filename, O_RDONLY);			//�ٴδ��ļ�
	if (fd == -1){
		perror("file open error.\n");
		exit(-1);
	}
	off_t f_size = 0;
	f_size = lseek(fd, 0, SEEK_END);    		//��ȡ�ļ�����
	lseek(fd, 0, SEEK_SET);				//�����ļ���дλ��
	while (lseek(fd, 0, SEEK_CUR) != f_size)	//��ȡ�ļ�
	{
		read(fd, buf, 1024);
		printf("%s\n", buf);
	}
	close(fd);
	return 0;
}
