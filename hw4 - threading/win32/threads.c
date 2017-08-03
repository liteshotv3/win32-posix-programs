#include <stdio.h>
#include <windows.h>

struct myStruct
{
	int threadIndex;
	int partition;
};

DWORD WINAPI asciiCounter (LPVOID Param)
{
	struct myStruct newThreadStruct = *(struct myStruct*)Param;
	printf("asciiCounter thread executes with Param: %d\n", Param);
	printf("asciiCounter thread containts in newThreadStruct.threadIndex = %d\n", newThreadStruct.threadIndex);
	return 0;
}

int main()
{
	DWORD ThreadId;
	HANDLE ThreadHandle;
	int Param =2;
	
	struct myStruct aStruct;
	aStruct.threadIndex = 3;
	printf("aStruct.x = %d\n", aStruct.threadIndex);
	
	ThreadHandle = CreateThread(NULL, 0, asciiCounter, &aStruct, 0, &ThreadId);
	
	if (ThreadHandle != NULL)
	{
		WaitForSingleObject(ThreadHandle, INFINITE);
	}
	
	CloseHandle(ThreadHandle);
	
	
	printf("Main file writes.\n");
	
	
	
	return 0;
}