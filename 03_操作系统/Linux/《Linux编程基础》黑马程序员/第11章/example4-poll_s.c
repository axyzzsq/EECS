#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include "wrap.h"
#define MAXLINE 80						//缓冲数组大小
#define SERV_PORT 8000					//端口号
#define OPEN_MAX 1024					//最大打开文件描述符数量
int main()
{
	int i, j, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	socklen_t clilen;
	struct pollfd client[OPEN_MAX];		//文件描述符与事件集合
	struct sockaddr_in cliaddr, servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	//初始化poll()的参数fds
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;	//设置listenfd监听普通读事件
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1; 		//将client[]中其余元素的fd成员初始化为-1
	maxi = 0; 					//记录client[]数组有效元素中最大元素下标
	//使用poll()机制循环检测文件描述符集合
	for (;;) {
		nready = poll(client, maxi + 1, -1);	//阻塞等待请求到达
		//通过listenfd状态判断是否有客户端连接请求，如有则建立连接
		if (client[0].revents & POLLRDNORM) {
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
			printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
			//将accept返回的connfd存放到client[]中的空闲位置
			for (i = 1; i < OPEN_MAX; i++){
				if (client[i].fd < 0) {
					client[i].fd = connfd;
					break;
				}
			}
			if (i == OPEN_MAX)
				perr_exit("too many clients");
			client[i].events = POLLRDNORM; 	//设置刚刚返回的connfd，监控读事件
			if (i > maxi)					//更新client[]中最大元素下标
				maxi = i;
			if (--nready <= 0) 				//若无就绪事件,回到poll阻塞
				continue;
		}
		//检测client[]，处理有就绪事件的文件描述符
		for (i = 1; i <= maxi; i++){
			if ((sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
					//比较errno，若为RST则表示连接中断
					if (errno == ECONNRESET){
						printf("client[%d] aborted connection\n", i);
						Close(sockfd);
						client[i].fd = -1;
					}
					else
						perr_exit("read error");
				}
				else if (n == 0) {//连接由客户端关闭
					printf("client[%d] closed connection\n", i);
					Close(sockfd);
					client[i].fd = -1;
				}
				else {//若成功读取数据，则对数据进行操作
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					Writen(sockfd, buf, n);
				}
				//当就绪文件描述符数量为0时，终止循环
				if (--nready <= 0)
					break;
			}
		}
	}
	return 0;
}
