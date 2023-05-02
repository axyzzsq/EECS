
#include <stdio.h>

class Person {
private:
	char *name;
	int age;
	char *work;

//类内声明,在类外实现函数
public:
	void setName(char *name);
	int setAge(int age);
	void printInfo(void);
};

//说明实现的是person类内的函数setName
void Person::setName(char *name)
{
	this->name = name;
}

//说明实现的是person类内的函数setAge
int Person::setAge(int age)
{
	if (age < 0 || age > 150)
	{
		this->age = 0;
		return -1;
	}
	this->age = age;
	return 0;
}

void Person::printInfo(void)
{
	printf("name = %s, age = %d, work = %s\n", name, age, work); 
}

int main(int argc, char **argv)
{
	Person per;

	//per.name = "zhangsan";
	per.setName("zhangsan");
	per.setAge(200);
	per.printInfo();
	
	return 0;
}

