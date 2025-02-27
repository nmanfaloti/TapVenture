#ifndef UI_H
#define UI_H

#include "aff.h"
#include "button.h"

#define MAX_NOTIF 10

typedef struct uiPage_s{
    uiContainer *container;
    ListeButton *buttonsList;
    ListeButtonImg *buttonsImgList;
    void (*init)(void); // Fonction d'initialisation de la page
}uiPage;

typedef struct uiNotif_s{
    char * title;
    char ** desc;
    int nbLignes;
    char * imgBackground;
    int tapToClose;
    int duration;
    int yOffset;
    SDL_Rect dest;
}Notif;

typedef struct uiNotifList_s{
    Notif *notif;
    int nbNotif;
}NotifList;

extern uiPage * currentpage;

void createButton(uiPage * page,SDL_Rect rect, SDL_Color color, int txtInd, int * info,float growEffect,SDL_Color colorHover,  int (*callFunction)(void **), int numArgs, ...);
void createImgButton(uiPage * page,SDL_Rect rect, char *texture, char *background, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...);

void createUIText(uiPage * page,int tradID, int * info, SDL_Rect dest, SDL_Color color);

void createPage(uiPage * page);
void destroyPage(uiPage * page);
int changePage(void * args[20]);

void createNotif(char * title, char * imgBackground, int tapToClose, int duration , SDL_Rect dest ,int yOffset, int nbLignes, ...);
void deleteNotif(int index);
void showNotif(Notif * notif);
void initNotifList();
void uiNotifHandle();
void destroyNotifList();

#endif // UI_H