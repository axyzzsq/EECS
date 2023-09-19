
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // double转为int
	char *str = "100ask.taobao.com";
	//reinterpret_cast<type-id>(expression)表示强制类型转换，但是没有运行时类型检查来保证转换的安全性。少用。
	int *p = reinterpret_cast<int *>(str); // char *转为int * 

	printf("i = %d, str = 0x%x, p = 0x%x\n", i, reinterpret_cast<unsigned int>(str), reinterpret_cast<unsigned int>(p));

	return 0;
}

