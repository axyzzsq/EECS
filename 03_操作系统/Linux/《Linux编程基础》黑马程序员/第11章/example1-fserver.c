#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "wrap.h"
#define MAXLINE 80						//���������С
#define SERV_PORT 8000					//�˿ں�
//�ӽ��̻��պ���
void do_sigchild(int num)
{
	while (waitpid(0, NULL, WNOHANG) > 0);
}
int main()
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	pid_t pid;
	struct sigaction newact;
	newact.sa_handler = do_sigchild;
	sigaction(SIGCHLD, &newact, NULL);		//�źŲ����봦�������ӽ��̣�
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	//���÷������˿ڵ�ַ
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	//ʹ��������˿ڰ�
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		pid = fork();			//�����ӽ���
		if (pid == 0) {
			//�ӽ��̴���ͻ�������
			Close(listenfd);
			while (1) {
				n = Read(connfd, buf, MAXLINE);
				if (n == 0) {
					printf("the other side has been closed.\n");
					break;
				}
				//��ӡ�ͻ��˶˿���Ϣ
				printf("received from %s at PORT %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));
				for (i = 0; i < n; i++)
					buf[i] = toupper(buf[i]);
				Write(connfd, buf, n);
			}
			Close(connfd);
			return 0;
		}
		else if (pid > 0) {
			Close(connfd);
		}
		else
			perr_exit("fork");
	}
	Close(listenfd);
	return 0;
}
