
/**
 * @file inv.c
 * @brief Implémentation des fonctions de gestion de l'inventaire.
 */
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/inv.h"

/// @brief Inventaire global.
inv * inventaire = NULL;

/**
 * @brief Affiche l'inventaire.
 *
 * @param inventaire Pointeur vers l'inventaire à afficher.
 */
void aff_inv(inv * inventaire){
    printf("label : lab\t");
    printf("|nom: n\t");
    printf("|rarity : rar\t");
    printf("|stat : st\t");
    printf("|emplacement : emp\t");
    printf("|stat a booster :st bo\t");
    if ( inventaire == NULL){
        printf("inventaire non init \n");
        return;
    }
    printf("    affichage   debut       \n");
    for( int i = 0 ; i < NB_EQU ; i++){
        printf("affichage item %d \t\t",i);
        aff_item(inventaire->liste[i]);
    }
    printf("    affichage   terminer      \n");
}

/**
 * @brief Affiche un item.
 *
 * @param item Pointeur vers l'item à afficher.
 */
void aff_item(item_t * item){
    if ( item == NULL) return;
    printf("------------\taff item\t--------------\n");
    printf("lab:%s\t",item->label);
    printf("|n:%s\t",item->nom);
    printf("|rar:%s\t",item->rarity);
    printf("|st:%d\t",item->stat);
    printf("|emp:%d\t",item->piece_equipement);
    printf("|st bo:%d\n",item->booste);
}

/**
 * @brief Détruit un item et libère la mémoire allouée.
 *
 * @param item Pointeur vers le pointeur de l'item à détruire.
 */
void dest_item(item_t ** item){
    if ( *item == NULL) return;
    free((*item)->nom);
    free((*item)->label);
    free((*item)->rarity);
    free(*item);
    *item = NULL;
}

/**
 * @brief Détruit un inventaire et libère la mémoire allouée.
 *
 * @param inventaire Pointeur vers le pointeur de l'inventaire à détruire.
 */
void dest_inv(inv ** inventaire){
    if ( *inventaire == NULL) return;
    for ( int i = 0 ; i < NB_EQU ; i++ ){
        dest_item(&(*inventaire)->liste[i]);
    }
    free(*inventaire);
    *inventaire = NULL;
}

/**
 * @brief Vérifie si un item est vide.
 *
 * @param item Pointeur vers l'item à vérifier.
 * @return int 1 si l'item est vide, 0 sinon.
 */
int est_vide(item_t * item){
    return item->label == NULL;
}

/**
 * @brief Initialise l'inventaire en remplissant les items à remplir.
 *
 * @param inventaire Pointeur vers l'inventaire à initialiser.
 */
void init_inv(inv *inventaire){
    for (int i = 0; i < NB_EQU; i++) {
        item_t *test = malloc(sizeof(item_t));
        test->label = NULL;
        test->nom = NULL;
        test->rarity = NULL;
        test->stat = 0;
        test->piece_equipement = CASQUE;
        test->booste = DEGAT;
        inventaire->liste[i] = test;
    }
}

/**
 * @brief Trouve le premier emplacement vide dans l'inventaire.
 *
 * @param inventaire Pointeur vers l'inventaire à vérifier.
 * @return int L'indice du premier emplacement vide, ou -1 si l'inventaire est plein.
 */
int prem_vide(inv * inventaire){
    for( int i = 0 ; i < NB_EQU ; i++)
        if (est_vide(inventaire->liste[i])) return i;
    return -1;
}

/**
 * @brief Vérifie si deux items peuvent être fusionnés.
 *
 * @param item1 Pointeur vers le premier item.
 * @param item2 Pointeur vers le second item.
 * @return int Code de retour indiquant si la fusion est possible.
 */
int droit_fusion(item_t * item1, item_t * item2){
    if (item1 == NULL || item2 == NULL) return -1; // item non recreer probleme code
    if (item1->label == NULL || item2->label == NULL) return -2; // item non remplis
    if (strcmp(item1->label, item2->label) != 0) return -4; // pas le même item
    if (strcmp(item1->rarity, item2->rarity) != 0) return -5; // pas la même rareté
    return 1; // fusion autorisée
}

/**
 * @brief Gère l'inventaire, en l'initialisant et en affichant les items.
 *
 * @param inventaire Pointeur vers le pointeur de l'inventaire à gérer.
 */
void gestion_inv(inv ** inventaire){
    printf("etape 1\n");
    if(*inventaire == NULL){
        *inventaire = malloc(sizeof(inv));
        printf("entrer init \n");
        init_inv((*inventaire));
    }

    printf ( " est vide (%d): %s \n ",4,est_vide((*inventaire)->liste[4])?"oui":"non");
    printf ( " premier vide : %d \n ",prem_vide((*inventaire)));
    printf ( "droit de fusion %d \n ",droit_fusion((*inventaire)->liste[4], (*inventaire)->liste[1]));

    printf("test affichage inv : \n");
    aff_inv(*inventaire);
    printf(" / fin etape 1 \n");

    dest_item(&((*inventaire)->liste[4]));
    printf("test affichage item 4 : \n");
    aff_item((*inventaire)->liste[4]);
}

/**
 * @brief Exemple d'initialisation et de destruction de l'inventaire.
 *
 * @code
 * gestion_inv(&inventaire);
 * printf("destruction inventaire : ");
 * dest_inv(&inventaire);
 * printf("OK\n");
 * @endcode
 */