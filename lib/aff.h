/**
 * @file aff.h
 * @brief Interface de gestion de l'affichage des éléments UI dans TapVenture.
 * 
 * Ce fichier définit les structures et fonctions nécessaires pour gérer l'affichage
 * des textes et des images dans l'interface utilisateur, ainsi que leur mise à jour.
 * Il inclut également des outils pour manipuler les dimensions relatives à l'écran.
 * @author Noam MANFALOTI
 * 
 * @date 2 avril 2025
 */

 #ifndef AFF_H
 #define AFF_H
 
 #include "SDL2/SDL.h"
 #include "SDL2/SDL_ttf.h"
 
 /**
  * @struct uiTxt
  * @brief Structure représentant un texte dans l'interface utilisateur.
  *
  * Cette structure contient les informations nécessaires pour afficher un texte,
  * y compris son contenu, sa position, sa couleur et sa texture.
  */
 typedef struct uiTxt_s {
     char *chaine;           /**< Contenu du texte à afficher */
     char *label;            /**< Étiquette associée au texte */
     SDL_Rect dest;          /**< Rectangle définissant la position et la taille du texte */
     SDL_Color color;        /**< Couleur du texte */
     SDL_Texture *texture;   /**< Texture SDL du texte */
 } uiTxt;
 
 /**
  * @struct uiImg
  * @brief Structure représentant une image dans l'interface utilisateur.
  *
  * Cette structure contient les informations nécessaires pour afficher une image,
  * y compris son étiquette, sa position et sa texture.
  */
 typedef struct uiImg_s {
     char *label;            /**< Étiquette associée à l'image */
     SDL_Rect dest;          /**< Rectangle définissant la position et la taille de l'image */
     SDL_Texture *texture;   /**< Texture SDL de l'image */
 } uiImg;
 
 /**
  * @struct uiContainer
  * @brief Conteneur regroupant les textes et images de l'interface utilisateur.
  *
  * Cette structure permet de gérer un ensemble de textes et d'images dans l'interface utilisateur.
  */
 typedef struct uiContainer_s {
     int nbTxt;              /**< Nombre de textes dans le conteneur */
     uiTxt *txt;             /**< Tableau de textes */
     int nbImg;              /**< Nombre d'images dans le conteneur */
     uiImg *img;             /**< Tableau d'images */
 } uiContainer;
 
 extern int widthscreen;     /**< Largeur de l'écran */
 extern int heightscreen;    /**< Hauteur de l'écran */
 
 /**
  * @fn void affiche_txt(uiTxt *txt)
  * @brief Affiche un texte dans l'interface utilisateur.
  *
  * Cette fonction utilise les informations contenues dans la structure `uiTxt`
  * pour afficher un texte à l'écran.
  *
  * @param txt Pointeur vers la structure `uiTxt` contenant les informations du texte.
  */
 void affiche_txt(uiTxt *txt);
 
 /**
  * @fn SDL_Rect getSizeForText(TTF_Font* font, char *txt, SDL_Rect dest)
  * @brief Calcule la taille d'un texte en fonction de sa police et de son contenu.
  *
  * Cette fonction retourne un rectangle SDL définissant la taille et la position
  * du texte à afficher. Afin d'avoir un texte bien proportionné.
  *
  * @param font Pointeur vers la police TTF utilisée pour le texte.
  * @param txt Chaîne de caractères représentant le texte.
  * @param dest Rectangle de destination initial.
  *
  * @return Un rectangle SDL contenant les dimensions calculées.
  */
 SDL_Rect getSizeForText(TTF_Font* font, char *txt, SDL_Rect dest);
 
 /**
  * @fn int vw(float percent)
  * @brief Calcule une largeur relative à l'écran en pourcentage.
  *
  * @param percent Pourcentage de la largeur de l'écran.
  *
  * @return La largeur calculée en pixels.
  */
 int vw(float percent);
 
 /**
  * @fn int vh(float percent)
  * @brief Calcule une hauteur relative à l'écran en pourcentage.
  *
  * @param percent Pourcentage de la hauteur de l'écran.
  *
  * @return La hauteur calculée en pixels.
  */
 int vh(float percent);
 
 /**
  * @fn void uiHandle()
  * @brief Gère l'affichage des éléments de l'interface utilisateur.
  *
  * Cette fonction met à jour et affiche tous les textes et appelle l'affichage des images de l'interface utilisateur.
  */
 void uiHandle();
 
 /**
  * @fn void uiImgHandle()
  * @brief Gère l'affichage des images de l'interface utilisateur.
  *
  * Cette fonction met à jour et affiche toutes les images de l'interface utilisateur.
  */
 void uiImgHandle();
 
 /**
  * @fn void initUItxt()
  * @brief Initialise les textes de l'interface utilisateur.
  *
  * Initialise les structures nécessaires pour gérer les textes dans l'interface utilisateur.
  */
 void initUItxt();
 
 /**
  * @fn void refreshMobHealth()
  * @brief Met à jour l'affichage de la vie du monstre.
  */
 void refreshMobHealth();
 
 /**
  * @fn void refreshMobLabel()
  * @brief Met à jour l'affichage du nom du monstre.
  */
 void refreshMobLabel();
 
 /**
  * @fn void refreshMobTexture()
  * @brief Met à jour la texture du monstre.
  */
 void refreshMobTexture();
 
 /**
  * @fn void refreshUI()
  * @brief Met à jour l'interface utilisateur.
  *
  * Cette fonction rafraîchit tous les éléments de l'interface utilisateur.
  */
 void refreshUI();
 
 /**
  * @fn void refreshMobKilled()
  * @brief Met à jour l'affichage du nombre de mobs tués.
  */
 void refreshMobKilled();
 
 /**
  * @fn void refreshCurrentLvl()
  * @brief Met à jour l'affichage du niveau actuel.
  */
 void refreshCurrentLvl();
 
 /**
  * @fn void setUiText(uiTxt *txt, char *text)
  * @brief Définit le contenu d'un texte dans l'interface utilisateur.
  *
  * Cette fonction met à jour le contenu d'un texte et recrée sa texture.
  *
  * @param txt Pointeur vers la structure `uiTxt` à mettre à jour.
  * @param text Chaîne de caractères représentant le nouveau contenu du texte.
  */
 void setUiText(uiTxt *txt, char *text);


 extern void init_inv_page();
 
 #endif // AFF_H