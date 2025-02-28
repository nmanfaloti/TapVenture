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
#include "../lib/challenge.h"

int widthscreen = 800;
int heightscreen = 500;

uiPage * currentpage;
uiPage mainpage;
uiPage settingspage;
NotifList notifList;

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

    float defaultratio = (float)w / h;
    float ratio = (float)dest.w / dest.h;

    float scale = (ratio > defaultratio) ? (float)dest.h / h : (float)dest.w / w;
    int new_w = (int)(w * scale);
    int new_h = (int)(h * scale);

    return (SDL_Rect){dest.x, dest.y, new_w, new_h};
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
        char remainingMob[challengeTarget];
        if (challengeActive) {
            sprintf(remainingMob, "/%d", level.mobToKill - 1);
            displayChallengeTimer(renderer, font);
        } 
        else sprintf(remainingMob, "/%d", level.mobToKill);
        affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(vw(64.5), vh(10), vh(6), vh(6.2)) , (SDL_Color){255, 255, 255, 255});
    }
    uiNotifHandle();
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

int changePage(void * args[20]){
    uiPage * page = args[0];
    if (currentpage != page){
        currentpage = page;
        printf("Changing page\n");
    }else{
        printf("Already on this page\n");
    }
    return 0;
}

void initMainPage(){
    createPage(&mainpage);
    currentpage = &mainpage;

    mainpage.container->nbTxt = 0;
    mainpage.container->txt = NULL;

    createUIText(&mainpage,VIE_MSG, &(level.monstre[level.currentLvl].mobHealth), getRectForCentenredCord(vw(50), vh(37), vh(50), vh(8)), (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,OR_MSG, &gold, (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,DMG_MSG, &damage_click, (SDL_Rect) {vw(80),vh(1), vh(30), vh(10)}, (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,MOB_MSG, &level.mobKilled, getRectForCentenredCord(vw(50), vh(10), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255});
    createUIText(&mainpage,LVL_MSG, &level.currentLvl, getRectForCentenredCord(vw(50), vh(4), vh(20), vh(7)), (SDL_Color){255, 255, 255, 255});
    
    createButton(&mainpage,getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)), (SDL_Color){0, 0, 255, 255}, CLICK_MSG, NULL, 1.1,(SDL_Color){255, 0, 0, 255}, attack, 1, &damage_click);
    createButton(&mainpage,getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, DMG_MSG, &(shop.nextPrice),1.05,(SDL_Color){100, 0, 0, 255}, upgradeButton, 3, &damage_click, &gold, &shop);
    createImgButton(&mainpage,getRectForCentenredCord(vw(50), vh(70), 100, 100), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Arrow_Down.png", 0, 5, attack, 1, &damage_click);
    createImgButton(&mainpage,getRectForCentenredCord(vw(90), vh(90), 50, 50), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Settings.png", 0, 3, changePage, 1, &settingspage);
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
    createButton(&settingspage,getRectForCentenredCord(vw(35), vh(20), vw(15), vh(10)), (SDL_Color){0, 150, 0, 255}, FR_MSG, NULL,1.05,(SDL_Color){100, 0, 0, 255}, SelectLanguage, 1, &fr_txt);
    createButton(&settingspage,getRectForCentenredCord(vw(65), vh(20), vw(15), vh(10)), (SDL_Color){0, 150, 0, 255}, EN_MSG, NULL,1.05,(SDL_Color){100, 0, 0, 255}, SelectLanguage, 1, &en_txt);
    createImgButton(&settingspage,getRectForCentenredCord(vw(95), vh(90), 50, 50), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Settings.png", 0, 3, changePage, 1, &mainpage);
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


/*              NOTIFICATIONS              */
void initNotifList(){
    notifList.nbNotif = 0;
    notifList.notif = NULL;
}

void createNotif(char * title, int titleYOffset,float titleSize,char * imgBackground, int tapToClose, int duration, SDL_Rect dest,int messYOffset,float messSize, int nbLignes, ...){
    if (notifList.nbNotif >= MAX_NOTIF){
        printf("createNotif: Max Notif reached\n");
        return;
    }
    if (notifList.notif == NULL){
        notifList.notif = malloc(sizeof(Notif));
    } else {
        notifList.notif = realloc(notifList.notif, sizeof(Notif) * (notifList.nbNotif + 1));
    }

    va_list args;
    va_start(args, nbLignes);
    char **desc = malloc(sizeof(char *) * nbLignes);
    for (int i = 0; i < nbLignes; i++){
        char * ligne = va_arg(args, char *);
        if (ligne == NULL){
            printf("createNotif: Missing Arguments\n");
            return;
        }
        desc[i] = strdup(ligne);
    }
    va_end(args);

    notifList.notif[notifList.nbNotif].title = title;
    notifList.notif[notifList.nbNotif].imgBackground = imgBackground;
    notifList.notif[notifList.nbNotif].tapToClose = tapToClose;
    notifList.notif[notifList.nbNotif].duration = duration;
    notifList.notif[notifList.nbNotif].dest = dest;
    notifList.notif[notifList.nbNotif].desc = desc;
    notifList.notif[notifList.nbNotif].nbLignes = nbLignes;
    notifList.notif[notifList.nbNotif].messYOffset = messYOffset;
    notifList.notif[notifList.nbNotif].messSize = messSize;
    notifList.notif[notifList.nbNotif].titleYOffset = titleYOffset;
    notifList.notif[notifList.nbNotif].titleSize = titleSize;
    notifList.nbNotif++;
}

void showNotif(Notif * notif){
    if (notif == NULL){
        printf("showNotif: notif is NULL\n");
        return;
    }

    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, notif->imgBackground);
    if (bgTexture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", IMG_GetError());
        return;
    }
    SDL_Rect bg = getRectForCentenredCord(notif->dest.x, notif->dest.y, notif->dest.w, notif->dest.h);
    SDL_RenderCopy(renderer, bgTexture, NULL, &bg);
    SDL_DestroyTexture(bgTexture);

    if (notif->title != NULL){
        affiche_txt(renderer, font, notif->title, getSizeForText(font, notif->title, getRectForCentenredCord(notif->dest.x, notif->dest.y+notif->titleYOffset, notif->dest.w / 2,notif->dest.h)), (SDL_Color){255, 255, 255, 255});
    } 
    if (notif->desc != NULL){
        int lineHeight = 15;
        int startY = notif->dest.y - ((notif->nbLignes * lineHeight)/2) + notif->messYOffset;
        for (int i = 0; i < notif->nbLignes; i++) {
            SDL_Rect lineDest = getRectForCentenredCord(notif->dest.x,startY + (i * lineHeight), notif->dest.w / 2 * notif->messSize, lineHeight*notif->messSize);
            affiche_txt(renderer, font, notif->desc[i], getSizeForText(font, notif->desc[i], lineDest), (SDL_Color){255, 255, 255, 255});
        }
    }
}

void markNotifForDeletion(int index) {
    if (index < 0 || index >= notifList.nbNotif) {
        return;
    }
    notifList.notif[index].duration = -1; // Marquer pour suppression
}

void deleteNotif(int index){
    if (index < 0 || index >= notifList.nbNotif){
        return;
    }
    for (int i = 0; i < notifList.notif[index].nbLignes; i++) {
        free(notifList.notif[index].desc[i]);
    }
    free(notifList.notif[index].desc);
    notifList.nbNotif--;

    if (notifList.nbNotif > 0) {
        notifList.notif = realloc(notifList.notif, sizeof(Notif) * notifList.nbNotif);
    } else {
        free(notifList.notif);
        notifList.notif = NULL;
    }
}


int lastNotifUpdate[MAX_NOTIF];
void uiNotifHandle() {
    if (notifList.notif == NULL || notifList.nbNotif == 0) {
        return;
    }
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < notifList.nbNotif; i++) {
        if (notifList.notif[i].duration > 0) {
            showNotif(&notifList.notif[i]);
        }

        if (currentTime - lastNotifUpdate[i] >= 1000) {
            lastNotifUpdate[i] = currentTime;
            notifList.notif[i].duration--;

            if (notifList.notif[i].duration <= 0) {
                //Pas supprimé directement pour éviter de modifier la liste en cours de parcours
                markNotifForDeletion(i);
            }
        }
    }
    // Supprimer les notifications marquées pour suppression
    for (int i = notifList.nbNotif - 1; i >= 0; i--) {
        if (notifList.notif[i].duration == -1) {
            deleteNotif(i);
        }
    }
}

void destroyNotifList() {
    if (notifList.notif == NULL) {
        return;
    } else {
        for (int i = notifList.nbNotif - 1; i >= 0; i--) {
            deleteNotif(i);
        }
        free(notifList.notif);
    }
    notifList.notif = NULL;
    notifList.nbNotif = 0;
}