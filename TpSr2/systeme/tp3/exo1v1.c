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
#define VAL 100



int main(int argc, char const *argv[])
{
    
    if(argc != 3){
        fprintf(stderr,"erreur");
        exit(0);
    }
    int na;
    int nf;
    int v;
    pid_t pid;
    na = atoi(argv[1]);
    nf = atoi(argv[2]);

    for(int i =0; i<na;i++){

        switch (fork())
        {
        case -1:
            perror("fork");
            exit(1);


        case 0:
            for (int e = 0; e < nf; e++)
            {
               printf("Activiter rang %d : identifiant %d\n",i,getpid());
            }
            exit(i);
        default:
            break;
        }
    }

    for(int i =0; i<na; i++){
        pid = wait(&v);
        printf("Valeur retourne par fils %d = %d\n",pid,WEXITSTATUS(v));
    }
}
