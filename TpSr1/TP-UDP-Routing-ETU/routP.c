
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> // struct sockaddr_in
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "tabrout.h"

#define BUF_SIZE 64 // we should receive less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number


/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE RECEPTEUR ONLY */
/* =================================================================== */
int main(int argc, char **argv) {

  // Usage routPrec IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPrec 10.1.1.1 1 2

  char idInitConfigFile [20]; //Id of the configuration file of the router
  char myId [32]; // String array representing the whole id of the Router

  routing_table_t myRoutingTable; //Routing TABLE



  /* Building ID Router from command args */
  sprintf(myId,"R%s %s",argv[2],argv[1]);
  printf("ROUTEUR : %s\n",myId );
  //printf("construction id fichier\n");
  /* Building Config File ID from command args */
  sprintf(idInitConfigFile,"R%sCfg",argv[2]);
  strcat(idInitConfigFile,".txt");
  //printf("\n Nom fichier Configuration : %s",idInitConfigFile);
  /* Loading My Routing Table from Initial Config file */
  init_routing_table(&myRoutingTable, idInitConfigFile);
  printf("ROUTEUR : %d entrées initialement chargées \n",myRoutingTable.nb_entry);
  display_routing_table(&myRoutingTable,myId);


    int sid;
    char entree [BUF_SIZE];
    int nbport = atoi(argv[2]);
    int nbportvoisin = atoi(argv[3]);

    int pid = fork();
    if(pid==-1){
        perror("fork");
        exit(1);
    }

    if(pid>0){ // Recepteur


        /* Creation socket UDP */

        if ( (sid = socket(AF_INET,SOCK_DGRAM,0)) < 0)
        {
            perror("Socket creation error");
            return EXIT_FAILURE;
        }
        /* Init caracteristiques du recepteur (struct sockaddr_in) */
        struct sockaddr_in sk_addr, dest;
        memset(&sk_addr, 0, sizeof(sk_addr));
        sk_addr.sin_family = AF_INET;
        sk_addr.sin_port = htons(NO_BASE_PORT+nbport);
        sk_addr.sin_addr.s_addr = inet_addr(LOCALHOST);; /* host to network byte order */
        // sk_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */

        //binding

        if (bind(sid, (struct sockaddr*) &sk_addr, sizeof(sk_addr)) == -1) {
            perror("Bind error");
            close(sid);
            return EXIT_FAILURE;
        }

        unsigned int len = sizeof(dest);
        int tailleTableRoutage;
        recvfrom(sid, &tailleTableRoutage, sizeof(int), 0, (struct sockaddr*) &dest, &len);

        for(int i = 0; i<tailleTableRoutage; i++) {
        //recieve message
        if(recvfrom(sid, entree, BUF_SIZE, 0, (struct sockaddr*)&dest, &len)<0){
            perror("sid");
            exit(1);
        }
        printf("message recu: %s\n", entree);
        //add new content to the routing table
        if(!is_present_entry_table(&myRoutingTable,entree))
            add_entry_routing_table(&myRoutingTable,entree);
        // Display new content of my routing table
        display_routing_table(&myRoutingTable,myId);
        }
    close(sid);
    exit(EXIT_SUCCESS);


    }else{ // Emeteur

        if ( (sid = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

  /* Init caracteristiques du recepteur (struct sockaddr_in) */
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(NO_BASE_PORT+nbportvoisin); /*host to network byte order */
    // dest.sin_addr.s_addr = inet_addr(LOCALHOST);; /* host to network byte order */
        int lenMyRoutingTable = myRoutingTable.nb_entry;

    if (sendto(sid, &lenMyRoutingTable, sizeof(lenMyRoutingTable), 0, (struct sockaddr*) &dest, sizeof(dest)) == -1){
        perror("Erreur envoie");
        exit(EXIT_FAILURE);
     }
    for(int i =0;i<myRoutingTable.nb_entry;i++){
      if(sendto(sid, myRoutingTable.tab_entry[i], BUF_SIZE, 0, (const struct sockaddr*)&dest, sizeof(dest))<0){
        perror("send");
        exit(1);
      }
      printf("message send: %s\n",myRoutingTable.tab_entry[i]);
    }
    close(sid);
    printf("sock close\n");
    exit(EXIT_SUCCESS);


    }
}