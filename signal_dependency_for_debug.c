/*
Evan Nibbe, a long-term project for prediction along the normal regression from the linear algebra A matrix to b vector, this forms the equivalent of the x vector between them, just with extra complications
	This dependency holds onto the numbers of what lines have been reached in AImappingA_to_b.c
	Using the signalling functions and binary, we can represent easily what lines have been reached using only 4 signals
	kill(pid, 23) //start of new signal-generated number (set current=0)
	kill(pid, 24) //0 binary, set current=current<<1;
	kill(pid, 25) //1 binary, set current=current<<1+1;
	kill(pid, 26) //end of number, add 1 to the respective dictionary entry for that (current) line number.
	kill(pid, 27) //print out the last number read and the values for all dictionary entries.

**Update: the calling code must be sent an interrupt once the requisite operation is complete in order to continue
** interrupting this program, thus synchronizes with the operation of this program.
	** will look for this pid_t in calling_pid.bin
	** sending signal 26
*/
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

pid_t toDebug=1073741824; //the calling code, contained at the program's pid
	//this can be used to sychronize all the interrupts sent to 
	//this program with the calling code
int current=0;
int wasInterrupted=0; //this forms a sort-of semaphore that simply exits the code if too many kill signals are sent at the same time.
#define MAX_LINES 2000 //base this off the number of lines in AImappingA_to_b.c
long statistic[MAX_LINES];

void signal27(int arg) {
	printf("The number of times each line was reached are:\n");
	for (int i=0; i<MAX_LINES; i++) {
		if (statistic[i]>0) printf("line %d, %ld\n", i, statistic[i]);
	}
	printf("the last line reached was %d\n", current);
	exit(0);
}
void signal23(int arg) {
	if (toDebug==1073741824) {
		FILE *fp=fopen("calling_pid.bin", "r");
		fread(&toDebug, sizeof(pid_t), 1, fp);
		fclose(fp);
	}
	current=0;
	kill(toDebug, 26);
}
void signal24(int arg) {
	current=current<<1;
	kill(toDebug, 26);
}
void signal25(int arg) {
	current=(current<<1)+1;
	kill(toDebug, 26);
}
void signal26(int arg) {
	if (current<MAX_LINES) {
		statistic[current]+=1;
	}
	else {
		printf("Line number was too high: %d\n", current);
		exit(-1);
	}
	kill(toDebug, 26);
}
int main(int argc, char *argv[]) {
	//skip first argument (name of program)
	//second argument is where the pid of this program should be placed for programs that would call to send kill signals to it.
	if (argc!=2) {
		printf("This program only takes one argument being the file name of where the pid to send kill signals to should be placed.\n");
		exit(-1);
	}
	signal(27, signal27);
	signal(26, signal26);
	signal(25, signal25);
	signal(24, signal24);
	signal(23, signal23);
	for (int i=0; i<MAX_LINES; i++) {
		statistic[i]=0;
	}
	FILE *fp=fopen(argv[1], "w");
	pid_t pid=getpid();
	fwrite(&pid, sizeof(pid_t), 1, fp);
	fclose(fp);
	while (1) {
		sleep(1); //wait in a loop until interrupted, at which point something will be edited or the program will close (with 27)

	}
	return 0; //program ends if the file names are not given as command-line arguments
}
