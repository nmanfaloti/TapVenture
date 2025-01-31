#ifndef UI_H
#define UI_H

#include "aff.h"
#include "button.h"

typedef struct uiPage_s{
    uiContainer *container;
    ListeButton *buttonsList;
    ListeButtonImg *buttonsImgList;
    void (*init)(void); // Fonction d'initialisation de la page
}uiPage;

extern uiPage * currentpage;

void createButton(uiPage * page,SDL_Rect rect, SDL_Color color, int txtInd, int * info,float growEffect,SDL_Color colorHover,  int (*callFunction)(void **), int numArgs, ...);
void createImgButton(uiPage * page,SDL_Rect rect, char *texture, char *background, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...);

void createUIText(uiPage * page,int tradID, int * info, SDL_Rect dest, SDL_Color color);

void createPage(uiPage * page);
void destroyPage(uiPage * page);
void changePage(uiPage * page);
#endif // UI_H