#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void erreur(const char *mess, int valExit){
    perror(mess);
    exit(valExit);
}


int main(){

    pid_t fpid;
    fpid = fork();

    if(fpid==-1)
        erreur("fork",1);
    
    if(fpid==0){ //fils
        execlp("ls","ls","-al",NULL);
        erreur("ls",1);
    }
    else{ //pere
        int exi;
        wait(&exi);
        execlp("date","date",NULL);
        erreur("date",1);
    }
    
}