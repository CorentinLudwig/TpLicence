/*================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// de récpeteur d'une annonce de routage
// émise depuis UN SEUL routeur voisin pour UN échange initial de routes
// T. Desprats - Novembre 2022

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

#define BUF_SIZE_IN 64 // we should receive less...
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

  /* A COMPLETER PAR LES ETUDIANTS ...
  ************************************/
    int sid;
    char entree [BUF_SIZE_IN];
    int nbport = atoi(argv[2]);
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
    sk_addr.sin_port = htons(NO_BASE_PORT+nbport); /* host to network byte order */
    sk_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */

    //binding

    if (bind(sid, (struct sockaddr*) &sk_addr, sizeof(sk_addr)) == -1) {
        perror("Bind error");
        close(sid);
        return EXIT_FAILURE;
    }

    unsigned int len = sizeof(dest);

    while (true) {
      //recieve message
      if(recvfrom(sid, entree, BUF_SIZE_IN, 0, (struct sockaddr*)&dest, &len)<0){
          perror("sid");
          exit(1);
      }
      printf("message recu\n");
      //add new content to the routing table
      if(!is_present_entry_table(&myRoutingTable,entree))
        add_entry_routing_table(&myRoutingTable,entree);      // Display new content of my routing table
      display_routing_table(&myRoutingTable,myId);
    }
  close(sid);
  exit(EXIT_SUCCESS);
 }
