#include "calc.h"

//使用回调函数和函数指针完成简单的计算器


void memu()
{
	printf("***********************************\n");
	printf("***   1.ADD             2.SUB   ***\n");
	printf("***   3.MUL             4.DIV   ***\n");
	printf("***   5.EXIT                    ***\n");
	printf("***********************************\n");
}
int main()
{

	//1.使用函数指针完成简单的计算器
	int input,x,y;
	
	int(*calc[])(int ,int) = { 0,ADD,SUB,MUL,DIV };//函数指针
	
	do 
	{
		memu();
		printf("input your choose：>");
		scanf("%d", &input);
		if (input > 0 && input < 5)
		{
			printf("input two numbers：>");
			scanf("%d%d", &x,&y);
			int ret=calc[input](x, y);//通过函数指针找到相应的函数计算
			printf("result：>%d\n",ret);
		}
		else if(input==0)
		{
			printf("exit");
		}
		else
		{
			printf("error input\n");
		}
	} while (input);
	
	//2使用回调函数完成简单的计算器
	do 
	{
		memu();
		printf("input your choose：>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			cacl1(ADD);
			break;
		case 2:
			cacl1(SUB);
			break;
		case 3:
			cacl1(MUL);
			break;
		case 4:
			cacl1(DIV);
			break;
		case 0:
			printf("exit");
			break;
		
		default:
			printf("error intput\n");
			break;
		}
	} while (input);

	return 0;
}