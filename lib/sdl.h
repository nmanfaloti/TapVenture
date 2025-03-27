#ifndef sdl_H
#define sdl_H


#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

/**
 * @file sdl.h
 * @brief Déclarations pour l'initialisation, la gestion et la fermeture du sous-système SDL dans TapVenture.
 *
 * Ce fichier contient les déclarations des fonctions d'initialisation de SDL, de gestion de la fenêtre et du renderer,
 * ainsi que celles permettant de libérer les ressources associées (textures, polices et curseurs).
 * 
 * @author Noam MANFALOTI
 * @date 25 janvier 2025
 */

/**
 * @def LINK_FONT
 * @brief Chemin du fichier de police utilisé par défaut dans TapVenture.
 */
#define LINK_FONT "assets/fonts/ChakraPetch-SemiBold.ttf"

/**
 * @brief Pointeur vers la fenêtre principale SDL.
 */
extern SDL_Window *window;

/**
 * @brief Pointeur vers le renderer SDL.
 */
extern SDL_Renderer *renderer;

/**
 * @brief Pointeur vers la police de caractères de taille standard.
 */
extern TTF_Font *font;

/**
 * @brief Pointeur vers la police de caractères de grande taille.
 */
extern TTF_Font *fontBig;

/**
 * @brief Pointeur vers le curseur actuellement utilisé dans le jeu.
 */
extern SDL_Cursor *cursor;

/**
 * @brief Pointeur vers le curseur classique (par défaut).
 */
extern SDL_Cursor *classicCursor;

/**
 * @brief Pointeur vers le curseur en forme d'épée.
 */
extern SDL_Cursor *swordCursor;

/**
 * @brief Pointeur vers le curseur en forme de main.
 */
extern SDL_Cursor *handCursor;

/**
 * @fn int init_SDL()
 * @brief Initialise le sous-système SDL et ses modules associés.
 *
 * Cette fonction initialise SDL, SDL_ttf, SDL_image et SDL_mixer. Elle crée également la fenêtre
 * et le renderer utilisés pour afficher le jeu.
 *
 * @return 0 en cas de succès, un code d'erreur sinon.
 */
int init_SDL();

/**
 * @fn void SDLExit()
 * @brief Ferme SDL et libère toutes les ressources associées.
 *
 * Cette fonction détruit la fenêtre, le renderer, les curseurs, libère les polices et
 * appelle les fonctions de fermeture pour SDL_mixer, SDL_ttf et SDL_image.
 */
void SDLExit();

/**
 * @fn void SDLScreenHandler()
 * @brief Gère le rafraîchissement de l'écran.
 *
 * Cette fonction efface le renderer, met à jour la scène et affiche le contenu rendu à l'écran.
 * Elle est généralement appelée dans la boucle principale du jeu.
 */
void SDLScreenHandler();

#endif // sdl_H