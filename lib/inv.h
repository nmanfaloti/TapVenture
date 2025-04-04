
#ifndef INV_H
#define INV_H


#define MAX_LIGNE 1000

#define NB_BOTTES 6
#define NB_PLASTRON 6
#define NB_CASQUE 6
#define NB_ARME 11
#define DROP_ITEM 100 // en pourcentage par nv.monstre 
#include "../lib/scroll.h"
/**
 * @file inv.h
 * @brief Déclarations et définitions pour la gestion de l'inventaire.
 */

 #define NB_EQU 40
 #define EQU_HEROS 5
 #define POS_HEROS_X 43 // 46/100
 #define BORDURE 10
 #define DECALAGE 1.5
 #define ITEM_SIZE 7
 
 #define BORDURE_PATH "assets/ui/background/bordure.png"
 #define BORDURE_LABEL "/bordure_label"
 
 /**
  * @brief Inventaire, place : inf, equipable : variable lim 10 max, drop : selon stat boss
  *        Stat booster : tous float variable selon rarity, stat : variable int represent pourcentage
  *        Rarity : variable (selon fusion), emplacement : CASQUE, PLASTRON, JAMBIERE, BOTTE, ARME
  */
 
 /**
  * @brief Énumération des statistiques à booster.
  */
 typedef enum {
     DEGAT,          ///< Dégâts
     VITESSE_ATTAQUE,///< Vitesse d'attaque
     OR              ///< Or
 } stat_a_booster;
 
 /**
  * @brief Énumération des emplacements d'équipement.
  */
 typedef enum {
     CASQUE,         ///< Casque
     PLASTRON,       ///< Plastron
     BOTTE,          ///< Botte
     ARME ,           ///< Arme
     NB_AMPLECEMENT
 } emplacement;
 
 /**
  * @brief Énumération des niveaux de rareté.
  */
 typedef enum {
     COMMUN,         ///< Commun
     UNCOMMUN,       ///< Peu commun
     RARE,           ///< Rare
     EPIC,           ///< Épique
     LEGENDARY       ///< Légendaire
 } rarity;
 
 /**
  * @brief Structure représentant un item.
  */
 typedef struct {
     char *nom;                         ///< Nom système
     char *label;                       ///< Nom affiché
     char *nom_fichier ;
     int pos_y ;
     emplacement piece_equipement;      ///< Emplacement de l'équipement
     stat_a_booster boost;             ///< Statistique à booster
     rarity rarity;                     ///< Rareté
     int stat;                          ///< Statistique (représente un pourcentage)
     int select_heros;
 } item_t;

typedef struct aff_inv_inf_s{
    SDL_Rect SDL_Rect ;
    int nb_collone ;
    int nb_ligne ;
    int nb_element ;
    int arrondis ;
    int decalage_bas ;
    int decalage_cote ;

} aff_inv_inf;


 /**
  * @brief Structure représentant un inventaire.
  */
 typedef struct {
    int id_scroll;
    aff_inv_inf info_inv;
    item_t **liste;  ///< Tableau dynamique de pointeurs sur items
    int nb_items;    ///< Nombre d’items réel
    int id_tuile;
} inv;

typedef struct {
    inv **inventaires;  // Tableau de pointeurs vers les inventaires
    int nb_inventaires; // Nombre d'inventaires dans la liste
} liste_inventaires;

extern liste_inventaires * list_inv ; 
//0 item joueur  / essamble des 1 model items
 // premier / (2 pour l'instant les item_init sont pas charger ) sont allouer aux item et le reste aux heros
 
 /**
  * @brief Initialise l'inventaire.
  *
  * \fn void init_inv(inv *inventaire)
  * @param inventaire Pointeur vers l'inventaire à initialiser.
  */
 void init_inv(inv *inventaire);

 /**
 * \brief Détruit un item et libère la mémoire allouée.
 * \fn void dest_item(item_t ** item)
 *
 * \param item Pointeur vers le pointeur de l'item à détruire.
 */
void dest_item(item_t ** item);

 /**
 * \brief destruction des info stocker par l'item pour le reutiliser.
 * \fn void liberer_item(item_t * item)
 *
 * \param item Pointeur sur item à libérer.
 */

void liberer_item(item_t * item);
 
 /**
  * @brief Gère l'inventaire.
  *
  * \fn void gestion_inv(inv **inventaire)
  * @param inventaire Pointeur vers le pointeur de l'inventaire à gérer.
  */
 extern void gestion_inv(inv ** inventaire, int NB_items,int scroll_id,SDL_Rect SDL_Rect ,int nb_collone ,int nb_ligne  ,
    int arrondis ,int decalage_bas ,int decalage_cote, int id_back_item);
 

 /**
 * \brief Détruit un inventaire et libère la mémoire allouée.
 * \fn void dest_inv(inv ** inventaire)
 *
 * \param inventaire Pointeur vers le pointeur de l'inventaire à détruire.
 */
void dest_inv(inv ** inventaire);
 
 /**
  * @brief Trouve le premier emplacement vide dans l'inventaire.
  *
  * \fn int prem_vide(inv *inventaire)
  * @param inventaire Pointeur vers l'inventaire à vérifier.
  * @return int L'indice du premier emplacement vide, ou -1 si l'inventaire est plein.
  */
 int prem_vide(inv *inventaire);
 
 /**
  * @brief Vérifie si un item est vide.
  *
  * \fn int est_vide(item_t *item)
  * @param item Pointeur vers l'item à vérifier.
  * @return int 1 si l'item est vide, 0 sinon.
  */
 int est_vide(item_t *item);
 
 /**
  * @brief Vérifie si deux items peuvent être fusionnés.
  *
  * \fn int droit_fusion(item_t *item1, item_t *item2)
  * @param item1 Pointeur vers le premier item.
  * @param item2 Pointeur vers le second item.
  * @return int Code de retour indiquant si la fusion est possible.
  */
 int droit_fusion(item_t *item1, item_t *item2);


 /**
 * \brief Génère un nombre de statistique influencé par un niveau.
 * \fn int generate_stat_nb(int x,int type,int stat_ameliorer ,int rarete,int nb_max_nv)
 *
 * \param x Niveau actuel.
 * \param type Type de statistique (0 = or, 1 = degat, 2 = vit_att).
 * \param stat_ameliorer Statistique à améliorer.
 * \param rarete Rareté de l'item.
 * \param nb_max_nv Niveau maximal.
 * \return int Nouvelle valeur de la statistique.
 */
int generate_stat_nb(int x,int type,int stat_ameliorer ,int rarete,int nb_max_nv);

 /**
 * \brief Fusionne deux items en augmentant leur rareté et en générant une nouvelle statistique.
 *
 * \fn void deb_fusion(item_t * item1, item_t * item2)
 * \param item1 Premier item à fusionner.
 * \param item2 Deuxième item à fusionner.
 */
void deb_fusion(item_t * item1, item_t * item2);

/**
 * @brief Dessine un cercle.
 * 
 * @param renderer Le renderer SDL sur lequel dessiner.
 * @param x La coordonnée x du centre du cercle.
 * @param y La coordonnée y du centre du cercle.
 * @param rayon Le rayon du cercle.
 */
void cercle_graf(int x, int y, int rayon);

/**
 * @brief Dessine un rectangle avec des coins arrondis.
 * 
 * @param renderer Le renderer SDL sur lequel dessiner.
 * @param x La coordonnée x du coin supérieur gauche du rectangle.
 * @param y La coordonnée y du coin supérieur gauche du rectangle.
 * @param w La largeur du rectangle.
 * @param h La hauteur du rectangle.
 * @param radius Le rayon des coins arrondis.
 * @param r La composante rouge de la couleur du rectangle.
 * @param g La composante verte de la couleur du rectangle.
 * @param b La composante bleue de la couleur du rectangle.
 */
void rectangle_arrondis(SDL_Rect SDL_Rect, int radius, int r, int g, int b);

/**
 * @brief Affiche plusieurs rectangles arrondis en grille.
 * 
 * @param pRenderer Le renderer SDL sur lequel dessiner.
 * @param scrool_pos La position de défilement.
 * @param cof_scrollbar_window Le coefficient de vitesse de défilement.
 * @param nb_collone Le nombre de colonnes.
 * @param nb_ligne Le nombre de lignes.
 * @param taille La taille de chaque rectangle.
 * @param decalage L'espace entre les rectangles.
 * @param arrondis Le rayon des coins arrondis.
 */
void aff_inv_graf(inv * inventaire);
/**
 * @brief Dessine une barre de défilement verticale.
 * 
 * @param renderer Le renderer SDL sur lequel dessiner.
 * @param scrollbar_position La position y du sommet de la barre de défilement.
 * @param scrollbar_height La hauteur de la barre de défilement.
 */
void draw_scrollbar(int scrollbar_x,int scrollbar_y, int scrollbar_height , int scrollbar_wheight ) ;

/**
 * @brief Calcule la hauteur totale du contenu.
 * 
 * @param nb_ligne Le nombre de lignes.
 * @param taille La taille de chaque rectangle.
 * @param decalage L'espace entre les rectangles.
 * @return La hauteur totale du contenu.
 */
int calculate_total_content_height(int nb_ligne, int taille, int decalage);


/**
 * @brief Calcule le coefficient de vitesse de défilement de la barre de défilement.
 * 
 * @param visible_area_height La hauteur de la zone visible.
 * @param total_content_height La hauteur totale du contenu.
 * @return Le coefficient de vitesse de défilement de la barre de défilement.
 */
float cof_scrollbar_window(int visible_area_height, int total_content_height);

/**
 * @brief Calcule la position maximale de la barre de défilement.
 * 
 * @param screen_height La hauteur de l'écran.
 * @param scrollbar_heigt La hauteur de la barre de défilement.
 * @return La position maximale de la barre de défilement.
 */
int calculate_scrollbar_max_position(int screen_height, int scrollbar_heigt , float cof);

/**
 * @brief Gère les événements de défilement.
 * 
 * @param event L'événement SDL à gérer.
 * @param scrollbar_position La position de la barre de défilement.
 * @param scrollbar_speed La vitesse de déplacement de la barre de défilement.
 * @param scrollbar_max_position La position maximale de la barre de défilement.
 */
extern void handle_inv_event(SDL_Event event);


/**
 * @brief Vérifie si les coordonnées de la souris sont dans les limites d'un carré.
 * 
 * @param x_deb La coordonnée x de départ du carré.
 * @param y_deb La coordonnée y de départ du carré.
 * @param x_fin La coordonnée x de fin du carré.
 * @param y_fin La coordonnée y de fin du carré.
 * @return 1 si les coordonnées de la souris sont dans les limites du carré, 0 sinon.
 */
int coordonner_realiste(int x_deb, int y_deb, int x_fin, int y_fin);

/**
 * @brief Calcule les dimensions de la zone de contenu.
 * 
 * @param x_larg Pointeur vers la largeur calculée.
 * @param y_haut Pointeur vers la hauteur calculée.
 * @param nb_ligne Le nombre de lignes.
 * @param nb_colonne Le nombre de colonnes.
 * @param largeur La largeur de chaque élément.
 * @param hauteur La hauteur de chaque élément.
 * @param decalage L'espace entre les éléments.
 */
void calculate_zone_content(int* x_larg, int* y_haut, int nb_ligne, int nb_colonne, int largeur, int hauteur, int decalage);

/**
 * @brief Calcule le numéro de case en fonction des coordonnées de la souris.
 * 
 * @param x La coordonnée x de départ.
 * @param y La coordonnée y de départ.
 * @param nb_collone Le nombre de colonnes.
 * @param nb_ligne Le nombre de lignes.
 * @param taille_largeur La largeur de chaque élément.
 * @param taille_longeur La longueur de chaque élément.
 * @param decalage L'espace entre les éléments.
 * @return Le numéro de case en fonction des coordonnées de la souris, ou -1 si aucune case n'est trouvée.
 */
int calcule_pos_inv();

extern void dest_all_inventaires();
extern void aff_all_inventaires() ;
extern void load() ;
extern void cleanup() ;
extern void drop_item();



extern void load_inv(const char *nom_fichier) ;
void init_inv_main() ;
extern item_t * load_item_ref(FILE *f) ;
extern item_t * load_item_joueur(FILE *f);


extern int stat_item_degat ;
extern int stat_item_temps ;
extern int boost_gold();
extern void gestion_stat_heros(int index_heros);
void save_inv(inv *inventaire, char *save) ;



extern void draw_rect_inv() ;
void racourcis_createUI(item_t * it,int x,int y,int id_tuille);
extern void makeSaveInventaire();
extern void refresh_inv();
extern void actualiser();
#endif