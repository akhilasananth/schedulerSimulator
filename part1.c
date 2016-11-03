#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//Enum
enum processState {new, ready, running, waiting, terminated};
const char* processState_Strings[] = {"New", "Ready", "Running", "Waiting","Terminated"};

//Structs
typedef struct process{
	char *pid;
	double aTime;
	double CPUTime;
	double IOFrequency;
	double IODuration;
	int exeTime;
	int waitingTime;
	int priority;
	enum processState ps;
} process;

typedef struct processList{
	process p;
	struct processList *next;
}processList;

//Variables
processList *head = NULL;
processList *temp = NULL;

processList *readyQueueHead = NULL;
processList *readyQueueTemp = NULL;

processList *waitingQueueHead = NULL;
processList *waitingQueueTemp = NULL;

process *runningProcess = NULL;

int currentTime;
int processExeTime;
int processFrequencyTime;

//Fils names
FILE *processes;
char *processesFilename = "processes.txt";

//Function Prototypes
void readFromFile(FILE *fp ,char * filename);
struct process dequeue(processList *lst);
void readyToRunning();
void runningToTerminated();
void runningToWaiting();
void waitingToReady();
int isEmpty(processList *list);
void putOnReadyQueue();
int getExeTime_milliseconds();
int getExeTime_Seconds();
void printStateChange(int time, char* pid, int oldState, int newState);


/*****************************************************************************************/
int main(){
	
	currentTime = getExeTime_milliseconds();
	
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

		process p1 = {processID2, arrivalTime2,CPUTime2, IOFrequency2, IODuration2,0,0,0, ready};
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
	printf("The current head is %s \n", lst->p.pid);
	
	return p;
}

//Puts a process in a linked list 
void enqueue(processList *lst, process proc){
	lst->p = proc;
	lst->next = (processList*)malloc(sizeof(processList));
	lst = lst->next;
	
}

//puts process in ready queue
void putOnReadyQueue(){
    
    readyQueueHead->p = dequeue(head);
    readyQueueHead->next = (processList*)malloc(sizeof(processList));
	readyQueueTemp = readyQueueHead;
	
	
    
}

//Process goes from ready to Running
//Done when running is empty and process is available in the ready queue
void readyToRunning(){
	if(isEmpty(readyQueueHead)){ //if(ready queue is !empty)
		if(runningProcess){ //if(running is not null)
			process p = dequeue(readyQueueHead); //get next process
			printStateChange(getExeTime_milliseconds(), p.pid, ready, running); //print state change;
			p.ps = running; //change state from ready to running;
			p.exeTime = p.waitingTime = getExeTime_Seconds();
			runningProcess = &p; //running = pop(readyQueue);
			//processFrequencyTime = processExeTime = getExeTime_Seconds();
		}//else
			//return
	}//else
		//return
}

//process goes from running to terminated if cpu time is reached
//otherwise checks if IOFrequency is reached
void runningToTerminated(){
	if(runningProcess){ //if(running is not null)
		process p = *runningProcess;
		if((getExeTime_Seconds()-processExeTime) >= p.CPUTime){ //if(processor running time >= CPUTime)
			p.ps = terminated; //change state to from running to terminated
			printStateChange(getExeTime_milliseconds(), p.pid, running, terminated); //print state change
			runningProcess = NULL; //clear running
		}//else
			runningToWaiting(); //runningToWaiting();
	}//else
		//return
}

//process goes from running to Waiting if IOFrequency is reached
void runningToWaiting(){
	process p = *runningProcess;
	if((getExeTime_Seconds()-processFrequencyTime) >= p.IOFrequency){ //if(IOFrequency of process is reached)
		//here we need a way to keep track of the processtime when in waiting
		p.ps = waiting; //change state of process
		printStateChange(getExeTime_milliseconds(), p.pid, running, waiting); //print state change
		enqueue(waitingQueueHead,p); //push into waitingQueue
		runningProcess = NULL; //clear running;
		processFrequencyTime = getExeTime_Seconds(); //reset frequency time
	} //else
		//return
}

//iterates through each process in the waiting queue
//each process goes from waiting to Ready if IODuration is reached
void waitingToReady(){
	while(!isEmpty(waitingQueueHead)){ //while(waiting Queue is !empty)
		//choose first process
		//if(IODuration of process is reached)
			//p.ps = ready; //change state of process
			//printStateChange(getExeTime_milliseconds(), p.pid, waiting, ready); //print state change
			//push into ready queue
	}
		
}

//return 0 if false
//return 1 if true
int isEmpty(processList *list){
	if(!list){return 1;}
	return 0;
}

//Returns the time in milliseconds from the time you press run
int getExeTime_milliseconds(){
	return clock();
}

//Returns the time in seconds from the time you press run
int getExeTime_Seconds(){
	int temp = getExeTime_milliseconds();
	return (temp/CLOCKS_PER_SEC);
}

void printStateChange(int time, char* pid, int oldState, int newState){
	printf("Time of transition: %d   Pid: %s   Old State: %s   New State: %s\n",
			time, pid, processState_Strings[oldState],processState_Strings[newState]);
}
	
