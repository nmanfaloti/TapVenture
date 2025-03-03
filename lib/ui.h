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

typedef struct uiPageHolder_s{
    uiPage * page;
    int pageNb;
}uiPageHolder;

typedef struct uiNotif_s{
    int tradId;
    char ** desc;
    char * label;
    int nbLignes;
    char * imgBackground;
    int tapToClose;
    int duration;
    int messYOffset;
    int titleYOffset;
    float messSize;
    float titleSize;
    SDL_Rect dest;
}Notif;

typedef struct uiNotifList_s{
    Notif *notif;
    int nbNotif;
}NotifList;

extern uiPage * currentpage;
extern uiPageHolder pageHolder;

void createButton(uiPage * page,SDL_Rect rect, char * pathImg, char *hoverPath,TTF_Font *Textfont,SDL_Color color, int txtInd, int * info,float growEffect, int (*callFunction)(void **), int numArgs, ...);
void createImgButton(uiPage * page,SDL_Rect rect, char *pathImg, char *pathBackground, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...);
void createUIText(uiPage * page,TTF_Font* font,int tradID,int * info, SDL_Rect dest, SDL_Color color, char * label);
void setButtonText(Button * button, const char * txt);
uiTxt * getTxtFromLabel(char * label);

void createPage(uiPage * page);
void destroyPage(uiPage * page);
int changePage(void * args[20]);

void initMainPage();
void initSettingsPage();
void initPage();
void destroyAllPages();

char * createNotif(int tradId, int titleYOffset,float titleSize,char * imgBackground, int tapToClose, int duration, SDL_Rect dest,int messYOffset,float messSize, int nbLignes, ...);
void showNotif(Notif * notif);
void initNotifList();
void uiNotifHandle();
void destroyNotifList();

#endif // UI_H