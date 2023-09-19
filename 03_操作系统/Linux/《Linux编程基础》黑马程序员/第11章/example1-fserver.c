#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "wrap.h"
#define MAXLINE 80						//缓冲数组大小
#define SERV_PORT 8000					//端口号
//子进程回收函数
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
	sigaction(SIGCHLD, &newact, NULL);		//信号捕获与处理（回收子进程）
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	//设置服务器端口地址
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	//使服务器与端口绑定
	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	Listen(listenfd, 20);
	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		pid = fork();			//创建子进程
		if (pid == 0) {
			//子进程处理客户端请求
			Close(listenfd);
			while (1) {
				n = Read(connfd, buf, MAXLINE);
				if (n == 0) {
					printf("the other side has been closed.\n");
					break;
				}
				//打印客户端端口信息
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
