/*************************************************************
 * proto_tdd_v0 -  émetteur                                   *
 * TRANSFERT DE DONNEES  v0                                   *
 *                                                            *
 *                                                            *
 * E. Lavinal - Univ. de Toulouse III - Paul Sabatier         *
 **************************************************************/

#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"
#include <stdio.h>
#include <stdlib.h>
/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char *argv[])
{

    unsigned char message[MAX_INFO]; /* message de l'application */
    int taille_msg = 1;              /* taille du message */
    paquet_t tab_p[SEQ_NUM_SIZE];    /*tabeau des paquet envoyer paquet utilisé par le protocole */
    int evenement = 0;
    paquet_t pack;       /*paquet d'pack recu par le protocole*/
    paquet_t connection; /*paquet de demande de connection et de deconnection*/

    int borne_inf = 0;
    int curseur = 0;
    int taille_fenetre = 7;
    int i; /*variable utiliser l'ors du revoit de paquet*/
    int test;
    int nb_trans = 0;
    int service; /*contient le service de la couche application*/

    /*recuperation de la fenetre si elle est passer en parametre*/
    if (argc == 2)
    {
        test = atoi(argv[1]);
        if (test >= SEQ_NUM_SIZE || test < 1)
        {
            printf("la fenetre doit etre un entier compris entre 1 et 8");
        }
        else
        {
            taille_fenetre = test;
        }
    }

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");

    de_application_mode_c(&service, message, &taille_msg);

    /*constrution du paquet de demande de connection*/
    connection.type = type_app_to_type_trans(service);
    connection.lg_info = 0;
    connection.num_seq = 0;
    connection.somme_ctrl = generer_checksum(connection);

    do
    {
        /* remise à la couche reseau */
        vers_reseau(&connection);
        nb_trans++;
        depart_temporisateur(10);
        evenement = attendre();
        if (evenement == -1)
        {
            de_reseau(&pack);
        }

    } while ((!verifier_checksum(pack) && nb_trans < NB_MAX_TRANS) && pack.type != CON_ACCEPT);

    printf("demande accepter\n");

    printf("pack type = %d\n", pack.type);

    /*traitement de la reponse reponse*/
    service = type_trans_to_type_app(pack.type);
    printf("service = %d\n", service);
    vers_application_mode_c(service, message, 0);

    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application_mode_c(&service, message, &taille_msg);

    /* tant que l'émetteur a des données à envoyer et n'as pas recu tout les acquitement */

    while (nb_trans < NB_MAX_TRANS &&
           (taille_msg != 0 || curseur != borne_inf))
    {
        printf("On entre dans la boucle\n");

        if (dans_fenetre(borne_inf, curseur, taille_fenetre) && taille_msg != 0)
        {
            /* construction paquet */
            for (int i = 0; i < taille_msg; i++)
            {
                tab_p[curseur].info[i] = message[i];
            }
            tab_p[curseur].lg_info = taille_msg;
            tab_p[curseur].type = DATA;
            tab_p[curseur].num_seq = curseur;
            tab_p[curseur].somme_ctrl = generer_checksum(tab_p[curseur]);
            printf("curseur = %d\n", curseur);

            /* remise à la couche reseau */
            vers_reseau(&tab_p[curseur]);

            /* lecture de donnees provenant de la couche application */

            de_application_mode_c(&service, message, &taille_msg);

            if (borne_inf == curseur)
            {
                depart_temporisateur(100);
            }
            curseur = inc(curseur);

            nb_trans = 0;
        }
        else
        {
            evenement = attendre();
            if (evenement == PAQUET_RECU)
            {
                /*reception et traitement des acquitement*/

                de_reseau(&pack);

                if (verifier_checksum(pack) && dans_fenetre(borne_inf, pack.num_seq, taille_fenetre))
                {

                    borne_inf = inc(pack.num_seq);

                    if (borne_inf == curseur)
                    {
                        arret_temporisateur();
                    }
                }
            }
            else
            {
                nb_trans++;
                i = borne_inf;
                depart_temporisateur(100);
                /*retransmission des paquet pas acquiter*/
                while (i != curseur)
                {
                    printf("i = %d\n", i);
                    vers_reseau(&tab_p[i]);
                    i = inc(i);
                }
            }
        }
    }

    /*envoit de la demande de deconnection*/
    connection.type = type_app_to_type_trans(service);
    connection.lg_info = 0;
    connection.num_seq = curseur;
    connection.somme_ctrl = generer_checksum(connection);

    printf("[TRP] envoie de demande de deconnection\n");

    pack.somme_ctrl = 0;

    do
    {
        /* remise à la couche reseau */
        vers_reseau(&connection);
        nb_trans++;
        depart_temporisateur(100);
        evenement = attendre();
        if (evenement == -1)
        {
            de_reseau(&pack);
        }

    } while ((!verifier_checksum(pack) && nb_trans < NB_MAX_TRANS) && pack.type != CON_CLOSE);

    if (nb_trans == NB_MAX_TRANS)
        fprintf(stderr, "[TRP] Erreur : Max recurssion, dernier ACK perdu\n");
    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}