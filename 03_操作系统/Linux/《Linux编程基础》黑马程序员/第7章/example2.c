#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	alarm(1);			//���ü�ʱ��
	while (1)				//ѭ����֤���̲��˳�
		printf("process will finish.\n");
	return 0;
}
