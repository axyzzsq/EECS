
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // doubleתΪint
	char *str = "100ask.taobao.com";
	//reinterpret_cast<type-id>(expression)��ʾǿ������ת��������û������ʱ���ͼ������֤ת���İ�ȫ�ԡ����á�
	int *p = reinterpret_cast<int *>(str); // char *תΪint * 

	printf("i = %d, str = 0x%x, p = 0x%x\n", i, reinterpret_cast<unsigned int>(str), reinterpret_cast<unsigned int>(p));

	return 0;
}

