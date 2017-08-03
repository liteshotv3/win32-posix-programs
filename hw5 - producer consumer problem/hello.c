#include <stdio.h>
#include <windows.h>
#define BUFFER_SIZE 16

struct item
{
	int x;
};

struct threadParams
{
	int in, out;
};

DWORD WINAPI producer (LPVOID param);
DWORD WINAPI consumer (LPVOID param);

struct item buffer[BUFFER_SIZE]; //Buffer empty when in == out, buffer full when ((int + 1) % BUFFER_SIZE) == out

int main()
{
	//struct item buffer[BUFFER_SIZE]; //Buffer empty when in == out, buffer full when ((int + 1) % BUFFER_SIZE) == out
	//int out = 0;
	//int in = 0;
	DWORD ThreadId;
	HANDLE ThreadHandle;
	struct threadParams params[NUM_THREADS];

	ThreadHandle = CreateThread(NULL, 0, producer, &params[i], 0, &ThreadId);

	


	return 0;
}

DWORD WINAPI producer (LPVOID param)
{
	struct threadParams nextPointer = *(struct threadParams*)param;
	struct item next_produced;

	while (1) {
		/* produce an item in next_produced */
		while (((nextPointer.in + 1) % BUFFER_SIZE) == nextPointer.out)//Means buffer is full
			; /* do nothing */
		buffer[nextPointer.in] = next_produced;
		nextPointer.in = (nextPointer.in++) % BUFFER_SIZE;

		return 0;
	}
}

DWORD WINAPI consumer (LPVOID param)
{
	struct threadParams nextPointer = *(struct threadParams*)param;
	struct item next_consumed;

	while (1)
		while(nextPointer.in == nextPointer.out) //means buffer is empty
			; /* do nothing */

	next_consumed = buffer[nextPointer.out];
	nextPointer.out = (nextPointer.out++) % BUFFER_SIZE;

	/*consume the item in next_consumed */

	return 0;
}
