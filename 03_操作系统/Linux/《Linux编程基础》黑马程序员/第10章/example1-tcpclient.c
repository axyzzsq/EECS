#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAXLINE 80
#define SERV_PORT 6666
int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;		//定义服务器地址结构体
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
	if (argc != 2) {
		fputs("usage: ./client message\n", stderr);
		exit(1);
	}
	str = argv[1];
	//创建客户端套接字文件
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//初始化服务器端口地址
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	//请求链接
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	//发送数据
	send(sockfd, str, strlen(str), 0);
	//接收客户端返回的数据
	n = recv(sockfd, buf, MAXLINE, 0);
	printf("Response from server:\n");
	//将客户端返回的数据打印到终端
	write(STDOUT_FILENO, buf, n);
	//关闭连接
	close(sockfd);
	return 0;
}
