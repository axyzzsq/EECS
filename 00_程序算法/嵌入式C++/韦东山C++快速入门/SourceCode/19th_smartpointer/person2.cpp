
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Person {

public:

	Person() {
		cout <<"Pserson()"<<endl;
	}


	~Person()
	{
		cout << "~Person()"<<endl;
	}
	void printInfo(void)
	{
		cout<<"just a test function"<<endl;
	}
};

void test_func(void)
{
	//Person *p = new Person();
	Person per; // 局部变量，用栈对象代替堆对象，当函数结束时，自动调用析构函数
	per.printInfo();
}

int main(int argc, char **argv)
{	
	int i;

	for (i = 0; i < 2; i++)
		test_func();
	return 0;
}

