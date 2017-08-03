/*********************************************************************\
| Program : hw6.c                                                     |
| Problem : Write a program that simulates FIFO(first in first out)   |
|           and LRU (least recently used) page-replacement algorithms.|
| Purpose : Demonstrate paging algorithms FIFO and LRU                |
| Author  : Artem Tolstov                                             |
| Date    : 4/21/2015                                                 |
\*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h.>

//allows the use of 'int bool' type with true and false
typedef int bool;
#define false 0
#define true 1

void pageSwap(struct frame *currentFrame, int address);
int victimSelector(struct frame allocatedFrames[], int framesNum);

struct frame
{
	bool isEmpty;
	int page;
	int count;
};

int main(int argc, char *argv[])
{
	//Declarations
	int pageSize, memSize, framesNum, i, emptyLeft, nextValue, pageNumber, pageFaults, address, breakcounter;
	struct frame *allocatedFrames;
	char string[6];
	int victim;
	bool pageExists = false;

	//Initializations
	pageSize = pow(2, atoi(argv[1]));
	memSize = pow(2, atoi(argv[2]));
	framesNum = atoi(argv[4]);
	allocatedFrames = (struct frame*) malloc(sizeof(struct frame) * framesNum);
	emptyLeft = framesNum;
	pageFaults = 0;
	breakcounter = 0;


	//Makes sure there are exactly 3 commandline arguments
	if (argc != 5 || (atoi(argv[4]) < 1))
	{
		printf("propers usage: %s int int (FIFO or LRU) (int > 0)", argv[0] );
	}
	else
	{
		//sets all .isEmpty in allocatedFrames[] as true
		for(i = 0; i < framesNum; i++)
		{
			allocatedFrames[i].isEmpty = true;
			allocatedFrames[i].count = 0;
			allocatedFrames[i].page = 'null';
		}

		//debug to see what is contained in our variables
		
		printf("pageSize = %d\nmemSize = %d\n", pageSize, memSize);
		printf("algorithm = %s\n", argv[3]);
		printf("frameNum = %d\n", framesNum);
		
	
		//Decide if using LRU or FIFO algorithm
		if (strcmp (argv[3], "FIFO") == 0)
		{
			//printf("string is FIFO\n");
			//1.Ask and receive input
			while(1)
			{
				//printf("\nEnter hex values: ");
				fgets(string, 6, stdin);
				if (strcmp(string, "\n") == 0)
				{
					break;
				}
				else
				{
					sscanf(string, "%x", &address);
				}
				if(address > memSize - 1)
				{
					printf("Requested address greater than %d.\n", memSize - 1);
					break;
				}
				//printf("Got %d\n", address); //Debugging purposes
				
				//2.Figure out which page input belongs in and set as pageNumber
				pageNumber = address/pageSize;
				//printf("pageNumber = %d\n", pageNumber);

				//3.Check if page number already exists in frames
				pageExists = false;
				for(i = 0; i < framesNum && pageExists == false; i++)
				{
					//if page exists set pageExists to true
					if(pageNumber == allocatedFrames[i].page)
					{
						pageExists = true;
						//printf("Page %d already in allocatedFrames[%d].\n", pageNumber, i); //for debugging purposes
					}
				}
				if (pageExists == false)
				{
						pageFaults++; //Keep track of pageFaults
				}

				//4.If pageExists == false choose victim frame
				if (pageExists == false)
				{
					//choose victim frame
					//only executes if empty frames left
					if(emptyLeft > 0)
					{
						//printf("looking for empty frame.\n");
						for(i = 0; i < framesNum; i++)
						{
							if(allocatedFrames[i].isEmpty == true)
							{
								victim = i;
								emptyLeft--;
								break;
							}
							//printf("part outside break executes\n");
						}
					}
					//chooses victim when no more empty frames left
					else
					{
						victim = victimSelector(allocatedFrames, framesNum);
						//printf("victim selected: %d\n", victim);
						//printf("victim.page: %d\n", allocatedFrames[victim]);
					}
					//use pageSwap() to replace victim with pageNumber
					pageSwap(&allocatedFrames[victim], pageNumber);
				
				}
				//5.Increment count of every frame in array, only matters when new page is swapped in, hence inside if statement
				for(i = 0; i < framesNum; i++)
				{
					allocatedFrames[i].count++;
				}
				//6.Display statues of pageFaults & frames
				//printf("\npageFaults: %d\n", pageFaults);
				/*
				for(i = 0; i < framesNum; i++)
				{
					printf("Frame: %d Page: %d isEmpty: %d ", i, allocatedFrames[i].page, allocatedFrames[i].isEmpty);
					printf("allocatedFrames[%d].count: %d\n", i, allocatedFrames[i].count);
				}
				*/
			}
			printf("\npageFaults: %d\n", pageFaults);
		}//end of fifo if statement
		else if (strcmp(argv[3], "LRU") == 0)
		{
			//printf("string is LRU\n");//1.Ask and receive input
			while(1)
			{
				/*
				for(i = 0; i < framesNum; i++)
				{
					printf("Frame: %d Page: %d isEmpty: %d\n", i, allocatedFrames[i].page, allocatedFrames[i].isEmpty);
				}*/
				//printf("\nEnter hex values: ");
				fgets(string, 6, stdin);
				if (strcmp(string, "\n") == 0)
				{
					break;
				}
				else
				{
					sscanf(string, "%x", &address);
				}
				if(address > memSize - 1)
				{
					printf("Requested address greater than %d.\n", memSize - 1);
					break;
				}
				//printf("Got %d\n", address); //Debugging purposes
				
				//2.Figure out which page input belongs in and set as pageNumber
				pageNumber = address/pageSize;
				//printf("pageNumber = %d\n", pageNumber);

				//3.Check if page number already exists in frames
				pageExists = false;
				for(i = 0; i < framesNum && pageExists == false; i++)
				{
					//if page exists set pageExists to true
					if(pageNumber == allocatedFrames[i].page)
					{
						pageExists = true;
						//printf("Page %d already in allocatedFrames[%d].\n", pageNumber, i); //for debugging purposes
						allocatedFrames[i].count = 0;
					}
				}
				if (pageExists == false)
				{
						pageFaults++; //Keep track of pageFaults
				}

				//4.If pageExists == false choose victim frame
				if (pageExists == false)
				{
					//choose victim frame
					//only executes if empty frames left
					if(emptyLeft > 0)
					{
						//printf("looking for empty frame.\n");
						for(i = 0; i < framesNum; i++)
						{
							if(allocatedFrames[i].isEmpty == true)
							{
								victim = i;
								emptyLeft--;
								break;
							}
							//printf("part outside break executes\n");
						}
					}
					//chooses victim when no more empty frames left
					else
					{
						victim = victimSelector(allocatedFrames, framesNum);
						//printf("victim selected: %d\n", victim);
						//printf("victim.page: %d\n", allocatedFrames[victim]);
					}
					//use pageSwap() to replace victim with pageNumber
					pageSwap(&allocatedFrames[victim], pageNumber);
				
				}
				//5.Increment count of every frame in array, only matters when new page is swapped in, hence inside if statement
				for(i = 0; i < framesNum; i++)
				{
					allocatedFrames[i].count++;
				}
				//6.Display statues of pageFaults & frames
				//printf("\npageFaults: %d\n", pageFaults);
				/*
				for(i = 0; i < framesNum; i++)
				{
					printf("Frame: %d Page: %d isEmpty: %d ", i, allocatedFrames[i].page, allocatedFrames[i].isEmpty);
					printf("allocatedFrames[%d].count: %d\n", i, allocatedFrames[i].count);
				}
				*/
			}printf("\npageFaults: %d\n", pageFaults);
		}
		else {
			printf("must select FIFO or LRU\n");
		}
		return 0;
	}
}

void pageSwap(struct frame *currentFrame, int address)
{
	currentFrame->isEmpty = false;
	currentFrame->page = address;
    currentFrame->count = 0;
	
}

int victimSelector(struct frame allocatedFrames[], int framesNum)
{
	int i;
	int victim;
	int count = 0;

	for(i = 0; i < framesNum; i++)
	{
		if (count < allocatedFrames[i].count)
		{
			count = allocatedFrames[i].count;
			victim = i;
			//printf("VictimSelector count becomes %d\n", count);
		}
	}

	return victim;
}