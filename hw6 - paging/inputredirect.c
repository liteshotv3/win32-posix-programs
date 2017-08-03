#include <stdio.h>

int main()
{
	char str[200];
	int count = 0;
	int address;
	
	while(1)
	{
		scanf("%x", &address);
		printf("Got %d\n", address);
		count++;
		/*if(fgetc(stdin) == '\n')
		{
			printf("Hello\n");
			break;
		}*/
       if (count > 105)
	   break;
	}
	

	return 0;
}