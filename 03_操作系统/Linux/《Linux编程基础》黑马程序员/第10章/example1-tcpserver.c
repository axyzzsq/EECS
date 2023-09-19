#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXLINE 80							//������ݳ���
#define SERV_PORT 6666						//�������˿ں�
int main(void)
{
	struct sockaddr_in servaddr, cliaddr; 	//�����������ͻ��˵�ַ�ṹ��
	socklen_t cliaddr_len; 					//�ͻ��˵�ַ����
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	//�������������׽����ļ�
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	//��ʼ���������˿ڵ�ַ
	bzero(&servaddr, sizeof(servaddr));	//���������˿ڵ�ַ����
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	//���׽����ļ���������˿ڵ�ַ��
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	//���������������������Ϊ20
	listen(listenfd, 20);
	printf("Accepting connections ...\n");
	//���տͻ������ݣ�����������
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		n = recv(connfd, buf, MAXLINE, 0);
		printf("received from %s at PORT %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
		for (i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		send(connfd, buf, n, 0);
		//�ر�����
		close(connfd);
	}
	return 0;
}
