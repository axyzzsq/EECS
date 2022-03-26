
#include "calc.h"


int ADD(int x, int y)
{
	return x+y;
}

int SUB(int x, int y)
{
	return x-y;
}

int MUL(int x, int y)
{
	return x*y;
}

int DIV(int x, int y)
{
	return x/y;
}

void cacl1(int(*caclf)(int x, int y))
{
	int x, y;
	printf("input two numbers:>");
	scanf("%d%d", &x, &y);
	int ret = caclf(x, y);//通过函数指针找到相应的函数计算
	printf("result: >%d\n", ret);
}
