#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

/*
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
const T& mymax(const T& a, const T& b)
{
	cout<<"1: "<<__PRETTY_FUNCTION__<<endl;
	return (a < b)? b : a;
}

template<typename T>
const T& mymax(T& a, T& b)
{
	cout<<"2: "<<__PRETTY_FUNCTION__<<endl;
	return (a < b)? b : a;
}

//普通函数优先于模板函数
const int& mymax(int& a, int& b)
{
	cout<<"3: "<<__PRETTY_FUNCTION__<<endl;
	return (a < b)? b : a;
}

int main(int argc, char **argv)
{
	int ia = 1;
	int ib = 2;

	/* 第一个模板，调用const T& mymax(const T& a, const T& b)的时候，实际传入的int，会被转换成
	 * const int&，所以调用的是const T& mymax(const int& a, const int& b)。
	 * 模板的类型转换值得就是int到const int的转换。
	 *
	 * 第二个模板，int转换成int(参数不需要转换，优先级排第一)
	 * 
	 * 第三个普通函数，int转换成int(参数不需要转换，优先级排第一)
	 */

	cout<<mymax(ia, ib)<<endl; 


	return 0;
}


