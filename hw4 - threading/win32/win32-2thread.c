//Author: Artem Tolstov
//Purpose: CSC415 HW4
//Function: Takes name of file from command line and counts the number of ascii characters using 
//			predefined number of threads then prints to the screen occurences of said characters.


#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //for threads
#define BUFFER_SIZE 65536
#define NUM_THREADS 2
void printOccurences(int array[NUM_THREADS][128]);
int charCounts[NUM_THREADS][128];
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
			for(i = 0; i < NUM_THREADS; i++)
			{
				for(j = 0; j < 128; j++)
				{
					charCounts[i][j] = 0;
				}
			}
			
			//Sends params with thread indexi to threads
			for(i = 0; i < NUM_THREADS; i++)
			{
				params[i].threadIndex = i;
				params[i].stringLengthInBuffer = strlen(buffer);
				ThreadHandle = CreateThread(NULL, 0, asciiCounter, &params[i], 0, &ThreadId);
				
			}
			
			//waits for threads to finish
			if (ThreadHandle != NULL)
			{
				WaitForSingleObject(ThreadHandle, INFINITE);
			}
	
			CloseHandle(ThreadHandle);
			printOccurences(charCounts);
		}
	}

	
    
}

//Prints letter if printable, Hex value if unprintable character.
//Combines the values of the different threads into [0][] section of the array.
void printOccurences(int array[NUM_THREADS][128])
{
    int i,j, holdIt;
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

    for(i = 0; i < 128; i++)
    {
        if((i > 32) && (i != 127))
        {
            printf("%d occurrences of %c\n", array[0][i], (char)i);
        }
        else
        {
            printf("%d occurrences of 0x%x\n", array[0][i], i);
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
		charCounts[thisThreadsParams.threadIndex][(int)buffer[i]]++;
		//Keeps the null characters in the buffer from writing
	}
	//final check to count remaining characters due to truncation in division
	if(thisThreadsParams.threadIndex + 1 == NUM_THREADS)
	{
		if(NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS) != thisThreadsParams.stringLengthInBuffer)
		{
			for(i = NUM_THREADS*(thisThreadsParams.stringLengthInBuffer/NUM_THREADS); i < thisThreadsParams.stringLengthInBuffer; i++)
			{
				charCounts[thisThreadsParams.threadIndex][(int)buffer[i]]++;
			}
		}
	}
	
	return 0;
}


