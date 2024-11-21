#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_BASSIN 5

void erreur(const char *mess, int valExit){
    perror(mess);
    exit(valExit);
}


int main(){
    int remplir[2],etat[2], pid, int_read,bassin = 0,i,retour;
    int N=10, un=1, nega=-1;
    int f_flags;
    if(pipe(remplir)==-1)
        erreur("tube",1);
    if(pipe(etat)==-1)
        erreur("tube",1);
    
    pid = fork();

    switch (pid)
    {
    case -1:
        erreur("fork",2);
        break;
    
    case 0:     //fils
        close(remplir[1]);
        while(read(remplir[0],&int_read,sizeof(int))>0){
            if(int_read>0){
                if(bassin>=MAX_BASSIN){
                    write(etat[1],&bassin,sizeof(int));
                } else {
                    bassin += int_read;
                    printf("*"); fflush(stdout);
                }
            } else {
                bassin=0;
                printf("\n");
            }
        }
        if(close(etat[1])==-1)
            erreur("etat",3);
        exit(1);

        break;
    default:    //pere

        f_flags = fcntl(etat[0], F_GETFL); /* R´ecup´eration des flags */
        f_flags |= O_NONBLOCK; /* Positionnement du flag de non blocage */
        fcntl(etat[0], F_SETFL, f_flags); /* Mis `a jour des flags */
        close(remplir[0]);
        close(etat[1]);


        for(i=0;i<=N;i++){
            if(read(etat[0],&int_read,sizeof(int))>0)
                write(remplir[1],&nega,sizeof(int));
            write(remplir[1],&un,sizeof(int));
            sleep(1);
        }
        close(remplir[1]);
        wait(&retour);
        printf("%d\n",retour);
        break;
        
    }
}