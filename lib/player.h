#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>

/**
 * \file player.h
 * \brief Déclarations pour la gestion du joueur dans TapVenture.
 *
 * Ce fichier contient les définitions, déclarations et macros nécessaires pour la gestion des données du joueur,
 * incluant le suivi de l'or et des dégâts infligés lors des interactions de jeu.
 * 
 * \author Noam MANFALOTI & Lucas DUPONT & Ibrahim SAPIEV
 * \date 24 janvier 2025
 */

/**
 * \def LLD_MAX
 * \brief Valeur maximale limite pour les entiers non signés de type long long.
 */
#define LLD_MAX 802379605485813759

/**
 * \brief Or possédé par le joueur.
 */
extern unsigned long long int gold;

/**
 * \brief Dégâts effectués par le joueur sur un clic.
 */
extern unsigned long long int damage_click;

/**
 * \fn void goldGainOffline(time_t lastSaveTime)
 * \brief Calcule et applique le gain d'or hors ligne.
 *
 * Cette fonction détermine la quantité d'or gagnée pendant l'absence du joueur, en fonction du temps écoulé
 * depuis la dernière sauvegarde, et met à jour la valeur de l'or en conséquence et affiche une notification.
 *
 * \param lastSaveTime Timestamp de la dernière sauvegarde.
 */
void goldGainOffline(time_t lastSaveTime);

/**
 * \fn void addGold(long long int goldToAdd)
 * \brief Ajoute une quantité d'or au joueur.
 *
 * Cette fonction ajoute la valeur spécifiée à l'or du joueur en tenant compte du multiplicateur éventuel.
 * Si l'ajout excède la capacité maximale, l'or est plafonné à LLD_MAX.
 *
 * \param goldToAdd Quantité d'or à ajouter.
 */
void addGold(long long int goldToAdd);

/**
 * \fn void setPlayerDamage(unsigned long long int damage)
 * \brief Met à jour les dégâts de base du joueur.
 *
 * Cette fonction définit la valeur des dégâts effectués par le joueur lors d'une action de clic.
 * Elle met également à jour l'affichage en conséquence.
 *
 * \param damage Nouvelle valeur de dégât à appliquer.
 */
void setPlayerDamage(unsigned long long int damage);

#endif // PLAYER_H