#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"
#define MAXLINE 80						//缓冲数组大小
#define SERV_PORT 8000					//端口号
int main()
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE]; 	//FD_SETSIZE 默认为1024
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];			//#define INET_ADDRSTRLEN 16
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);		//默认最大128
	maxfd = listenfd;
	maxi = -1;
	//初始化监控列表
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1; 		//使用-1初始化client[]中元素
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset); 	//将listenfd添加到文件描述符集中
	//循环监测处于连接状态进程的文件描述符
	for (;;) {
		//使用变量rset获取文件描述符集合
		rset = allset;
		//记录就绪进程数量
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");
		if (FD_ISSET(listenfd, &rset)){//有新连接请求到达则进行连接便处理连接请求
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr,
				&cliaddr_len);
			printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
			for (i = 0; i < FD_SETSIZE; i++)
			if (client[i] < 0) {
				client[i] = connfd; 	//将文件描述符connfd保存到client[]中
				break;
			}
			if (i == FD_SETSIZE) { 		//判断连接数是否已达上限
				fputs("too many clients\n", stderr);
				exit(1);
			}
			FD_SET(connfd, &allset); 	//添加新文件描述符到监控信号集中
			if (connfd > maxfd) 		//更新最大文件描述符
				maxfd = connfd;
			if (i > maxi) 				//更新client[]最大下标值
				maxi = i;
			//若无文件描述符就绪，便返回select，继续阻塞监测剩余的文件描述符
			if (--nready == 0)
				continue;
		}
		//遍历文件描述符集，处理已就绪的文件描述符
		for (i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				//n=0，client就绪但未读到数据，表示client将关闭连接
				if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
					//关闭服务器端连接
					Close(sockfd);
					FD_CLR(sockfd, &allset); //清除集合中对应的文件描述符
					client[i] = -1;
				}
				else {						//处理获取的数据
					int j;
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					Write(sockfd, buf, n);
				}
				if (--nready == 0)
					break;
			}
		}
	}
	Close(listenfd);
	return 0;
}
