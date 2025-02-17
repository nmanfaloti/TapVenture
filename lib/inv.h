/**
 * @file inv.h
 * @brief Déclarations et définitions pour la gestion de l'inventaire.
 */
#define NB_EQU 40
#define EQU_HEROS 5

/**
 * @brief Inventaire, place : inf, equipable : variable lim 10 max, drop : selon stat boss
 *        Stat booster : tous float variable selon rarity, stat : variable int represent pourcentage
 *        Rarity : variable (selon fusion), emplacement : CASQUE, PLASTRON, JAMBIER, BOTTE, ARME
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
    JAMBIERE,       ///< Jambière
    BOTTE,          ///< Botte
    ARME            ///< Arme
} emplacement;

/**
 * @brief Structure représentant un item.
 */
typedef struct {
    char *nom;               ///< Nom système
    char *label;             ///< Nom affiché
    emplacement piece_equipement; ///< Emplacement de l'équipement
    stat_a_booster booste;   ///< Statistique à booster
    char *rarity;            ///< Rareté
    int stat;                ///< Statistique (représente un pourcentage)
} item_t;

/**
 * @brief Structure représentant un inventaire.
 */
typedef struct {
    item_t *liste[NB_EQU]; ///< Liste des items dans l'inventaire
} inv;

/**
 * @brief Affiche l'inventaire.
 *
 * @param inventaire Pointeur vers l'inventaire à afficher.
 */
void aff_inv();

/**
 * @brief Importe l'inventaire.
 */
void import_inv();

/**
 * @brief Initialise l'inventaire.
 *
 * @param inventaire Pointeur vers l'inventaire à initialiser.
 */
void init_inv(inv *inventaire);

/**
 * @brief Sauvegarde l'inventaire.
 */
void save_inv();

/**
 * @brief Gère l'inventaire.
 *
 * @param inventaire Pointeur vers le pointeur de l'inventaire à gérer.
 */
void gestion_inv(inv **inventaire);

/**
 * @brief Affiche un item.
 *
 * @param item Pointeur vers l'item à afficher.
 */
void aff_item(item_t *item);

/**
 * @brief Trouve le premier emplacement vide dans l'inventaire.
 *
 * @param inventaire Pointeur vers l'inventaire à vérifier.
 * @return int L'indice du premier emplacement vide, ou -1 si l'inventaire est plein.
 */
int prem_vide(inv *inventaire);

/**
 * @brief Vérifie si un item est vide.
 *
 * @param item Pointeur vers l'item à vérifier.
 * @return int 1 si l'item est vide, 0 sinon.
 */
int est_vide(item_t *item);

/**
 * @brief Vérifie si deux items peuvent être fusionnés.
 *
 * @param item1 Pointeur vers le premier item.
 * @param item2 Pointeur vers le second item.
 * @return int Code de retour indiquant si la fusion est possible.
 */
int droit_fusion(item_t *item1, item_t *item2);
