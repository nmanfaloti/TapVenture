/**
 * @file prestige.h
 * @brief Interface de gestion du système de prestige dans TapVenture.
 *
 * Ce fichier définit les structures et fonctions nécessaires pour gérer le système de prestige,
 * y compris les arbres de prestige, les items de prestige, et leurs effets sur le jeu.
 * Il inclut également des outils pour l'achat d'items, la gestion des points de prestige,
 * et les mises à jour des pages de prestige.
 * @author Noam MANFALOTI
 * @date 28 fevrier 2025
 */

 #ifndef PRESTIGE_H
 #define PRESTIGE_H
 
 #include <SDL2/SDL.h>
 
 #define MAX_PRESTIGE_ITEMS 8 /**< Nombre maximum d'items dans un arbre de prestige */
 #define NB_PRESTIGE_ITEMS_PER_PAGE 4 /**< Nombre maximum d'items affichés par page */
 
 /**
  * @struct prestigeItem
  * @brief Structure représentant un item de prestige.
  *
  * Cette structure contient les informations nécessaires pour gérer un item de prestige,
  * y compris son nom, sa description, son coût, son état d'achat, et son effet.
  */
 typedef struct prestigeItem_s {
     char *name;                      /**< Nom de l'item de prestige */
     char *description;               /**< Description de l'item de prestige */
     int cost;                        /**< Coût de l'item en points de prestige */
     int owned;                       /**< Indique si l'item a été acheté (1) ou non (0) */
     float value;                     /**< Valeur associée à l'effet de l'item */
     void (*effect)(float);           /**< Pointeur vers la fonction appliquant l'effet de l'item */
     int index;                       /**< Index de l'item dans l'arbre de prestige */
     SDL_Rect pos;                    /**< Position et taille de l'item dans l'interface utilisateur */
 } prestigeItem;
 
 /**
  * @struct prestigeList
  * @brief Structure représentant une liste d'items de prestige.
  *
  * Cette structure contient un tableau d'items de prestige et leur nombre total.
  */
 typedef struct prestigeList_s {
     prestigeItem *items;             /**< Tableau d'items de prestige */
     int count;                       /**< Nombre total d'items dans la liste */
 } prestigeList;
 
 /**
  * @struct prestigeTree
  * @brief Structure représentant les arbres de prestige.
  *
  * Cette structure contient trois listes d'items de prestige : Gold, Damage, et Prestige.
  */
 typedef struct prestigeTree_s {
     prestigeList *Gold;              /**< Liste des items de l'arbre Gold */
     prestigeList *Damage;            /**< Liste des items de l'arbre Damage */
     prestigeList *Prestige;          /**< Liste des items de l'arbre Prestige */
 } prestigeTree_t;
 
 extern int prestigePoints;           /**< Nombre de points de prestige du joueur */
 extern float goldModifier;           /**< Modificateur de gain d'or lié au prestige */
 extern float damageModifier;         /**< Modificateur de dégâts lié au prestige */
 extern float prestigeModifier;       /**< Modificateur de points de prestige lié au prestige */
 
 /**
  * @fn void initPrestige()
  * @brief Initialise le système de prestige.
  *
  * Cette fonction configure les arbres de prestige et prépare les items de prestige.
  */
 void initPrestige();
 
 /**
  * @fn void addPrestigeItem(prestigeList *list, char *name, char *description, int cost, void (*effect)(float), float value)
  * @brief Ajoute un item à une liste de prestige.
  *
  * Cette fonction crée un nouvel item de prestige et l'ajoute à la liste spécifiée.
  *
  * @param list Pointeur vers la liste de prestige où ajouter l'item.
  * @param name Nom de l'item.
  * @param description Description de l'item.
  * @param cost Coût de l'item en points de prestige.
  * @param effect Fonction appliquant l'effet de l'item.
  * @param value Valeur associée à l'effet de l'item.
  */
 void addPrestigeItem(prestigeList *list, char *name, char *description, int cost, void (*effect)(float), float value);
 
 /**
  * @fn int getMaxPrestigeItems(char *selectedTree)
  * @brief Récupère le nombre maximum d'items achetés dans un arbre de prestige.
  *
  * Cette fonction parcourt un arbre de prestige et retourne le nombre d'items achetés.
  *
  * @param selectedTree Nom de l'arbre de prestige ("Gold", "Damage", ou "Prestige").
  * @return Le nombre d'items achetés dans l'arbre.
  */
 int getMaxPrestigeItems(char *selectedTree);
 
 /**
  * @fn void goldPrestige(float value)
  * @brief Applique un effet de prestige augmentant les gains d'or.
  *
  * @param value Valeur de l'effet à appliquer.
  */
 void goldPrestige(float value);
 
 /**
  * @fn void damagePrestige(float value)
  * @brief Applique un effet de prestige augmentant les dégâts.
  *
  * @param value Valeur de l'effet à appliquer.
  */
 void damagePrestige(float value);
 
 /**
  * @fn void prestigePrestige(float value)
  * @brief Applique un effet de prestige augmentant les points de prestige.
  *
  * @param value Valeur de l'effet à appliquer.
  */
 void prestigePrestige(float value);
 
 /**
  * @fn void printTree()
  * @brief Affiche les informations des arbres de prestige dans la console.
  *
  * Cette fonction est utilisée pour le débogage et affiche les noms, descriptions,
  * coûts et états des items dans les arbres de prestige.
  */
 void printTree();
 
 /**
  * @fn void buyPrestigeItem(char *type, int index, int pay)
  * @brief Achète un item de prestige.
  *
  * Cette fonction vérifie si un item peut être acheté, applique son effet, et met à jour son état.
  *
  * @param type Nom de l'arbre de prestige ("Gold", "Damage", ou "Prestige").
  * @param index Index de l'item dans l'arbre.
  * @param pay Indique si le coût doit être déduit (1) ou non (0).
  */
 void buyPrestigeItem(char *type, int index, int pay);
 
 /**
  * @fn void checkDisplayPrestigeItemText()
  * @brief Vérifie et met à jour l'affichage des textes des items de prestige.
  *
  * Cette fonction gère l'affichage des descriptions et coûts des items de prestige
  * en fonction des interactions de l'utilisateur.
  */
 void checkDisplayPrestigeItemText();
 
 /**
  * @fn void destroyPrestigeList()
  * @brief Détruit les listes d'items de prestige.
  *
  * Cette fonction libère les ressources associées aux arbres de prestige.
  */
 void destroyPrestigeList();
 
 /**
  * @fn void refreshPrestigeTreeSize()
  * @brief Rafraîchit la taille de l'arbre de prestige.
  *
  * Cette fonction met à jour la taille de l'arbre de prestige en fonction de la taille de la fenêtre.
  */
 void refreshPrestigeTreeSize();
 /**
  * @fn void refreshPrestigePage()
  * @brief Rafraîchit la page de prestige.
  *
  * Cette fonction détruit et recrée la page de prestige pour refléter les changements.
  */
 void refreshPrestigePage();
 
 /**
  * @fn void prestigeKeepHero(float value)
  * @brief Applique un effet de prestige permettant de conserver les héros.
  *
  * @param value Valeur de l'effet à appliquer.
  */
 void prestigeKeepHero(float value);
 
 /**
  * @fn void prestigeKeepHeroLevel(float value)
  * @brief Applique un effet de prestige permettant de conserver le niveau des héros.
  *
  * @param value Valeur de l'effet à appliquer.
  */
 void prestigeKeepHeroLevel(float value);
 
 /**
  * @fn void doPrestige()
  * @brief Effectue une action de prestige.
  *
  * Cette fonction applique les effets de prestige, met à jour les points de prestige,
  * et rafraîchit la page de prestige.
  */
 void doPrestige();
 
 /**
  * @fn int canBuy(char *type, int index)
  * @brief Vérifie si un item de prestige peut être acheté.
  *
  * Cette fonction vérifie si les conditions nécessaires pour acheter un item de prestige
  * sont remplies (points de prestige suffisants, items précédents achetés, etc.).
  *
  * @param type Nom de l'arbre de prestige ("Gold", "Damage", ou "Prestige").
  * @param index Index de l'item dans l'arbre.
  * @return 1 si l'item peut être acheté, 0 sinon.
  */
 int canBuy(char *type, int index);
 
 #endif // PRESTIGE_H