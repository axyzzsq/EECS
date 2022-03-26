#include <stdio.h>

#define   MAX  20 
#define   MIN  10 

//#define  _DEBUG
#define   SetBit(x)  (1<<x) 

int main(int argc, char* argv[])
{
    printf("Hello World \n");
    printf("MAX = %d,MIN = %d,MAX + MIN = %d\n",MAX,MIN,MAX + MIN); 

#ifdef _DEBUG
    printf("SetBit(5) = %d,SetBit(6) = %d\n",SetBit(5),SetBit(6));
    printf("SetBit( SetBit(2) ) = %d\n",SetBit( SetBit(2) ));		
#endif    
	return 0;
}

