/*************************************************************
 * proto_tdd_v0 -  récepteur                                  *
 * TRANSFERT DE DONNEES  v0                                   *
 *                                                            *
 *                                                            *
 * E. Lavinal - Univ. de Toulouse III - Paul Sabatier         *
 **************************************************************/

#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"
#include <stdio.h>

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char *argv[]) {
  unsigned char message[MAX_INFO]; /* message pour l'application */
  paquet_t paquet;                 /* paquet utilisé par le protocole */
  int fin = 0;                     /* condition d'arrêt */

  paquet_t acquitement;
  uint8_t numseq = 0;

  init_reseau(RECEPTION);

  printf("[TRP] Initialisation reseau : OK.\n");
  printf("[TRP] Debut execution protocole transport.\n");

  /* tant que le récepteur reçoit des données */
  while (!fin) {

    // attendre(); /* optionnel ici car de_reseau() fct bloquante */
    de_reseau(&paquet);

    if (verifier_checksum(paquet)) {
      if (paquet.num_seq == numseq) {

        /* extraction des donnees du paquet recu */
        for (int i = 0; i < paquet.lg_info; i++) {
          message[i] = paquet.info[i];
        }

        /*envoi de l'aquitement*/
        acquitement.type = ACK;
        acquitement.num_seq = paquet.num_seq;
		acquitement.lg_info = 0;
        acquitement.somme_ctrl = generer_checksum(acquitement);
        vers_reseau(&acquitement);

        numseq = inc(numseq);
        /* remise des données à la couche application */
        fin = vers_application(message, paquet.lg_info);
      } else {

        /*envoi de l'aquitement*/
        acquitement.type = ACK;
        acquitement.num_seq = (numseq + SEQ_NUM_SIZE -1) % SEQ_NUM_SIZE;
        acquitement.somme_ctrl = generer_checksum(acquitement);
        vers_reseau(&acquitement);
      }
    }
  }

  printf("[TRP] Fin execution protocole transport.\n");
  return 0;
}
