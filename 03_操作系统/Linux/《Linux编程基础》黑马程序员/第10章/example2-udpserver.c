#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <ctype.h>
#define MAXLINE 80							//最大数据长度
#define SERV_PORT 6666						//服务器端口号
int main(void)
{
	struct sockaddr_in servaddr, cliaddr;	//定义服务器与客户端地址结构体
	socklen_t cliaddr_len;					//客户端地址长度
	int sockfd;								//服务器socket文件描述符
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//创建服务器端套接字文件
	//初始化服务器端口地址
	bzero(&servaddr, sizeof(servaddr));	//地址结构体清零
	servaddr.sin_family = AF_INET;			//指定协议族
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);	//指定端口号
	//绑定服务器端口地址
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("Accepting connections ...\n");
	//数据传输
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		//接收数据
		n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr*)&cliaddr,
			&cliaddr_len);
		if (n == -1)
			perror("recvfrom error");
		printf("received from %s at PORT %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
		//服务器端操作，小写转大写
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		n = sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr,
			sizeof(cliaddr));
		if (n == -1)
			perror("sendto error");
	}
	close(sockfd);
	return 0;
}
