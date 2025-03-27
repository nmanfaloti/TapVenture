#ifndef INPUT_USER_H
#define INPUT_USER_H

#include "SDL2/SDL.h"

/**
 * @file input_user.h
 * @brief Déclarations pour la gestion des entrées utilisateur dans TapVenture.
 * 
 * Ce fichier contient la déclaration de la fonction de traitement des événements SDL ainsi que
 * la déclaration des variables globales pour stocker les coordonnées de la souris.
 * 
 * \author Noam MANFALOTI & Lucas DUPONT & Ibrahim SAPIEV
 * \date 24 janvier 2025
 */

/**
 * \fn int input_event(SDL_Event event)
 * \brief Traite un événement SDL.
 *
 * Cette fonction gère un événement SDL (par exemple, un clic ou un mouvement de souris) et met
 * à jour les états correspondants dans le jeu.
 *
 * \param event L'événement SDL à traiter.
 * \return 1 si l'événement a été traité avec succès, 0 si l'événement demande la fermeture de l'application.
 */
int input_event(SDL_Event event);

/**
 * \brief Coordonnée X actuelle de la souris.
 */
extern int mouseX;

/**
 * \brief Coordonnée Y actuelle de la souris.
 */
extern int mouseY;

#endif // INPUT_USER_H