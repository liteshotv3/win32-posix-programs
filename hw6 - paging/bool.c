#include <stdio.h>

//allows the use of 'int bool' type with true and false
typedef int bool;
#define false 0
#define true 1

int main ()
{
	bool mybooltype= false;
	printf("hello\n");
	if (mybooltype)
	{
		printf("mybooltype is set to true.\n");
	}

	printf("setting to true\n");
	mybooltype = true;
	if (mybooltype)
	{
		printf("mybooltype is set to true the second time.\n");
	}

	return 0;
}
