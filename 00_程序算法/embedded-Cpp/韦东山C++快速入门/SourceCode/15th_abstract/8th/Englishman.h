
#ifndef _ENGLISHMAN_H
#define _ENGLISHMAN_H

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "Human.h"

using namespace std;

class Englishman : public Human {
private:
	char address[1000];
	int age;
public:
	void eating(void);
	void wearing(void);
	void driving(void);
	Englishman();
	Englishman(char *name, int age, char *address);
	~Englishman();
};

#endif

