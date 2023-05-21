
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

//sp类的作用是，当sp对象销毁时，自动调用析构函数，从而自动释放堆对象
//sp用来声明局部变量，当函数结束时，自动调用析构函数，从而自动释放堆对象
class sp {
private:
	Person *p;

public:
	sp() : p(0) {} // 构造函数初始化列表
	
	//sp(Person *other) : p(other) {} // 构造函数初始化列表
	sp(Person *other)
	{
		cout<<"sp(const Person *other)"<<endl;
		p = other;
	}

	//析构函数释放堆对象
	~sp()
	{
		cout<<"~sp()"<<endl;
		if (p)
			delete p;
	}

	//重载->操作符，使得sp对象可以像指针一样使用
	//opreator->返回的是指向Person对象的指针,这个运算符通常用于重载智能指针类
	Person *operator->()
	{
		return p;
	}
	
};

void test_func(void)
{
	//s作为局部变量，当函数结束时，自动调用析构函数，从而自动释放堆对象
	sp s = new Person();
	
	//s是一个sp对象的实例，sp需要实现->操作符重载
	s->printInfo();

	//Person *p = new Person();
	//p->printInfo();
	//delete p;

	
}

int main(int argc, char **argv)
{	
	int i;

	for (i = 0; i < 2; i++)
		test_func();
	return 0;
}

