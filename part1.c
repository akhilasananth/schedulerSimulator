#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void delayms(){
    ms = (long)time(NULL) * 1000;
    while((long)time(NULL) > ms);
}

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

processList *readyQueue = NULL;
process runningProcess;


FILE *processes;


//Fils names
char *processesFilename = "processes.txt";

//Function Prototypes

void readFromFile(FILE *fp ,char * filename);


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
	
