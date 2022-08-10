#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include "wrap.h"
#define MAXLINE 80						//���������С
#define SERV_PORT 8000					//�˿ں�
#define OPEN_MAX 1024					//�����ļ�����������
int main()
{
	int i, j, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];
	socklen_t clilen;
	struct pollfd client[OPEN_MAX];		//�ļ����������¼�����
	struct sockaddr_in cliaddr, servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	//��ʼ��poll()�Ĳ���fds
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;	//����listenfd������ͨ���¼�
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1; 		//��client[]������Ԫ�ص�fd��Ա��ʼ��Ϊ-1
	maxi = 0; 					//��¼client[]������ЧԪ�������Ԫ���±�
	//ʹ��poll()����ѭ������ļ�����������
	for (;;) {
		nready = poll(client, maxi + 1, -1);	//�����ȴ����󵽴�
		//ͨ��listenfd״̬�ж��Ƿ��пͻ�����������������������
		if (client[0].revents & POLLRDNORM) {
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
			printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
				ntohs(cliaddr.sin_port));
			//��accept���ص�connfd��ŵ�client[]�еĿ���λ��
			for (i = 1; i < OPEN_MAX; i++){
				if (client[i].fd < 0) {
					client[i].fd = connfd;
					break;
				}
			}
			if (i == OPEN_MAX)
				perr_exit("too many clients");
			client[i].events = POLLRDNORM; 	//���øոշ��ص�connfd����ض��¼�
			if (i > maxi)					//����client[]�����Ԫ���±�
				maxi = i;
			if (--nready <= 0) 				//���޾����¼�,�ص�poll����
				continue;
		}
		//���client[]�������о����¼����ļ�������
		for (i = 1; i <= maxi; i++){
			if ((sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ((n = Read(sockfd, buf, MAXLINE)) < 0) {
					//�Ƚ�errno����ΪRST���ʾ�����ж�
					if (errno == ECONNRESET){
						printf("client[%d] aborted connection\n", i);
						Close(sockfd);
						client[i].fd = -1;
					}
					else
						perr_exit("read error");
				}
				else if (n == 0) {//�����ɿͻ��˹ر�
					printf("client[%d] closed connection\n", i);
					Close(sockfd);
					client[i].fd = -1;
				}
				else {//���ɹ���ȡ���ݣ�������ݽ��в���
					for (j = 0; j < n; j++)
						buf[j] = toupper(buf[j]);
					Writen(sockfd, buf, n);
				}
				//�������ļ�����������Ϊ0ʱ����ֹѭ��
				if (--nready <= 0)
					break;
			}
		}
	}
	return 0;
}
