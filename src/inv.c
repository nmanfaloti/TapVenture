
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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 500;
const int SCROLLBAR_WIDTH = 20;
const int VISIBLE_AREA_HEIGHT = 500;
const int NB_COLLONE = 4;
const int NB_LIGNE = 15;
const int ITEM_SIZE = 80;
const int DEPLACEMENT = 20;
const int ARRONDIS = 20;

liste_inventaires * list_inv = NULL ;


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

item_t * generation_item(char * name , char * label ){
    item_t * it = malloc(sizeof(item_t));
    it->nom = strdup(name);
    it->label = strdup(label);
    return it ;
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
    if ((*item)->nom != NULL) {
        free((*item)->nom);
        (*item)->nom = NULL;
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
int recherche_emplacement_valide(inv * inventaire){
    if (inventaire == NULL )return -1 ;
    for ( int i = 0 ; i < inventaire->nb_items ; i++){
        item_t * path = inventaire ->liste[i] ;
        if ( path->label == NULL || path->nom == NULL){
            return i ;
        }
    }
    return -1 ;
}

int calculate_total_content_size(int nb_ligne, int taille, int decalage) {
    if (nb_ligne == 1) 
        return taille;
    return (nb_ligne * taille) + ((nb_ligne - 1) * decalage);
}

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

void gestion_inv(inv **inventaire, int NB_items, int id_scroll, SDL_Rect SDL_Rect, int nb_colonne, int nb_ligne, int nb_element, int arrondis, 
int decalage_bas, int decalage_cote) {
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
        printf("\n\n total_content_height : %d   total_content_width : %d",total_content_height,total_content_width);
        update_scroll(&id_scroll);


        // Initialisation des données de l'inventaire
        (*inventaire)->id_scroll = id_scroll;
        SDL_Rect.x += scroll_liste[id_scroll].zone_interaction.x;
        SDL_Rect.y += scroll_liste[id_scroll].zone_interaction.y;
        insert_inf_aff(*inventaire, SDL_Rect, nb_colonne, nb_ligne, nb_element, arrondis, decalage_bas, decalage_cote);

        (*inventaire)->liste = malloc(NB_items * sizeof(item_t *));
        (*inventaire)->nb_items = NB_items;  // Stocke le nombre d'items

        printf("Inventaire initialisé avec %d items max \n", NB_items);
    }
}
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

char * item_rondam(int * piece_equipement){
    int liste[] = {NB_CASQUE,NB_CASQUE + NB_PLASTRON,NB_CASQUE + NB_PLASTRON + NB_BOTTES,NB_CASQUE + NB_PLASTRON + NB_BOTTES + NB_ARME} ;
    int nb_item = NB_CASQUE + NB_PLASTRON + NB_BOTTES + NB_ARME ;
    int id_item = rand() % nb_item  + 1 ;
    int i = 0 ,run = 1 ;
    for ( ; i < 4  && run ; i ++){
        if ( id_item < liste[i] && i!=0 ){
            id_item -= liste[i] ;
            run = 0 ;
        }
    }
    *piece_equipement = i ;
    char * mot = malloc ( strlen("11.png") + 1 ) ;
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
    char * type = realloc(type , strlen(prefix) + strlen(mot) + 1);
    strcpy(type,prefix);
    strcat(type,mot);
    free(mot);
    return type ;
}

int chercher_select(){
    if ( !list_inv )return -1 ;
    int id_scroll_select = select_quelle_scroll(3) ;
    for ( int i = 0 ;  i < list_inv->nb_inventaires ; i++)
        if (list_inv->inventaires[i]->id_scroll == id_scroll_select)return i ;
    return -1 ;
}

// Fonction pour calculer le numéro de case en fonction des coordonnées
int calcule_pos_inv() {
    int id_inv = chercher_select();
    printf("--%d--\n",id_inv);
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
    return elem_numb;
}


void aff_inv_graf(inv* inventaire) {
        aff_inv_inf path = inventaire->info_inv;
        int id_scroll = inventaire->id_scroll;
        int x = scroll_liste[id_scroll].zone_interaction.x;
        int y = scroll_liste[id_scroll].zone_interaction.y;
        int conteur = 0 ;
        y -= scroll_liste[id_scroll].scroll_pos;
        for (int h = 0; h < path.nb_ligne; h++) {
            for (int i = 0; i < path.nb_collone && conteur < inventaire->nb_items ; i++ , conteur ++ ) {
                // Assurez-vous que les dimensions de SDL_Rect sont correctes
                char *chemin = malloc(strlen("assets/ui/") + strlen(inventaire->liste[h+i]->label) + 1);
                strcpy(chemin, "assets/ui/");
                strcat(chemin, inventaire->liste[h+i]->label);
                createUIImg(&pageHolder.page[3], chemin, (SDL_Rect){x, y, 64, 64}, inventaire->liste[h+i]->label);
                rectangle_arrondis((SDL_Rect){x,y,path.SDL_Rect.w,path.SDL_Rect.h}, path.arrondis, 128, 128, 128);
                x += path.SDL_Rect.w + path.decalage_cote;  // Mise à jour correcte de la position horizontale
                free(chemin);
            }
            x = scroll_liste[id_scroll].zone_interaction.x; // Réinitialisation de x pour une nouvelle ligne
            y += path.SDL_Rect.h + path.decalage_bas;  // Mise à jour de la position verticale
        }
}

void handle_inv_event(SDL_Event event) {
    handle_scroll_event(event);

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int clicked_case = calcule_pos_inv();
        if (clicked_case != -1) {
            printf("Vous avez cliqué sur le carré numéro : %d\n", clicked_case+1);
        }
    }
}


void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

 void aff_all_inventaires() {
    printf("\n\n -------aff inv all ----------\n\n"); 
    for (int i = 0; i < list_inv->nb_inventaires; i++) {
        //aff_inv_graf(list_inv->inventaires[i]);  // Appel à aff_inv_graf pour chaque inventaire
    }
    aff_scrollbar_simple(NULL, NULL);
}
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
    printf("porbleme 1 ;");
    list_inv->inventaires = realloc(list_inv->inventaires, (list_inv->nb_inventaires + nb_inventaires) * sizeof(inv *));
    for (int i = 0; i < nb_inventaires; i++) {
        printf("%d",i);
        inv *inventaire = va_arg(args, inv *);  // Récupère l'inventaire passé en paramètre
        list_inv->inventaires[list_inv->nb_inventaires] = inventaire;
        list_inv->nb_inventaires++;
    }

    va_end(args);
}

extern void load(){
    inv *inventaire_item = NULL;
    inv *inventaire_heros = NULL;

    int id_scroll1 = creation_scroll((SDL_Rect){0, 0, 0, VISIBLE_AREA_HEIGHT},
                                     (SDL_Rect){0, 0, SCROLLBAR_WIDTH, VISIBLE_AREA_HEIGHT - 100},
                                     0, 30,NULL);

    int id_scroll2 = creation_scroll((SDL_Rect){WINDOW_WIDTH/2, 0,WINDOW_WIDTH/2, VISIBLE_AREA_HEIGHT},
                                     (SDL_Rect){800-20, 0, SCROLLBAR_WIDTH, VISIBLE_AREA_HEIGHT - 10},
                                     0, 30,NULL);
    printf("ajoute inv -----");

    gestion_inv(&inventaire_item,100, id_scroll1, (SDL_Rect){0, 10, ITEM_SIZE, ITEM_SIZE},
    NB_COLLONE, NB_LIGNE, NB_LIGNE * NB_COLLONE, ARRONDIS, DEPLACEMENT, DEPLACEMENT);

    printf("ajoute inv 2 -----");
    gestion_inv(&inventaire_heros,20, id_scroll2, (SDL_Rect){0, 10,WINDOW_WIDTH/2 -20 , 200},
    1, 10, 1 * 10, ARRONDIS, 20, 0);

    printf("ajoute inv liste global -----");


    ajouter_inventaire(2 , inventaire_item , inventaire_heros);

}


void init_liste_inventaires(){
    list_inv = malloc (sizeof(liste_inventaires));
    list_inv->inventaires = malloc(sizeof(inv *));
    list_inv->nb_inventaires = 0 ;
    load();
}

int chercher_label(char * label , inv * inventaire){
    for ( int i = 0 ; i < inventaire ->nb_items ; i++){
        if ( strcmp ( label , inventaire->liste[i]->label))
            return i ;
    } 
    return -1 ;
}
void drop_item(){

    if ( rand() % 100 + 1 > DROP_ITEM * level.mobKilled ) return ; // chance de drop 1 item 
    int emplacement_vide = recherche_emplacement_valide(list_inv->inventaires[0]);
    if (emplacement_vide == -1){
        //notif inv full
        printf("inventaire remplis\n");
        return ;
    }

    int type_stat = rand() % 3 + 1 ;
    int stat = generate_stat_nb(25,type_stat,1,0,70);// a remplacer 1 et dernier stat 
    
    int piece_equipement ;
    char * nom_path_icon = item_rondam(&piece_equipement);
    int id_ref = chercher_label(nom_path_icon,list_inv->inventaires[1]);

    char * nom = list_inv->inventaires[1]->liste[id_ref]->nom ;

    item_t * it = generation_item(nom,nom_path_icon);
    it->stat = stat ;
    it -> booste = type_stat ;
    it->piece_equipement = piece_equipement ;
    it->rarity = COMMUN ;

    list_inv->inventaires[0]->liste[emplacement_vide] = it;
}

int comparer_items(const void *a, const void *b) {
    item_t *itemA = *(const item_t **)a;
    item_t *itemB = *(const item_t **)b;

    if (!itemA || !itemB || !itemA->nom || !itemB->nom) return 0;
    return strcmp(itemA->nom, itemB->nom);
}

void trier_inventaire(inv *inventaire) {
    if (!inventaire || !inventaire->liste || inventaire->nb_items <= 0) {
        printf("Erreur : Inventaire vide ou invalide.\n");
        return;
    }

    qsort(inventaire->liste, inventaire->nb_items, sizeof(item_t *), comparer_items);
}

#define MAX_LIGNE 500

item_t * load_item_joueur(FILE *f) {
    item_t *it = malloc(sizeof(item_t));
    char ligne[MAX_LIGNE];
    char temp_name[256];
    char temp_label[256];
    while (fgets(ligne, MAX_LIGNE, f)) {
        if (strstr(ligne, "}")) break;
        if (strstr(ligne, "\"nom\"")) {
            sscanf(ligne, " \"nom\": \"%s[^\"]\"", temp_name);
        } else if (strstr(ligne, "\"label\"")) {
            sscanf(ligne, " \"label\": \"%s[^\"]\"", temp_label);
        } else if (strstr(ligne, "\"piece_equipement\"")) {
            sscanf(ligne, " \"piece_equipement\": %d", &it->piece_equipement);
        } else if (strstr(ligne, "\"booste\"")) {
            sscanf(ligne, " \"booste\": %d", &it->booste);
        } else if (strstr(ligne, "\"rarity\"")) {
            sscanf(ligne, " \"rarity\": %d", &it->rarity);
        } else if (strstr(ligne, "\"stat\"")) {
            sscanf(ligne, " \"stat\": %d", &it->stat);
        }
    }
    it->nom = malloc ( strlen ( temp_name) + 1);
    it->label = malloc ( strlen ( temp_label) + 1);

    strcpy(it->nom, temp_name);
    strcpy(it->label, temp_label);

    return it;
}

item_t * load_item_ref(FILE *f) {
    item_t *it = malloc(sizeof(item_t));
    char ligne[MAX_LIGNE];
    char temp_name[256];
    char temp_label[256];
    while (fgets(ligne, MAX_LIGNE, f)) {
        if (strstr(ligne, "}")) break;
        if (strstr(ligne, "\"nom\"")) {
            sscanf(ligne, " \"nom\": \"%s[^\"]\"", temp_name);
        } else if (strstr(ligne, "\"label\"")) {
            sscanf(ligne, " \"label\": \"%s[^\"]\"", temp_label);
        }
    }
    it->nom = malloc ( strlen ( temp_name) + 1);
    it->label = malloc ( strlen ( temp_label) + 1);

    strcpy(it->nom, temp_name);
    strcpy(it->label, temp_label);

    return it;
}


void load_inv(const char *nom_fichier, inv * inventaire ) {
    FILE *f = fopen(nom_fichier, "r");

    if ( !f )return ;
    char ligne[MAX_LIGNE];

    while (fgets(ligne, MAX_LIGNE, f) && ! ( inventaire->nb_items >= inventaire->nb_items)) {
        if (strstr(ligne, "{")) {  // Début d'un objet item_t
            inventaire->liste[inventaire->nb_items] = load_item_joueur(f);
            if (inventaire->liste[inventaire->nb_items]) inventaire->nb_items ++ ;
        }
    }

    fclose(f);
}

void load_inv_ref(const char *nom_fichier, inv * inventaire ) {
    FILE *f = fopen(nom_fichier, "r");

    if ( !f )return ;
    char ligne[MAX_LIGNE];

    while (fgets(ligne, MAX_LIGNE, f) && ! ( inventaire->nb_items >= inventaire->nb_items)) {
        if (strstr(ligne, "{")) {  // Début d'un objet item_t
            inventaire->liste[inventaire->nb_items] = load_item_ref(f);
            if (inventaire->liste[inventaire->nb_items]) inventaire->nb_items ++ ;
        }
    }

    fclose(f);
}