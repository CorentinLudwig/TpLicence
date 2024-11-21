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




struct message
{
    int nb;
    int capteur;
    pid_t pid;
};

int nb;
int capteur;


int tube[2];
int TIME;

void sigint_handler(int sig) { 
    struct message message;
    message.capteur = capteur; 
     message.nb = nb;
    message.pid = getpid();
    write(tube[1],&message,sizeof(struct message));

    alarm(TIME);
    }

void sigint_handler2(int sig) { 

    exit(0);
    }


int main(int argc, char const *argv[])
{
    if(argc != 4){
        fprintf(stdout,"connard");
        exit(0);
    }

    int NF,NBL,pid;

    struct message message;
    struct sigaction sa;

    NF = atoi(argv[1]);
    NBL = atoi(argv[2]);
    TIME = atoi(argv[3]);

    char inutile[10];
    pipe(tube);

    for (int i = 0; i < NF; i++)
    {
        
        switch (fork())
        {
        case -1:
            exit(1);
            break;
        case 0:

            
            sa.sa_handler = sigint_handler;
            sigemptyset(&(sa.sa_mask));
            sa.sa_flags = 0;
            sigaction(SIGALRM, &sa, NULL);
            sa.sa_handler = sigint_handler2;
            sigaction(5, &sa, NULL);
            
            close(tube[0]);

            capteur = i; 
            nb = 0;
            alarm(TIME);
            while(1){
                nb += read(STDIN_FILENO,inutile,sizeof(inutile));
            }



        default:
            break;
        }
    }
    

    close(tube[1]);

    while(read(tube[0],&message,sizeof(struct message))>0){
        printf("Pere - capteur %d: nb vehicule = %d\n",message.capteur,message.nb);
        if(message.nb >= NBL){
            kill(message.pid,5);
        }

    }
    close(tube[0]);
    while (1)
    {
        pid = wait(NULL);

        switch (errno)
        {
        case EINTR:
            break;

        case ECHILD:
            return 0;

        default:
            printf("fils %d fini\n", pid);
        }
    }

    return 0;
}
