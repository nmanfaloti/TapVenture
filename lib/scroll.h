#ifndef SCROLL_H
#define SCROLL_H

#define NOMBRE_MAX_SCROLL 10 

#include <SDL2/SDL.h>


/**
 * @struct scroll_inf
 * @brief Structure représentant les informations d'un élément de défilement.
 */
typedef struct scroll_inf_s {
   SDL_Rect zone_interaction; /**< Zone d'interaction avec la barre de défilement. */
   SDL_Rect zone_scroll; /**< Zone de la barre de défilement. */
   SDL_Rect rect_scroll; /**< Rectangle représentant la position actuelle du scroll. */

   float coef; /**< Coefficient déterminant la taille totale de la zone défilable. */
   int empl_alloue; /**< Indique si le scroll est initialisé (1 = oui, 0 = non). */
   int selection_scroll; /**< Indique la sélection actuelle du scroll. */
   int scroll_pos; /**< Position actuelle du scroll. */
   int scrollbar_max_position; /**< Position maximale du scroll. */
   int total_content_height; /**< Hauteur totale du contenu à défiler. */
   int speed; /**< Vitesse de déplacement du scroll. */
   int min_pos; /**< Position minimale du scroll. */
   char label[100]; /**< Libellé associé au scroll. */
} scroll_inf;

/**
 * @var scroll_inf scroll_liste[NOMBRE_MAX_SCROLL]
 * @brief Tableau contenant tous les scrolls actifs.
 */
extern scroll_inf scroll_liste[NOMBRE_MAX_SCROLL];

/**
 * @fn void clear_scroll()
 * @brief Réinitialise tous les scrolls.
 */
extern void clear_scroll();

/**
 * @fn int est_dans_scroll(SDL_Rect *elem_pos, SDL_Rect zone_interaction)
 * @brief Vérifie si un élément est dans la zone d'interaction du scroll.
 * @return 1 si l'élément est dans la zone d'interaction, 0 sinon.
 */
extern int est_dans_scroll(SDL_Rect *elem_pos, SDL_Rect zone_interaction);

/**
 * @fn void init_scroll()
 * @brief Initialise le système de scroll.
 */
extern void init_scroll();

/**
 * @fn float cof_scrollbar_window(int visible_area_height, int total_content_height)
 * @brief Calcule le coefficient entre la zone visible et la hauteur totale du contenu.
 * @return Coefficient de scroll.
 */
float cof_scrollbar_window(int visible_area_height, int total_content_height);

/**
 * @fn int calculate_scrollbar_height(int visible_area_height, float cof)
 * @brief Calcule la hauteur de la barre de défilement.
 * @return Hauteur calculée pour la scrollbar.
 */
int calculate_scrollbar_height(int visible_area_height, float cof);

/**
 * @fn int creation_scroll(SDL_Rect zone_interaction, SDL_Rect zone_scroll, int total_content_height, 
 *                         int speed, int *erreur_activer, char **label)
 * @brief Crée un nouvel élément de scroll.
 * @return Identifiant du scroll créé.
 */
extern int creation_scroll(SDL_Rect zone_interaction, SDL_Rect zone_scroll, 
                           int total_content_height, int speed, int *erreur_activer, char **label);

/**
 * @fn void aff_scrollbar_simple(SDL_Color *color_scrollbar, SDL_Color *color_rect)
 * @brief Affiche une barre de défilement simple avec les couleurs spécifiées.
 */
extern void aff_scrollbar_simple(SDL_Color *color_scrollbar, SDL_Color *color_rect);

/**
 * @fn int first_pos_scroll()
 * @brief Récupère la première position de scroll disponible.
 * @return Index du premier scroll libre.
 */
int first_pos_scroll();

/**
 * @fn int select_quelle_scroll(int nb)
 * @brief Sélectionne un scroll en fonction de son numéro.
 * @return Identifiant du scroll sélectionné.
 */
int select_quelle_scroll(int nb);

/**
 * @fn void handle_scroll_event(SDL_Event event)
 * @brief Gère les événements liés au système de scroll.
 */
extern void handle_scroll_event(SDL_Event event);

/**
 * @fn void update_scroll(int *id_scroll)
 * @brief Met à jour la position du scroll spécifié.
 */
extern void update_scroll(int *id_scroll);

/**
 * @fn int chercher_id_scroll_par_label(const char *label_recherche)
 * @brief Recherche un scroll à partir de son libellé.
 * @return Identifiant du scroll correspondant ou -1 si non trouvé.
 */
extern int chercher_id_scroll_par_label(const char *label_recherche);
#endif // SCROLL_H