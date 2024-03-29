#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

/*
//重载来实现不同类型的参数
int& max(int& a, int& b)
{
	return (a < b)? b : a;
}

double& max(double& a, double& b)
{
	return (a < b)? b : a;
}

float& max(float& a, float& b)
{
	return (a < b)? b : a;
}
*/

template<typename T>
T& mymax(T& a, T& b)
{
	cout<<__PRETTY_FUNCTION__<<endl;  //打印函数名
	return (a < b)? b : a;
}

//模板函数指的是把函数的参数类型化，而模板类指的是把类的成员类型化。
//把参数类型作为参数。则面对不同的参数类型，不需要重载函数，只需要一个模板函数即可。
//编译器在编译的时候会根据传入的参数展开模板函数。
//从模板展开得到不同的函数，这个过程叫做模板的实例化。
int main(int argc, char **argv)
{
	int ia = 1, ib = 2;
	float fa = 1, fb = 2;
	double da = 1, db = 2;
	
	mymax(ia, ib);
	mymax(fa, fb);
	mymax(da, db);

	return 0;
}


