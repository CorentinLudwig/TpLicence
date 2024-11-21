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

int i =0;
int tube[2];
int nb;


void sigint_handler(int sig) { 
        char buffer[128];
        sprintf(buffer,"Message du pere : %d",i);
        write(tube[1],buffer,strlen(buffer));
        i++;
        if(i>=nb){
            close(tube[1]);
        }
        (void)sig;
    }



int main(int argc, char const *argv[])
{
    if(argc != 3){
        fprintf(stderr,"erreur");
        exit(0);
    }
    int len;
    int timem;
    char buffer[128];
    nb = atoi(argv[2]);
    timem = atoi(argv[1]);
    if(pipe(tube)==-1){
        perror("pipe");
        exit(0);
    }
    switch(fork()){
        case -1:
            perror("fork");
            exit(0);
        case 0:
            close(tube[1]);
            read(tube[0],&len,sizeof(int));
            while(read(tube[0],buffer,len)){
                printf("Message recu %s\n",buffer);
            }
            close(tube[0]);
            exit(0);

        default:
            close(tube[0]);
            sprintf(buffer,"Message du pere : 0");
            len = strlen(buffer)+1;
            write(tube[1],&len,sizeof(int));

            struct sigaction sa;
            sa.sa_handler = sigint_handler;
            sigemptyset(&(sa.sa_mask));
            sa.sa_flags = 0;
            sigaction(SIGALRM, &sa,NULL);

            (void) nb;


            for(int i = 0; i<nb;i++){
                alarm(timem);
                pause();
            }
            break;
    }

    wait(NULL);
    return 0;
}
