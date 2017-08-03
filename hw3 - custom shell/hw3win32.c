#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define BUFFER_SIZE 1024

int main()
{

    char buffer[BUFFER_SIZE];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char* commandLine = NULL;
    char* nextToken = NULL;

    /* allocate memory */
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));


    printf("Loop begins\n");
    while(1){
        //prints shell prompt, takes input
        printf("Myshell> ");
		fgets(buffer, BUFFER_SIZE, stdin);
        //scanf("%s", buffer);
        //printf("\nbuffer contains: %s \n\n", buffer);

        //used to tokenize our command up to when we hit enter
        commandLine = strtok_s(buffer, "\n", &nextToken);

        //exits the while loop
        if(strcmp(buffer, "exit") == 0)
        break;
		
		//creates a process that will send command to outer shell
		CreateProcess
		(
                NULL,
                commandLine,
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi
        );
		
		// waits for created process to finish
		WaitForSingleObject(pi.hProcess, INFINITE);
		//printf("**Child Complete**\n");


    }
    //printf("END WHILE LOOP\n");
	CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
