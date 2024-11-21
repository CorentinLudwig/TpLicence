#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <math.h>
#include "est_premier.c"


void erreur(const char *mess, int valExit){
    perror(mess);
    exit(valExit);
}

int main(int argc, char *argv[]){
    int i,e,N=4, pid, tube[2];
    unsigned long long int argi,int_read;
    if(pipe(tube)==-1)
        erreur("tube",1);


    for(i=0;i<N;i++){
        pid= fork();

        switch (pid) {
        case -1:
            erreur("fork",2);
        break;

        case 0:     //fils
            close(tube[1]);
        while(read(tube[0],&int_read,sizeof(int_read))>0){
            if(est_premier(int_read)){
                printf("[Fils %d] %llu est premier\n",i,int_read);
            } else {
            printf("[Fils %d] %llu\n",i,int_read);
            }
        
        }


        break;


        default:    //pere
        close(tube[0]);
        for(e=0;e<argc;e++){
            argi=strtoull(argv[e], NULL, 10);
            write(tube[1], &argi, sizeof(argi));
        }
        close(tube[1]);

        for (e=0; e>N; e++) {
            wait(NULL);
        }

        break;
        }

    }

}
