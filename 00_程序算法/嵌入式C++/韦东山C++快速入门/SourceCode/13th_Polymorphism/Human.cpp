#include <iostream>
#include <string.h>
#include <unistd.h>

/* 这个程序是为了说明多态的概念，多态的概念是指在继承关系中，
   子类可以重写父类的方法，这样在调用的时候，就会根据对象的类型
   来调用不同的方法*/

using namespace std;

class Human
{
public:
	void eating(void)
	{
		cout << "use hand to eat" << endl;
	}
};

// Englishman类继承了Human类，重写了eating方法
class Englishman : public Human
{
public:
	void eating(void)
	{
		cout << "use knife to eat" << endl;
	}
};

// Chinese类继承了Human类，重写了eating方法
class Chinese : public Human
{
public:
	void eating(void)
	{
		cout << "use chopsticks to eat" << endl;
	}
};

// test_eating函数的参数是Human类的引用，这样就可以传入Human类的
void test_eating(Human &h)
{
	h.eating();
}

int main(int argc, char **argv)
{
	Human h;
	Englishman e;
	Chinese c;

	//传入的h e c并没有分辨出来，都是Human类的引用
	test_eating(h); 
	test_eating(e); 
	test_eating(c); 

	return 0;
}
