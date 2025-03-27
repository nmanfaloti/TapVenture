#ifndef AUDIO_H
#define AUDIO_H 

#include <stdbool.h>
#include "../lib/sdl.h"

/**
 * @file audio.h
 * @brief Interface de gestion audio pour TapVenture.
 *
 * Ce fichier définit les types et fonctions permettant la gestion de l'audio
 * dans TapVenture, incluant le chargement, la lecture et la libération des ressources
 * audio pour les musiques et effets sonores.
 * @author Lucas DUPONT
 * @date 23 mars 2025
 */

/**
 * @enum musicIndex
 * @brief Enumération des indices pour les musiques et effets sonores.
 */
typedef enum {
    MUSIC_GAME,          /**< Musique de jeu principale */
    MUSIC_CLICK,         /**< Effet sonore lors d'un clic */
    MUSIC_LEVEL_UP,      /**< Effet sonore pour l'augmentation de niveau */
    MUSIC_LEVEL_UP_BOSS, /**< Effet sonore pour l'augmentation de niveau face au boss */
    MUSIC_COIN,          /**< Effet sonore de collecte de pièce */
    MUSIC_ATTACK,        /**< Effet sonore d'attaque */
    MUSIC_COUNT          /**< Nombre total d'éléments */
} musicIndex;

/**
 * @enum canalsIndex
 * @brief Enumération des indices pour les canaux audio.
 */
typedef enum {
    CANAL_MUSIC_BACK, /**< Canal pour la musique de fond */
    CANAL_EFFECT,     /**< Canal pour les effets sonores */
    CANAL_COUNT       /**< Nombre total de canaux */
} canalsIndex;

/**
 * @fn int initMusic()
 * @brief Initialise le système audio pour TapVenture.
 *
 * Configure et initialise les ressources nécessaires à la gestion audio 
 * pour le jeu.
 *
 * @return 0 en cas de succès, une valeur différente en cas d'erreur.
 */
int initMusic();

/**
 * @fn bool loadMusic(musicIndex musicIndex, char * path)
 * @brief Charge un fichier audio à partir du chemin donné.
 *
 * Charge la musique ou l'effet sonore associé à l'indice spécifié.
 *
 * @param musicIndex Indice correspondant à la musique ou l'effet sonore à charger.
 * @param path Chemin vers le fichier audio.
 * @return true si le chargement est réussi, false sinon.
 */
bool loadMusic(musicIndex musicIndex, char * path);

/**
 * @fn void playMusic(int musicIndex, canalsIndex canalsIndex, int nbLoops)
 * @brief Joue une musique ou un effet sonore.
 *
 * Lance la lecture de l'audio spécifié sur le canal choisi.
 *
 * @param musicIndex Indice de la musique ou de l'effet sonore à jouer.
 * @param canalsIndex Indice du canal sur lequel jouer l'audio.
 * @param nbLoops Nombre de fois que l'audio doit être reproduit (0 pour boucle infinie).
 */
void playMusic(int musicIndex, canalsIndex canalsIndex, int nbLoops);

/**
 * @fn void stopAllMusic()
 * @brief Arrête la lecture de tous les audios.
 *
 * Stoppe la lecture de toutes les musiques et effets sonores en cours.
 */
void stopAllMusic();

/**
 * @fn void freeAllMusic()
 * @brief Libère les ressources audio.
 *
 * Libère toutes les ressources allouées pour la gestion audio.
 */
void freeAllMusic();

#endif // AUDIO_H