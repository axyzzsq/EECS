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
	virtual Human *test(void)
	{
		cout << "Human's test" << endl;
		return this;
	}
};

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
	virtual Englishman *test(void)
	{
		cout << "Englishman's test" << endl;
		return this;
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
	virtual Chinese *test(void)
	{
		cout << "Chinese's test" << endl;
		return this;
	}
};

//虚函数表中包含类型信息，所以可以通过虚函数表来判断对象的类型
//dynamic_cast实现类型判断是根据虚函数表的指针的值来判断的，从而知道对象是否是某个类型
//dynamic_cast只能用在含有虚函数的类上，因为只有含有虚函数的类才有虚函数表

void test_eating(Human &h)
{
	Englishman *pe;
	Chinese *pc;

	h.eating();

	/* 想分辨这个"人"是英国人还是中国人? */
	if (pe = dynamic_cast<Englishman *>(&h))
		cout << "This human is Englishman" << endl;
	else
		cout << "This human is not Englishman" << endl;

	if (pc = dynamic_cast<Chinese *>(&h))
		cout << "This human is Chinese" << endl;
	else
		cout << "This human is not Chinese" << endl;
}

int main(int argc, char **argv)
{
	Human h;
	Englishman e;
	Chinese c;

	test_eating(h);
	test_eating(e);
	test_eating(c);

	return 0;
}
