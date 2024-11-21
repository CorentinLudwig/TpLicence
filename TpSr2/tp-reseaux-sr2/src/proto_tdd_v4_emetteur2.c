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
#define NB_MAX_TRANSMISSION 50
/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char *argv[])
{

	 unsigned char message[MAX_INFO]; /* message de l'application */
	 int taille_msg = 1;					/* taille du message */
	 paquet_t tab_p[SEQ_NUM_SIZE];					/*tabeau des paquet envoyer paquet utilisé par le protocole */
	 int evenement;
	 paquet_t pack; /*paquet d'pack recu par le protocole*/
     paquet_t connection;   /*paquet de demande de connection et de deconnection*/

	int borne_inf = 0;
	int curseur = 0;
	int taille_fenetre = 7;
	int i; /*variable utiliser l'ors du revoit de paquet*/
	int test;
	int nb_trans = 0;
    int service;        /*contient le service de la couche application*/

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


    
    de_application_mode_c(&service,message,&taille_msg);

    

    /*envoit de la demande de connection*/
    connection.type = type_app_to_type_trans(service);
    

    
    vers_reseau(&connection);

    /*reception de la reponse*/

    de_reseau(&connection);
    service = type_trans_to_type_app(connection.type);
    // printf("service = %d",service);
    // printf("connection type = %d",connection.type);
    vers_application_mode_c(service, message, 0);

    printf("[TRP] Debut execution protocole transport.\n");

    // /* lecture de donnees provenant de la couche application */
    // de_application_mode_c(&service,message,&taille_msg);

    /* tant que l'émetteur a des données à envoyer */

    while (nb_trans < NB_MAX_TRANS &&
           (taille_msg != 0 || curseur != borne_inf)) {
		printf("On entre dans la boucle\n");

		if (dans_fenetre(borne_inf, curseur, taille_fenetre) && taille_msg != 0)
		{

            /* lecture de donnees provenant de la couche application */
			// printf("de application\n");
			de_application_mode_c(&service,message,&taille_msg);


			/* construction paquet */
			for (int i = 0; i < taille_msg; i++)
			{
				tab_p[curseur].info[i] = message[i];
			}
			tab_p[curseur].lg_info = taille_msg;
			tab_p[curseur].type = type_app_to_type_trans(service);
			tab_p[curseur].num_seq = curseur;
			tab_p[curseur].somme_ctrl = generer_checksum(tab_p[curseur]);
			printf("curseur = %d\n", curseur);
			vers_reseau(&tab_p[curseur]);



			//printf("fin de application\n");

			if (borne_inf == curseur)
			{
				depart_temporisateur(100);
			}
			curseur = inc(curseur); 
			//printf("fin du premier if\n");

			nb_trans = 0;
		}
		else
		{
			evenement = attendre();
			if (evenement == PAQUET_RECU)
			{
				// printf("borne inf = %d\n", borne_inf);
				de_reseau(&pack);

				// printf("dans fenetre = %d\n", dans_fenetre(borne_inf, pack.num_seq, taille_fenetre));
				// printf("checksum = %d\n", verifier_checksum(pack));
				// verifier_checksum(pack) &&
				if ( verifier_checksum(pack) && dans_fenetre(borne_inf, pack.num_seq, taille_fenetre))
				{

					borne_inf = inc(pack.num_seq);
					// printf("inc borne inf = %d\n", borne_inf);

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
				while (i != curseur)
				{
					printf("i = %d\n", i);
					vers_reseau(&tab_p[i]);
					i = inc(i);
				}
			}
		}
	}



	if (nb_trans) fprintf(stderr, "[TRP] Erreur : Max recurssion, dernier ACK perdu\n");
	printf("[TRP] Fin execution protocole transport.\n");
	return 0;
}