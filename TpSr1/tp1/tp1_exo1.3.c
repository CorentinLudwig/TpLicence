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


void afficher_info(){
    printf("%d\n",getpid());
    printf("%d\n",getppid());
    printf("%d\n",getpgrp());
    printf("%s\n",getlogin());
    printf("%d\n",getuid());
    printf("%d\n",getgid());
    exit(0);
}

int main(){
    int fpid;
    fpid = fork();
    if(fpid==-1)
        erreur("fork",1);
    if(fpid){ //pere
        afficher_info();
        int exi;
        wait(&exi);
        printf("%d\n",exi);
        exit(0);
    }
    else  { //fils
        afficher_info();
        exit(0);
    }
    }