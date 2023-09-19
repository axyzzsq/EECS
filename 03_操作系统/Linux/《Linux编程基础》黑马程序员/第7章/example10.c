#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
void do_sig_child(int signo)				//�źŴ�����
{
	int status;
	pid_t pid;
	while ((pid = waitpid(0, &status, WNOHANG)) > 0) {//�ж��ӽ���״̬
		if (WIFEXITED(status))
			printf("child %d exit %d\n", pid, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
	}
}
int main(void)
{
	pid_t pid;
	int i;
	for (i = 0; i < 10; i++) {
		if ((pid = fork()) == 0)				//����һ���ӽ���
			break;
		else if (pid < 0)					//�ݴ���
			sys_err("fork");
	}
	if (pid == 0) {							//�ӽ���ִ������
		int n = 1;
		while (n--) {
			printf("child ID %d\n", getpid());
			sleep(1);
		}
		return i + 1;
	}
	else if (pid > 0) {						//������ִ������
		struct sigaction act;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, NULL);		//ע�Ჶ����
		while (1) {		                       //��֤����������
			printf("Parent ID %d\n", getpid());
			sleep(1);
		}
	}
	return 0;
}
