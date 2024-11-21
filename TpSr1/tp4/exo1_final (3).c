#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>


void erreur(const char *mess, int valExit){
    perror(mess);
    exit(valExit);
}

int main(){
    int N = 5;
    int tube[2];
    if(pipe(tube)==-1){
        erreur("tube",1);
    }

    int pid,int_w;
    pid = fork();

    int i,int_read,int_wait;

    switch (pid)
    {
    case -1:
        erreur("fork",2);
        break;
    
    case 0: // fils
        close(tube[1]);
        while(read(tube[0],&int_read,sizeof(int))>0){

            printf("%d\n", int_read);
        }
        close(tube[0]);

        exit(1);
        break;
    default: //pere
        close(tube[0]);
        for(i=0;i<=N;i++){
            if(write(tube[1],&i,sizeof(int))==-1)
                erreur("write",4);
            sleep(1);
        }

        if(close(tube[1])==-1)
            erreur("close",4);
        pid_t pid_fils = wait(&int_wait);
        printf("%d, %d\n", pid_fils,WEXITSTATUS(int_w));
        break;
    }

    return 1;
} 