
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
 @fn void gestion_inv(inv **inventaire, int NB_items, int scroll_id, SDL_Rect SDL_Rect, 
 *                      int nb_collone, int nb_ligne, int arrondis, int decalage_bas, 
 *                      int decalage_cote, int id_back_item)
 * @brief Gère l'affichage et l'interaction avec l'inventaire.
 * @param inventaire Pointeur vers l'inventaire.
 * @param NB_items Nombre total d'objets dans l'inventaire.
 * @param scroll_id Identifiant du défilement.
 * @param SDL_Rect Rectangle SDL définissant la position et la taille de l'affichage.
 * @param nb_collone Nombre de colonnes de l'inventaire.
 * @param nb_ligne Nombre de lignes de l'inventaire.
 * @param arrondis Rayon d'arrondi des bords de l'affichage.
 * @param decalage_bas Décalage vers le bas pour l'affichage.
 * @param decalage_cote Décalage latéral pour l'affichage.
 * @param id_back_item Identifiant de l'arrière-plan des objets.
 */
extern void gestion_inv(inv **inventaire, int NB_items, int scroll_id, SDL_Rect SDL_Rect, 
    int nb_collone, int nb_ligne, int arrondis, int decalage_bas, 
    int decalage_cote, int id_back_item);

/**
* @brief Détruit l'inventaire et libère la mémoire associée.
* 
* @param inventaire Pointeur vers l'inventaire.
*/
void dest_inv(inv **inventaire);

/**
* @brief Trouve la première position vide dans l'inventaire.
* 
* @param inventaire Pointeur vers l'inventaire.
* @return Index de la première position vide, -1 si l'inventaire est plein.
*/
int prem_vide(inv *inventaire);

/**
* @brief Vérifie si un objet est vide.
* 
* @param item Pointeur vers l'objet.
* @return 1 si l'objet est vide, 0 sinon.
*/
int est_vide(item_t *item);

/**
* @brief Vérifie si deux objets peuvent être fusionnés.
* 
* @param item1 Premier objet.
* @param item2 Deuxième objet.
* @return 1 si la fusion est possible, 0 sinon.
*/
int droit_fusion(item_t *item1, item_t *item2);

/**
* @brief Génère une statistique aléatoire pour un objet.
* 
* @param x Valeur de base.
* @param type Type de statistique.
* @param stat_ameliorer Niveau d'amélioration.
* @param rarete Niveau de rareté.
* @param nb_max_nv Nombre maximum de niveaux.
* @return Valeur générée pour la statistique.
*/
int generate_stat_nb(int x, int type, int stat_ameliorer, int rarete, int nb_max_nv);

/**
* @brief Fusionne deux objets.
* 
* @param item1 Premier objet.
* @param item2 Deuxième objet.
*/
void deb_fusion(item_t *item1, item_t *item2);

/**
* @brief Dessine un cercle graphique.
* 
* @param x Coordonnée X du centre.
* @param y Coordonnée Y du centre.
* @param rayon Rayon du cercle.
*/
void cercle_graf(int x, int y, int rayon);

/**
* @brief Dessine un rectangle aux coins arrondis.
* 
* @param SDL_Rect Rectangle SDL définissant la zone de dessin.
* @param radius Rayon des coins arrondis.
* @param r Valeur de la couleur rouge.
* @param g Valeur de la couleur verte.
* @param b Valeur de la couleur bleue.
*/
void rectangle_arrondis(SDL_Rect SDL_Rect, int radius, int r, int g, int b);

/**
* @brief Affiche l'inventaire graphiquement.
* 
* @param inventaire Pointeur vers l'inventaire.
*/
void aff_inv_graf(inv *inventaire);

/**
* @brief Dessine une barre de défilement.
* 
* @param scrollbar_x Position X.
* @param scrollbar_y Position Y.
* @param scrollbar_height Hauteur totale.
* @param scrollbar_wheight Hauteur de la partie défilable.
*/
void draw_scrollbar(int scrollbar_x, int scrollbar_y, int scrollbar_height, int scrollbar_wheight);

/**
* @brief Gère les événements liés à l'inventaire.
* 
* @param event Événement SDL capturé.
*/
extern void handle_inv_event(SDL_Event event);

/**
* @brief Calcule la position d'un élément dans l'inventaire.
* 
* @return Position calculée.
*/
int calcule_pos_inv();

/**
* @brief Détruit tous les inventaires chargés.
*/
extern void dest_all_inventaires();

/**
* @brief Affiche tous les inventaires.
*/
extern void aff_all_inventaires();

/**
* @brief Charge les ressources de l'inventaire.
*/
extern void load();

/**
* @brief Dépose un objet de l'inventaire.
*/
extern void drop_item();

/**
* @brief Charge un inventaire à partir d'un fichier.
* 
* @param nom_fichier Nom du fichier à charger.
*/
extern void load_inv(const char *nom_fichier);

/**
* @brief Initialise l'inventaire principal.
*/
void init_inv_main();

/**
* @brief Charge un objet de référence depuis un fichier.
* 
* @param f Fichier source.
* @return Pointeur vers l'objet chargé.
*/
extern item_t *load_item_ref(FILE *f);

/**
* @brief Charge un objet appartenant au joueur depuis un fichier.
* 
* @param f Fichier source.
* @return Pointeur vers l'objet chargé.
*/
extern item_t *load_item_joueur(FILE *f);

/**
* @brief Variable globale représentant les dégâts des objets.
*/
extern int stat_item_degat;

/**
* @brief Variable globale représentant le temps des objets.
*/
extern int stat_item_temps;

/**
* @brief Applique un bonus d'or.
* 
* @return Valeur du bonus.
*/
extern int boost_gold();

/**
* @brief Gère les statistiques d'un héros.
* 
* @param index_heros Index du héros concerné.
*/
extern void gestion_stat_heros(int index_heros);

/**
* @brief Sauvegarde un inventaire dans un fichier.
* 
* @param inventaire Pointeur vers l'inventaire à sauvegarder.
* @param save Nom du fichier de sauvegarde.
*/
void save_inv(inv *inventaire, char *save);

/**
* @brief Dessine le cadre de l'inventaire.
*/
extern void draw_rect_inv();

/**
* @brief Crée une interface utilisateur pour un raccourci.
* 
* @param it Pointeur vers l'objet associé.
* @param x Position X.
* @param y Position Y.
* @param id_tuille Identifiant de la tuile associée.
*/
void racourcis_createUI(item_t *it, int x, int y, int id_tuille);

/**
* @brief Effectue une sauvegarde de l'inventaire.
*/
extern void makeSaveInventaire();

/**
* @brief Rafraîchit l'affichage de l'inventaire.
*/
extern void refresh_inv();

/**
* @brief Met à jour l'inventaire et son affichage.
*/
extern void actualiser();
#endif