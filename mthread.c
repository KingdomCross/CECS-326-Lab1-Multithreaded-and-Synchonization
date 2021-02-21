/*
Alex Chheng
Steven Barberi
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include <stdint.h>
/*
void *func(void *var){
    sleep(2);
    printf("We are using thread \n");
    return NULL;
}

int main(){
    pthread_t t_id;
    printf("Before using threading \n");
    pthread_create(&t_id, NULL, func, NULL);
    pthread_join(t_id, NULL);

    printf("After thread \n");

    exit(0);
}
*/

int SharedVariable = 0;

void SimpleThread(int which){
    int num, val;
    
    for(num = 0; num < 20; num++){
        if (random() > RAND_MAX/2)
        usleep(500);

    val = SharedVariable;
    printf("*** thread %d sees value %d\n", which, val);
    SharedVariable = val + 1;
    }
    val = SharedVariable;
    printf("*** thread %d sees final value %d\n", which, val);
}
