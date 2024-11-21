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

int compteur1;
int compteur2;
int NB;

void sigint_handler(int sig) { 
    if(sig == 5){
        compteur1 += NB;
        printf("Pere - nombre de vehicule capteur 0 = %d\n",compteur1);
    } else {
        compteur2 += NB;
        printf("Pere - nombre de vehicule capteur 1 = %d\n",compteur2);
    }

    }



    int main(int argc, char const *argv[])
    {
        if (argc != 3)
        {
            fprintf(stdout, "connard");
            exit(0);
        }

        int NBL, comteur = 0,pid;
        char inutil[10];

        compteur1 = 0;
        compteur2 = 0;
        NB = atoi(argv[2]);
        NBL = atoi(argv[1]);


            struct sigaction sa;
            sa.sa_handler = sigint_handler;
            sigemptyset(&(sa.sa_mask));
            sa.sa_flags = 0;
            sigaction(5, &sa, NULL);
            sigaction(6, &sa, NULL);

        for (int i = 0; i < 2; i++)
        {
            switch (fork())
            {
            case -1:
                perror("fork");
                exit(1);

            case 0:

                while (comteur < NBL)
                {
                    comteur += read(STDIN_FILENO, inutil, 1);
                    printf("fils %d fini de lire: compteur = %d\n",i,comteur);
                    if (comteur % NB == 0)
                    {
                        printf("fils %d envoie signal\n",i);
                        if(kill(getppid(), 5+i) == -1){
                            perror("kill");
                            exit(2);
                        }
                    }
                }

                exit(0);

            default:
                break;
            }
        }

        while(1){
            pid = wait(NULL);

            switch(errno){
                case EINTR:
                    break;

                case ECHILD:
                    return 0;

                default:
                    printf("fils %d fini\n",pid);
            }
        }        

        return 0;
}
