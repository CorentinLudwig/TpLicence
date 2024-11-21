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

struct info{
    int nf;
    int id;
};


void* thread_fonction(void *arg){
    struct info *info = (struct info*) arg;

    for(int i; i<info->nf;i++){
        printf("Thread %d : id = %ld\n",info->id,pthread_self());
    }

    return (void*)pthread_self();
}


int main(int argc, char const *argv[])
{
    
    if(argc != 3){
        fprintf(stderr, "Usage: %s <num_threads> <num_iterations>\n", argv[0]);
        exit(0);
    }
    int na;
    int nf;
    na = atoi(argv[1]);
    nf = atoi(argv[2]);
    long unsigned int retour;

    pthread_t  pthread[na];

    for(int i = 0; i < na;i++){
        struct info *info = malloc(sizeof(struct info));
        if (info == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        info->nf = nf;
        info->id = i;
        pthread_create(&pthread[i], NULL, thread_fonction, info);
    }


    for (int i = 0; i < na; i++)
    {
        pthread_join(pthread[i],(void **)&retour);
        printf("Valeur retourner par le thread %d = %ld\n",i,retour);
    }
    


}
