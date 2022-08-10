#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include "wrap.h"
#define MAXLINE 80							//缓冲数组大小
#define SERV_PORT 8000						//端口号
#define OPEN_MAX 1024						//最大打开文件描述符数量
int main()
{
	int i, j, maxi, listenfd, connfd, sockfd;
	int nready, efd, res;
	ssize_t n;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	socklen_t clilen;
	int client[OPEN_MAX];
	struct sockaddr_in cliaddr, servaddr;
	struct epoll_event tep, ep[OPEN_MAX];
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	//初始化client集合
	for (i = 0; i < OPEN_MAX; i++)
		client[i] = -1;
	maxi = -1;							//初始化maxi
	efd = epoll_create(OPEN_MAX);		//创建epoll句柄
	if (efd == -1)
		perr_exit("epoll_create");
	//初始化tep
	tep.events = EPOLLIN;
	tep.data.fd = listenfd;
	//为服务器进程注册事件（listenfd）
	res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
	if (res == -1)
		perr_exit("epoll_ctl");
	for (;;) {
		nready = epoll_wait(efd, ep, OPEN_MAX, -1);//阻塞监听
		if (nready == -1)
			perr_exit("epoll_wait");
		//处理就绪事件
		for (i = 0; i < nready; i++) {
			if (!(ep[i].events & EPOLLIN))
				continue;
			//若fd为listenfd，表示有连接请求到达
			if (ep[i].data.fd == listenfd) {
				clilen = sizeof(cliaddr);
				connfd = Accept(listenfd, (struct sockaddr *)&cliaddr,
					&clilen);
				printf("received from %s at PORT %d\n", inet_ntop(AF_INET,
					&cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));		//字节序转换
				//将accept获取到的文件描述符保存到client[]数组中
				for (j = 0; j < OPEN_MAX; j++)
				if (client[j] < 0) {
					client[j] = connfd;
					break;
				}
				if (j == OPEN_MAX)
					perr_exit("too many clients");
				if (j > maxi)
					maxi = j;					//更新最大文件描述符
				tep.events = EPOLLIN;
				tep.data.fd = connfd;
				//为新建立连接的进程注册事件
				res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
				if (res == -1)
					perr_exit("epoll_ctl");
			}
			else {//若fd不等于listenfd，表示就绪的是各路连接
				sockfd = ep[i].data.fd;
				n = Read(sockfd, buf, MAXLINE);
				if (n == 0) {//若读取的字符个数为0表示对应客户端进程将关闭连接
					for (j = 0; j <= maxi; j++) {
						if (client[j] == sockfd) {
							client[j] = -1;
							break;
						}
					}
					//取消监听
					res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
					if (res == -1)
						perr_exit("epoll_ctl");
					Close(sockfd);		//服务器端关闭连接
					printf("client[%d] closed connection\n", j);
				}
				else {
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					Writen(sockfd, buf, n);
				}
			}
		}
	}
	close(listenfd);
	close(efd);
	return 0;
}
