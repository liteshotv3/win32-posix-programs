#include <stdio.h>
#include <windows.h>     //semaphores and threads

#define BUFFER_SIZE 16


HANDLE full; //tells producer if there is no room to produce
HANDLE empty; //should tell consumer if there is anything to consume
HANDLE binarySemaphore; //to allow access in and out of the buffer
int main()
{
	full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL); //tells consumer that there are 0 full slots, consumer can't consume
	empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL); //starts with 16 since there are 16 empty slots that producers can fill
	binarySemaphore = CreateSemaphore(NULL, 1, 1, NULL); //allows access 1 at a time to global buffer

	WaitForSingleObject(empty, INFINITE); //producers wait for empty slots to fill
	WaitForSingleObject(binarySemaphore, INFINITE); //wait for binary semaphore
	WaitForSingleObject(full, INFINITE); // consumers wait for full slots to consume

	ReleaseSemaphore(full, 1, NULL); //producers release full slots
	ReleaseSemaphore(empty, 1, NULL); //consumers release empty slots, after consuming them
	ReleaseSemaphore(binarySemaphore, 1, NULL);// everything must release the binary semaphore after its done working with it
					// (handle to semaphore, increase count by one, not interested in previous count)
	return 0;
}