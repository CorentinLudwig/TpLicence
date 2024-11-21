/* =================================================================== */
// Progrmame Serveur qui calcule le résultat d'un coup joué à partir
// des coordonnées reçues de la part d'un client "joueur".
// Version ITERATIVE : 1 seul client/joueur à la fois
/* =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "tresor.h"

#define N 10


/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */
int main(int argc, char **argv) {
    int sid;
    int nbport = atoi(argv[1]); 
    int res;
    int lig, col;
    char rep[6];
    char mess[3];
    int x_tresor = 4, y_tresor = 5;
    pid_t pid;

    if(nbport==-1){
        perror(argv[1]);
        exit(1);
    }

    //creation socket
    if ( (sid = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

    /* Init caracteristiques serveur distant (struct sockaddr_in) */


    struct sockaddr_in sk_addr;
    memset(&sk_addr, 0, sizeof(sk_addr));
    sk_addr.sin_family = AF_INET;
    sk_addr.sin_port = htons(nbport); /* host to network byte order */
    sk_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */

    //binding

    if (bind(sid, (struct sockaddr*) &sk_addr, sizeof(sk_addr)) == -1) {
        perror("Bind error");
        close(sid);
        return EXIT_FAILURE;
    }

    //etat d'ecoute



    while(true){

        if (listen(sid, 15) == -1) {
            perror("Listen error");
            close(sid);
            return EXIT_FAILURE;
    }

        pid=fork();
        if(pid==-1){
            perror("fork");
            exit(1);
        }

        if(pid){
            //accepter la connection
            int newsock;
            struct sockaddr_in cli_adr;
            socklen_t cli_adr_len = sizeof(cli_adr);

            newsock = accept(sid, (struct sockaddr *)&cli_adr, &cli_adr_len);
            if (newsock < 0) {
            perror("accept failed");
            close(sid);
            exit(EXIT_FAILURE);
            }

            /* Tentatives du joueur : stoppe quand tresor trouvé */
            do{
                //read
                if(recv(newsock,rep,sizeof(rep),0)==-1){
                    perror("sid");
                    exit(2);
                }

                //convertion reponce
                sscanf(rep,"%d %d",&lig,&col);

                res = recherche_tresor(N, x_tresor, y_tresor, lig, col);

                /* Construction requête (serialisation en chaines de caractères) */
                sprintf(mess,"%d",res);

                /* Envoi de la reponce au client(send) */
                if(send(newsock,mess,sizeof(mess),0)==-1){
                    perror("sid");
                    exit(2);
                }


            }while (res);

            close(newsock);
            exit(0);
        }
    }   
    return 0;
} // end main