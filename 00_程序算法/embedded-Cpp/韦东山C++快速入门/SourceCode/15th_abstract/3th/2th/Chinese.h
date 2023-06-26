
#ifndef _CHINESE_H
#define _CHINESE_H

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "Human.h"

using namespace std;

//直接从Human类继承名字设置和获取的方法
//当共同方法有成千上万个的时候继承可以减少代码量
class Chinese : public Human{
public:
	void eating(void);
	void wearing(void);
	void driving(void);
	~Chinese();
};

#endif

