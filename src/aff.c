#include <SDL2/SDL.h>   
#include <stdlib.h>   
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/aff.h"
#include "../lib/ui.h"

#include "../lib/lang.h"
#include "../lib/sdl.h"
#include "../lib/combat.h"
#include "../lib/button.h"
#include "../lib/player.h"
#include "../lib/boutique.h"

int widthscreen = 800;
int heightscreen = 500;

uiPage * currentpage;
uiPage mainpage;
uiPage settingspage;

void affiche_txt(SDL_Renderer* pRenderer, TTF_Font* font, char * txt, SDL_Rect dest, SDL_Color color){
    SDL_Surface* texte = TTF_RenderText_Blended(font, txt, color); // Crée un surface qui contient le texte

    if (texte == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, texte); // Crée la texture qu'on va afficher a partir de la surface    
    if (texture == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }
    double angle = 0;
    SDL_Point center = { widthscreen / 2, heightscreen / 2 };
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_RenderCopyEx(pRenderer, texture, NULL, &dest, angle, &center, flip);
    SDL_FreeSurface(texte);
    texture = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
}

SDL_Rect getSizeForText(TTF_Font* font, char * txt, SDL_Rect dest){
    int w, h;
    TTF_SizeText(font, txt, &w, &h);
    return (SDL_Rect){dest.x, dest.y, w, h};
}

// Viewport width
int vw(float percent) {
    return (int)(widthscreen * percent / 100.0);
}
//Viewport height
int vh(float percent) {
    return (int)(heightscreen * percent / 100.0);
}

void createUIText(uiPage * page,int tradID, int * info, SDL_Rect dest, SDL_Color color){
    if (page->container->txt == NULL) {
        page->container->txt = malloc(sizeof(uiTxt));
    } else {
        page->container->txt = realloc(page->container->txt, sizeof(uiTxt) * (page->container->nbTxt + 1));
    }
    page->container->txt[page->container->nbTxt].tradID = tradID;
    page->container->txt[page->container->nbTxt].info = info;
    page->container->txt[page->container->nbTxt].dest = dest;
    page->container->txt[page->container->nbTxt].color = color;
    page->container->nbTxt++;
}


void uiHandle(){
    char *txt;
    char info[50];
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        if (currentpage->container->txt[i].info != NULL){
            sprintf(info, "%d", *currentpage->container->txt[i].info);
            const char *trad = Traduction(currentpage->container->txt[i].tradID);
            txt = malloc(strlen(trad) + strlen(info) + 3);
            sprintf(txt, "%s: %s", trad, info);

        } else {
            const char *trad = Traduction(currentpage->container->txt[i].tradID);
            txt = malloc(strlen(trad) + 1);
            sprintf(txt, "%s", trad);
        }
        affiche_txt(renderer, font, txt, currentpage->container->txt[i].dest, currentpage->container->txt[i].color);
        free(txt);
    }
    if (currentpage == &mainpage){
        char remainingMob[10];
        sprintf(remainingMob, "/%d", level.mobToKill);
        affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(vw(64.5), vh(10), vh(6), vh(6.2)) , (SDL_Color){255, 255, 255, 255});
    }
}

void refreshMobHealth(){
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        if (currentpage->container->txt[i].tradID == VIE_MSG){
            currentpage->container->txt[i].info = &(level.monstre[level.currentLvl].mobHealth);
            break; // Break pour sortir plus vite de la boucle une fois qu'on a trouvé ce qu'on voulais modifier
        }
    }
}

void destroyUItxt(uiContainer * container){
    free(container->txt);
    container->txt = NULL;
    container->nbTxt = 0;
}

/*              PAGES              */    

void createPage(uiPage * page){
    page->container = malloc(sizeof(uiContainer));
    page->container->nbTxt = 0;
    page->container->txt = NULL;
    page->buttonsList = malloc(sizeof(ListeButton));
    page->buttonsList->nbButton = 0;
    page->buttonsList->buttons = NULL;
    page->buttonsImgList = malloc(sizeof(ListeButtonImg));
    page->buttonsImgList->nbButton = 0;
    page->buttonsImgList->buttons = NULL;
}

void destroyPage(uiPage * page){
    if (page->container->txt != NULL){
        destroyUItxt(page->container);
    }
    if (page->buttonsList->buttons != NULL){
        destroyButton(page->buttonsList);
    }
    if (page->buttonsImgList->buttons != NULL){
        destroyButtonImg(page->buttonsImgList);
    }
}

void changePage(uiPage * page){
    currentpage = page;
}

void initMainPage(){
    createPage(&mainpage);
    currentpage = &mainpage;

    mainpage.container->nbTxt = 0;
    mainpage.container->txt = NULL;

    createUIText(&mainpage,VIE_MSG, &(level.monstre[level.currentLvl].mobHealth), getRectForCentenredCord(vw(50), vh(37), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,OR_MSG, &gold, (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,DMG_MSG, &damage_click, (SDL_Rect) {vw(90),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,MOB_MSG, &level.mobKilled, getRectForCentenredCord(vw(50), vh(10), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,LVL_MSG, &level.currentLvl, getRectForCentenredCord(vw(50), vh(4), vh(20), vh(7)), (SDL_Color){255, 255, 255, 255});
    
    createButton(&mainpage,getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)), (SDL_Color){0, 0, 255, 255}, CLICK_MSG, NULL, 1.1,(SDL_Color){255, 0, 0, 255}, attack, 1, &damage_click);
    createButton(&mainpage,getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, DMG_MSG, &(shop.nextPrice),1.05,(SDL_Color){100, 0, 0, 255}, upgradeButton, 3, &damage_click, &gold, &shop);
    createImgButton(&mainpage,getRectForCentenredCord(vw(50), vh(70), 100, 100), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Arrow_Down.png", 0, 5, attack, 1, &damage_click);
}
char *fr_txt, *en_txt; 
void initSettingsPage(){
    createPage(&settingspage);

    settingspage.container->nbTxt = 0;
    settingspage.container->txt = NULL; 

    fr_txt = malloc(strlen("French") + 1);  
    sprintf(fr_txt, "French");
    en_txt = malloc(strlen("English") + 1);
    sprintf(en_txt, "English");

    createUIText(&settingspage,SETTING_MSG,NULL, getRectForCentenredCord(vw(50), vh(5), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255});
    createButton(&settingspage,getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, FR_MSG, NULL,1.05,(SDL_Color){100, 0, 0, 255}, SelectLanguage, 1, &fr_txt);
    createButton(&settingspage,getRectForCentenredCord(vw(50), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, EN_MSG, NULL,1.05,(SDL_Color){100, 0, 0, 255}, SelectLanguage, 1, &en_txt);
}


void initPage(){
    initMainPage();
    initSettingsPage();
}

void destroyAllPages(){
    destroyPage(&mainpage);
    destroyPage(&settingspage);
    free(fr_txt);
    free(en_txt);
}