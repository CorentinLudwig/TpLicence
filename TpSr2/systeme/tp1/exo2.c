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



int main(int argc, char const *argv[])
{
    if(argc !=2){
        fprintf(stderr,"erreur\n");
        exit(1);
    }

    int NBS,NBF;

    NBS = atoi(argv[1]);
    NBF = atoi(argv[0]);


    for (int i = 0; i < NBF; i++)
    {
        printf("Je suis le processus %d, il est \n",getpid());
        sleep(NBS);
    }
    
    return 0;
}
