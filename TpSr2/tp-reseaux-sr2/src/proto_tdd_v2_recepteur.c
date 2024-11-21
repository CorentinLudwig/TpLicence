/*************************************************************
 * proto_tdd_v0 -  récepteur                                  *
 * TRANSFERT DE DONNEES  v0                                   *
 *                                                            *
 *                                                            *
 * E. Lavinal - Univ. de Toulouse III - Paul Sabatier         *
 **************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char *argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t paquet;                 /* paquet utilisé par le protocole */
    int fin = 0;                     /* condition d'arrêt */

    paquet_t acquitement;
    uint8_t numseq = 0;

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* tant que le récepteur reçoit des données */
    while (!fin)
    {
        de_reseau(&paquet);

        if (verifier_checksum(paquet))
        {

            /*envoi de l'aquitement*/
            acquitement.type = ACK;
            acquitement.num_seq = paquet.num_seq;
            vers_reseau(&acquitement);

            if (paquet.num_seq == numseq)
            {
                numseq++;
                /* extraction des donnees du paquet recu */
                for (int i = 0; i < paquet.lg_info; i++)
                {
                    message[i] = paquet.info[i];
                }

                /* remise des données à la couche application */
                fin = vers_application(message, paquet.lg_info);
            }
        }
    }

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
