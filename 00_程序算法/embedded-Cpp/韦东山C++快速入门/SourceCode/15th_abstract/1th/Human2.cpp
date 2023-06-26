#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Human
{
private:
	int a;

public:
	// 一个函数前面是virtual，后面加上=0，这个函数就是纯虚函数
	// 在基类中，如果有一个纯虚函数，那么这个类就是抽象类
	// 抽象类不能实例化对象，只能作为基类，用于派生子类
	virtual void eating(void) = 0;
	virtual void wearing(void) = 0;
	virtual void driving(void) = 0;
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
	void wearing(void)
	{
		cout << "wear english style" << endl;
	}
	void driving(void)
	{
		cout << "drive english car" << endl;
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
	void wearing(void)
	{
		cout << "wear chinese style" << endl;
	}
	
	/*
	void driving(void)
	{
		cout << "drive chinese car" << endl;
	}
	*/
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

class Guangximan : public Chinese
{
	void driving(void)
	{
		cout << "drive guangxi car" << endl;
	}
};

int main(int argc, char **argv)
{
	// Human h; 	//抽象类不能实例化对象
	Englishman e;
	Guangximan g;

	return 0;
}
