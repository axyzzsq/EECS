
#include <stdio.h>

struct person {
	char *name;
	int age;
	char *work;

	void (*printInfo)(struct person *per); //函数指针，一个指向函数的指针
};

void printInfo(struct person *per)
{
	printf("name = %s, age = %d, work = %s\n", per->name, per->age, per->work);
}
int main(int argc, char **argv)
{
	struct person persons[] = {
		{"zhangsan", 10, "teacher", printInfo},
		{"lisi", 16, "doctor", printInfo},
	};
	
	persons[0].printInfo(&persons[0]);
	persons[1].printInfo(&persons[1]);
	
	return 0;
}

