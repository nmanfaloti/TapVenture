#ifndef SCROLL_H
#define SCROLL_H

#define NOMBRE_MAX_SCROLL 10 

#include "SDL2/SDL.h"

typedef struct scroll_inf_s{
   //val fix ecrant
   SDL_Rect zone_interaction ; //a init ( zone interaction scroll + zone affichage element contenue dans la zone )
   SDL_Rect zone_scroll; //a init
   SDL_Rect rect_scroll; //a calculer

   float coef ;// zone_interaction * coef = zone total 
   int empl_alloue ;//scroll est il init / faut il l'afficher ? 1 = oui , 0 = non
   //position de deplacement 
   int selection_scroll;
   int scroll_pos ;
   int scrollbar_max_position ;
   int total_content_height;
   int speed ;
   int min_pos ;
}scroll_inf;

extern scroll_inf scroll_liste[NOMBRE_MAX_SCROLL];

extern int est_dans_scroll(SDL_Rect * elem_pos,SDL_Rect zone_interaction);
extern void init_scroll();
float cof_scrollbar_window(int visible_area_height, int total_content_height);
int calculate_scrollbar_height(int visible_area_height,float cof);
extern int creation_scroll(SDL_Rect zone_interaction , SDL_Rect zone_scroll , int total_content_height , int speed , int * erreur_activer );
extern void aff_scrollbar_simple(SDL_Color *color_scrollbar, SDL_Color *color_rect);
int first_pos_scroll();
int select_quelle_scroll(int nb);
extern void handle_scroll_event(SDL_Event event);
extern void update_scroll(int * id_scroll);
#endif // SCROLL_H