#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Human
{
private:
	int a;

public:
	virtual void eating(void)
	{
		cout << "use hand to eat" << endl;
	}
};

class Englishman : public Human
{
public:
	void eating(void)
	{
		cout << "use knife to eat" << endl;
	}
};

class Chinese : public Human
{
public:
	void eating(void)
	{
		cout << "use chopsticks to eat" << endl;
	}
};

void test_eating(Human &h)
{
	h.eating();
}

int main(int argc, char **argv)
{
	Human h;
	Englishman e;
	Chinese c;

	test_eating(h);
	test_eating(e);
	test_eating(c);

	/* 
	   通过sizeof可以看出，虚函数会在对象中占用4个字节的空间，
	   通过去掉或者注释掉Human类中的虚函数(virtual)，可以看出，
	   虚函数会在对象中占用4个字节的空间 
	*/
	cout << "sizeof(Human) = " << sizeof(h) << endl;
	cout << "sizeof(Englishman) = " << sizeof(e) << endl;
	cout << "sizeof(Chinese) = " << sizeof(c) << endl;

	return 0;
}
