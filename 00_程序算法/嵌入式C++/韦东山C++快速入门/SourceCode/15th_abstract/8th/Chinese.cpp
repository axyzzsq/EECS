
#include "Chinese.h"


void Chinese::eating(void) 
{ 
	cout<<"use chopsticks to eat"<<endl; 
}

void Chinese::wearing(void) 
{
	cout<<"wear chinese style"<<endl; 
}

void Chinese::driving(void) 
{
	cout<<"drive chinese car"<<endl; 
}

Chinese::~Chinese() 
{ 
	cout<<"~Chinese()"<<endl; 
}

//工厂函数
Human& CreateChinese(char *name, int age, char *address)
{
	//使用*操作符解引用这个指针，得到一个Chinese对象的引用，然后返回这个引用。
	return *(new Chinese());
}

