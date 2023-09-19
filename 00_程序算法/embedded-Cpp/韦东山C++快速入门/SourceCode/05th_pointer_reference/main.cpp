
#include <iostream>

using namespace std;

int add_one(int a)
{
	a = a+1;
	return a;
}

int add_one(int *a) //传址,可以对传进来的参数进行改值
{
	*a = *a + 1;
	return *a;
}

int add_one_ref(int &b) //& 引用符
{
	b = b+1;
	return b;
}

//引用是别名，初始化为某个变量的别名，二者指向同一块内存
int main(int argc, char **argv)
{
	int a = 99;
    int &c = a; //引用一定要在定义的时候直接去初始化
	cout<<add_one(a)<<endl;
	cout<<"a = "<<a<<endl;

	cout<<add_one(&a)<<endl;
	cout<<"a = "<<a<<endl;

	cout<<add_one_ref(a)<<endl;
	cout<<"a = "<<a<<endl;

        c++;
        
	cout<<"a = "<<a<<endl;
	cout<<"c = "<<c<<endl;

	return 0;
}

