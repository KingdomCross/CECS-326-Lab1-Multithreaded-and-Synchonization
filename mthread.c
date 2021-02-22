/*
CECS 326 Lab 1 Multithreaded and Synchonization
Alex Chheng
Steven Barberi
02/21/21
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

int sharedVariable = 0; // Initialize variable
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Initialize the mutex lock which locks the critical section
pthread_barrier_t barrier; // Create a variable to block the final variable from showing early

void SimpleThread(int which){
	int num, val;
	for(num = 0; num < 20; num++){
		if(random() > RAND_MAX/2){
			usleep(500);
		}
	
	#ifdef PTHREAD_SYNC // Use in a command line to call a mutex lock
		pthread_mutex_lock(&lock);
		//locking the critical section
	#endif
		val = sharedVariable;
		printf("***thread%d sees value %d\n", which,val);
		sharedVariable= val + 1;
	#ifdef PTHREAD_SYNC // The exit of the lock
		pthread_mutex_unlock(&lock);
		//unlocking critical section
		//Will now allow another thread to execute
	#endif
	}
	#ifdef PTHREAD_SYNC 
	// Also included in a command line to call a barrier from showing the threads early
		pthread_barrier_wait(&barrier);
	#endif
	val = sharedVariable; // Make sharedVariable be the same as val
	printf("Thread%d sees final value %d\n", which, val);
	// The final result of the thread which should show at the end
}

void *ThreadID(void * argThread){
	int thread_id = (long) argThread;
	SimpleThread(thread_id); // Call function with type casted
}

int main(int argCommand, char *argInput[]){ // Take the command line and user string input

	int threadNumber = atoi(argInput[1]);//converts string to int
	
	if(argCommand != 2){//Parameter checking
		printf("Error: Please enter only one parameter."); // Notify user of error
		return(0);
	}
	
	if (threadNumber < 1){ // Check if variable is a positive 1 or above, if yes proceed
		printf("Error: Enter greater than or equal to 1 whole number"); // Notify user of error
		return 0;
	}
	
	if(pthread_barrier_init(&barrier, NULL, threadNumber)){
		printf("Error: Barrier is not created"); // Notify user of pthread error with barrier
		return -1;
	}

	pthread_t pthreadArray[threadNumber];

	for(int i = 0; i < threadNumber; i++){ // loop for number of thread
		pthread_create(&pthreadArray[i], NULL, ThreadID,(void *)(long) i);//creates the threads
	}

	for (int i = 0; i < threadNumber; i++){ // loop for number of thread
		pthread_join(pthreadArray[i], NULL);
	}
	
	return 0;
}
