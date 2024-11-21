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


int main(int argc, char const *argv[])
{
        
    if(argc != 6){
        fprintf(stderr, "Usage: %s <num_threads> <num_iterations>\n", argv[0]);
        exit(0);
    }
    int NP,NC,depots,consommation,len;

    NP = atoi(argv[1]);
    NC = atoi(argv[2]);
    depots = atoi(argv[3]);
    consommation = atoi(argv[4]);
    len = atoi(argv[5]);

}