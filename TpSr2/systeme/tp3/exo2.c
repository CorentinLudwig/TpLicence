#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int solde;

void* thread_fonction(void *arg){
    int *NB = (int*) arg;
    int debit;
    for (int i = 0; i < *NB; i++)
    {
        debit = rand()%101-50;

        if(debit>=0){
            solde+=debit;
            printf("credit %ld : credit %d -> solde = %d\n",pthread_self(),debit,solde);
        } else {
            if((-debit)>solde){
                printf("solde insufisante\n");
                solde+=debit;
                printf("debit %ld : debit %d -> solde = %d\n",pthread_self(),debit,solde);
            } else {
                solde+=debit;
                printf("debit %ld : debit %d -> solde = %d\n",pthread_self(),debit,solde);
            }
        }
    }
    

    

    return (void*)pthread_self();
}


int main(int argc, char const *argv[])
{
        
    if(argc != 4){
        fprintf(stderr, "Usage: %s <num_threads> <num_iterations>\n", argv[0]);
        exit(0);
    }
    int NT;
    int NB;
    srand(time(NULL));
    NT = atoi(argv[1]);
    solde = atoi(argv[2]);
    NB = atoi(argv[3]);

    pthread_t  pthread[NT];


    for(int i = 0; i < NT;i++){
        pthread_create(&pthread[i], NULL, thread_fonction, &NB);
    }


    for (int i = 0; i < NT; i++)
    {
        pthread_join(pthread[i],NULL);
        printf("thread %d fini\n",i);
    }


    return 0;
}
