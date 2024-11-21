#include <stdio.h>
#include "couche_transport.h"
#include "services_reseau.h"
#include "application.h"

/* ************************************************************************** */
/* *************** Fonctions utilitaires couche transport ******************* */
/* ************************************************************************** */

uint8_t generer_checksum(paquet_t p){
    int somme =0;
    somme^=p.type;
    somme^=p.num_seq;
    somme^=p.lg_info;

    for (int i = 0; i < p.lg_info; i++)
    {
        somme^=p.info[i];
    }
    
    return somme;
}

int verifier_checksum(paquet_t p){
    return p.somme_ctrl == generer_checksum(p);
}

int inc(int i){
    return (i+1)%SEQ_NUM_SIZE;
}

int type_app_to_type_trans(int type){
    switch (type) {
        case T_CONNECT:
            return CON_REQ;
        case T_CONNECT_ACCEPT:
            return CON_ACCEPT;
        case T_CONNECT_REFUSE:
            return CON_REFUSE;
        case T_DATA:
            return DATA;
        case T_DISCONNECT:
            return CON_CLOSE;
        default:
            return -1;
    }
}

int type_trans_to_type_app(int type){
    switch (type) {
        case CON_REQ:
            return T_CONNECT;
        case CON_ACCEPT:
            return T_CONNECT_ACCEPT;
        case CON_REFUSE:
            return T_CONNECT_REFUSE;
        case DATA:
            return T_DATA;
        case CON_CLOSE:
            return T_DISCONNECT;
        default:
            return -1;
    }
}


/* ===================== Fenêtre d'anticipation ============================= */

/*--------------------------------------*/
/* Fonction d'inclusion dans la fenetre */
/*--------------------------------------*/
int dans_fenetre(unsigned int inf, unsigned int pointeur, int taille) {

    unsigned int sup = (inf+taille-1) % SEQ_NUM_SIZE;

    return
        /* inf <= pointeur <= sup */
        ( inf <= sup && pointeur >= inf && pointeur <= sup ) ||
        /* sup < inf <= pointeur */
        ( sup < inf && pointeur >= inf) ||
        /* pointeur <= sup < inf */
        ( sup < inf && pointeur <= sup);
}
