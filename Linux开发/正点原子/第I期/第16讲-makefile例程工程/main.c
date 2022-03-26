#include <stdio.h>
#include "input.h"
#include "calcu.h"
int main(int argc, char *argv[])
{
	int a, b, num;
	input_int(&a, &b);
	num = calcu(a, b);
	printf("%d + %d = %d\r\n", a, b, num);
 }