/**
 * @file inv.h
 * @brief Déclarations et définitions pour la gestion de l'inventaire.
 */

 #define NB_EQU 40
 #define EQU_HEROS 5
 
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
     JAMBIERE,       ///< Jambière
     BOTTE,          ///< Botte
     ARME            ///< Arme
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
     emplacement piece_equipement;      ///< Emplacement de l'équipement
     stat_a_booster booste;             ///< Statistique à booster
     rarity rarity;                     ///< Rareté
     int stat;                          ///< Statistique (représente un pourcentage)
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
  * \fn void aff_inv(inv *inventaire)
  * @param inventaire Pointeur vers l'inventaire à afficher.
  */
 void aff_inv(inv *inventaire);

 /**
 * \brief Affiche un item.
 * \fn void aff_item(item_t * item)
 *
 * \param item Pointeur vers l'item à afficher.
 */
void aff_item(item_t * item);
 
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
 void gestion_inv(inv **inventaire);
 
 /**
  * @brief Affiche un item.
  *
  * \fn void aff_item(item_t *item)
  * @param item Pointeur vers l'item à afficher.
  */
 void aff_item(item_t *item);

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
 * \brief Gère l'inventaire, en l'initialisant et en affichant les items.
 * \fn void gestion_inv(inv ** inventaire)
 * 
 * \param inventaire Pointeur vers le pointeur de l'inventaire à gérer.
 */
void gestion_inv(inv ** inventaire);

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
 