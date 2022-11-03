
//引入namespace防止协同开发的时候重命名导致编译报错
//不是所有的函数都声明在类中，非类中成员函数的重名会导致编译报错
namespace C {

class Dog {
private:
	char *name;
	int age;
public:
	void setName(char *name);
	int setAge(int age);
	void printInfo(void);
};

void printVersion(void);

}

