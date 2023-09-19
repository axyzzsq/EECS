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
	virtual ~Human()
	{
		cout << "~Human()" << endl;
	}
};

//如果覆写的类的析构函数没有加virtual，则会造成内存泄漏
//因为在调用delete的时候，只会调用父类的析构函数，而不会调用子类的析构函数
//在此例子中会造成Englishman和Chinese的析构函数没有被调用，
//而只调用了Human的析构函数，造成内存泄漏
//正常析构的时候会先调用子类的析构函数，再调用父类的析构函数
class Englishman : public Human
{
public:
	void eating(void)
	{
		cout << "use knife to eat" << endl;
	}
	virtual ~Englishman()
	{
		cout << "~Englishman()" << endl;
	}
};

class Chinese : public Human
{
public:
	void eating(void)
	{
		cout << "use chopsticks to eat" << endl;
	}
	virtual ~Chinese()
	{
		cout << "~Chinese()" << endl;
	}
};

void test_eating(Human h)
{
	h.eating();
}

int main(int argc, char **argv)
{
	Human *h = new Human;
	Englishman *e = new Englishman;
	Chinese *c = new Chinese;

	Human *p[3] = {h, e, c};
	int i;

	for (i = 0; i < 3; i++)
	{
		p[i]->eating();
		delete p[i];
	}

	return 0;
}
