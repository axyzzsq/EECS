
#include <iostream>
#include <string.h>

using namespace std;

class Person {
private:
	char *name;
	int age;
	char *work;

public:

	Person() 
	{
		cout <<"Pserson()"<<endl;
	}
	Person(char *name) 
	{
		//在构造函数申请内存来存放参数
		cout <<"Pserson(char *)"<<endl;

		/*这行代码的作用是在堆上为对象的name成员变量分配内存空间，
		并将其指针赋值给this->name。其中，strlen(name) + 1是为了
		保证能够存储name字符串以及字符串结尾的'\0'字符。使用new关键字
		可以动态地在堆上分配内存空间，而不是在栈上分配，从而避免了
		栈溢出的问题。需要注意的是，在使用完这段内存空间后，需要
		使用delete关键字手动释放内存，否则会导致内存泄漏。*/
		
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}

	Person(char *name, int age, char *work = "none") 
	{
		cout <<"Pserson(char*, int)"<<endl;
		this->age = age;

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		this->work = new char[strlen(work) + 1];
		strcpy(this->work, work);
	}

	
	void setName(char *n)
	{
		name = n;
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
		//printf("name = %s, age = %d, work = %s\n", name, age, work); 
		cout<<"name = "<<name<<", age = "<<age<<", work = "<<work<<endl;
	}
};

int main(int argc, char **argv)
{
	Person per("zhangsan", 16);
	Person per2;   /* 调用无参构造函数 */
	Person per3(); /* int fun(); */

	Person *per4 = new Person;
	Person *per5 = new Person();

	Person *per6 = new Person[2];

	Person *per7 = new Person("lisi", 18, "student");
	Person *per8 = new Person("wangwu", 18);

	per.printInfo();
	per7->printInfo();
	per8->printInfo();

	delete per4;
	delete per5;
	delete []per6;
	delete per7;
	delete per8;

	return 0;
}

