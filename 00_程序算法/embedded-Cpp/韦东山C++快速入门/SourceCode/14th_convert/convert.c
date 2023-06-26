
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // double转为int
	char *str = "100ask.taobao.com";
	int *p = str; // char *转为int * 

	// %x表示16进制输出，%02x表示输出2位16进制，不足补0,%08x表示输出8位16进制，不足补0
	printf("i = %d, str = 0x%x, p = 0x%x\n", i, str, p);

	return 0;
}

