/*
Alex Chheng
Steven Barberi
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barrier;

void SimpleThread(int which){
    int num, val;
    
    for(num = 0; num < 20; num++){
        if (random() > RAND_MAX/2){
        usleep(500);
        }
    #ifdef PTHREAD_SYNC
    	pthread_mutex_lock(&lock);
    #endif
    	printf("*** thread %d sees value %d\n", which, val);
    	SharedVariable = val + 1;
    #ifdef PTHREAD_SYNC
    	pthread_mutex_unlock(&lock);
    #endif
    }
    #ifdef PTHREAD_SYNC
    	pthread_barrier_wait(&barrier);
    #endif
    val = SharedVariable;
    printf("*** thread %d sees final value %d\n", which, val);
}

void *ThreadID(void *arg){
    int thread_id = (long) arg;
    SimpleThread(thread_id);
}

void *func(void *var){
    sleep(2);
    printf("We are using thread \n");
    return NULL;
}

int main(int argc, char *argv[]){
    int i;
    int thread_count;
    if (argc != 2){
        printf("Error: Enter one parameter only for the command line");
        return(0);
    }
    else{
        thread_count = atoi(argv[1]);
        if(thread_count < 1){
            printf("Enter whole number 1 or higher");
            return 0;
        }
       
        if(pthread_barrier_init(&barrier, NULL, thread_count)){
            printf("Error: Barrier not created");
            return -1;
        }
        
        pthread_t array[thread_count];

        for(i = 0; i < thread_count; i++){
            pthread_create(&array[i], NULL, ThreadID, (void *)(long) i);
        }

        for(i = 0; i < thread_count; i++){
            pthread_join(array[i], NULL);
        }
    }
    return 0;
    /*
    pthread_t t_id;
    printf("Before using threading \n");
    pthread_create(&t_id, NULL, func, NULL);
    pthread_join(t_id, NULL);

    printf("After thread \n");

    exit(0);
    */
}
