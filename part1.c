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
void putOnReadyQueue();
void runProcess(process p);
long double getExeTime_milliseconds();
double getExeTime_Seconds();

//Variables
enum processState {new, ready, running, waiting, terminated};
const char* processState_Strings[] = {"New", "Ready", "Running", "Waiting","Terminated"};
processState s[5]; s[0] = new; s[1] = ready; s[2] = running; s[3] = waiting; s[3] = terminated;

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

processList *readyQueue = NULL;
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

//puts process in ready queue
void putOnReadyQueue(){
    for(int i=0; i<(temp->p.aTime + 1); i++){
        if((head->p.aTime)== i){
            head->p.ps = running;
            runProcess(head->p);
        }
        
    }
    
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
	if(isEmpty(readyQueue) == 1){ //if(ready queue is !empty)
		if(runningProcess != NULL){ //if(running is !empty)
			process *p = getNextFromReadyQueue();
			printStateChange(getExeTime_milliseconds(), p.pid, ready, running); //print state change;
			p.ps = running;//change state from ready to running;
			runningProcess = waitingQueue.pop();//running = pop(readyQueue);
		}//else
			//return
	}//else
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

//return 0 if false
//return 1 if true
int isEmpty(processList *list){
	return 0;
}

//iterates through the ready queue and gets the next process to run
process getNextFromReadyQueue(){
	return 	NULL;
}

//Returns the time in milliseconds from the time you press run
long double getExeTime_milliseconds(){
	return (long double)(getExeTime_Seconds()/CLOCKS_PER_SEC);
}

//Returns the time in seconds from the time you press run
double getExeTime_Seconds(){
	return (double)clock();
}

void printStateChange(long double time, char *pid, enum oldState, enum newState){
	printf("Time of transition: %ld   |Pid: %c   |Old State: %c   |New State: %c\n",
			time, *pid, processState_Strings[oldState],processState_Strings[newState]);
}
	
