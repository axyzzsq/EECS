#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
//socket()
int Socket(int family, int type, int protocol)
{
	int n;
	if ((n = socket(family, type, protocol)) < 0)//��socket����ʧ��
		perr_exit("socket error");
	return n;
}
//bind()
void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)				//��bind()����ʧ��
		perr_exit("bind error");
}
//listen()
void Listen(int fd, int backlog)
{
	if (listen(fd, backlog) < 0)				//��listen()����ʧ��
		perr_exit("listen error");
}
//connect()
void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)				//��connect()����ʧ��
		perr_exit("connect error");
}
//accept()
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int n;
again:
	if ((n = accept(fd, sa, salenptr)) < 0) {	//��accept()����ʧ��
		if ((errno == ECONNABORTED) || (errno == EINTR))
			goto again;
		else
			perr_exit("accept error");
	}
	return n;
}
//close()
void Close(int fd)
{
	if (close(fd) == -1)						//�ر�socketʧ��
		perr_exit("close error");
}
//read()
ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t n;
again:
	if ((n = read(fd, ptr, nbytes)) == -1) {	//��ȡ����ʧ��
		if (errno == EINTR)
			goto again;							//�ض�fd
		else
			return -1;
	}
	return n;
}
//write()
ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
	ssize_t n;
again:
	if ((n = write(fd, ptr, nbytes)) == -1) {	//д���ݳ���
		if (errno == EINTR)
			goto again;							//����д��
		else
			return -1;
	}
	return n;
}
void perr_exit(const char *s)
{
	perror(s);
	exit(1);
}
