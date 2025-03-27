#ifndef COMBAT_H
#define COMBAT_H

#include "SDL2/SDL.h"

/**
 * \file combat.h
 * \brief Déclarations pour le système de combat du jeu TapVenture.
 * 
 * Ce fichier contient les déclarations des fonctions et variables pour gérer le système de combat
 * dans le jeu TapVenture, incluant les structures, les niveaux, les monstres, les attaques et les animations.
 * \author Noam MANFALOTI & Ibrahim SAPIEV
 * \date 20 décembre 2024
 */

/**
 * \struct monstreInfo
 * \brief Structure représentant les informations d'un monstre.
 *
 * Contient la santé actuelle, la santé initiale ainsi que les valeurs minimales et maximales
 * de pièces attribuées lors de sa défaite.
 */
typedef struct {
    unsigned long long int mobHealth; /*!< Santé actuelle du monstre */
    unsigned long long int iniHealth;   /*!< Santé initiale du monstre */
    unsigned long long int coinMax;     /*!< Nombre maximum de pièces attribuées */
    unsigned long long int coinMin;     /*!< Nombre minimum de pièces attribuées */
} monstreInfo;

/**
 * \struct levelInfo
 * \brief Structure représentant le niveau de combat.
 *
 * Contient la liste des monstres, les paramètres de temps, le nombre de monstres abattus,
 * le niveau courant, le niveau maximal, ainsi que des étiquettes et images associées.
 */
typedef struct {
    monstreInfo monstre[51]; /*!< Tableau contenant les informations des monstres */
    int timeToKill;          /*!< Temps nécessaire pour tuer le monstre courant */
    unsigned int startTimer; /*!< Timer de début du niveau */
    int mobKilled;           /*!< Nombre de monstres tués */
    int mobToKill;           /*!< Nombre de monstres à tuer pour terminer le niveau */
    int currentLvl;          /*!< Niveau de combat courant */
    int maxLevel;            /*!< Niveau maximal de combat atteint */
    char * label[5];         /*!< Tableau d'étiquettes pour le niveau ou les monstres */
    char * img[5];           /*!< Tableau des chemins vers les images correspondantes */
} levelInfo;

extern levelInfo level;

/**
 * \fn char * getCurrentMobLabel()
 * \brief Récupère l'étiquette du monstre courant.
 *
 * \return Une chaîne de caractères contenant l'étiquette du monstre courant.
 */
char * getCurrentMobLabel();

/**
 * \fn char * getCurrentMobImgPath()
 * \brief Récupère le chemin de l'image du monstre courant.
 *
 * \return Une chaîne de caractères contenant le chemin de l'image du monstre courant.
 */
char * getCurrentMobImgPath();

/**
 * \fn int attack(void * args[20])
 * \brief Effectue une attaque sur le monstre courant.
 *
 * \param args Tableau d'arguments nécessaires pour réaliser l'attaque.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int attack(void * args[20]);

/**
 * \fn int initLevel(monstreInfo monstre[])
 * \brief Initialise le niveau de combat.
 *
 * Configure et initialise les paramètres relatifs aux monstres et au niveau.
 *
 * \param monstre Tableau contenant les informations de chaque monstre.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int initLevel(monstreInfo monstre[]);

/**
 * \fn int InitBoss(int diffTime)
 * \brief Initialise le boss en fonction d'un délai donné.
 *
 * \param diffTime Délai pour l'initialisation du boss.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int InitBoss(int diffTime);

/**
 * \fn int isBoss(int currentLvl)
 * \brief Vérifie si le niveau courant correspond à un boss.
 *
 * \param currentLvl Niveau de combat actuel.
 *
 * \return 1 si le niveau correspond à un boss, 0 sinon.
 */
int isBoss(int currentLvl);

/**
 * \fn int setLevel(levelInfo *level, int lvl)
 * \brief Met à jour le niveau de combat.
 *
 * Change le niveau courant et actualise les paramètres associés.
 *
 * \param level Pointeur vers la structure levelInfo.
 * \param lvl Nouveau niveau à définir.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int setLevel(levelInfo *level, int lvl);

/**
 * \fn void mobHandler()
 * \brief Gère les comportements des monstres.
 *
 * Met à jour régulièrement l'état et le comportement des monstres pendant le combat.
 */
void mobHandler();

/**
 * \fn void mobAnimationHandler()
 * \brief Gère les animations des monstres.
 *
 * Actualise et gère les animations des monstres pendant le combat.
 */
void mobAnimationHandler();

/**
 * \fn void displayTimers()
 * \brief Affiche les minuteries liées au combat.
 *
 * Affiche notamment le timer du boss et autres minuteries relatives au niveau en cours.
 */
void displayTimers();

/**
 * \fn int changeLevel(void * l[20])
 * \brief Change le niveau de combat.
 *
 * Modifie le niveau courant en fonction des arguments passés.
 *
 * \param l Tableau d'arguments nécessaires pour effectuer le changement de niveau.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int changeLevel(void * l[20]);

#endif // COMBAT_H