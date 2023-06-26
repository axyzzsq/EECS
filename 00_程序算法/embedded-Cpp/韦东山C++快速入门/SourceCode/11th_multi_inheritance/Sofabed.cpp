#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Sofa {
public:
	void watchTV(void) { cout<<"watch TV"<<endl; }
};

class Bed {
public:
	void sleep(void) { cout<<"sleep"<<endl; }
};

class Sofabed : public Sofa, public Bed {
};

int main(int argc, char **argv)
{
	Sofabed s;
	s.watchTV(); //继承于Sofa
	s.sleep(); //继承于Bed
	return 0;
}


