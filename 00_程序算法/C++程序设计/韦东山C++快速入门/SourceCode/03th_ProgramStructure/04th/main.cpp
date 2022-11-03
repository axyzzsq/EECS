
#include <stdio.h>
#include "person.h"
#include "dog.h"

/* global namespace */

/* ��A::Person����global namespace, �Ժ����ʹ��Person����ʾA::Person */
using A::Person;

/* ��C::Dog����global namespace, �Ժ����ʹ��Dog����ʾC::Dog */
using C::Dog;

using A::printVersion;
using C::printVersion;

int main(int argc, char **argv)
{
	/* local namespace */
	//using A::Person;
	//using C::Dog;

	Person per;
	per.setName("zhangsan");
	per.setAge(16);
	per.printInfo();

	Dog dog;
	dog.setName("wangcai");
	dog.setAge(1);
	dog.printInfo();

	A::printVersion();
	C::printVersion();
	return 0;
}

