
/**
 * \file inv.c
 * \brief Implémentation des fonctions de gestion de l'inventaire.
 */




//liste de heros global / struct heros avec liste d'item

//faire une fonction raffraichissmeent qui rafrechis que si action 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../lib/inv.h"
#include "../lib/sdl.h"
#include "../lib/input_user.h"
#include "../lib/heros.h"
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/aff.h"
#include "../lib/lang.h"
#include "../lib/chaine.h"

const int proportion_item_carrer_prio = 0 ;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 500;
const int SCROLLBAR_WIDTH = 20;
const int VISIBLE_AREA_HEIGHT = 500;
const int NB_COLLONE = 4;
const int NB_LIGNE = 25;
int ITEM_SIZE = 64;
const int DEPLACEMENT = 20;
const int ARRONDIS = 20;

#define LABEL_HEROS "zone_interaction_scroll" 



void draw_rect(SDL_Rect rect) {
    SDL_Color color = {255,0,0,255};
    // Définir la couleur du rendu
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Dessiner le contour du rectangle
    SDL_RenderDrawRect(renderer, &rect);

    // Réinitialiser la couleur du rendu si nécessaire (ex: en blanc)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

liste_inventaires * list_inv = NULL ;
int conteur_item_ref_img = 0  ;

//test item 
int existe_item(item_t * it){
    return (it != NULL && it -> nom_fichier != NULL && it -> nom != NULL && it -> label != NULL);
}

int comparer_items(const void *a, const void *b) {
    item_t *itemA = *(item_t **)a;
    item_t *itemB = *(item_t **)b;

    // Check if any of the items or their 'nom' fields are NULL
    if (!itemA || !itemA->nom) return itemB && itemB->nom ? -1 : 0;
    if (!itemB || !itemB->nom) return 1;

    return strcmp(itemA->nom, itemB->nom);
}

int droit_fusion(item_t * item1, item_t * item2){
    if (item1 == NULL || item2 == NULL) return -1; // item non recreer probleme code
    if (item1->nom_fichier == NULL || item2->nom_fichier == NULL) return -2; // item non remplis
    if (strcmp(item1->nom_fichier, item2->nom_fichier) != 0) return -4; // pas le même item
    if (item1->rarity != item2->rarity) return -5; // pas la même rareté
    return 1; // fusion autorisée
}

//allocation et destruction item / inv 
item_t * generation_item(char * name , char * nom_fichier ){
    item_t * it = malloc(sizeof(item_t));
    it->nom = strdup(name);
    it->nom_fichier = strdup(nom_fichier);
    return it ;
}

void dest_item(item_t ** item){
    if ( *item == NULL) return;
    if ((*item)->nom != NULL) {
        free((*item)->nom);
        (*item)->nom = NULL;
    }
    if ((*item)->nom_fichier != NULL) {
        free((*item)->nom_fichier);
        (*item)->nom_fichier = NULL;
    }
    if ((*item)->label != NULL) {
        free((*item)->label);
        (*item)->label = NULL;
    }
    free(*item);
    *item = NULL;
}

void dest_inv(inv **inventaire) {
    if (*inventaire == NULL)
        return;

    if ((*inventaire)->liste != NULL) {
        for (int i = 0; i < (*inventaire)->nb_items; i++) {
            dest_item(&(*inventaire)->liste[i]); 
        }
        free((*inventaire)->liste);
        (*inventaire)->liste = NULL ;
    }
    free(*inventaire);
    *inventaire = NULL;
}



void init_inv(inv *inventaire){
    for (int i = 0; i < inventaire->nb_items; i++) {
        item_t * it = malloc(sizeof(item_t));
        it->nom_fichier = NULL;
        it->nom = NULL;
        it->label = NULL;
        it->rarity = 0;
        it->stat = 0;
        it->piece_equipement = 0;
        it->boost = 0;
        inventaire -> liste[i] = it ;
    }
}

int chercher_nom_fichier(char * nom_fichier , inv * inventaire){
    if ( inventaire ==NULL)return -1 ;
    for ( int i = 0 ; i < inventaire ->nb_items ; i++){
        if ( existe_item(inventaire->liste[i]) && nom_fichier && inventaire->liste[i]->nom_fichier)
            if (strcmp ( nom_fichier , inventaire->liste[i]->nom_fichier))return i ;
    } 
    return -1 ;
}

// insertion des information
void insert_inf_aff(inv * inventaire,SDL_Rect SDL_Rect ,int nb_collone ,int nb_ligne ,int nb_element ,
    int arrondis ,int decalage_bas ,int decalage_cote ){
    inventaire->info_inv.SDL_Rect = SDL_Rect ;
    inventaire->info_inv.nb_collone = nb_collone ;
    inventaire->info_inv.nb_ligne = nb_ligne ;
    inventaire->info_inv.nb_element = nb_element ;
    inventaire->info_inv.arrondis = arrondis ;
    inventaire->info_inv.decalage_bas = decalage_bas ;
    inventaire->info_inv.decalage_cote = decalage_cote ;
}

//recherche 

int prem_vide(inv * inventaire){
    if (inventaire == NULL )return - 1 ;
    for( int i = 0 ; i < NB_EQU ; i++)
        if (!existe_item(inventaire->liste[i])) return i;
    return -1;
}

int chercher_select(){
    if ( !list_inv )return -1 ;
    int id_scroll_select = select_quelle_scroll(3) ;
    for ( int i = 0 ;  i < list_inv->nb_inventaires ; i++)
        if (list_inv->inventaires[i]->id_scroll == id_scroll_select)return i ;
    return -1 ;
}




//calcule 

int calculate_total_content_size(int nb_ligne, int taille, int decalage) {
    if (nb_ligne == 1) 
        return taille;
    return (nb_ligne * taille) + ((nb_ligne - 1) * decalage);
}

// Fonction pour calculer le numéro de case en fonction des coordonnées
int calcule_pos_inv(item_t ** it) {
    int id_inv = chercher_select();
    if (id_inv == -1 || !list_inv || id_inv >= list_inv->nb_inventaires) 
        return -1; 

    inv *inventaire = list_inv->inventaires[id_inv];

    if (!inventaire) 
        return -1;
        
    SDL_Point start = {inventaire->info_inv.SDL_Rect.x,inventaire->info_inv.SDL_Rect.y};
    int hauteur_elem =  inventaire->info_inv.decalage_bas + inventaire->info_inv.SDL_Rect.h ;
    int largeur_elem = inventaire->info_inv.decalage_cote + inventaire->info_inv.SDL_Rect.w ;
    int nb_colonnes = inventaire->info_inv.nb_collone;
    int nb_lignes = inventaire->info_inv.nb_ligne;
    int items_par_page = inventaire->info_inv.nb_element;

    int pos_y = mouseY + scroll_liste[inventaire->id_scroll].scroll_pos - start.y;
    int h = pos_y / hauteur_elem ;
    int w = (mouseX - start.x)/largeur_elem ;
    int elem_numb = h * nb_colonnes + w ;
    if (( w >= nb_colonnes || h >= nb_lignes) && ( items_par_page < elem_numb))
        return -1;
    if (it != NULL ) *it = inventaire->liste[elem_numb];
    return elem_numb;
}

//gestion item 

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
        item1->stat = generate_stat_nb(1, item1->boost, item1->stat, item1->rarity, 1);
        dest_item(&item2);
    }
}


void generation_label(char ** label ){
    conteur_item_ref_img++;
    char label_temp[50] ;
    sprintf(label_temp,"item %d",conteur_item_ref_img);
    *label = malloc ( strlen(label_temp)+1);
    strcpy(*label,label_temp);
}

char * item_rondam(int * piece_equipement){
    int liste[] = {NB_CASQUE, NB_PLASTRON, NB_BOTTES, NB_ARME};
    int nb_item = NB_CASQUE + NB_PLASTRON + NB_BOTTES + NB_ARME ;
    
    int id_item = rand() % nb_item + 1; // Générer un ID valide (de 0 à nb_item - 1)
    int i = 0;
    // Trouver à quelle catégorie appartient l'item
    for (; i < 4 && id_item > liste[i]; i++) {
        id_item -= liste[i] ; // Ajuster id_item
    }
    *piece_equipement = i ;
    char * mot = malloc ( strlen("999.png") + 1 ) ;
    sprintf(mot,"%d.png",id_item);
    const char *prefix;

    switch(i){
        case CASQUE : 
            prefix = "casque-";
            break ;
        case PLASTRON : 
            prefix = "plastron-";
            break ;
        case BOTTE :
            prefix = "botte-";
            break ;

        case ARME :
            prefix = "arme-";
            break ;
    }

    char * type = malloc( strlen(prefix) + strlen(mot) + 1);
    strcpy(type,prefix);
    strcat(type,mot);
    free(mot);
    return type ;
}


void drop_item(){

    if (rand() % 100 + 1 > SDL_min(DROP_ITEM * level.mobKilled, 100)) return;
    int emplacement_vide = prem_vide(list_inv->inventaires[0]);
    if (emplacement_vide == -1){
        //notif inv full
        return ;
    }
    int type_stat = rand() % 3 + 1 ;
    int stat = generate_stat_nb(25,type_stat,1,0,70);// a remplacer 1 et dernier stat 
    
    int piece_equipement ;
    char * nom_path_icon = item_rondam(&piece_equipement);
    int id_ref = chercher_nom_fichier(nom_path_icon,list_inv->inventaires[1]);
    if ( id_ref == -1){
        return ;
    }
    char * nom = list_inv->inventaires[1]->liste[id_ref]->nom ;

    item_t * it = generation_item(nom,nom_path_icon);
    it->stat = stat ;
    it -> boost = type_stat ;
    it->piece_equipement = piece_equipement ;
    it->rarity = COMMUN ;
    generation_label(&(it -> label));
    list_inv->inventaires[0]->liste[emplacement_vide] = it;

    aff_inv_inf * raccoursis = &(list_inv->inventaires[0]->info_inv );
    int nb_colone = raccoursis->nb_collone;
    int emp_ligne = emplacement_vide/nb_colone ;
    int emp_colone = emplacement_vide % nb_colone ;
    int x = emp_colone * (raccoursis->decalage_cote + raccoursis->SDL_Rect.w)  + raccoursis->SDL_Rect.x ;
    int y = emp_ligne * (raccoursis->decalage_bas + raccoursis->SDL_Rect.h)  + raccoursis->SDL_Rect.y ;
    it->pos_y = y ;

    racourcis_createUI(it,x,y,0);
}
//gestion inv 


void dest_all_inventaires() {
    if(list_inv == NULL) return ;
    for (int i = 0; i < list_inv->nb_inventaires; i++) {
        dest_inv(&(list_inv->inventaires[i]));  // Appel à dest_inv pour chaque inventaire
    }
    free(list_inv->inventaires);  // Libération du tableau d'inventaires
    list_inv->inventaires = NULL;
    list_inv->nb_inventaires = 0;
    free(list_inv);
    list_inv = NULL ;
}


void ajouter_inventaire(int nb_inventaires, ...) {
    va_list args;
    va_start(args, nb_inventaires);
    list_inv->inventaires = realloc(list_inv->inventaires, (list_inv->nb_inventaires + nb_inventaires) * sizeof(inv *));
    for (int i = 0; i < nb_inventaires; i++) {

        inv *inventaire = va_arg(args, inv *);  // Récupère l'inventaire passé en paramètre
        list_inv->inventaires[list_inv->nb_inventaires++] = inventaire;
    }

    va_end(args);
}

void init_liste_inventaires(){
    list_inv = malloc (sizeof(liste_inventaires));
    list_inv->inventaires = malloc(sizeof(inv *));
    list_inv->nb_inventaires = 0 ;
    load();
}



void gestion_inv(inv **inventaire, int NB_items, int id_scroll, SDL_Rect SDL_Rect, int nb_colonne, int nb_ligne, int arrondis, 
    int decalage_bas, int decalage_cote , int id_back_item) {
            if (*inventaire == NULL) {
                // Allocation de la structure de l'inventaire
                *inventaire = malloc(sizeof(inv));
                if (*inventaire == NULL) {
                printf("Erreur d'allocation mémoire pour l'inventaire\n");
                return;
            }
    
            int total_content_height = calculate_total_content_size(nb_ligne, SDL_Rect.h, decalage_bas);
            int total_content_width = calculate_total_content_size(nb_colonne, SDL_Rect.w, decalage_cote);
    
            scroll_liste[id_scroll].total_content_height = total_content_height ;
            scroll_liste[id_scroll].zone_interaction.w = total_content_width ;
            update_scroll(&id_scroll);
     
    
            // Initialisation des données de l'inventaire
            (*inventaire)->id_scroll = id_scroll;
            SDL_Rect.x += scroll_liste[id_scroll].zone_interaction.x;
            SDL_Rect.y += scroll_liste[id_scroll].zone_interaction.y;
            insert_inf_aff(*inventaire, SDL_Rect, nb_colonne, nb_ligne, NB_items, arrondis, decalage_bas, decalage_cote);
            //malloc un tableaux de pointeur sur item 
            (*inventaire)->liste = malloc(NB_items * sizeof(item_t *));
            (*inventaire)->nb_items = NB_items;  // Stocke le nombre d'items
    
            
            (*inventaire)->id_tuile = id_back_item;
            init_inv(*inventaire);
        }
}

void trier_inventaire(inv *inventaire) {
    if (!inventaire || !inventaire->liste || inventaire->nb_items <= 0) {
        printf("Erreur : Inventaire vide ou invalide.\n");
        return;
    }

    qsort(inventaire->liste, inventaire->nb_items, sizeof(item_t *), comparer_items);
}

//affichage graffique pixel 

void cercle_graf(int x, int y, int rayon) {
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
void rectangle_arrondis(SDL_Rect Rect, int radius , int r , int g , int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Dessiner les coin arrondi  / dessine des cercle pour les couper pour avoir le coin
    cercle_graf(Rect.x + radius, Rect.y + radius, radius);// haut / gauche
    cercle_graf(Rect.x + Rect.w - radius, Rect.y + radius, radius);// haut / droit
    cercle_graf(Rect.x + radius, Rect.y + Rect.h - radius, radius);// bas / gauche
    cercle_graf(Rect.x + Rect.w - radius, Rect.y + Rect.h - radius, radius) ;// bas / droit

    //dessine le corps
    SDL_Rect rects[] = {
        {Rect.x + radius, Rect.y, Rect.w - 2 * radius, radius}, // haut
        {Rect.x + radius, Rect.y + Rect.h - radius, Rect.w - 2 * radius, radius}, // bas
        {Rect.x,Rect. y + radius, Rect.w   , Rect.h - 2 * radius}//milieu
    };
    SDL_RenderFillRect(renderer, &rects[0]);
    SDL_RenderFillRect(renderer, &rects[1]);
    SDL_RenderFillRect(renderer, &rects[2]);

}

//init aff 


void racourcis_createUI(item_t * it,int x,int y,int id_tuille){
    char temp[70] , label[100];
    sprintf(temp,"assets/ui/background/tile_item_%d.png",id_tuille);
    sprintf(label,"tile_item_%d.png/%s",id_tuille,it->label);

    createUIImg(&pageHolder.page[4], temp, (SDL_Rect){x-DECALAGE, y - DECALAGE, TAILLE_ITEM + 2 * DECALAGE, TAILLE_ITEM + 2 * DECALAGE}, label);

    char *chemin = malloc(strlen("assets/ui/icons/items/") + strlen(it->nom_fichier) + 1);
    strcpy(chemin, "assets/ui/icons/items/");
    strcat(chemin, it->nom_fichier);
    createUIImg(&pageHolder.page[4], chemin, (SDL_Rect){x, y, TAILLE_ITEM, TAILLE_ITEM}, it->label);


    free(chemin);
}


void aff_inv_graf(inv* inventaire) {
    int id_scroll = inventaire->id_scroll;
    int x = inventaire->info_inv.SDL_Rect.x;
    int y = inventaire->info_inv.SDL_Rect.y;
    int conteur = 0 ;
    y -= scroll_liste[id_scroll].scroll_pos;
    for (int h = 0; h < inventaire->info_inv.nb_ligne && conteur < inventaire->nb_items; h++) {
        for (int i = 0; i < inventaire->info_inv.nb_collone && conteur < inventaire->nb_items ; i++ , conteur ++ ) {
            if (existe_item(inventaire->liste[conteur])){
                racourcis_createUI(inventaire->liste[conteur], x, y,inventaire->id_tuile);
                inventaire->liste[conteur]->pos_y = y ;
                x += inventaire->info_inv.SDL_Rect.w + inventaire->info_inv.decalage_cote;  // Mise à jour correcte de la position horizontale
            }
            if ( x == inventaire->info_inv.SDL_Rect.x + inventaire->info_inv.nb_collone * (inventaire->info_inv.SDL_Rect.w + inventaire->info_inv.decalage_cote)){
                x = inventaire->info_inv.SDL_Rect.x; // Réinitialisation de x pour une nouvelle ligne
                y += inventaire->info_inv.SDL_Rect.h + inventaire->info_inv.decalage_bas;  // Mise à jour de la position verticale
            }
        }
    }
}


void aff_heros(){
    int taille_hauteur = list_inv->inventaires[2]->info_inv.SDL_Rect.h + DECALAGE;
    int taille_largeur = list_inv->inventaires[2]->info_inv.SDL_Rect.w ;
    for (int heroIndex = 0; heroIndex < HEROS_COUNT; heroIndex++){
        char imgHero[50],heroLabel[15];
        sprintf(imgHero, "assets/ui/heros/hero%d.png", heroIndex);
        sprintf(heroLabel, "hero%d", heroIndex);
        createUIImg(&pageHolder.page[4], imgHero, getRectForCentenredCord(vw(POS_HEROS_X),  heroIndex* taille_hauteur + DECALAGE, taille_largeur, taille_hauteur ), heroLabel);
    }
    char * label = LABEL_HEROS;
    creation_scroll((SDL_Rect){vw(POS_HEROS_X)-taille_largeur/2, BORDURE/2, taille_largeur,WINDOW_HEIGHT -BORDURE}, 
                                     (SDL_Rect){vw(POS_HEROS_X)+1, 1, -1, -1},
                                     0, 0,NULL,&label); //scroll bidon pour juste utiliser la zone d'interaction 
} 



void aff_background_fond(){
    char chemin[]= "assets/ui/background/cobble.png";
    createUIImg(&pageHolder.page[4], chemin, (SDL_Rect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, "/global_inv/background_inv_fond");
}

void aff_background_bordure(){
    char chemin[]= "assets/ui/background/bordure.png";
    createUIImg(&pageHolder.page[4], chemin, (SDL_Rect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, "/global_inv/background_inv_bordure");
}




void aff_all_inventaires() {
    aff_background_fond();
    for (int i = 0; i < list_inv->nb_inventaires; i++) {
        if ( i != 1  ){
            aff_inv_graf(list_inv->inventaires[i]);  // Appel à aff_inv_graf pour chaque inventaire
        }
    }
    aff_heros();
    aff_scrollbar_simple(NULL, NULL);
    aff_background_bordure();

}



//actualisation 
void img_inv_actualliser(inv * inventaire){
    if ( inventaire == NULL)return ;
    for ( int i = 0 ; i < inventaire->nb_items ; i++){
        if (existe_item(inventaire->liste[i]) ){
            char temp[100];
            item_t * it = inventaire->liste[i] ;
            uiImg * path_img = getImgFromLabel(it->label) ;
            path_img -> dest . y  = it->pos_y - scroll_liste[inventaire->id_scroll].scroll_pos ;

            sprintf(temp,"tile_item_%d.png/%s",inventaire->id_tuile,it->label);
            uiImg * path_img_back = getImgFromLabel(temp) ;
            path_img_back -> dest . y  = path_img -> dest . y - DECALAGE;
            path_img_back -> dest . x  = path_img -> dest . x - DECALAGE;
            
        }
    }
}
#define DECALE_HEROS 20


void actualiser_heros(){
    for (int heroIndex = 0; heroIndex < HEROS_COUNT; heroIndex++){
        char heroLabel[15];
        sprintf(heroLabel, "hero%d", heroIndex);
        uiImg * path_img = getImgFromLabel(heroLabel) ;
        path_img -> dest . y  =  heroIndex * (path_img->dest.h + DECALE_HEROS)  +  BORDURE - scroll_liste[1].scroll_pos ;
    }
}

int click_heros_pos(){
    if ( HEROS_COUNT == 0 ) return -1 ;
    char heroLabel[15];
    sprintf(heroLabel, "hero%d", HEROS_COUNT-1);
    uiImg * path_img = getImgFromLabel("hero0") ;
    uiImg * path_img_max = getImgFromLabel(heroLabel) ;

    int taille = path_img -> dest . h ;
    int total_hauteur  = path_img_max -> dest . y - path_img -> dest . y ; 
    int id_case = ( mouseY - BORDURE - (path_img -> dest . y - BORDURE ) ) / (taille + DECALE_HEROS)    ;
    printf("\n --------------\n");
    printf("taille:%d , total_hauteur:%d , decalage:%d , id_case:%d %d",taille,total_hauteur,DECALE_HEROS,id_case , path_img -> dest . y);
    printf("\n --------------\n");
    return id_case ;

}

void img_inv_actualliser_all(){
    for (int i = 0; i < list_inv->nb_inventaires; i++) {
        if ( i != 1  ){
            if ( list_inv->inventaires[i] )
                img_inv_actualliser(list_inv->inventaires[i]);  // Appel à aff_inv_graf pour chaque inventaire
        }
    }
    actualiser_heros();
}

//a trier





//aff item 
void aff_item(item_t * it){
    if ( !existe_item(it) )return ;
    printf("item nom : %s\n",it->nom);
    printf("item label : %s\n",it->label);
    printf("item nom_fichier : %s\n",it->nom_fichier);
    printf("item piece_equipement : %d\n",it->piece_equipement);
    printf("item stat_a_booster : %d\n",it->boost);
    printf("item rareter : %d\n",it->rarity);
    printf("item rareter : %d\n",it->stat);
}

void aff_stat_item(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
        int clicked_case = calcule_pos_inv(NULL);
        int inv_select = chercher_select();
        if (inv_select == -1 || clicked_case == -1 )return ;
        item_t * it = list_inv -> inventaires[inv_select] -> liste [clicked_case] ;
        aff_item(it);

    }
}



// drag and drop 

typedef struct {
    int actif;          // Indique si le drag est actif
    int index;          // Index de l'élément dans l'inventaire source
    int inv_id;         // ID de l'inventaire source
    int target_inv_id;  // ID de l'inventaire cible
} drag_t;
drag_t drag = {0, -1, -1, 0};

// Détection du clic sur un item
void detecter_click_item() {
    item_t * it ;
    int clicked_case = calcule_pos_inv(&it);  // Trouve la case cliquée

    if (clicked_case != -1 && existe_item(it)) {
        drag.actif = 1;
        drag.index = clicked_case;
        drag.inv_id = chercher_select();
        printf("Drag activé : index=%d, inv_id=%d\n", drag.index, drag.inv_id);
    }
}



void lacher_item() {
    if (!drag.actif)
        return;  // Vérifier si le drag est actif

    int case_finale = calcule_pos_inv(NULL);  // Calculer la case cible
    if (case_finale != -1) {

        drag.target_inv_id = chercher_select();
        printf("case finale : %d scol_id : %d \n",case_finale,drag.target_inv_id);
        
        // Vérifier si l'inventaire source et cible sont valides
        if (drag.inv_id < 0 || drag.inv_id >= list_inv->nb_inventaires) {
            printf("Erreur : inv_id invalide : %d\n", drag.inv_id);
            return;
        }
        
        if (drag.target_inv_id < 0 || drag.target_inv_id >= list_inv->nb_inventaires) {
            printf("Erreur : target_inv_id invalide : %d\n", drag.target_inv_id);
            return;
        }
        
        inv *source_inv = list_inv->inventaires[drag.inv_id];  // Inventaire source
        inv *target_inv = list_inv->inventaires[drag.target_inv_id];  // Inventaire cible

        // Vérifier si les inventaires sont valides
        if (source_inv == NULL || target_inv == NULL) {
            printf("Erreur : Inventaire source ou cible NULL.\n");
            return;
        }

        // Vérification si l'index de drag est valide pour l'inventaire source
        if (drag.index >= 0 && drag.index < source_inv->nb_items) {
            // Si la case cible est occupée
            if (existe_item(target_inv->liste[case_finale])) {
                item_t *temp = target_inv->liste[case_finale];
                target_inv->liste[case_finale] = source_inv->liste[drag.index];
                source_inv->liste[drag.index] = temp;

                // Calcul des positions
                aff_inv_inf *raccoursis_case_final = &(target_inv->info_inv);  // Raccourci pour les informations de la case finale
                if (raccoursis_case_final == NULL) {
                    printf("Erreur : raccoursis_case_final NULL\n");
                    return;
                }

                int nb_colone_final = raccoursis_case_final->nb_collone;
                if (nb_colone_final <= 0) {
                    printf("Erreur : nombre de colonnes invalide : %d\n", nb_colone_final);
                    return;
                }

                aff_inv_inf *raccoursis_drag = &(source_inv->info_inv);  // Raccourci pour les informations de l'inventaire source
                if (raccoursis_drag == NULL) {
                    printf("Erreur : raccoursis_drag NULL\n");
                    return;
                }

                int nb_colone_drag = raccoursis_drag->nb_collone;
                if (nb_colone_drag <= 0) {
                    printf("Erreur : nombre de colonnes invalide pour drag : %d\n", nb_colone_drag);
                    return;
                }

                // Calcul des positions
                int emp_ligne_case_finale = case_finale / nb_colone_final;
                int emp_colone_case_finale = case_finale % nb_colone_final;
                int x_case_finale = emp_colone_case_finale * (raccoursis_case_final->decalage_cote + raccoursis_case_final->SDL_Rect.w) + raccoursis_case_final->SDL_Rect.x;
                int y_case_finale = emp_ligne_case_finale * (raccoursis_case_final->decalage_bas + raccoursis_case_final->SDL_Rect.h) + raccoursis_case_final->SDL_Rect.y;

                int emp_ligne_drag = drag.index / nb_colone_drag;
                int emp_colone_drag = drag.index % nb_colone_drag;
                int x_drag = emp_colone_drag * (raccoursis_drag->decalage_cote + raccoursis_drag->SDL_Rect.w) + raccoursis_drag->SDL_Rect.x;
                int y_drag = emp_ligne_drag * (raccoursis_drag->decalage_bas + raccoursis_drag->SDL_Rect.h) + raccoursis_drag->SDL_Rect.y;

                printf("x:%d %d %d %d %d\n",x_drag ,emp_colone_drag,raccoursis_drag->decalage_cote,raccoursis_drag->SDL_Rect.w,raccoursis_drag->SDL_Rect.x);

                // Mise à jour des positions X et Y des éléments déplacés
                if (target_inv->liste[case_finale] != NULL) {
                    target_inv->liste[case_finale]->pos_y = y_case_finale;
                }
                if (source_inv->liste[drag.index] != NULL) {
                    source_inv->liste[drag.index]->pos_y = y_drag;
                }

                // Mise à jour des images des éléments déplacés
                uiImg *path_img_case_finale = NULL;
                uiImg *path_img_drag = NULL;

                if (existe_item(target_inv->liste[case_finale])) {
                    path_img_case_finale = getImgFromLabel(target_inv->liste[case_finale]->label);
                }

                if (existe_item(source_inv->liste[drag.index])) {
                    path_img_drag = getImgFromLabel(source_inv->liste[drag.index]->label);
                }

                if (path_img_case_finale != NULL) {
                    path_img_case_finale->dest.x = x_case_finale;
                    path_img_case_finale->dest.y = y_case_finale;
                }
                if (path_img_drag != NULL) {
                    path_img_drag->dest.x = x_drag;
                    path_img_drag->dest.y = y_drag;
                }

            } else {
                // Déplacer l'item dans une case vide dans l'inventaire cible
                target_inv->liste[case_finale] = source_inv->liste[drag.index];
                source_inv->liste[drag.index] = NULL;

                // Recalculer la position de l'élément déplacé
                aff_inv_inf *raccoursis = &(target_inv->info_inv);
                if (raccoursis == NULL) {
                    printf("Erreur : raccoursis NULL pour target_inv\n");
                    return;
                }

                int nb_colone = raccoursis->nb_collone;
                if (nb_colone <= 0) {
                    printf("Erreur : nombre de colonnes invalide pour target_inv : %d\n", nb_colone);
                    return;
                }

                int emp_ligne_case_finale = case_finale / nb_colone;
                int emp_colone_case_finale = case_finale % nb_colone;
                int x_case_finale = emp_colone_case_finale * (raccoursis->decalage_cote + raccoursis->SDL_Rect.w) + raccoursis->SDL_Rect.x;
                int y_case_finale = emp_ligne_case_finale * (raccoursis->decalage_bas + raccoursis->SDL_Rect.h) + raccoursis->SDL_Rect.y;

                // Mise à jour de la position Y de l'élément déplacé
                if (target_inv->liste[case_finale] != NULL) {
                    target_inv->liste[case_finale]->pos_y = y_case_finale;
                }

                // Mise à jour de la position X de l'élément déplacé
                uiImg *path_img_case_finale = NULL;
                if (existe_item(target_inv->liste[case_finale])) {
                    path_img_case_finale = getImgFromLabel(target_inv->liste[case_finale]->label);
                }

                if (path_img_case_finale != NULL) {
                    path_img_case_finale->dest.x = x_case_finale;
                    path_img_case_finale->dest.y = y_case_finale;
                }

            }

            // Réinitialiser l'état du drag
            drag.actif = 0;
            drag.index = -1;
            drag.target_inv_id = -1;  // Réinitialiser l'inventaire cible après le déplacement
        } else {
            printf("Erreur : index de drag invalide : %d\n", drag.index);
        }
    }
}


//load and save 


extern void load(){
    inv *inventaire_item = NULL;
    inv *inv_ref = NULL;
    inv *inventaire_heros = NULL;

    //creation zone interaction extention : scroll
    int id_scroll1 = creation_scroll((SDL_Rect){70, 64, 0, VISIBLE_AREA_HEIGHT},
                                     (SDL_Rect){0, 0, SCROLLBAR_WIDTH, VISIBLE_AREA_HEIGHT - 100},
                                     0, 30,NULL,NULL);

    int id_scroll2 = creation_scroll((SDL_Rect){WINDOW_WIDTH/2, 64,WINDOW_WIDTH/2, VISIBLE_AREA_HEIGHT},
                                     (SDL_Rect){WINDOW_WIDTH-20, 0, SCROLLBAR_WIDTH, VISIBLE_AREA_HEIGHT - 10},
                                     0, 10,NULL,NULL);

    int id_NULL = creation_scroll((SDL_Rect){-10, -10, 0, 0}, 
                                     (SDL_Rect){-9, -9, -1, -1},
                                     0, 0,NULL,NULL); // scroll bidon pour scoquer les item  qui ne seront pas afficher 


    //creation et association des zone d'interaction scroll aux inventaire 
    gestion_inv(&inventaire_item,100, id_scroll1, (SDL_Rect){DECALAGE, DECALAGE, ITEM_SIZE, ITEM_SIZE},
    3, NB_LIGNE , ARRONDIS, DEPLACEMENT, DEPLACEMENT,0);

    int total_item = NB_BOTTES + NB_PLASTRON + NB_CASQUE + NB_ARME ;
    gestion_inv(&inv_ref,total_item, id_NULL, (SDL_Rect){0, 0,0 , 0},
    0, 0 , 0, 0, 0, 0);

    int decalage_interne_item_background_item = DECALAGE * 2 ;
    gestion_inv(&inventaire_heros,HEROS_COUNT * NB_AMPLECEMENT, id_scroll2, (SDL_Rect){DECALAGE, 10,ITEM_SIZE, ITEM_SIZE},
    NB_AMPLECEMENT, HEROS_COUNT , ARRONDIS, decalage_interne_item_background_item + DECALAGE, 20,0);

    ajouter_inventaire(3 , inventaire_item , inv_ref ,inventaire_heros);

}



// booste hero 
int stat_item_degat ;
int stat_item_temps ;

//diferent de chaque heros
void gestion_stat_heros(int index_heros){
    stat_item_degat = 0 ;
    stat_item_temps = 0 ;
    inv * inv_heros = list_inv->inventaires[2];
    for ( int i = 0 ; i < NB_AMPLECEMENT ; i++){
        if (existe_item(inv_heros->liste[index_heros * NB_AMPLECEMENT + i ])){
            
            int stat = inv_heros->liste[index_heros * NB_AMPLECEMENT + i ]->stat;
            int type = inv_heros->liste[index_heros * NB_AMPLECEMENT + i ]->boost;
            switch (type){
            case DEGAT:
                stat_item_degat += stat;
                break;
            case VITESSE_ATTAQUE:
                stat_item_temps += stat;
                break;
            default:
                break;
            }
        }  

    }
    stat_item_degat = (1+stat_item_degat/100);//en pourcentage 
    stat_item_temps = (1+stat_item_temps/100);
}

//commun a tous les heros
int boost_gold(){
    int boost_or_drop = 0 ;
    inv * inv_heros = list_inv->inventaires[2];
    for ( int i = 0 ; i < NB_AMPLECEMENT * HEROS_COUNT ; i++){
        if (existe_item(inv_heros->liste[i])){
            int type = inv_heros->liste[i]->boost;
            if ( type == OR){
                boost_or_drop += inv_heros->liste[i]->stat;
            }
        }  
    }
    return (1+boost_or_drop/100) ;
}


// load et save 

item_t * load_item(FILE *f, int type ) { // type = 0 : ref-items ou type = 1 : inv joueur
    item_t *it = malloc(sizeof(item_t));
    char ligne[MAX_LIGNE];
    char temp_name[256] = {0};
    char temp_nom_fichier[256] = {0};

    while (fgets(ligne, MAX_LIGNE, f)) {
        if (strstr(ligne, "}")) break;
        if (strstr(ligne, "\"nom\"")) {
            sscanf(ligne, " \"nom\": \"%255[^\"]\"", temp_name);
        } else if (strstr(ligne, "\"nom_fichier\"")) {
            sscanf(ligne, " \"nom_fichier\": \"%255[^\"]\"", temp_nom_fichier);
        }
        
        if (type) {  // Charger tous les champs si nécessaire
            if (strstr(ligne, "\"piece_equipement\"")) {
                sscanf(ligne, " \"piece_equipement\": %d", (int *)&it->piece_equipement);
            } else if (strstr(ligne, "\"boost\"")) {
                sscanf(ligne, " \"boost\": %d", (int *)&it->boost);
            } else if (strstr(ligne, "\"rarity\"")) {
                sscanf(ligne, " \"rarity\": %d",(int *)&it->rarity);
            } else if (strstr(ligne, "\"stat\"")) {
                sscanf(ligne, " \"stat\": %d", &it->stat);
            }
        }
    } 

    if (temp_name[0] != '\0') {
        it->nom = strdup(temp_name);
    }
    if (temp_nom_fichier[0] != '\0') {
        it->nom_fichier = strdup(temp_nom_fichier);
    }

    if (it->nom && it->nom_fichier) 
        generation_label(&(it->label)); 
    else it->label = strdup("default_label");
    it->nom = strdup(temp_name);
    it->nom_fichier = strdup(temp_nom_fichier);

    return it;
}

void load_inv(const char *nom_fichier) {
    if ( nom_fichier == NULL )return ;
    int id = strcmp("save/item_inv.json",nom_fichier) == 0 ? 1 : 0 ;
    inv * inventaire = list_inv->inventaires[!id];
    FILE *f = fopen(nom_fichier, "r");
    if (!f) return;

    char ligne[MAX_LIGNE];
    int compteur = 0 ;
    while (fgets(ligne, MAX_LIGNE, f) && inventaire->nb_items > compteur) {
        if (strstr(ligne, "{")) {  // Début d'un item
            
            inventaire->liste[compteur] = load_item(f, id); // Chargement complet
            if (inventaire->liste[compteur]) {
                compteur++;
            }
        }
    }

    fclose(f);
}



void save_inv(inv *inventaire, char *save) {
    FILE *f = fopen(save, "w");
    if (!f) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier pour l'enregistrement: %s\n", save);
        return;
    }

    // Démarrer l'écriture du fichier
    fprintf(f, "{\n");
    fprintf(f, " \"inventaire\": [\n");

    // Parcourir chaque item dans l'inventaire et le sauvegarder
    for (int i = 0; i < inventaire->nb_items; i++) {
        item_t *it = inventaire->liste[i];

        if (i > 0) {
            fprintf(f, ",\n");
        }

        fprintf(f, "  {\n");

        if (it->nom) {
            fprintf(f, "   \"nom\": \"%s\",\n", it->nom);
        }
        if (it->nom_fichier) {
            fprintf(f, "   \"nom_fichier\": \"%s\",\n", it->nom_fichier);
        }

        fprintf(f, "   \"piece_equipement\": %d,\n", it->piece_equipement);
        fprintf(f, "   \"boost\": %d,\n", it->boost);
        fprintf(f, "   \"rarity\": %d,\n", it->rarity);
        fprintf(f, "   \"stat\": %d", it->stat);

        // Sauvegarder le label si il existe
        if (it->label) {
            fprintf(f, ",\n   \"label\": \"%s\"\n", it->label);
        }

        fprintf(f, "  }");
    }

    // Fin de l'écriture
    fprintf(f, "\n ]\n");
    fprintf(f, "}\n");

    fclose(f);
}

void init_inv_main(){
    init_liste_inventaires();
    load_inv("save/item_inv.json");
    load_inv("save/item_ref.json");
}






// interaction inv 

void handle_inv_event(SDL_Event event) {
    handle_scroll_event(event);
    aff_stat_item(event);
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        int id_scroll = select_quelle_scroll(3);
        if (id_scroll == chercher_id_scroll_par_label(LABEL_HEROS)){
            printf("heros select : %d\n",click_heros_pos());
        }
        else detecter_click_item();
    }
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {

        lacher_item();
    }
    if ( event.type == SDL_MOUSEWHEEL || event.button.button == SDL_BUTTON_LEFT  ){

        img_inv_actualliser_all();
        
    }
    
}

void draw_rect_inv(){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_rect((SDL_Rect){70, 64, 3 * 80, VISIBLE_AREA_HEIGHT});
    draw_rect((SDL_Rect){WINDOW_WIDTH/2, 64,WINDOW_WIDTH/2, VISIBLE_AREA_HEIGHT});
    draw_rect((SDL_Rect){vw(POS_HEROS_X)-64/2, BORDURE / 2, 64,WINDOW_HEIGHT -BORDURE});
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}