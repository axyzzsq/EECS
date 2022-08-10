#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "wrap.h"
#define MAXLINE 80								//缓冲数组大小
#define SERV_PORT 8000							//端口号
struct s_info {
	struct sockaddr_in cliaddr;
	int connfd;
};
//请求处理函数
void *do_work(void *arg)
{
	int n, i;
	struct s_info *ts = (struct s_info*)arg;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	//使子线程处于分离态，保证子线程资源可被回收
	pthread_detach(pthread_self());
	while (1) {
		n = Read(ts->connfd, buf, MAXLINE);
		if (n == 0) {
			printf("the other side has been closed.\n");
			break;
		}
		printf("received from %s at PORT %d\n",
			inet_ntop(AF_INET, &(*ts).cliaddr.sin_addr, str, sizeof(str)),
			ntohs((*ts).cliaddr.sin_port));
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		Write(ts->connfd, buf, n);
	}
	Close(ts->connfd);
}
int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	int i = 0;
	pthread_t tid;
	struct s_info ts[383];
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		ts[i].cliaddr = cliaddr;
		ts[i].connfd = connfd;
		//创建子线程，处理客户端请求
		pthread_create(&tid, NULL, do_work, (void*)&ts[i]);
		i++;
	}
	return 0;
}
