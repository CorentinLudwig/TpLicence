/*************************************************************
 * proto_tdd_v0 -  émetteur                                   *
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
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char *argv[])
{
    unsigned char message[MAX_INFO]; /* message de l'application */
    int taille_msg;                  /* taille du message */
    paquet_t paquet;                 /* paquet utilisé par le protocole */

    uint8_t numseq = 0;
    int evenement;
    paquet_t acquitement;
    acquitement.num_seq = -1;
    int nbTransmission = 0;

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_msg);

    /* tant que l'émetteur a des données à envoyer */
    while (taille_msg != 0)
    {
        nbTransmission = 0;

        /* construction paquet */
        for (int i = 0; i < taille_msg; i++)
        {
            paquet.info[i] = message[i];
        }
        paquet.lg_info = taille_msg;
        paquet.type = DATA;
        paquet.num_seq = numseq;
        paquet.somme_ctrl = generer_checksum(paquet);

        do
        {
            /* remise à la couche reseau */
            vers_reseau(&paquet);
            nbTransmission++;
            depart_temporisateur(10);
            evenement = attendre();
            if (evenement == -1)
            {
                de_reseau(&acquitement);
            }

        } while ((acquitement.num_seq != numseq && nbTransmission < NB_MAX_TRANS));

        arret_temporisateur();
        numseq++;
        /* lecture de donnees provenant de la couche application */
        de_application(message, &taille_msg);
    }

    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
