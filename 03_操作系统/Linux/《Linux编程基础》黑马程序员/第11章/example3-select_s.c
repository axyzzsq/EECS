#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"
#define MAXLINE 80						//���������С
#define SERV_PORT 8000					//�˿ں�
int main()
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE]; 	//FD_SETSIZE Ĭ��Ϊ1024
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
	Listen(listenfd, 20);		//Ĭ�����128
	maxfd = listenfd;
	maxi = -1;
	//��ʼ������б�
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1; 		//ʹ��-1��ʼ��client[]��Ԫ��
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset); 	//��listenfd��ӵ��ļ�����������
	//ѭ����⴦������״̬���̵��ļ�������
	for (;;) {
		//ʹ�ñ���rset��ȡ�ļ�����������
		rset = allset;
		//��¼������������
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");
		if (FD_ISSET(listenfd, &rset)){//�����������󵽴���������ӱ㴦����������
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr,
				&cliaddr_len);
			printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
			for (i = 0; i < FD_SETSIZE; i++)
			if (client[i] < 0) {
				client[i] = connfd; 	//���ļ�������connfd���浽client[]��
				break;
			}
			if (i == FD_SETSIZE) { 		//�ж��������Ƿ��Ѵ�����
				fputs("too many clients\n", stderr);
				exit(1);
			}
			FD_SET(connfd, &allset); 	//������ļ�������������źż���
			if (connfd > maxfd) 		//��������ļ�������
				maxfd = connfd;
			if (i > maxi) 				//����client[]����±�ֵ
				maxi = i;
			//�����ļ��������������㷵��select�������������ʣ����ļ�������
			if (--nready == 0)
				continue;
		}
		//�����ļ����������������Ѿ������ļ�������
		for (i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				//n=0��client������δ�������ݣ���ʾclient���ر�����
				if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
					//�رշ�����������
					Close(sockfd);
					FD_CLR(sockfd, &allset); //��������ж�Ӧ���ļ�������
					client[i] = -1;
				}
				else {						//�����ȡ������
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
