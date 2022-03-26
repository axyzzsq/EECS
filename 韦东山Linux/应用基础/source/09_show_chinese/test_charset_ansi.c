#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *str = "AÖÐ";
	int i;
	
	printf("str's len = %d\n", (int)strlen(str));
	printf("Hex code: ");
	for (i = 0; i < strlen(str); i++)
	{
		printf("%02x ", (unsigned char)str[i]);
	}
	printf("\n");
	return 0;
}