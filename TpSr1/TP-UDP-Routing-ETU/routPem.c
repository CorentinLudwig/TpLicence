/*=============================================================*/
//Programme simulant un protocole de routage dynamique simplifié
// Ce programme code uniquement le comportement
// d'émetteur d'une annonce de routage
// vers UN SEUL routeur voisin pour UN échange initial de routes
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

#define BUF_SIZE_OUT 64 // we should send less...
#define IPV4_ADR_STRLEN 16  // == INET_ADDRSTRLEN
#define LOCALHOST "127.0.0.1"
#define NO_BASE_PORT 17900  // base number for computing real port number


/* =================================================================== */
/* FONCTION PRINCIPALE : PEER PROCESSUS DE ROUTAGE ROLE EMETTEUR ONLY  */
/* =================================================================== */

int main(int argc, char **argv) {

  // Usage routPem IDIP@ssRouter  MyNumberRouter NeigborNumberRouter
  // Example routPem 10.1.1.1 1 2

  char idInitConfigFile [20]; //Id of the configuration file of the router
  char myId [32]; // String array representing the whole id of the Router
  routing_table_t myRoutingTable; //Routing TABLE
  int sid;
  int nbport = atoi(argv[3]);

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

  /* A COMPLETER PAR LES ETUDIANTS ....
  ***************************************/
  /* Creation socket UDP */

    if ( (sid = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

  /* Init caracteristiques du recepteur (struct sockaddr_in) */
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(NO_BASE_PORT+nbport); /*host to network byte order */
    dest.sin_addr.s_addr = INADDR_ANY;

    for(int i =0;i<myRoutingTable.nb_entry;i++){
      if(sendto(sid, myRoutingTable.tab_entry[i], BUF_SIZE_OUT, 0, (const struct sockaddr*)&dest, sizeof(dest))<0){
        perror("send");
        exit(1);
      }
      printf("message send: %s\n",myRoutingTable.tab_entry[i]);
    }
    close(sid);
    printf("sock close\n");
    exit(EXIT_SUCCESS);
 }
