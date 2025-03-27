#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <time.h>
/**
 * @file challenge.h
 * @brief Déclarations pour le système de challenge du jeu TapVenture.
 * 
 * Ce fichier contient les déclarations des fonctions et variables pour gérer le système de challenge
 * dans le jeu TapVenture, incluant la structure, le lancement, la réinitialisation et la mise à jour des challenges.
 * @author Ibrahim SAPIEV
 * @date 20 fevrier 2025
 */

/**
 * @struct Challenge_t
 * @brief Structure représentant un challenge dans le jeu.
 *
 * Contient les informations essentielles pour gérer un challenge, notamment son état,
 * le temps de démarrage, la durée, la cible à atteindre, la récompense octroyée, le cooldown
 * ainsi que le timestamp de la dernière exécution.
 */
typedef struct {
    int active;             /**< Indique si un challenge est en cours (1) ou non (0). */
    unsigned int startTime; /**< Instant de lancement du challenge (en millisecondes pour SDL). */
    int duration;           /**< Durée du challenge en secondes. */
    int target;             /**< Nombre de monstres à tuer pour réussir le challenge. */
    int reward;             /**< Récompense en or  accordée en cas de succès. */
    int cooldown;           /**< Durée d'attente (en secondes) entre deux challenges. */
    time_t lastTime;        /**< Timestamp de la fin du dernier challenge pour le cooldown. */
} Challenge_t;

/**
 * @brief Variable globale représentant le challenge.
 */
extern Challenge_t challenge;

/**
 * @fn int launchChallenge()
 * @brief Lance un nouveau challenge.
 * Cette fonction initialise un nouveau challenge si aucun challenge n'est en cours.
 *
 * @return int Retourne 0 en cas de succès, ou une valeur non nulle en cas d'erreur.
 */
int launchChallenge();

/**
 * @fn void resetChallenge()
 * @brief Réinitialise l'état du challenge.
 *
 * Permet de remettre à zéro le challenge en cours, notamment en réinitialisant le nombre
 * de monstres tués et en rétablissant le nombre de monstres à tuer par défaut.
 */
void resetChallenge();

/**
 * @fn void updateChallenge()
 * @brief Met à jour l'état du challenge.
 *
 * Cette fonction est appelée régulièrement pour vérifier l'avancée du challenge (temps écoulé,
 * progression, etc.) et déclencher la fin du challenge en cas d'échec ou de succès.
 */
void updateChallenge();

#endif // CHALLENGE_H