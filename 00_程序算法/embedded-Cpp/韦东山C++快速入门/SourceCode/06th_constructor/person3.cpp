
#include <iostream>
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
		cout <<"Pserson(char *)"<<endl;
		this->name = name;
	}

	Person(char *name, int age, char *work = "none")  //表示如果构造函数没有传入第三个参数，则默认参数为none
	{
		cout <<"Pserson(char*, int)"<<endl;
		this->name = name;
		this->age = age;
		this->work = work;
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
	Person per2;   /* 调用无参构造函数  */
	Person per3(); /* int fun(); 不会调用构造函数 */

	Person *per4 = new Person;  /* 调用无参构造函数  */
	Person *per5 = new Person();/* 也是调用无参构造函数  */

	Person *per6 = new Person[2];  //调用两次构造函数

	Person *per7 = new Person("lisi", 18, "student");
	Person *per8 = new Person("wangwu", 18);

	per.printInfo();
	per7->printInfo();
	per8->printInfo();
	
	//手动释放
	delete per4;
	delete per5;
	delete []per6;
	delete per7; 
	delete per8;

	return 0;
}

