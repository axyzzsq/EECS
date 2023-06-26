
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // double转为int
	//const表示只读，不能修改，但是可以强制转换为非const，
	//这里如果没有加const，编译器会报warning，因为const char *转为可读可写的char *是不安全的。
	const char *str = "100ask.taobao.com";
	
	//加上const_cast表示强制转换为非const
	char *str2 = const_cast<char *>(str);

	//reinterpret_cast<type-id>表示把一个指针转换为另一个指针类型
	int *p = reinterpret_cast<int *>(str2); // char *转为int * 

	printf("i = %d, str = 0x%x, p = 0x%x\n", i, reinterpret_cast<unsigned int>(str), reinterpret_cast<unsigned int>(p));

	return 0;
}

