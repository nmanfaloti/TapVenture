#include <SDL2/SDL.h>
#include <math.h>
#include <string.h>
#include "../lib/scroll.h"
#include "../lib/sdl.h"
#include "../lib/input_user.h"


scroll_inf scroll_liste[NOMBRE_MAX_SCROLL];


void clear_scroll(){
    for (int i = 0 ; i < NOMBRE_MAX_SCROLL ; i ++){
        scroll_liste[i].empl_alloue = 0 ;
    }
}

//est se que l'elem est dans la zone de scroll
//a utiliser pour l'affichage des element 
extern int est_dans_scroll(SDL_Rect * elem_pos, SDL_Rect zone_interaction) {
    SDL_Rect val_defaut ={mouseX,mouseY,0,0} ;
    // Vérifie si l'élément est à l'intérieur de la zone d'interaction
    if (elem_pos == NULL )elem_pos = &val_defaut ;
    if (elem_pos->x + elem_pos->w <= zone_interaction.x || elem_pos->x >= zone_interaction.x + zone_interaction.w) {
        return 0; // L'élément est à gauche ou à droite de la zone
    }

    if (elem_pos->y + elem_pos->h <= zone_interaction.y || elem_pos->y >= zone_interaction.y + zone_interaction.h) {
        return 0; // L'élément est au-dessus ou en dessous de la zone
    }

    return 1; // L'élément est à l'intérieur de la zone
}

//rien alloue ( remplisage apres)
extern void init_scroll(){
    for ( int i = 0 ; i < NOMBRE_MAX_SCROLL ; i++ )
        scroll_liste[i].empl_alloue = 0 ;
}

float cof_scrollbar_window(int visible_area_height, int total_content_height){
    return (total_content_height/(visible_area_height*1.0));
}

int calculate_scrollbar_height(int visible_area_height,float cof) {
    return round(visible_area_height/cof);
}

int chercher_id_scroll_par_label(const char *label_recherche) {
    if (label_recherche == NULL) return -1; // Vérification pour éviter une erreur

    for (int i = 0; i < NOMBRE_MAX_SCROLL; i++) {
        if (scroll_liste[i].empl_alloue && scroll_liste[i].label != NULL) {
            if (strcmp(scroll_liste[i].label, label_recherche) == 0) {
                return i; // Retourne l'index du scroll correspondant au label
            }
        }
    }
    return -1; // Label non trouvé
}

extern void update_scroll(int * id_scroll){
    int i = 0;
    int deplacement = 1 ;
    if ( id_scroll != NULL){
        i = * id_scroll ;
        deplacement = NOMBRE_MAX_SCROLL;
    }
    for ( ; i < NOMBRE_MAX_SCROLL ; i += deplacement ){
        if ( scroll_liste[i].empl_alloue != 0){

            scroll_liste[i].rect_scroll = scroll_liste[i].zone_scroll ;
            float coef = cof_scrollbar_window(scroll_liste[i].zone_interaction.h, scroll_liste[i].total_content_height);
            scroll_liste[i].rect_scroll.h = calculate_scrollbar_height(scroll_liste[i].zone_scroll.h, coef);
            scroll_liste[i].coef = coef ;
            scroll_liste[i].min_pos = scroll_liste[i].zone_scroll.y ;
            if (scroll_liste[i].total_content_height >  scroll_liste[i].zone_scroll.h){
                scroll_liste[i].scrollbar_max_position = scroll_liste[i].total_content_height - scroll_liste[i].zone_scroll.h;
            }
            else {
                scroll_liste[i].scrollbar_max_position = 10 ;
                scroll_liste[i].speed = 0 ;
            }
        }
        
    }
}

//remplisage de la structure a emplacement  non alloue 
int creation_scroll(SDL_Rect zone_interaction , SDL_Rect zone_scroll , int total_content_height , int speed , int * erreur_activer ,char **label ){
    int creation_succes = 0 ;
    //SDL_GetWindowSize(window, &largeur, &hauteur) ;
    for ( int i = 0 ; i < NOMBRE_MAX_SCROLL ; i++ ){
        if ( scroll_liste[i].empl_alloue == 0){
            if ( erreur_activer != NULL && !est_dans_scroll(&zone_scroll,zone_interaction) ){
                //printf("la zone de scroll  (%d) n'est pas dans la zone interaction(visible) = probleme ( bizarre) \n\t\t elem : %d , %d ,%d , %d \n\t\t zone : %d , %d ,%d , %d \n",i+1,zone_scroll.x,zone_scroll.y,zone_scroll.w,zone_scroll.h,zone_interaction.x,zone_interaction.y,zone_interaction.w,zone_interaction.h);
                exit(2);
            }
            scroll_liste[i].zone_interaction = zone_interaction;
            scroll_liste[i].zone_scroll = zone_scroll ; 
            scroll_liste[i].rect_scroll = zone_scroll ;
            scroll_liste[i].speed = speed ;

            float coef = cof_scrollbar_window(zone_interaction.h, total_content_height);

            scroll_liste[i].rect_scroll.h = calculate_scrollbar_height(zone_scroll.h, coef);
            scroll_liste[i].coef = coef ;
            scroll_liste[i].selection_scroll = 0;
            scroll_liste[i].scroll_pos = 0;
            scroll_liste[i].empl_alloue = 1 ;
            scroll_liste[i].min_pos = zone_scroll.y ;
            scroll_liste[i].total_content_height = total_content_height ;
            scroll_liste[i].scrollbar_max_position = total_content_height - zone_scroll.h;

            creation_succes = 1 ; 
            if ( label != NULL){
                strcpy(scroll_liste[i].label, *label);
                scroll_liste[i].label[sizeof(scroll_liste[i].label) - 1] = '\0';
            }
            return i ;
        }
    }
    if ( !creation_succes ){
        //printf("probleme allocation nb de scroll max\n");
        exit(3);
    }
    return 1 ;
}

void aff_scrollbar_simple(SDL_Color *color_scrollbar, SDL_Color *color_rect){
    // Définir les couleurs par défaut si NULL
    SDL_Color default_color_scrollbar = {200, 200, 200, 255};  // Gris clair
    SDL_Color default_color_rect = {128,128,128,255};       // Bleu

    if (color_scrollbar == NULL)
        color_scrollbar = &default_color_scrollbar;

    if (color_rect == NULL)
        color_rect = &default_color_rect;

    for (int i = 0; i < NOMBRE_MAX_SCROLL; i++) {
        if (scroll_liste[i].empl_alloue) {
            // Dessiner le fond de la scrollbar avec la couleur rect
            SDL_SetRenderDrawColor(renderer, color_rect->r, color_rect->g, color_rect->b, color_rect->a);
            SDL_RenderFillRect(renderer, &scroll_liste[i].zone_scroll);

            // Définir la couleur de la scrollbar
            SDL_SetRenderDrawColor(renderer, color_scrollbar->r, color_scrollbar->g, color_scrollbar->b, color_scrollbar->a);

            // Vérification de la mise à jour de la position
            SDL_Rect rect = scroll_liste[i].rect_scroll;
            rect.y = scroll_liste[i].zone_scroll.y + (scroll_liste[i].scroll_pos * (scroll_liste[i].zone_scroll.h - scroll_liste[i].rect_scroll.h)) / scroll_liste[i].scrollbar_max_position;

            // Appliquer un facteur de déplacement (coef) ou autre logique
            SDL_RenderFillRect(renderer, &rect);  // Dessiner la scrollbar

        }
    }
}
//nb : 0 renvoie sur quelle scroll est la souris 
//nb : 1 renvoie quelle scroll select 
//nb : 2 renvoie quelle scroll select et le deselect
int select_quelle_scroll(int nb){
    for ( int i = 0 ; i < NOMBRE_MAX_SCROLL ; i++){
        if ( (nb == 0 && est_dans_scroll(NULL,scroll_liste[i].zone_scroll)) || ( (nb == 1 || nb ==2 ) && scroll_liste[i].selection_scroll == 1) ||  ( est_dans_scroll(NULL,scroll_liste[i].zone_interaction ) && nb == 3 )){
            if ( nb == 2 )scroll_liste[i].selection_scroll = 0 ;
            return i ;
        }
    }
    return -1 ;
}

void apply_scroll_limits(int id_scroll) {
    if (scroll_liste[id_scroll].scroll_pos < scroll_liste[id_scroll].min_pos) {
        scroll_liste[id_scroll].scroll_pos = scroll_liste[id_scroll].min_pos;
    } else if (scroll_liste[id_scroll].scroll_pos > scroll_liste[id_scroll].scrollbar_max_position) {
        scroll_liste[id_scroll].scroll_pos = scroll_liste[id_scroll].scrollbar_max_position;
    }
}


extern void handle_scroll_event(SDL_Event event) {
    static int scroll_interact = -1;  // Variable statique pour suivre la scrollbar sélectionnée
    int mouse_y = 0 ;
    int first_scroll;

    // Gérer le défilement de la souris (molette)
    if (event.type == SDL_MOUSEWHEEL) {
        if (scroll_interact == -1  && (first_scroll = select_quelle_scroll(3)) != -1) {
            // Ajoute ou soustrait la vitesse de défilement selon la direction de la molette
            scroll_liste[first_scroll].scroll_pos += (event.wheel.y > 0 ? -scroll_liste[first_scroll].speed : scroll_liste[first_scroll].speed);
            apply_scroll_limits(first_scroll);  // Applique les limites après chaque mise à jour
        } else if (scroll_interact != -1) {
            // Si une scrollbar est sélectionnée, ajuster la position selon la direction de la molette
            scroll_liste[scroll_interact].scroll_pos += (event.wheel.y > 0 ? -scroll_liste[scroll_interact].speed : scroll_liste[scroll_interact].speed);
            apply_scroll_limits(scroll_interact);  // Applique les limites
        }
    }

    // Gérer l'événement de clic de la souris pour sélectionner la scrollbar
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (scroll_interact == -1) {
            scroll_interact = select_quelle_scroll(0);  // Sélectionne une scrollbar si le clic est sur une scrollbar valide
        }

        if (scroll_interact != -1) {
            // Calcul de l'offset : calculer l'offset de la souris par rapport à la scrollbar
            mouse_y = mouseY - scroll_liste[scroll_interact].rect_scroll.h /2;
            // Met à jour la position de la scrollbar en tenant compte de l'offset et du facteur de défilement (coef)
            scroll_liste[scroll_interact].scroll_pos = 
            (mouse_y - scroll_liste[scroll_interact].zone_scroll.y) * 
            scroll_liste[scroll_interact].scrollbar_max_position / 
            (scroll_liste[scroll_interact].zone_scroll.h - scroll_liste[scroll_interact].rect_scroll.h);

            apply_scroll_limits(scroll_interact);  // Applique les limites immédiatement après la mise à jour
        }
    }

    // Gérer l'événement de relâchement du bouton de la souris pour désélectionner la scrollbar
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        if (scroll_interact != -1) {
            scroll_liste[scroll_interact].selection_scroll = 0 ;
            scroll_interact = -1;  // Réinitialise la sélection lorsque le bouton de la souris est relâché
        }
    }

    // Gérer le déplacement de la souris pour déplacer la scrollbar
    if (event.type == SDL_MOUSEMOTION && scroll_interact != -1) {
        // Calculer la nouvelle position en fonction du mouvement de la souris
        mouse_y = mouseY - scroll_liste[scroll_interact].rect_scroll.h / 2;
        // Déplacer la scrollbar en tenant compte du décalage de la souris
        scroll_liste[scroll_interact].scroll_pos = 
            (mouse_y - scroll_liste[scroll_interact].zone_scroll.y) * 
            scroll_liste[scroll_interact].scrollbar_max_position / 
            (scroll_liste[scroll_interact].zone_scroll.h - scroll_liste[scroll_interact].rect_scroll.h);

        apply_scroll_limits(scroll_interact);  // Applique les limites pour éviter de dépasser
    }

}
