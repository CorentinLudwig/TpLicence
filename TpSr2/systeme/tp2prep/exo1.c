#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    if(argc != 2){
        fprintf(stderr,"erreur");
    }

    int tube[2], int_read, n;

    pipe(tube);
    switch (fork())
    {
    case -1:
        //erreur
        break;
    
    case 0:
        close(tube[1]);
        while(read(tube[0],&int_read,sizeof(int))>0){
            printf("%d\n",int_read);
        }
        close(tube[0]);
        exit(0);
    
    default:
        n = atoi(argv[1]);
        close(tube[0]);
        for(int i = 0; i<n; i++){
            write(tube[1],&i,sizeof(int));
        }
        close(tube[1]);
        wait(NULL);
        exit(0);
        break;
    }
    
}
