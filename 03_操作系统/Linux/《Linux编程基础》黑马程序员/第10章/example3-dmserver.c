#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#define QLEN 10
//�������������̣��ɹ�����0��������С��0��errno
int serv_listen(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	//��������domain�׽���
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return(-1);
	//ɾ���׽����ļ����������ļ����ڵ���bind()��ʧ��	
	unlink(name);
	//��ʼ���׽��ֽṹ���ַ
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	if (bind(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -2;
		goto errout;
	}
	if (listen(fd, QLEN) < 0) { //��֪�ں�����һ������������
		rval = -3;
		goto errout;
	}
	return(fd);
errout:
	err = errno;
	close(fd);
	errno = err;
	return(rval);
}
int serv_accept(int listenfd, uid_t *uidptr)
{
	int clifd, len, err, rval;
	time_t staletime;
	struct sockaddr_un un;
	struct stat statbuf;
	len = sizeof(un);
	if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)
		return(-1);
	//�ӵ��õ�ַ��ȡ�ͻ��˵�uid
	len -= offsetof(struct sockaddr_un, sun_path); //��ȡ·��������
	un.sun_path[len] = 0; 	//Ϊ·�����ַ��������ֹ��
	if (stat(un.sun_path, &statbuf) < 0) {
		rval = -2;
		goto errout;
	}
	if (S_ISSOCK(statbuf.st_mode) == 0) {
		rval = -3; 				//������ֵΪ-3��˵���ⲻ��һ��socket�ļ�
		goto errout;
	}
	if (uidptr != NULL)
		*uidptr = statbuf.st_uid; 		//����uid�ĵ�����ָ��
	//���˳ɹ���ȡ·����
	unlink(un.sun_path);
	return(clifd);
errout:
	err = errno;
	close(clifd);
	errno = err;
	return(rval);
}
int main(void)
{
	int lfd, cfd, n, i;
	uid_t cuid;
	char buf[1024];
	lfd = serv_listen("foo.socket");
	if (lfd < 0) {
		switch (lfd) {
		case -3:perror("listen"); break;
		case -2:perror("bind"); break;
		case -1:perror("socket"); break;
		}
		exit(-1);
	}
	cfd = serv_accept(lfd, &cuid);
	if (cfd < 0) {
		switch (cfd) {
		case -3:perror("not a socket"); break;
		case -2:perror("a bad filename"); break;
		case -1:perror("accept"); break;
		}
		exit(-1);
	}
	while (1) {
	r_again:
		n = read(cfd, buf, 1024);
		if (n == -1) {
			if (errno == EINTR)
				goto r_again;
		}
		else if (n == 0) {
			printf("the other side has been closed.\n");
			break;
		}
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		write(cfd, buf, n);
	}
	close(cfd);
	close(lfd);
	return 0;
}
