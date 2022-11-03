
#include <stdio.h>
#include "person.h"
#include "dog.h"

int main(int argc, char **argv)
{
	//引入命名空间之后,要声明一个命名空间内的对象，必须声明命名空间
	//在main中直接声明 Person A会导致报错
	A::Person per; //调用某个函数的时候指明是哪个命名空间
	per.setName("zhangsan");
	per.setAge(16);
	per.printInfo();

	C::Dog dog;
	dog.setName("wangcai");
	dog.setAge(1);
	dog.printInfo();

	A::printVersion();
	C::printVersion();
	return 0;
}

