#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Point {
private:
	int x;
	int y;

public:
	Point() {}

	/* 
	 * 这是一个Point类的构造函数，它接受两个整数参数x和y，并将它们分别赋值给类的
	 * 私有成员变量x和y。这个构造函数使用了成员初始化列表，这是一种更高效的初始
	 * 化方式，因为它避免了先默认初始化再赋值的过程。
	 */
	Point(int x, int y) : x(x), y(y) {}

	int getX(){ return x; }
	int getY(){ return y; }
	void setX(int x){ this->x = x; }
	void setY(int y){ this->y = y; }
	void printInfo()
	{
		cout<<"("<<x<<", "<<y<<")"<<endl;
	}
};

Point add(Point &p1, Point &p2)
{
	Point n;
	n.setX(p1.getX()+p2.getX());
	n.setY(p1.getY()+p2.getY());
	return n;
}

int main(int argc, char **argv)
{
	Point p1(1, 2);
	Point p2(2, 3);

	Point sum = add(p1, p2);
	sum.printInfo();

	return 0;
}

