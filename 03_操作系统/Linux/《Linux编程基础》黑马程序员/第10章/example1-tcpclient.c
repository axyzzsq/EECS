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
	struct sockaddr_in servaddr;		//�����������ַ�ṹ��
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
	if (argc != 2) {
		fputs("usage: ./client message\n", stderr);
		exit(1);
	}
	str = argv[1];
	//�����ͻ����׽����ļ�
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//��ʼ���������˿ڵ�ַ
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	//��������
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	//��������
	send(sockfd, str, strlen(str), 0);
	//���տͻ��˷��ص�����
	n = recv(sockfd, buf, MAXLINE, 0);
	printf("Response from server:\n");
	//���ͻ��˷��ص����ݴ�ӡ���ն�
	write(STDOUT_FILENO, buf, n);
	//�ر�����
	close(sockfd);
	return 0;
}
