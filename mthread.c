/*
CECS 326 Lab 1 Multithreaded and Synchonization
Alex Chheng
Steven Barberi
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <unistd.h>

int sharedVariable = 0; // Initialize variable
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Initialize the mutex lock
pthread_barrier_t hold; // Create a variable to block the final variable from showing early

void SimpleThread(int which){
	int num, val;
	for(num = 0; num < 20; num++){
		if(random() > RAND_MAX/2){
			usleep(500);
		}
	
	#ifdef PTHREAD_SYNC // Use in a command line to call a mutex lock
		//locks other threads from entering at the same time as the first one is being implemented
		pthread_mutex_lock(&lock);
	#endif
		val = sharedVariable;
		printf("***thread%d sees value %d\n", which, val); // Curent thread
		sharedVariable = val + 1; // Thread sharing
	#ifdef PTHREAD_SYNC // The exit of the lock
		//unlocks to let the next thread go in
		pthread_mutex_unlock(&lock);
	#endif
	}
	#ifdef PTHREAD_SYNC // Also included in a command line to call a barrier from showing the threads early
		//syncs other threads together
		//waits till all threads are here and releases them
		pthread_barrier_wait(&hold);
	#endif
	val = sharedVariable; // Make sharedVariable be the same as val
	printf("Thread%d sees final value %d\n", which, val); // The final result of the thread which should show at the end
}

void *ThreadID(void * argThread){
	int threadID = (long) argThread;//type casted 
	SimpleThread(threadID); // Call function with type casted
}

int main(int argCommand, char *argInput[]){ // Take the command line and user string input
	
	if(argCommand != 2){//checks to see if only one parameter was taken
		printf("Error: Enter only one parameter in the command line"); // Notify user of error
		return 0; // return and stop
	}

	int threadNumber = atoi(argInput[1]); // converting user string input to int

	if (threadNumber < 1){ // Check if variable is a positive 1 or above, if yes proceed
		printf("Error: Enter greater than or equal to 1 whole number"); // Notify user of error
		return 0; // return and stop
	}
	
	if (pthread_barrier_init(&hold, NULL, threadNumber)){//initialized to check if any thread is blocked
		printf("Error: Barrier is not created"); // Notify user of pthread error with barrier
		return -1;
	}

	pthread_t pthreadArray[threadNumber]; //create a unique data type for each threads with array

	for (int n = 0; n < threadNumber; n++){ // Loop for number of thread
		pthread_create(&pthreadArray[n], NULL, ThreadID, (void *)(long) n); // It is create a threads for each array
	}
	for (int m = 0; m < threadNumber; m++){ // loop for number of thread
		pthread_join(pthreadArray[m], NULL);//waits for current thread to finish before calling the next one
	}
	return 0;
}
