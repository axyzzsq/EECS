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
//创建服务器进程，成功返回0，出错返回小于0的errno
int serv_listen(const char *name)
{
	int fd, len, err, rval;
	struct sockaddr_un un;
	//创建本地domain套接字
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return(-1);
	//删除套接字文件，避免因文件存在导致bind()绑定失败	
	unlink(name);
	//初始化套接字结构体地址
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	if (bind(fd, (struct sockaddr *)&un, len) < 0) {
		rval = -2;
		goto errout;
	}
	if (listen(fd, QLEN) < 0) { //告知内核这是一个服务器进程
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
	//从调用地址获取客户端的uid
	len -= offsetof(struct sockaddr_un, sun_path); //获取路径名长度
	un.sun_path[len] = 0; 	//为路径名字符串添加终止符
	if (stat(un.sun_path, &statbuf) < 0) {
		rval = -2;
		goto errout;
	}
	if (S_ISSOCK(statbuf.st_mode) == 0) {
		rval = -3; 				//若返回值为-3，说明这不是一个socket文件
		goto errout;
	}
	if (uidptr != NULL)
		*uidptr = statbuf.st_uid; 		//返回uid的调用者指针
	//到此成功获取路径名
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
