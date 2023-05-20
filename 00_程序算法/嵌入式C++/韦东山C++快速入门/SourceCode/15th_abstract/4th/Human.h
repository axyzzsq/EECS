

#ifndef _HUMAN_H
#define _HUMAN_H

#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Human {
private:
	char *name;

public:
	void setName(char *name);
	char *getName(void);
	//如果没有加virtual，子类全部都会调用父类的方法，全部输出use hand to eat
	virtual void eating(void){cout<<"use hand to eat"<<endl;}
	virtual void wearing(void){}
	virtual void driving(void){}
	
};

#endif

