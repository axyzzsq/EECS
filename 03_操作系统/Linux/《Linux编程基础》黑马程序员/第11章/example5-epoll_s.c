#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include "wrap.h"
#define MAXLINE 80							//���������С
#define SERV_PORT 8000						//�˿ں�
#define OPEN_MAX 1024						//�����ļ�����������
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
	//��ʼ��client����
	for (i = 0; i < OPEN_MAX; i++)
		client[i] = -1;
	maxi = -1;							//��ʼ��maxi
	efd = epoll_create(OPEN_MAX);		//����epoll���
	if (efd == -1)
		perr_exit("epoll_create");
	//��ʼ��tep
	tep.events = EPOLLIN;
	tep.data.fd = listenfd;
	//Ϊ����������ע���¼���listenfd��
	res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
	if (res == -1)
		perr_exit("epoll_ctl");
	for (;;) {
		nready = epoll_wait(efd, ep, OPEN_MAX, -1);//��������
		if (nready == -1)
			perr_exit("epoll_wait");
		//��������¼�
		for (i = 0; i < nready; i++) {
			if (!(ep[i].events & EPOLLIN))
				continue;
			//��fdΪlistenfd����ʾ���������󵽴�
			if (ep[i].data.fd == listenfd) {
				clilen = sizeof(cliaddr);
				connfd = Accept(listenfd, (struct sockaddr *)&cliaddr,
					&clilen);
				printf("received from %s at PORT %d\n", inet_ntop(AF_INET,
					&cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));		//�ֽ���ת��
				//��accept��ȡ�����ļ����������浽client[]������
				for (j = 0; j < OPEN_MAX; j++)
				if (client[j] < 0) {
					client[j] = connfd;
					break;
				}
				if (j == OPEN_MAX)
					perr_exit("too many clients");
				if (j > maxi)
					maxi = j;					//��������ļ�������
				tep.events = EPOLLIN;
				tep.data.fd = connfd;
				//Ϊ�½������ӵĽ���ע���¼�
				res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
				if (res == -1)
					perr_exit("epoll_ctl");
			}
			else {//��fd������listenfd����ʾ�������Ǹ�·����
				sockfd = ep[i].data.fd;
				n = Read(sockfd, buf, MAXLINE);
				if (n == 0) {//����ȡ���ַ�����Ϊ0��ʾ��Ӧ�ͻ��˽��̽��ر�����
					for (j = 0; j <= maxi; j++) {
						if (client[j] == sockfd) {
							client[j] = -1;
							break;
						}
					}
					//ȡ������
					res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
					if (res == -1)
						perr_exit("epoll_ctl");
					Close(sockfd);		//�������˹ر�����
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
