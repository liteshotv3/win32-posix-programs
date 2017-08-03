/*********************************************************************\
| Program : win32.2.c                                                 |
| Problem : Create a program that reads and writes to a bounded buffer|
|           without overwriting any of the array by using semaphores  |
|           to control access to the buffer.                          |
| Purpose : Practice the consumer-producer problem.                   |
| Author  : Artem Tolstov                                             |
| Date    : 4/15/2015                                                 |
\*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //for posix threads
#include <semaphore.h> //for semaphores
#include <math.h>        //pow

#define BUFFER_SIZE 16 

void *consumerThread (void *param);
void *producerThread (void *param);
//DWORD WINAPI consumerThread (LPVOID param);
//DWORD WINAPI producerThread (LPVOID param);

pthread_mutex_t myMutex;
struct threadParams
{
	int thread_number;
	//int producers, consumers, items;
	
};

//Global Variables
int buffer[BUFFER_SIZE];
int bufferPTR = 0;
//HANDLE full; //tells producer if there is no room to produce
//HANDLE empty; //should tell consumer if there is anything to consume
pthread_mutex_t myMutex;
sem_t empty;
sem_t full;
//HANDLE binarySemaphore; //to allow access in and out of the buffer
//int producers, consumers, items;
int numProduced = 0;
int numConsumed = 0;
int globalProducers;
int globalConsumers;
int globalItems;

int main(int argc, char *argv[])
{
	//Declarations
	int producers = pow(2, atoi(argv[1]));
	int consumers = pow(2, atoi(argv[2]));
	int items = pow(2, atoi(argv[3]));
	pthread_t *ThreadIdProducers;
	pthread_t *ThreadIdConsumers;
	pthread_attr_t attr;
	//HANDLE *prodHandle = (HANDLE*) malloc(sizeof(HANDLE)*producers);
	//HANDLE *conHandle = (HANDLE*) malloc(sizeof(HANDLE)*consumers);
	
	int i = 0;
	globalProducers = producers;
	globalConsumers = consumers;
	globalItems = items;
	struct threadParams *params;
	ThreadIdProducers = (pthread_t*) malloc(sizeof(pthread_t) * producers);
	ThreadIdConsumers = (pthread_t*) malloc(sizeof(pthread_t) * consumers);
	params = (struct threadParams*) malloc(sizeof(struct threadParams) * producers);

	//1. Parse all of the command-line parameters
	//Makes sure there are exactly 3 commandline arguments
	if (argc != 4)
	{
		printf("propers usage: %s int int int", argv[0] );
	}
	else
	{
		//2. Print them in a message
		//Prints to screen our command line args (string form)
		//printf("\ncommand line arguments are: %s %s %s\n\n", argv[1], argv[2], argv[3]);
		
		//Prints to screen to contents of producers, consumers, items
		printf("producers: 2^%d = %d\n", atoi(argv[1]), producers);
		printf("consumers: 2^%d = %d\n", atoi(argv[2]), consumers);
		printf("    items: 2^%d = %d\n", atoi(argv[3]), items);
		
		//3. Initialize the synchronization objects
		//full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL); //tells consumer that there are 0 full slots, consumer can't consume
		//empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL); //starts with 16 since there are 16 empty slots that producers can fill
		//binarySemaphore = CreateSemaphore(NULL, 1, 1, NULL); //allows access 1 at a time to global buffer

		sem_init(&empty, 0, BUFFER_SIZE); 
		sem_init(&full, 0, 0);	
		pthread_mutex_init(&myMutex, NULL);	

		//4. Spawn all of the threads/
		//printf("producers = %d\nconsumers = %d\n", producers, consumers);
		for(i = 0; i < producers; i++)
		{
			pthread_attr_init(&attr);
			params[i].thread_number = i; 
			//params[i].producers = producers;
			//params[i].consumers = consumers;
			//params[i].items = items;
			//prodHandle[i] = CreateThread(NULL, 0, producerThread, &params[i], 0, NULL);
			pthread_create(&ThreadIdProducers[i], &attr, producerThread, &params[i]);
			/*pthread_exit(0);
			if(prodHandle[i] == NULL)
			{
				fprintf(stderr, "Error: CreateThread, producer thread %i.\n",prodHandle[i]);
				ExitProcess(i);
			}*/
		}

		for(i = 0; i < consumers; i++)
		{
			//conHandle[i] = CreateThread(NULL, 0, consumerThread, &params[0], 0, NULL);
			pthread_create(&ThreadIdConsumers[i], &attr, consumerThread, &params[i]);
			/*
			if(conHandle[i] == NULL)
			{
				fprintf(stderr, "Error: CreateThread, producer thread %i.\n",conHandle[i]);
				ExitProcess(i);
			}*/
		}

		//5. Wait for all the threads to complete,
		for(i = 0; i < producers; i++)
		{
			pthread_join(ThreadIdProducers[i], NULL);
		}
		for(i = 0; i < consumers; i++)
		{
			pthread_join(ThreadIdConsumers[i], NULL);
		}
		//pthread_exit(0);
		sem_destroy(&empty);
		sem_destroy(&full);
		pthread_mutex_destroy(&myMutex);
		//WaitForMultipleObjects(producers, prodHandle, TRUE, INFINITE);
		//WaitForMultipleObjects(consumers, conHandle, TRUE, INFINITE);

		//6. Print a final message
		printf("\nAll producer and consumer threads have finished.\n");
		printf("Number of producers: %d\nNumber of consumers: %d\nNumber of items per producers: %d\n\n", producers, consumers, items);
		printf("Number Produced: %d\nNumber Consumed: %d\n", numProduced, numConsumed);
		free(params);
		
	}
	return 0;
}

void *consumerThread (void *param)
{
	struct threadParams thisThreadsParams = *(struct threadParams*)param;
	int producers = globalProducers;//thisThreadsParams.producers;
	int consumers = globalConsumers;//thisThreadsParams.consumers;
	int items = globalItems;//thisThreadsParams.items;
	int ItemsToConsume = (producers*items)/consumers;
	int i;
	for(i = 0; i < ItemsToConsume; i++)
	{
		sem_wait(&full);
		//WaitForSingleObject(full, INFINITE);
		pthread_mutex_lock(&myMutex);
		//WaitForSingleObject(binarySemaphore, INFINITE);
		/*critical section */
		bufferPTR--;
		printf("%d consumed from buffer[%d]\n", buffer[bufferPTR], bufferPTR);
		numConsumed++;
		/*end critical section */
		sem_post(&empty);
		//ReleaseSemaphore(empty, 1, NULL);
		pthread_mutex_unlock(&myMutex);
		//ReleaseSemaphore(binarySemaphore, 1, NULL);
	}
	return 0;
}

void *producerThread (void *param)
{
	struct threadParams thisThreadsParams = *(struct threadParams*)param;
	int producers = globalProducers;//thisThreadsParams.producers;
	int consumers = globalConsumers;//thisThreadsParams.consumers;
	int items = globalItems;//thisThreadsParams.items;
	int threadNum = thisThreadsParams.thread_number;

	int counter = 0;
	int item;
	int totalItems = items;
	int i = 0;

	for(i = 0; i < totalItems; i++)
	{
		sem_wait(&empty);
		//WaitForSingleObject(empty, INFINITE); //producers wait for empty slots to fill
		pthread_mutex_lock(&myMutex);
		//WaitForSingleObject(binarySemaphore, INFINITE); //wait for binary semaphore
		/*critical section */
		item = threadNum * numProduced++ + counter++;
		buffer[bufferPTR++] = item;
		printf("Item: %d created\n", item);
		/*critical section ends */
		sem_post(&full);
		//ReleaseSemaphore(full, 1, NULL);
		pthread_mutex_unlock(&myMutex);
		//ReleaseSemaphore(binarySemaphore, 1, NULL);
	}
	return 0;
}
