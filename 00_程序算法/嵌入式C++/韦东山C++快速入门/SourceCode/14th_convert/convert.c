
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // doubleתΪint
	char *str = "100ask.taobao.com";
	int *p = str; // char *תΪint * 

	// %x��ʾ16���������%02x��ʾ���2λ16���ƣ����㲹0,%08x��ʾ���8λ16���ƣ����㲹0
	printf("i = %d, str = 0x%x, p = 0x%x\n", i, str, p);

	return 0;
}

