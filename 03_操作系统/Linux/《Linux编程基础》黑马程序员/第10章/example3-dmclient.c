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
//创建客户端进程，成功返回0，出错返回小于0的errno
int cli_conn(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	//创建本地套接字domain
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return(-1);
	//使用自定义地址填充socket地址结构体
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05d", CLI_PATH, getpid());
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	unlink(un.sun_path); //避免因文件已存在导致的bind()失败
	if (bind(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -2;
		goto errout;
	}
	//使用服务器进程地址填充socket地址结构体
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
	fd = cli_conn("foo.socket");		//套接字文件为foo.socket
	if (fd < 0) {						//容错处理
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
