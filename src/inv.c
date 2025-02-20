
/**
 * \file inv.c
 * \brief Implémentation des fonctions de gestion de l'inventaire.
 */

 #include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../lib/inv.h"

/// \brief Inventaire global.
inv * inventaire = NULL;

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

void aff_item(item_t * item){
    if ( item == NULL) return;
    printf("------------\taff item\t--------------\n");
    printf("lab:%s\t",item->label);
    printf("|n:%s\t",item->nom);
    printf("|rar:%d\t",item->rarity);
    printf("|st:%d\t",item->stat);
    printf("|emp:%d\t",item->piece_equipement);
    printf("|st bo:%d\n",item->booste);
}


void dest_item(item_t ** item){
    if ( *item == NULL) return;
    free((*item)->nom);
    free((*item)->label);
    free(*item);
    *item = NULL;
}

void liberer_item(item_t * item){
    free(item->nom);
    item->nom = NULL ;
    free(item->label);
    item->label = NULL ;
    item->piece_equipement = CASQUE ;
    item->booste = DEGAT ;
    item->rarity = COMMUN;
    item->stat = 0 ;
}

void dest_inv(inv ** inventaire){
    if ( *inventaire == NULL) return;
    for ( int i = 0 ; i < NB_EQU ; i++ ){
        dest_item(&(*inventaire)->liste[i]);
    }
    free(*inventaire);
    *inventaire = NULL;
}

int est_vide(item_t * item){
    return item->label == NULL;
}

void init_inv(inv *inventaire){
    for (int i = 0; i < NB_EQU; i++) {
        item_t *test = malloc(sizeof(item_t));
        test->label = NULL;
        test->nom = NULL;
        test->rarity = COMMUN;
        test->stat = 0;
        test->piece_equipement = CASQUE;
        test->booste = DEGAT;
        inventaire->liste[i] = test;
    }
}

int prem_vide(inv * inventaire){
    for( int i = 0 ; i < NB_EQU ; i++)
        if (est_vide(inventaire->liste[i])) return i;
    return -1;
}

int droit_fusion(item_t * item1, item_t * item2){
    if (item1 == NULL || item2 == NULL) return -1; // item non recreer probleme code
    if (item1->label == NULL || item2->label == NULL) return -2; // item non remplis
    if (strcmp(item1->label, item2->label) != 0) return -4; // pas le même item
    if (item1->rarity != item2->rarity) return -5; // pas la même rareté
    return 1; // fusion autorisée
}

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
 * \brief Exemple d'initialisation et de destruction de l'inventaire.
 *
 * \code
 * gestion_inv(&inventaire);
 * printf("destruction inventaire : ");
 * dest_inv(&inventaire);
 * printf("OK\n");
 * \endcode
 */
 int generate_stat_nb(int x,int type,int stat_ameliorer ,int rarete,int nb_max_nv){// type = {or =  var : 0,7 // degat = var : 1 // vit_att = var : 0,1 }
    float var ;
    if (!type)var = 1.0; // si type = degat
    else var = (type == 1) ? 0.1 : 0.7 ;

    int taille = 10 ;

    int som_croi = 0 ;
    int liste_stat[] = { 4, 6 , 9 , 16  ,  30 , 15 , 10 , 8 , 4 , 3};
    int random_nb = rand() % 100 + 1 ;
    float decalage = ((float)x / nb_max_nv) / 1.5;   // pourcentage localiser par le niv diviser par 1.5
    float rar ;
    switch (rarete){
    case COMMUN:rar = 1 ; break;
    case UNCOMMUN:rar = 1.1 ; break;
    case RARE:rar = 1.25 ; break;
    case EPIC:rar = 1.4 ; break;
    case LEGENDARY:rar = 1.6 ; break;
    default:break;

    }
    for ( int i = 0 ; i < taille/2 ; i++ ){
        int invers = taille - i - 1;
        int calcule = round(decalage * liste_stat[i]) ;
        liste_stat[i] -= calcule;
        liste_stat[invers] += calcule;
    }
    for (int i = 0; i < taille; i++) {
        som_croi += liste_stat[i];
        if(som_croi >= random_nb)
            return stat_ameliorer + round(( i + 1 ) * rar * var) ;
    }

    return -1;
 }
void deb_fusion(item_t * item1, item_t * item2) {
    if (droit_fusion(item1, item2) == 1) {
        item1->rarity++;        // rareté up / ex: commun -> uncommun / uncommun -> rare etc
        item1->stat = generate_stat_nb(1, item1->booste, item1->stat, item1->rarity, 1);
        dest_item(&item2);
    }
}

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void selection( int mouse_x,int mouse_y,inv * inventaire);

void cercle_graf(SDL_Renderer* renderer, int x, int y, int rayon) {
    for (int w = 0; w < rayon * 2; w++) {
        for (int h = 0; h < rayon * 2; h++) {
            int dx = rayon - w , dy = rayon - h; 
            if ((dx*dx + dy*dy) <= (rayon * rayon)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

// Fonction pour dessiner un rectangle avec des bords arrondis
void rectangle_arrondis(SDL_Renderer* renderer, int x, int y, int w, int h, int radius , int r , int g , int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Dessiner les coin arrondi  / dessine des cercle pour les couper pour avoir le coin
    cercle_graf(renderer, x + radius, y + radius, radius);// haut / gauche
    cercle_graf(renderer, x + w - radius, y + radius, radius);// haut / droit
    cercle_graf(renderer, x + radius, y + h - radius, radius);// bas / gauche
    cercle_graf(renderer, x + w - radius, y + h - radius, radius);// bas / droit

    //dessine le corps
    SDL_Rect rects[] = {
        {x + radius, y, w - 2 * radius, radius}, // haut
        {x + radius, y + h - radius, w - 2 * radius, radius}, // bas
        {x, y + radius, w   , h - 2 * radius},//milieu
    };
    SDL_RenderFillRect(renderer, &rects[0]);
    SDL_RenderFillRect(renderer, &rects[1]);
    SDL_RenderFillRect(renderer, &rects[2]);

}

void aff_inv_graf(SDL_Renderer* pRenderer, int scrool_pos ,int cof_scrollbar_window, int nb_collone , int nb_ligne ,int taille, int decalage, int arrondis){
    int x = 10 , y = 10 - scrool_pos * cof_scrollbar_window ;
    int widthscreen = 800 ; //800
    int heightscreen = 500 ; //500

    for (int h = 0 , deb = x ; h < nb_ligne ; h ++){
        for ( int i = 0 ; i < nb_collone ; i++ ){
            rectangle_arrondis(pRenderer, x, y,taille, taille, arrondis,128,128,128);
            x += taille + decalage ;
        }
        if ( h%2 == 0) rectangle_arrondis(pRenderer, x, y,380, taille*2, arrondis,128,128,128); // heros
        x = deb ;
        y += taille + decalage ;
    }
    
}

void draw_scrollbar(SDL_Renderer* renderer, int scrollbar_position, int scrollbar_height) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect scrollbar = {780, scrollbar_position, 20, scrollbar_height};
    SDL_RenderFillRect(renderer, &scrollbar);
}

int calculate_total_content_height(int nb_ligne, int taille, int decalage) {
    return ((nb_ligne) * (taille + decalage));
}

int calculate_scrollbar_height(int visible_area_height, int total_content_height) {
    float scroll_proportion = (float)visible_area_height / total_content_height;
    return (int)(scroll_proportion * visible_area_height);
}

int cof_scrollbar_window(int visible_area_height, int total_content_height){
    return (int)(total_content_height/visible_area_height);
}

int calculate_scrollbar_max_position(int screen_height, int scrollbar_heigt) {
    return screen_height - scrollbar_heigt;
}

void handle_scroll_event(SDL_Event event, int* scrollbar_position,int scrollbar_speed, int scrollbar_max_position) {
    if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
            *scrollbar_position -= scrollbar_speed;
        } else if (event.wheel.y < 0) {
            *scrollbar_position += scrollbar_speed;
        }
        if (*scrollbar_position < 0) {
            *scrollbar_position = 0;
        } else if (*scrollbar_position > scrollbar_max_position) {
            *scrollbar_position = scrollbar_max_position;
        }
    }
}
