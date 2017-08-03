#include <stdio.h>
#include <string.h>


int main()
{
    char str[6];
    int address;
    
    while(1)
    {
        fgets(str, 6, stdin);
        if( strcmp(str,"\n") == 0)
        {
            return;
        }
        else
        {
            sscanf(str, "%x", &address);
            printf("%d\n", address);
        }
    }
    

    return 0;
}

