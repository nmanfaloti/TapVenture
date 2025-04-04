/**
 * @file button.h
 * @brief Interface de gestion des boutons dans TapVenture.
 *
 * Ce fichier définit les structures et fonctions nécessaires pour créer, afficher,
 * et gérer les boutons dans l'interface utilisateur. Il inclut également des outils
 * pour manipuler les textures, les événements de clic, et les états des boutons.
 * @author Noam MANFALOTI
 * @date 2 avril 2025
 */

 #ifndef BUTTON_H
 #define BUTTON_H 
 
 #include "sdl.h"
 
 /**
  * @struct Button
  * @brief Structure représentant un bouton dans l'interface utilisateur.
  *
  * Cette structure contient les informations nécessaires pour afficher et gérer un bouton,
  * y compris ses dimensions, ses textures, son texte, et les fonctions associées.
  */
 typedef struct {
     SDL_Rect rect;                  /**< Rectangle définissant la position et la taille du bouton */
     SDL_Rect iniRect;               /**< Rectangle initial du bouton */
     char *pathImg;                  /**< Chemin vers l'image de fond du bouton */
     float growEffect;               /**< Effet de grossissement lors du survol */
     int *info;                      /**< Informations supplémentaires associées au bouton */
     int text;                       /**< Identifiant du texte (clé d'énumération pour la traduction) */
     int (*callFunction)(void **);   /**< Pointeur sur la fonction exécutée lors du clic */
     void **args;                    /**< Arguments passés à la fonction associée */
     SDL_Texture *texture;           /**< Texture de l'image de fond du bouton */
     SDL_Texture *initialTexture;    /**< Texture initiale du bouton */
     SDL_Texture *selectedTexture;   /**< Texture affichée lors du survol du bouton */
     SDL_Texture *textTexture;       /**< Texture du texte du bouton */
     SDL_Rect textRect;              /**< Rectangle définissant la position du texte */
     SDL_Rect textIniRect;           /**< Rectangle initial définissant la position du texte */
     SDL_Color textColor;            /**< Couleur du texte */
     TTF_Font *font;                 /**< Police utilisée pour le texte */
 } Button;
 
 /**
  * @struct ButtonImg
  * @brief Structure représentant un bouton avec une image dans l'interface utilisateur.
  *
  * Cette structure contient les informations nécessaires pour afficher et gérer un bouton
  * avec une image, y compris ses dimensions, ses textures, et les fonctions associées.
  */
 typedef struct {
     SDL_Rect rect;                  /**< Rectangle définissant la position et la taille du bouton */
     SDL_Rect iniRect;               /**< Rectangle initial du bouton */
     char *pathImg;                  /**< Chemin vers l'image principale du bouton */
     char *pathBackground;           /**< Chemin vers l'image de fond du bouton */
     int offsetLogoX;                /**< Décalage horizontal de l'image */
     int offsetLogoY;                /**< Décalage vertical de l'image */
     int (*callFunction)(void **);   /**< Pointeur sur la fonction exécutée lors du clic */
     void **args;                    /**< Arguments passés à la fonction associée */
     SDL_Texture *imgTexture;        /**< Texture de l'image principale du bouton */
     SDL_Texture *backgroundTexture; /**< Texture de l'image de fond du bouton */
     char *label;                    /**< Étiquette associée au bouton */
 } ButtonImg;
 
 /**
  * @struct ListeButton
  * @brief Structure représentant une liste de boutons.
  *
  * Cette structure permet de gérer un ensemble de boutons dans une page de l'interface utilisateur.
  */
 typedef struct {
     int nbButton;                   /**< Nombre de boutons dans la liste */
     Button *buttons;                /**< Tableau de boutons */
 } ListeButton;
 
 /**
  * @struct ListeButtonImg
  * @brief Structure représentant une liste de boutons avec des images.
  *
  * Cette structure permet de gérer un ensemble de boutons avec des images dans une page de l'interface utilisateur.
  */
 typedef struct {
     int nbButton;                   /**< Nombre de boutons avec des images dans la liste */
     ButtonImg *buttons;             /**< Tableau de boutons avec des images */
 } ListeButtonImg;
 
 /**
  * @fn void ButtonHandle()
  * @brief Gère l'affichage et les interactions des boutons.
  *
  * Cette fonction met à jour l'état des boutons, gère les événements de clic,
  * et applique les effets visuels associés.
  */
 void ButtonHandle();
 
 /**
  * @fn void initListButton(ListeButton *listeButton)
  * @brief Initialise une liste de boutons.
  *
  * Alloue la mémoire nécessaire pour gérer une liste de boutons et initialise ses valeurs.
  *
  * @param listeButton Pointeur vers la liste de boutons à initialiser.
  */
 void initListButton(ListeButton *listeButton);
 
 /**
  * @fn void initListButtonImg(ListeButtonImg *listeButtonImg)
  * @brief Initialise une liste de boutons avec des images.
  *
  * Alloue la mémoire nécessaire pour gérer une liste de boutons avec des images et initialise ses valeurs.
  *
  * @param listeButtonImg Pointeur vers la liste de boutons avec des images à initialiser.
  */
 void initListButtonImg(ListeButtonImg *listeButtonImg);
 
 /**
  * @fn void refreshButtonShop()
  * @brief Met à jour l'affichage des boutons de la boutique.
  *
  * Cette fonction met à jour les textes et les états des boutons de la boutique en fonction des données actuelles.
  */
 void refreshButtonShop();
 
 /**
  * @fn ButtonImg *getImgButtonFromLabel(char *label)
  * @brief Récupère un bouton avec une image à partir de son étiquette.
  *
  * Parcourt la liste des boutons avec des images pour trouver celui correspondant à l'étiquette spécifiée.
  *
  * @param label Étiquette du bouton recherché.
  * @return Pointeur vers le bouton correspondant, ou `NULL` si aucun bouton ne correspond.
  */
 ButtonImg *getImgButtonFromLabel(char *label);
 
 /**
  * @fn void setImgButtonTexture(ButtonImg *button, char *pathImg, char *backgroundImg)
  * @brief Définit les textures d'un bouton avec une image.
  *
  * Charge et applique les textures principale et de fond pour un bouton avec une image.
  *
  * @param button Pointeur vers le bouton à modifier.
  * @param pathImg Chemin vers l'image principale.
  * @param backgroundImg Chemin vers l'image de fond.
  */
 void setImgButtonTexture(ButtonImg *button, char *pathImg, char *backgroundImg);
 
 /**
  * @fn void destroyAllButtonFromPage(ListeButton *listeButton)
  * @brief Détruit tous les boutons d'une page.
  *
  * Libère les ressources associées à tous les boutons d'une liste.
  *
  * @param listeButton Pointeur vers la liste de boutons à détruire.
  */
 void destroyAllButtonFromPage(ListeButton *listeButton);
 
 /**
  * @fn void destroyAllButtonImgFromPage(ListeButtonImg *listeButtonImg)
  * @brief Détruit tous les boutons avec des images d'une page.
  *
  * Libère les ressources associées à tous les boutons avec des images d'une liste.
  *
  * @param listeButtonImg Pointeur vers la liste de boutons avec des images à détruire.
  */
 void destroyAllButtonImgFromPage(ListeButtonImg *listeButtonImg);
 
 /**
  * @fn void drawButton(Button *button)
  * @brief Dessine un bouton à l'écran.
  *
  * Affiche le bouton en utilisant ses textures et ses dimensions.
  *
  * @param button Pointeur vers le bouton à dessiner.
  */
 void drawButton(Button *button);
 
 /**
  * @fn int checkButton(SDL_Rect rect, int mouse_x, int mouse_y)
  * @brief Vérifie si un clic de souris se trouve dans un bouton.
  *
  * @param rect Rectangle définissant la position et la taille du bouton.
  * @param mouse_x Position X de la souris.
  * @param mouse_y Position Y de la souris.
  * @return 1 si le clic est dans le bouton, 0 sinon.
  */
 int checkButton(SDL_Rect rect, int mouse_x, int mouse_y);
 
 /**
  * @fn SDL_Rect getRectForCentenredCord(int x, int y, int w, int h)
  * @brief Calcule un rectangle centré à partir de coordonnées.
  *
  * @param x Coordonnée X du centre.
  * @param y Coordonnée Y du centre.
  * @param w Largeur du rectangle.
  * @param h Hauteur du rectangle.
  * @return Rectangle SDL centré.
  */
 SDL_Rect getRectForCentenredCord(int x, int y, int w, int h);
 
 /**
  * @fn void drawButtonImg(ButtonImg *button)
  * @brief Dessine un bouton avec une image à l'écran.
  *
  * Affiche le bouton avec ses textures et ses dimensions.
  *
  * @param button Pointeur vers le bouton avec une image à dessiner.
  */
 void drawButtonImg(ButtonImg *button);
 
 #endif // BUTTON_H