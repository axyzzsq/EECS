
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Person {
private:
	static int cnt;
	char *name;
	int age;
	char *work;

public:

	static int getCount(void); /* 在类内声明,类外实现 */

	Person() {//cout <<"Pserson()"<<endl;
		name = NULL;
		work = NULL;
		cnt++;
	}
	Person(char *name) 
	{
		//cout <<"Pserson(char *)"<<endl;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->work = NULL;
		cnt++;
	}

	Person(char *name, int age, char *work = "none") 
	{
		cout <<"Pserson(char*, int), name = "<<name<<", age= "<<age<<endl;
		this->age = age;

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		this->work = new char[strlen(work) + 1];
		strcpy(this->work, work);
		cnt++;
	}

	Person(Person &per) 
	{
		cout <<"Pserson(Person &)"<<endl;
		this->age = per.age;

		this->name = new char[strlen(per.name) + 1];
		strcpy(this->name, per.name);

		this->work = new char[strlen(per.work) + 1];
		strcpy(this->work, per.work);
		cnt++;
	}

	~Person()
	{
		cout << "~Person()"<<endl;
		if (this->name) {
			cout << "name = "<<name<<endl;
			delete this->name;
		}
		if (this->work) {
			cout << "work = "<<work<<endl;
			delete this->work;
		}
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

int Person::cnt = 0; /* 定义和初始化 */

int Person::getCount(void) 
{ 
	return cnt; 
}


int main(int argc, char **argv)
{
	Person p[100];
	cout << "person number = "<<Person::getCount()<<endl;
	cout << "person number = "<<p[0].getCount()<<endl;
	cout << "person number = "<<p[1].getCount()<<endl;

	return 0;
}

