/**
 * @file ui.h
 * @brief Interface de gestion de l'interface utilisateur (UI) dans TapVenture.
 *
 * Ce fichier contient les déclarations des structures et fonctions nécessaires
 * pour gérer les pages, les boutons, les textes, les images et les notifications
 * dans l'interface utilisateur du jeu.
 * Il contient toutes les elements/fonctions de l'interface utilisateur qui ont besoin d'éléments de bouton et d'aff.
 */

 #ifndef UI_H
 #define UI_H
 
 #include "aff.h"
 #include "button.h"
 
 #define MAX_NOTIF 10 /**< Nombre maximum de notifications affichables. */
 
 /**
  * @struct uiPage_s
  * @brief Structure représentant une page de l'interface utilisateur.
  *
  * Cette structure contient les éléments d'une page, tels que les conteneurs,
  * les listes de boutons et les fonctions d'initialisation.
  */
 typedef struct uiPage_s {
     uiContainer *container; /**< Conteneur des éléments de la page. */
     ListeButton *buttonsList; /**< Liste des boutons de la page. */
     ListeButtonImg *buttonsImgList; /**< Liste des boutons d'image de la page. */
     void (*init)(void); /**< Fonction d'initialisation de la page. */
 } uiPage;
 
 /**
  * @struct uiPageHolder_s
  * @brief Structure contenant les pages de l'interface utilisateur.
  *
  * Cette structure permet de gérer plusieurs pages et de suivre la page active.
  */
 typedef struct uiPageHolder_s {
     uiPage *page; /**< Tableau de pages. */
     int pageNb; /**< Nombre total de pages. */
 } uiPageHolder;
 
 /**
  * @struct uiNotif_s
  * @brief Structure représentant une notification dans l'interface utilisateur.
  *
  * Cette structure contient les informations nécessaires pour afficher une notification,
  * telles que le texte, l'image de fond, la durée et les positions.
  */
 typedef struct uiNotif_s {
     char *desc; /**< Description de la notification. */
     char *imgBackground; /**< Chemin de l'image de fond. */
     int duration; /**< Durée d'affichage de la notification (en secondes). */
     int messYOffset; /**< Décalage vertical du message. */
     int titleYOffset; /**< Décalage vertical du titre. */
     float messSize; /**< Taille du texte du message. */
     float titleSize; /**< Taille du texte du titre. */
     SDL_Rect dest; /**< Position et taille de la notification. */
     SDL_Texture *descTexture; /**< Texture du message. */
     SDL_Rect descRect; /**< Rectangle du message. */
     SDL_Texture *titleTexture; /**< Texture du titre. */
     SDL_Rect titleRect; /**< Rectangle du titre. */
     SDL_Texture *imgTexture; /**< Texture de l'image de fond. */
 } Notif;
 
 /**
  * @struct uiNotifList_s
  * @brief Structure contenant une liste de notifications.
  *
  * Cette structure permet de gérer plusieurs notifications simultanément.
  */
 typedef struct uiNotifList_s {
     Notif *notif; /**< Tableau de notifications. */
     int nbNotif; /**< Nombre de notifications actives. */
 } NotifList;
 
 extern uiPage *currentpage; /**< Pointeur vers la page actuellement affichée. */
 extern uiPageHolder pageHolder; /**< Gestionnaire des pages de l'interface utilisateur. */
 
 /**
  * @brief Crée un bouton dans une page.
  *
  * @param page La page où ajouter le bouton.
  * @param rect Rectangle définissant la position et la taille du bouton.
  * @param pathImg Chemin de l'image du bouton. (SVG)
  * @param hoverPath Chemin de l'image du bouton en survol.
  * @param Textfont Police utilisée pour le texte du bouton.
  * @param color Couleur du texte.
  * @param txtInd Indice du texte.
  * @param info Informations supplémentaires.
  * @param growEffect Effet de grossissement au survol.
  * @param callFunction Fonction appelée lors du clic sur le bouton.
  * @param numArgs Nombre d'arguments passés à la fonction.
  * @param ... Arguments supplémentaires.
  */
 void createButton(uiPage *page, SDL_Rect rect, char *pathImg, char *hoverPath, TTF_Font *Textfont, SDL_Color color, int txtInd, int *info, float growEffect, int (*callFunction)(void **), int numArgs, ...);
 
 /**
  * @brief Crée un bouton d'image dans une page. Si pathImg (icon) est NULL le pathBackground (fond) utilise un PNG au lieu d'un SVG.
  *
  * 
  * @param page La page où ajouter le bouton.
  * @param rect Rectangle définissant la position et la taille du bouton.
  * @param pathImg Chemin de l'image du bouton. (SVG ou NULL)
  * @param pathBackground Chemin de l'image de fond. (SVG/PNG)
  * @param offsetLogoX Décalage horizontal du logo.
  * @param offsetLogoY Décalage vertical du logo.
  * @param callFunction Fonction appelée lors du clic sur le bouton.
  * @param label Étiquette du bouton.
  * @param numArgs Nombre d'arguments passés à la fonction.
  * @param ... Arguments supplémentaires.
  */
 void createImgButton(uiPage *page, SDL_Rect rect, char *pathImg, char *pathBackground, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), char *label, int numArgs, ...);
 
 /**
  * @brief Crée un texte dans une page.
  *
  * @param page La page où ajouter le texte.
  * @param font Police utilisée pour le texte.
  * @param chaine Contenu du texte.
  * @param dest Rectangle définissant la position et la taille du texte.
  * @param color Couleur du texte.
  * @param label Étiquette du texte.
  */
 void createUIText(uiPage *page, TTF_Font *font, char *chaine, SDL_Rect dest, SDL_Color color, char *label);
 
 /**
  * @brief Définit le texte d'un bouton.
  *
  * @param button Le bouton à modifier.
  * @param txt Le texte à définir.
  */
 void setButtonText(Button *button, const char *txt);
 
 /**
  * @brief Récupère un texte à partir de son étiquette.
  *
  * @param label L'étiquette du texte.
  * @return Pointeur vers le texte correspondant.
  */
 uiTxt *getTxtFromLabel(char *label);
 
 /**
  * @brief Crée une image dans une page.
  *
  * @param page La page où ajouter l'image. (PNG)
  * @param path Chemin de l'image.
  * @param dest Rectangle définissant la position et la taille de l'image.
  * @param label Étiquette de l'image.
  */
 void createUIImg(uiPage *page, char *path, SDL_Rect dest, char *label);
 
 /**
  * @brief Récupère une image à partir de son étiquette.
  *
  * @param label L'étiquette de l'image.
  * @return Pointeur vers l'image correspondante.
  */
 uiImg *getImgFromLabel(char *label);
 
 /**
  * @brief Crée une page.
  *
  * @param page La page à créer.
  */
 void createPage(uiPage *page);
 
 /**
  * @brief Détruit une page.
  *
  * @param page La page à détruire.
  */
 void destroyPage(uiPage *page);
 
 /**
  * @brief Change la page affichée.
  *
  * @param args Arguments nécessaires pour le changement de page.
  * @return Code de retour.
  */
 int changePage(void *args[20]);
 
 /**
  * @brief Initialise la page principale.
  */
 void initMainPage();
 
 /**
  * @brief Initialise la page des paramètres.
  */
 void initSettingsPage();
 
 /**
  * @brief Initialise une page.
  */
 void initPage();
 
 /**
  * @brief Détruit toutes les pages.
  */
 void destroyPages();
 
 /**
  * @brief Détruit toutes les pages et leurs contenus.
  */
 void destroyAllPages();
 
 /**
  * @brief Détruit une image de l'interface utilisateur.
  *
  * @param img L'image à détruire.
  * @param page La page contenant l'image.
  */
 void destroyUIImg(uiImg *img, uiPage *page);
 
 /**
  * @brief Détruit un texte de l'interface utilisateur.
  *
  * @param txt Le texte à détruire.
  * @param page La page contenant le texte.
  */
 void destroyUITxt(uiTxt *txt, uiPage *page);
 
 /**
  * @brief Crée une notification.
  *
  * @param title Titre de la notification.
  * @param titleYOffset Décalage vertical du titre.
  * @param titleSize Taille du titre.
  * @param imgBackground Chemin de l'image de fond. (PNG)
  * @param duration Durée d'affichage de la notification.
  * @param dest Rectangle définissant la position et la taille de la notification.
  * @param messYOffset Décalage vertical du message.
  * @param messSize Taille du message.
  * @param notifMess Contenu du message.
  */
 void createNotif(char *title, int titleYOffset, float titleSize, char *imgBackground, int duration, SDL_Rect dest, int messYOffset, float messSize, char *notifMess);
 
 /**
  * @brief Affiche une notification.
  *
  * @param notif La notification à afficher.
  */
 void showNotif(Notif *notif);
 
 /**
  * @brief Initialise la liste des notifications.
  */
 void initNotifList();
 
 /**
  * @brief Gère l'affichage des notifications.
  */
 void uiNotifHandle();
 
 /**
  * @brief Détruit la liste des notifications.
  */
 void destroyNotifList();
 
 #endif // UI_H