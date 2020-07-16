#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 1000

struct process_desc{
    int process_id;
    int arrival_time;
    int burst_time;
    int priority;
};

int main(int argc, char *argv[]) {

    FILE *fp;
    printf("filename: %s\n",argv[1]);
    fp = fopen(argv[1], "r");// Get file name from terminal
    char str[MAXCHAR];
    int lineCounter=0;// Variable will hold the number lines
    while (fgets(str, MAXCHAR, fp) != NULL) {
        printf("%s", str);//Reading the text file
        lineCounter++;// For every line increment the lineCounter
    }
    printf("\nNumber of processes : %d\n",lineCounter);
    fclose(fp);

    struct process_desc processes[lineCounter];// Create process array with the length of number of lines in text file

    char str2[MAXCHAR]; // Will hold the every line

    fp = fopen(argv[1], "r"); // Read the input.txt
    int index=0;
    int desc =0;
    while (fgets(str2, MAXCHAR, fp) != NULL) { // Read until there no line to read
        printf("\nFor process %d",index+1);
        printf("\n%s", str2);
        char * split = strtok(str2,","); // Split the input when it see a ','
        desc=0; // Will hold the value which description of process is split
        while (split != NULL){
            if (desc == 0){ // For process id
                processes[index].process_id = atoi(split);
                printf("\nprocess_id : %d",processes[index].process_id);
            }else if (desc == 1){ // For arrival time
                processes[index].arrival_time = atoi(split);
                printf("\narrival time : %d",processes[index].arrival_time);
            }else if (desc == 2){ // For burst time
                processes[index].burst_time = atoi(split);
                printf("\nburst time : %d",processes[index].burst_time);
            }else{ // For priority
                processes[index].priority = atoi(split);
                printf("\npriority : %d\n",processes[index].priority);
            }
            split = strtok(NULL, ",");
            desc++;
        }
        index++;
    }
    fclose(fp); // Close the file

    printf("\n");

    struct process_desc s_processes[lineCounter]; // This will work like a accumulator means hold and change the processes value

    for(int i=0; i<lineCounter;i++){ // Copy the original processes
        s_processes[i].process_id = processes[i].process_id;
        s_processes[i].arrival_time = processes[i].arrival_time;
        s_processes[i].burst_time = processes[i].burst_time;
        s_processes[i].priority = processes[i].priority;
    }

    double averageTurnAroundTime=0; // Will hold the average turn around time
    double averageWaitingTime=0; // Will hold the average waiting time
    double averageResponseTime=0; // Will hold the average response time
    int smallestRemainingTimeIndex; // Will hold the index which process has the smallest burst time
    int allDone=0; // To check all processes are done
    int time = 0; // The time

    while(allDone != lineCounter){ // Work until all processes are completed
        smallestRemainingTimeIndex = 0; // Doesn't matter just for compare
        for(int j =0; j<lineCounter;j++){ // Check every process which has the smallest burst time
            if((s_processes[smallestRemainingTimeIndex].burst_time>s_processes[j].burst_time && s_processes[j].arrival_time<time && s_processes[j].burst_time !=0) || (s_processes[smallestRemainingTimeIndex].burst_time ==0 && s_processes[j].burst_time != 0)){
                // When burst time reaches 0 it has the smallest value but we shouldn't compare it that is because there is or in the if statement first is smallest burst time bigger than 0 second is burst time equal 0 and there is time variable also
                smallestRemainingTimeIndex = j; // The smallest index
            }
        }
        printf("Time t=%d, Process %d is using the CPU\n",time,s_processes[smallestRemainingTimeIndex].process_id);
        if (s_processes[smallestRemainingTimeIndex].burst_time !=0 && time!=0 && s_processes[smallestRemainingTimeIndex].arrival_time <time) { //  smallest index >0 and time !=0 and process is arrived
            if(s_processes[smallestRemainingTimeIndex].burst_time == processes[smallestRemainingTimeIndex].burst_time){ // If burst time is equal to original burst time that means it is first time process using the CPU
                averageResponseTime = averageResponseTime + (double)(time - s_processes[smallestRemainingTimeIndex].arrival_time); // Than keep the response time
            }
            s_processes[smallestRemainingTimeIndex].burst_time--; // Process used CPU
            if(s_processes[smallestRemainingTimeIndex].burst_time==0){ // If burst time became 0 after decrement that means process is finished
                printf("Process %d finished at t=%d", s_processes[smallestRemainingTimeIndex].process_id, time);
                printf("\n");
                averageTurnAroundTime = averageTurnAroundTime + (double)(time - processes[smallestRemainingTimeIndex].arrival_time); // Than keep the turn around time
                averageWaitingTime = averageWaitingTime + (double)(time - processes[smallestRemainingTimeIndex].arrival_time - processes[smallestRemainingTimeIndex].burst_time); // Than keep the waiting time
            }
        }
        allDone=0; // For every end of the time we don't know if all processes are finished
        for(int i=0; i<lineCounter; i++){
            if(s_processes[i].burst_time==0){ // If burst time is 0 for a process
                allDone++; // That means one of the processes is finished
            }
        }
        time++; // Increment the time
    }

    printf("\nAll processes are completed.\n");

    averageTurnAroundTime = averageTurnAroundTime/lineCounter; // Calculate the average turn around time
    averageWaitingTime = averageWaitingTime/lineCounter; // Calculate the average waiting time
    averageResponseTime = averageResponseTime/lineCounter; // Calculate the average response time

    printf("\nThe average turnaround time: %.2f", averageTurnAroundTime);
    printf("\nThe average waiting time: %.2f", averageWaitingTime);
    printf("\nThe average response time: %.2f\n", averageResponseTime);

    return 0;
}
