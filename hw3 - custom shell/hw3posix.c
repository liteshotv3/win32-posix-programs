#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

int main() 
{
 pid_t pid = 0;
 char buffer[BUFFER_SIZE];
 char* token = NULL;
 char* args[5];
 int counter = 0;


 while(1)
 {
   printf("myshell>");	
   //scanf("%s", buffer);
   fgets(buffer, BUFFER_SIZE, stdin);
   //printf("buffer contains: %s\n", buffer);
   //if(strcmp(buffer, "exit") == 0)
   //	exit(0);

   token = (char *)strtok(buffer, " \n");
   counter = 0;
   while(token != NULL)
   {
     args[counter] = token;
     token = (char *)strtok(NULL, " \n");
     counter++;
   }
   args[counter] = NULL;
   if(strcmp(args[0], "exit") == 0)
	exit(0);
   
   pid = fork();
   if (pid == 0)
   {
    //printf("Child process says HI\n");
    execlp(args[0], args[0], args[1], args[2], args[3], (char *)NULL);
    exit(-1);
   }
   else 
   {
    wait(NULL);
   }
   
 }
printf("End While Loop\n\n");
return 0;
}
