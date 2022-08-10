#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#define CLI_PATH "/var/tmp/" /* +5 for pid = 14 chars */
//�����ͻ��˽��̣��ɹ�����0��������С��0��errno
int cli_conn(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	//���������׽���domain
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return(-1);
	//ʹ���Զ����ַ���socket��ַ�ṹ��
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05d", CLI_PATH, getpid());
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	unlink(un.sun_path); //�������ļ��Ѵ��ڵ��µ�bind()ʧ��
	if (bind(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -2;
		goto errout;
	}
	//ʹ�÷��������̵�ַ���socket��ַ�ṹ��
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	if (connect(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -4;
		goto errout;
	}
	return(fd);
errout:
	err = errno;
	close(fd);
	errno = err;
	return(rval);
}
int main(void)
{
	int fd, n;
	char buf[1024];
	fd = cli_conn("foo.socket");		//�׽����ļ�Ϊfoo.socket
	if (fd < 0) {						//�ݴ���
		switch (fd) {
		case -4:perror("connect"); break;
		case -3:perror("listen"); break;
		case -2:perror("bind"); break;
		case -1:perror("socket"); break;
		}
		exit(-1);
	}
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		write(fd, buf, strlen(buf));
		n = read(fd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, n);
	}
	close(fd);
	return 0;
}
