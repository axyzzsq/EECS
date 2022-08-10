#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
int main()
{
	struct stat sbuf;
	int ret = 2;
	ret = stat("a.out", &sbuf);
	if (ret == -1){
		perror("stat error:");
		exit(1);
	}
	printf("len=%ld\n", sbuf.st_size);
	return 0;
}
