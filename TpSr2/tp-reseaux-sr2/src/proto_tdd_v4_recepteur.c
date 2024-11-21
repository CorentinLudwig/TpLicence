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
int main(int argc, char *argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t paquet;                 /* paquet utilisé par le protocole */
    int rep;
    int service;
    paquet_t acquitement;
    uint8_t numseq = 0;

    init_reseau(RECEPTION);
    printf("[TRP] Initialisation reseau : OK.\n");

    do{
    /*reception de la demande de connection*/
    de_reseau(&paquet);
    }while (!verifier_checksum(paquet));

    /*construction de paquet de acceptation de la connection ou du refu*/
    service = type_trans_to_type_app(paquet.type);
    rep = vers_application_mode_c(service, message, 0);

    if (rep == -1)
    {
        fprintf(stderr, "erreur de vers application\n");
        return (1);
    }

    acquitement.type = type_app_to_type_trans(rep);
    acquitement.lg_info = 0;
    acquitement.num_seq = 0;
    acquitement.somme_ctrl = generer_checksum(acquitement);

    do{

        /*envoie de la reponse*/
        vers_reseau(&acquitement);
        de_reseau(&paquet);

    }while(paquet.type == CON_REQ);

    printf("[TRP] Debut execution protocole transport.\n");
    service = T_DATA;

    /* tant que le récepteur ne recoi pas une demande de deconnection */
    while (service != T_DISCONNECT)
    {
        de_reseau(&paquet);

        if (verifier_checksum(paquet))
        {
            if (paquet.num_seq == numseq)
            {

                /* extraction des donnees du paquet recu */
                for (int i = 0; i < paquet.lg_info; i++)
                {
                    message[i] = paquet.info[i];
                }

                service = type_trans_to_type_app(paquet.type);
                /* remise des données à la couche application */
                vers_application_mode_c(service, message, paquet.lg_info);

                /*envoi de l'aquitement*/
                if(service == T_DISCONNECT){
                    acquitement.type = CON_CLOSE_ACK;
                    printf("[TRP] envoie ack deconnection\n");
                } else {
                    acquitement.type = ACK;
                }
                acquitement.num_seq = paquet.num_seq;
                acquitement.lg_info = 0;
                acquitement.somme_ctrl = generer_checksum(acquitement);
                vers_reseau(&acquitement);

                numseq = inc(numseq);
            }
            else
            {

                /*envoi de l'aquitement si le paquet a le mauvais numero de sequence*/
                acquitement.type = ACK;
                acquitement.num_seq = (numseq + SEQ_NUM_SIZE - 1) % SEQ_NUM_SIZE;
                acquitement.somme_ctrl = generer_checksum(acquitement);
                vers_reseau(&acquitement);
            }
        }
    }

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}