#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void delayms(){
    ms = (long)time(NULL) * 1000;
    while((long)time(NULL) > ms);
}
//Function Prototypes

void readFromFile(FILE *fp ,char * filename);
process dequeue(processList *lst);
void putOnReadyQueue();
void runProcess(process p);
long double getExeTime_milliseconds();
double getExeTime_Seconds();

//Variables
enum processState {new, ready, running, waiting, terminated}; 

typedef struct process{
	char *pid;
	double aTime;
	double CPUTime;
	double IOFrequency;
	double IODuration;
	int priority;
	enum processState ps;
} process;

typedef struct processList{
	process p;
	struct processList *next;
}processList;

processList *head = NULL;
processList *temp = NULL;

processList *readyQueueHead = NULL;
processList *readyQueueTemp = NULL;

process runningProcess;

int currentTime = getExeTime_milliseconds();


FILE *processes;


//Fils names
char *processesFilename = "processes.txt";



/*****************************************************************************************/
int main(){
	
	head = (processList*)malloc(sizeof(processList));
	
	readFromFile(processes,processesFilename);

	return 0;
}
/*****************************************************************************************/

//Reads from the input data text file and stores data in linked list
void readFromFile(FILE *fp ,char * filename){

	char line[100];
	
	char processID[2];
	double arrivalTime;
	double CPUTime;
	double IOFrequency;
	double IODuration;
	
	char processID2[2];
	double arrivalTime2;
	double CPUTime2;
	double IOFrequency2;
	double IODuration2;
	
	fp = fopen( filename,"r");
	
	//Gets the first process and assigns it to the head 
	fgets(line, sizeof(line), fp);
	sscanf(line, "%s\t%lf\t%lf\t%lf\t%lf\n", processID, &arrivalTime,&CPUTime,&IOFrequency,&IODuration);
	process p = {processID, arrivalTime,CPUTime, IOFrequency, IODuration, 0, ready};
	head->p = p;
	head->next = (processList*)malloc(sizeof(processList));
	temp = head;
	
	printf("Current process pid = %s\n", head->p.pid);
	
	for(;fgets(line, sizeof(line), fp) !=NULL;){

	    temp = temp->next;
		
		sscanf(line, "%s\t%lf\t%lf\t%lf\t%lf\n", processID2, &arrivalTime2,&CPUTime2,&IOFrequency2,&IODuration2);

		process p1 = {processID2, arrivalTime2,CPUTime2, IOFrequency2, IODuration2, 0, ready};
		temp->p = p1;
	    temp->next = (processList*)malloc(sizeof(processList));
		printf("Current process pid = %s\n", temp->p.pid);
		printf("head value = %s\n", head->p.pid);
		
	
	}
	fclose(fp);
}

//Dequeues a linked list FCFS. 
//Input: head of the list
//Output: process that got popped off the list
process dequeue(processList *lst){
	process p = lst->p;
	
	lst->p = lst->next->p;
	lst->next = lst->next->next;
	printf("The current head is %s \n", lst->p);
	
	return p;
}

//puts process in ready queue
void putOnReadyQueue(){
    
    readyQueueHead->p = dequeue(head);
    readyQueueHead->next = (processList*)malloc(sizeof(processList));
	readyQueueTemp = readyQueueHead;
	
	
    
}

//Runs a process for its I/O frequency time

void runProcess(process p){
    runningProcess = p;
    sleep((p.IOFrequency)/1000);
    
    return;
}

//Process goes from ready to Running
//Done when running is empty and process is available in the ready queue
void readyToRunning(){
	//if(ready queue is !empty)
		//if(running is !empty)
			//change state from ready to running;
			//print state change;
			//running = pop(readyQueue);
		//else
			//return
	//else
		//return
}

//process goes from running to terminated if cpu time is reached
//otherwise checks if IOFrequency is reached
void runningToTerminated(){
	//if(running is !empty)
		//if(processor running time >= CPUTime)
			//change state to from running to terminated
			//print state change
			//clear running
		//else
			//runningToWaiting();
	//else
		//return
}

//process goes from running to Waiting if IOFrequency is reached
void runningToWaiting(){
	//if(IOFrequency of process is reached)
		//change state of process
		//print state change
		//push into waitingQueue
	//else
		//return
}

//iterates through each process in the waiting queue
//each process goes from waiting to Ready if IODuration is reached
void waitingToReady(){
	//while(waiting Queue is empty)
		//choose first process
		//if(IODuration of process is reached)
			//change state of process
			//print state change
			//push into ready queue
		
}

//Returns the time in milliseconds from the time you press run
long double getExeTime_milliseconds(){
	return (long double)(getExeTime_Seconds()/CLOCKS_PER_SEC);
}

//Returns the time in seconds from the time you press run
double getExeTime_Seconds(){
	return (double)clock();
}
	
