#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    if(argc != 4){
        fprintf(stderr,"erreur");
    }

    int nf,nbvm,nbv,nv = 0,nvc = 0;
    
    

    nf =atoi(argv[1]);
    nbvm =atoi(argv[2]);
    nbv =atoi(argv[3]);

    int tab[nf][2];

    for (int i = 0; i < nf; i++)
    {
        pipe(tab[i]);
        switch (fork())
        {
        case -1:
            //erreur
            break;
        
        case 0:
            close(tab[i][0]);

            while(nv>nbvm){
                getchar();
                nv++;
                nvc++;
                if(nvc>=nbv){
                    write(tab[i][1],&nv,sizeof(int));
                    nvc=0;
                }
            }
            close(tab[i][1]);
            exit(0);

        default:
            close(tab[i][1]);

        }
    }

    
}
