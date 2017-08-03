/*********************************************************************\
| Program : posix.1.race.c                                            |
| Problem :                                                           |
| Purpose :                                                           |
| Author  : Artem Tolstov                                             |
| Date    : 4/15/2015                                                 |
\*********************************************************************/

//Author: Artem Tolstov
//Purpose: CSC415 HW4
//Function: Takes name of file from command line and counts the number of ascii characters using 
//			predefined number of threads then prints to the screen occurences of said characters.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h> //for threads
#include <pthread.h> //for posix threads
#define BUFFER_SIZE 65536
#define NUM_THREADS 4
void printOccurences(int array[128]);
int charCounts[128];
char buffer[BUFFER_SIZE];
struct threadParams
{
	int threadIndex;
	int stringLengthInBuffer;
	
};
//DWORD WINAPI asciiCounter (LPVOID param);
void *asciiCounter (void *param);

int main(int argc, char *argv[])
{
    //Declarations
    FILE *filePointer;
    int i,j,x;
	int partition;
	struct threadParams params[NUM_THREADS];
	pthread_t ThreadId;
	//HANDLE ThreadHandle;
	pthread_attr_t attr;
	

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
			printf("length of string in file is: %d\n", (int)strlen(buffer));
			
			
			//Initialize array charCounts to 0
			for(j = 0; j < 128; j++)
			{
				charCounts[j] = 0;
			}
			
			
			//Sends params with thread indexi to threads
			pthread_attr_init(&attr);
			for(i = 0; i < NUM_THREADS; i++)
			{
				params[i].threadIndex = i;
				params[i].stringLengthInBuffer = strlen(buffer);
				pthread_create(&ThreadId, &attr, asciiCounter, &params[i]);
				//ThreadHandle = CreateThread(NULL, 0, asciiCounter, &params[i], 0, &ThreadId);
				
			}
			
							
			//Im trying to create a race condition, so I made the thread get created 
			//in its own for loop.
			for (i = 0; i < NUM_THREADS; i++)
			{
				pthread_create(&ThreadId, &attr, asciiCounter, &params[i]);
			}
			pthread_join(ThreadId, NULL);
			//waits for threads to finish
			//if (ThreadHandle != NULL)
			//{
			//	WaitForSingleObject(ThreadHandle, INFINITE);
			//}
	
			//CloseHandle(ThreadHandle);			
			printOccurences(charCounts);
			pthread_exit(0);
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

void *asciiCounter (void *param)
{
	struct threadParams thisThreadsParams = *(struct threadParams*)param;
	int i;
	//printf("asciiCounter has threadIndex = %d\n", thisThreadsParams.threadIndex);
	for(i = thisThreadsParams.threadIndex * (thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i < (thisThreadsParams.threadIndex+1)*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i++)
	{
		//printf("%c is value of %d\n", buffer[i], (int)buffer[i]);

		charCounts[(int)buffer[i]]++;
		//Keeps the null characters in the buffer from writing
	}
	//final check to count remaining characters due to truncation in division
	if(thisThreadsParams.threadIndex + 1 == NUM_THREADS)
	{
		if(NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS) != thisThreadsParams.stringLengthInBuffer)
		{
			for(i = NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i < thisThreadsParams.stringLengthInBuffer; i++)
			{
				charCounts[(int)buffer[i]]++;
			}
		}
	}
	
	return 0;
}


