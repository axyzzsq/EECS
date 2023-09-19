
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "RefBase.h"

using namespace std;
using namespace android::RSC;


class Person : public LightRefBase<Person>{

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


template<typename T>
void test_func(sp<T> &other)
{
	sp<T> s = other;

	cout<<"In test_func: "<<s->getStrongCount()<<endl;

	s->printInfo();

	//Person *p = new Person();
	//p->printInfo();
	//delete p;

	
}

int main(int argc, char **argv)
{	
	int i;

	/* ����"Person *"; ��"sp<Person>"������"Person *"
	 * Person *per; 
	 * ��2�ֲ���: per->XXXx, (*per).XXX
	 * spҲӦ������2�в���:
	 *            sp->XXX, (*sp).XXX
	 *
	 */
	sp<Person> other = new Person();

	(*other).printInfo();
	cout<<"Before call test_func: "<<other->getStrongCount()<<endl;

	for (i = 0; i < 2; i++)
	{
		test_func(other);
		cout<<"After call test_func: "<<other->getStrongCount()<<endl;
	}
	return 0;
}

