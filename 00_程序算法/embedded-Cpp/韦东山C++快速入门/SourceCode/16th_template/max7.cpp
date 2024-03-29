#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;


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

#if 0
const int& mymax(int& a, int& b)
{
	cout<<"3: "<<__PRETTY_FUNCTION__<<endl;
	return (a < b)? b : a;
}
#endif

template<typename T>
const T mymax(T a, T b)
{
	cout<<"4: "<<__PRETTY_FUNCTION__<<endl;
	return (a < b)? b : a;
}


int main(int argc, char **argv)
{
	int ia = 1;
	int ib = 2;

	cout<<mymax(ia, ib)<<endl;

	return 0;
}


