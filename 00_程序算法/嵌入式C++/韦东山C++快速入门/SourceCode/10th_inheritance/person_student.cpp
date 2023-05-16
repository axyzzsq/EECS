
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Person {
private:
	char *name;
	int age;

public:
	int address;
	
	Person() {//cout <<"Pserson()"<<endl;
		name = NULL;
	}
	Person(char *name) 
	{
		//cout <<"Pserson(char *)"<<endl;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}

	Person(char *name, int age) 
	{
		cout <<"Pserson(char*, int), name = "<<name<<", age= "<<age<<endl;
		this->age = age;

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}

	Person(Person &per) 
	{
		cout <<"Pserson(Person &)"<<endl;
		this->age = per.age;

		this->name = new char[strlen(per.name) + 1];
		strcpy(this->name, per.name);
	}

	~Person()
	{
		cout << "~Person()"<<endl;
		if (this->name) {
			cout << "name = "<<name<<endl;
			delete this->name;
		}
	}

	void setName(char *name)
	{
		if (this->name) {
			delete this->name;
		}
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}
	int setAge(int a)
	{
		if (a < 0 || a > 150)
		{
			age = 0;
			return -1;
		}
		age = a;
		return 0;
	}
	void printInfo(void)
	{
		cout<<"Person printInfo() "<<endl;
		cout<<"name = "<<name<<", age = "<<age<<endl;
	}
};


class Student : public Person {
private:
	int grade;
	void setGrade(int grade) {this->grade = grade;}
	int getGrade(void) {return grade;}
public:
	void printInfo(void)
	{
		cout<<"Student printInfo()"<<endl;
		Person::printInfo();
	}
};

void test_func(Person &p)
{
	p.printInfo();
}

int main(int argc, char **argv)
{
	Person p("lisi", 16);

	Student s;
	s.setName("zhangsan");
	s.setAge(16);

	test_func(p);

	/* test_func(s); 调用了 test_func 函数，并将 s 作为参数传递给它。
	test_func 函数的参数是一个 Person 类型的引用，因此 s 会被隐式转
	换为 Person 类型。在函数内部，p.printInfo() 调用了 Person 类的 
	printInfo 函数，因为 p 是一个 Person 类型的引用。由于 Student 类
	继承自 Person 类，因此 s 对象也具有 printInfo 函数。在 main 函数的
	结尾，s.printInfo() 被调用，这将调用 Student 类的 printInfo 函数，
	该函数首先调用 Person 类的 printInfo 函数，然后输出 Student 类的信息。*/
	
	test_func(s);  /* Person &p = s里面的Person部分;
	               * p引用的是"s里面的Person部分"
	               */
	s.printInfo();

	return 0;
}

