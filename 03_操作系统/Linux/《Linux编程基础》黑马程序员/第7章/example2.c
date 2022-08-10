#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	alarm(1);			//设置计时器
	while (1)				//循环保证进程不退出
		printf("process will finish.\n");
	return 0;
}
