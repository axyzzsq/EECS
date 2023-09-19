#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <ctype.h>
#define MAXLINE 80							//������ݳ���
#define SERV_PORT 6666						//�������˿ں�
int main(void)
{
	struct sockaddr_in servaddr, cliaddr;	//�����������ͻ��˵�ַ�ṹ��
	socklen_t cliaddr_len;					//�ͻ��˵�ַ����
	int sockfd;								//������socket�ļ�������
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//�������������׽����ļ�
	//��ʼ���������˿ڵ�ַ
	bzero(&servaddr, sizeof(servaddr));	//��ַ�ṹ������
	servaddr.sin_family = AF_INET;			//ָ��Э����
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);	//ָ���˿ں�
	//�󶨷������˿ڵ�ַ
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("Accepting connections ...\n");
	//���ݴ���
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		//��������
		n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr*)&cliaddr,
			&cliaddr_len);
		if (n == -1)
			perror("recvfrom error");
		printf("received from %s at PORT %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			ntohs(cliaddr.sin_port));
		//�������˲�����Сдת��д
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
