#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
	int status;
	pid_t pid, w;
	pid = fork();
	if (pid == -1){
		perror("fork error");
		exit(1);
	}
	else if (pid == 0){
		sleep(3);
		printf("Child process:pid=%d\n", getpid());
		exit(5);
	}
	else if (pid > 0){
		w = wait(&status);
		if (WIFEXITED(status)){
			printf("Child process pid=%d exit normally.\n", w);
			printf("Return Code:%d\n", WEXITSTATUS(status));
		}
		else
			printf("Child process pid=%d exit abnormally.\n", w);
	}
	return 0;
}
