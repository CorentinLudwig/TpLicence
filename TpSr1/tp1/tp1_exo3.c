#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "decoupe.h"

void erreur(const char *mess, int valExit){
    perror(mess);
    exit(valExit);
}

int main(int argc, char *argv[]){

    for(int i=1;i<argc;i++){
        pid_t fpid;
        fpid = fork();

        if(fpid==-1)
            erreur("fork",1);

        if(fpid){ //pere
            printf("J’ai delegue sleep %d a 1208. J’attends sa fin...\n",fpid);
            wait(NULL);
            printf("%d termine.\n",fpid);

        }
        else  { //fils
            char *tab[NBMOTSMAX];
            Decoupe(argv[i],tab);

            execvp(tab[0],tab);



        }
    }
}