
#ifndef HEROS_H
#define HEROS_H

#include <stdbool.h>
/**
 * @file heros.h
 * @brief Interface de gestion des héros dans TapVenture.
 *
 * Ce fichier définit les constantes, types et fonctions pour l'initialisation et la gestion
 * des héros, incluant le calcul des dégâts, l'évolution et l'affichage.
 * @author Lucas DUPONT
 * @date 20 décembre 2024
 */

/**
 * @def NB_HEROS_PER_PAGE_H
 * @brief Nombre de héros affichés par ligne dans la page des héros.
 */
#define NB_HEROS_PER_PAGE_H 3

/**
 * @def NB_HEROS_PER_PAGE_W
 * @brief Nombre de héros affichés par colonne dans la page des héros.
 */
#define NB_HEROS_PER_PAGE_W 4

/**
 * @enum herosIndex
 * @brief Indice des héros.
 *
 * Enumération des indices des héros disponibles dans le jeu.
 */
typedef enum {
    HERO0,
    HERO1,
    HERO2,
    HERO3,
    HERO4,
    HERO5,
    HERO6,
    HERO7,
    HERO8,
    HERO9,
    HERO10,
    HERO11,
    HEROS_COUNT /**< Nombre total de héros */
} herosIndex;

/**
 * @struct hero
 * @brief Structure représentant un héros.
 *
 * Contient les informations relatives aux caractéristiques du héros telles que les
 * dégâts, le prix, le niveau, le cooldown entre les attaques et le temps de la dernière attaque.
 */
typedef struct {
    unsigned long long int degat;     /**< Dégâts infligés par le héros */
    unsigned long long int prix;       /**< Prix d'amélioration ou d'achat du héros */
    int level;                         /**< Niveau actuel du héros */
    int cooldown;                      /**< Temps de recharge entre les attaques (en ms) */
    unsigned int lastAttack;           /**< Horodatage de la dernière attaque */
} hero_t;

/**
 * @fn int initHeros()
 * @brief Initialise la structure de tous les héros.
 *
 * Cette fonction initialise les valeurs par défaut de chaque héros.
 *
 * @return 0 en cas de succès, un code d'erreur sinon.
 */
int initHeros();

/**
 * @fn int attackHeros()
 * @brief Effectue une attaque avec les héros.
 *
 * Cette fonction gère la logique de l'attaque des héros sur la cible.
 *
 * @return Un entier indiquant le résultat de l'attaque.
 */
int attackHeros();

/**
 * @fn int upgradeHero(int HeroIndex, bool pay)
 * @brief Améliore les caractéristiques d'un héros.
 *
 * Permet d'améliorer un héros en fonction de son indice et si le paiement est effectué.
 *
 * @param HeroIndex Indice du héros à améliorer.
 * @param pay Booléen indiquant si le paiement est effectué.
 * @return Un entier indiquant le succès ou l'échec de l'amélioration.
 */
int upgradeHero(int HeroIndex, bool pay);

/**
 * @fn int upgradeHeroCB(void * args[20])
 * @brief Callback pour l'amélioration d'un héros.
 *
 * Fonction callback utilisée lors de l'interaction avec l'interface graphique pour améliorer un héros.
 *
 * @param args Tableau de pointeurs d'arguments.
 * @return Un entier indiquant le résultat de l'opération.
 */
int upgradeHeroCB(void * args[20]);

/**
 * @fn int makeHeroAtLevel(int heroIndex, int level)
 * @brief Définit le niveau initial d'un héros.
 *
 * Permet de configurer le niveau d'un héros selon son indice.
 *
 * @param heroIndex Indice du héros.
 * @param level Niveau à attribuer.
 * @return Un entier indiquant le succès ou l'échec de l'opération.
 */
int makeHeroAtLevel(int heroIndex, int level);

/**
 * @fn unsigned long long int getHeroPriceByMultiplicator(int heroIndex)
 * @brief Calcule le prix d'achat du héros en fonction du multiplicateur choisi.
 *
 * Permet d'obtenir le prix actualisé selon le multiplicateur de l'achat.
 *
 * @param heroIndex Indice du héros.
 * @return Le prix calculé.
 */
unsigned long long int getHeroPriceByMultiplicator(int heroIndex);

/**
 * @fn unsigned long long int getHeroPrice(int heroIndex)
 * @brief Récupère le prix de base du héros.
 *
 * Retourne le prix courant du héros.
 *
 * @param heroIndex Indice du héros.
 * @return Le prix du héros.
 */
unsigned long long int getHeroPrice(int heroIndex);

/**
 * @fn unsigned long long int herosDPS(int indiceHero)
 * @brief Calcule les dégâts par seconde (DPS) d'un héros.
 *
 * Cette fonction calcule la contribution du héros en terme de DPS en fonction de son niveau et de ses caractéristiques.
 *
 * @param indiceHero Indice du héros.
 * @return Les dégâts par seconde du héros.
 */
unsigned long long int herosDPS(int indiceHero);

/**
 * @fn unsigned long long int herosAllDPS()
 * @brief Calcule le DPS total de tous les héros.
 *
 * Additionne le DPS de tous les héros actifs.
 *
 * @return Le DPS cumulé de tous les héros.
 */
unsigned long long int herosAllDPS();

/**
 * @fn unsigned long long int herosGoldGenBySec()
 * @brief Calcule la génération d'or par seconde des héros.
 *
 * Cette fonction sert à déterminer combien d'or est généré par les héros par seconde.
 *
 * @return L'or généré par seconde.
 */
unsigned long long int herosGoldGenBySec();

/**
 * @fn void initHerosPage()
 * @brief Initialise la page d'affichage des héros.
 *
 * Prépare la page dédiée à l'affichage des héros dans l'interface utilisateur.
 */
void initHerosPage();

/**
 * @fn void loadHerosMatrice()
 * @brief Charge la matrice d'affichage des héros.
 *
 * Organise les héros dans une grille pour l'affichage dans l'interface utilisateur.
 */
void loadHerosMatrice();

/**
 * @fn void updateHeroShopPage()
 * @brief Met à jour la page de la boutique des héros.
 *
 * Actualise l'affichage et les informations dans la boutique dédiée aux héros.
 */
void updateHeroShopPage();

/**
 * @fn void updateHeroIInShopPage(int heroIndex)
 * @brief Met à jour l'affichage d'un héros dans la boutique.
 *
 * Rafraîchit l'affichage des informations d'un héros donné dans la boutique.
 *
 * @param heroIndex Indice du héros à mettre à jour.
 */
void updateHeroIInShopPage(int heroIndex);

/**
 * @fn int changeMultiplicator()
 * @brief Change le multiplicateur d'achat.
 *
 * Permet de passer au multiplicateur suivant pour l'achat groupé dans la boutique des héros.
 *
 * @return Le nouveau multiplicateur.
 */
int changeMultiplicator();

/**
 * @brief Tableau global contenant tous les héros.
 */
extern hero_t heros[HEROS_COUNT];

#endif // HEROS_H