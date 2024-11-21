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
#include "tresor.h"

/* =================================================================== */
/* FONCTION PRINCIPALE : SERVEUR ITERATIF                              */
/* =================================================================== */
int main(int argc, char **argv) {
    int sid;
    int nbport = atoi(argv[1]); 
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

    return 0;
} // end main
