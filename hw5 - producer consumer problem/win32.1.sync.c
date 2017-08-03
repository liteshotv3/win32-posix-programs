/*********************************************************************\
| Program : win32.1.sync.c                                            |
| Problem : Write to a buffer using multiple threads and use a        |
|           semaphore to protect the counts in the array you are      |
|           writing to so that the counts remain accurate.            |
| Purpose : Practice implementing semaphores.                         |
| Author  : Artem Tolstov                                             |
| Date    : 4/15/2015                                                 |
\*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //for threads
#define BUFFER_SIZE 65536
#define NUM_THREADS 4
HANDLE ghSemaphore;
void printOccurences(int array[128]);
int charCounts[128];
char buffer[BUFFER_SIZE];
struct threadParams
{
	int threadIndex;
	int stringLengthInBuffer;
	
};
DWORD WINAPI asciiCounter (LPVOID param);

int main(int argc, char *argv[])
{
    //Declarations
    FILE *filePointer;
    int i,j,x;
	int partition;
	struct threadParams params[NUM_THREADS];
	DWORD ThreadId;
	HANDLE ThreadHandle;
	

    //Makes sure there are 2 arguments in the command line.
    if (argc != 2 )
	{
		printf( "proper usage: %s filename.extension", argv[0] );
	}
    else
	{
		//Reports if file does not exist.
		filePointer = fopen( argv[1], "r" );

		if (filePointer == 0)
		{
			printf ("Could not open file\n");
		}
		else
		{
		    i = 0;
			while (((x = fgetc(filePointer)) != EOF) && (i < BUFFER_SIZE))
			{
				//Put next character into i position of buffer, increment i.
			    buffer[i] = x; i++;
				//printf( "%c", x);
			}
			fclose( filePointer );
			
			//For debugging
			//printf( "\nbuffer contains: %s\n", buffer );
			printf("length of string in file is: %d\n", strlen(buffer));
			
			
			//Initialize array charCounts to 0
			for(j = 0; j < 128; j++)
			{
				charCounts[j] = 0;
			}
			
			
			//Sends params with thread indexi to threads
			for(i = 0; i < NUM_THREADS; i++)
			{
				params[i].threadIndex = i;
				params[i].stringLengthInBuffer = strlen(buffer);
				//ThreadHandle = CreateThread(NULL, 0, asciiCounter, &params[i], 0, &ThreadId);
				
			}
			
			ghSemaphore = CreateSemaphore( 
							NULL,           // default security attributes
							1,              // initial count
							1,              // maximum count
							NULL);          // unnamed semaphore
							
			if (ghSemaphore == NULL) 
			{
				printf("CreateSemaphore error: %d\n", GetLastError());
				return 1;
			}
							
			//Im trying to create a race condition, so I made the thread get created 
			//in its own for loop.
			for (i = 0; i < NUM_THREADS; i++)
			{
				ThreadHandle = CreateThread(NULL, 0, asciiCounter, &params[i], 0, &ThreadId);
			}
			
			//waits for threads to finish
			if (ThreadHandle != NULL)
			{
				WaitForSingleObject(ThreadHandle, INFINITE);
			}
	
			CloseHandle(ThreadHandle);
			CloseHandle(ghSemaphore);
			printOccurences(charCounts);
		}
	}

	
    
}

//Prints letter if printable, Hex value if unprintable character.
//Combines the values of the different threads into [0][] section of the array.
void printOccurences(int array[128])
{
	int i,j, holdIt;
	/*
	// Adds up the different instances of the array in array[0][i];
    
    for(i = 0; i < 128; i++)
    {
        //holdIt = array[0][i]; 
		//array[0][i] = 0;
        for(j = 1; j < NUM_THREADS; j++)
        {
           array[0][i] +=  array[j][i];
        }
        //array[0][i] = array[0][i] + holdIt;
    }
	*/

    for(i = 0; i < 128; i++)
    {
        if((i > 32) && (i != 127))
        {
            printf("%d occurrences of %c\n", array[i], (char)i);
        }
        else
        {
            printf("%d occurrences of 0x%x\n", array[i], i);
        }
    }
}

DWORD WINAPI asciiCounter (LPVOID param)
{
	struct threadParams thisThreadsParams = *(struct threadParams*)param;
	int i;
	//printf("asciiCounter has threadIndex = %d\n", thisThreadsParams.threadIndex);
	for(i = thisThreadsParams.threadIndex * (thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i < (thisThreadsParams.threadIndex+1)*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i++)
	{
		//printf("%c is value of %d\n", buffer[i], (int)buffer[i]);
		
		WaitForSingleObject(ghSemaphore, INFINITE);
                       // (handle to semaphore, zero-second time-out interval)
		charCounts[(int)buffer[i]]++;
		//Keeps the null characters in the buffer from writing
		
		ReleaseSemaphore(ghSemaphore, 1, NULL);       
					// (handle to semaphore, increase count by one, not interested in previous count)
	}
	//final check to count remaining characters due to truncation in division
	if(thisThreadsParams.threadIndex + 1 == NUM_THREADS)
	{
		if(NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS) != thisThreadsParams.stringLengthInBuffer)
		{
			for(i = NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i < thisThreadsParams.stringLengthInBuffer; i++)
			{
				WaitForSingleObject(ghSemaphore, INFINITE);
				charCounts[(int)buffer[i]]++;
				ReleaseSemaphore(ghSemaphore, 1, NULL); 
			}
		}
	}
	
	return 0;
}


