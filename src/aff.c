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
uiPageHolder pageHolder;

NotifList notifList;

void affiche_txt(uiTxt * txt){
    if (txt == NULL || txt->texture == NULL){
        return;
    }
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, txt->texture, NULL, &txt->dest, 0, NULL, flip);
}

uiTxt * getTxtFromLabel(char * label){
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        if (strcmp(currentpage->container->txt[i].label, label) == 0){
            return &currentpage->container->txt[i];
        }
    }
    return NULL;
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

void refreshMobHealth(){
    uiTxt * txtHolder = getTxtFromLabel("mobHealth");
    if (txtHolder == NULL){
        return;
    }
    char txt[50];
    sprintf(txt, "%s: %d", Traduction(txtHolder->tradID), level.monstre[level.currentLvl].mobHealth);
    setUiText(txtHolder, txt);
}
void refreshCurrentLvl(){
    uiTxt * txtHolder = getTxtFromLabel("currentLvl");
    if (txtHolder == NULL){
        return;
    }
    char txt[50];
    sprintf(txt, "%s: %d", Traduction(txtHolder->tradID), level.currentLvl);
    setUiText(txtHolder, txt);
}

void refreshMobKilled(){
    uiTxt * txtHolder = getTxtFromLabel("mobKilled");
    if (txtHolder == NULL){
        return;
    }
    char txt[50];
    sprintf(txt, "%s: %d/%d", Traduction(txtHolder->tradID), level.mobKilled, level.mobToKill);
    setUiText(txtHolder, txt);
}

void createUIText(uiPage * page,TTF_Font* font,int tradID,int * info, SDL_Rect dest, SDL_Color color, char * label){
    if (page->container->txt == NULL) {
        page->container->txt = malloc(sizeof(uiTxt));
    } else {
        page->container->txt = realloc(page->container->txt, sizeof(uiTxt) * (page->container->nbTxt + 1));
    }
    page->container->txt[page->container->nbTxt].tradID = tradID;
    page->container->txt[page->container->nbTxt].info = info;
    page->container->txt[page->container->nbTxt].dest = dest;
    page->container->txt[page->container->nbTxt].color = color;
    page->container->txt[page->container->nbTxt].label = malloc(strlen(label) + 1);
    strcpy(page->container->txt[page->container->nbTxt].label, label);
    
    // Création de la texture du texte avec l'info si spécifié
    SDL_Surface *textSurface;
    if (info == NULL) { // Gestion du cas ou le texte n'a pas besoin d'info
        textSurface = TTF_RenderText_Blended(font, Traduction(tradID), color);
    }else{
        // Gestion du cas ou le texte a besoin d'info
        char infoTxt[50];
        sprintf(infoTxt, "%d", *info);
        char *txt=malloc(sizeof(char) * strlen(Traduction(tradID)) + sizeof(char) * strlen(infoTxt) + 3);
        sprintf(txt, "%s: %d", Traduction(tradID), *info);
        textSurface = TTF_RenderText_Blended(font, txt, color);
        free(txt);
    }
    if (!textSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to render text: %s\n", TTF_GetError());
        return;
    }
    // Création de la texture du texte
    page->container->txt[page->container->nbTxt].texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!page->container->txt[page->container->nbTxt].texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
    page->container->nbTxt++;
}


void uiHandle(){
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        affiche_txt(&currentpage->container->txt[i]);
    }
    // if (currentpage == &pageHolder.page[0]){
    //     char remainingMob[challengeTarget];
    //     if (challengeActive) {
    //         sprintf(remainingMob, "/%d", level.mobToKill - 1);
    //         displayChallengeTimer(font);
    //     } 
    //     else sprintf(remainingMob, "/%d", level.mobToKill);
    //     affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(vw(64.5), vh(10), vh(6), vh(6.2)) , (SDL_Color){255, 255, 255, 255});
    // }
    uiNotifHandle();
}

void destroyUItxt(uiTxt * txt){
    if (txt) {
        if (txt->label) {
            free(txt->label);
            txt->label = NULL;
        }
        if (txt->texture) {
            SDL_DestroyTexture(txt->texture);
            txt->texture = NULL;
        }
    }
}

void setUiText(uiTxt *txt, const char *text) {
    if (!txt) {
        SDL_Log("setUiText: txt is NULL");
        return;
    }
    
    if (!text) {
        SDL_Log("setUiText: text is NULL");
        return;
    }
    // Détruire l'ancienne texture
    if (txt->texture) {
        SDL_DestroyTexture(txt->texture);
        txt->texture = NULL;
    }
    // Création de la nouvelle texture du texte
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, txt->color);
    if (!textSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "setUiText: Failed to render text: %s\n", TTF_GetError());
        return;
    }
    // Créer la texture à partir de la surface
    txt->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!txt->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "setUiText: Failed to create texture: %s\n", SDL_GetError());
        return;
    }
}

void refreshUILanguage(){
    if (pageHolder.page == NULL) {
        return;
    }
    // Mettre à jour les textes UI de la page principale
    if (pageHolder.page[0].container != NULL && pageHolder.page[0].container->txt != NULL) {
        for (int i = 0; i < pageHolder.page[0].container->nbTxt; i++) {
            if (pageHolder.page[0].container->txt[i].info != NULL) {
                char txt[50] = "";
                sprintf(txt, "%s: %d", Traduction(pageHolder.page[0].container->txt[i].tradID), 
                       *pageHolder.page[0].container->txt[i].info);
                setUiText(&pageHolder.page[0].container->txt[i], txt);
            } else {
                setUiText(&pageHolder.page[0].container->txt[i], Traduction(pageHolder.page[0].container->txt[i].tradID));
            }
        }
    }
    
    // Mettre à jour les textes UI de la page des paramètres
    if (pageHolder.pageNb > 1 && pageHolder.page[1].container != NULL && 
        pageHolder.page[1].container->txt != NULL) {
        for (int i = 0; i < pageHolder.page[1].container->nbTxt; i++) {
            if (pageHolder.page[1].container->txt[i].info != NULL) {
                char txt[50] = "";
                sprintf(txt, "%s: %d", Traduction(pageHolder.page[1].container->txt[i].tradID), 
                       *pageHolder.page[1].container->txt[i].info);
                setUiText(&pageHolder.page[1].container->txt[i], txt);
            } else {
                setUiText(&pageHolder.page[1].container->txt[i], Traduction(pageHolder.page[1].container->txt[i].tradID));
            }
        }
    }
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

void destroyPage(uiPage *page) {
    if (page == NULL) {
        return;
    }
    // Libérer les textes UI
    if (page->container != NULL) {
        if (page->container->txt != NULL) {
            for (int i = 0; i < page->container->nbTxt; i++) {
                destroyUItxt(&page->container->txt[i]);
            }
            free(page->container->txt);
            page->container->txt = NULL;
        }
        free(page->container);
        page->container = NULL;
    }
    // Libérer les boutons standards
    if (page->buttonsList != NULL) {
        if (page->buttonsList->buttons != NULL) {
            destroyAllButtonFromPage(page->buttonsList);
        } else {
            free(page->buttonsList);
        }
        page->buttonsList = NULL;
    }
    // Libérer les boutons avec image
    if (page->buttonsImgList != NULL) {
        if (page->buttonsImgList->buttons != NULL) {
            destroyAllButtonImgFromPage(page->buttonsImgList);
        } else {
            free(page->buttonsImgList);
        }
        page->buttonsImgList = NULL;
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
int SelectScreen(void * l[20]){
    char **lang = (char **)l[0];
    if (strcmp(*lang, "Window") == 0) {
        SDL_SetWindowFullscreen(window, 0);
        SDL_GetWindowSize(window, &widthscreen, &heightscreen);
        return 1;
    } else if (strcmp(*lang, "Fullscreen") == 0) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetRendererOutputSize(renderer, &widthscreen, &heightscreen);
        return 1;
    }
    return 0;
}
void initMainPage(){
    createPage(&pageHolder.page[0]);
    currentpage = &pageHolder.page[0];

    pageHolder.page[0].container->nbTxt = 0;
    pageHolder.page[0].container->txt = NULL;

    createUIText(&pageHolder.page[0],font,VIE_MSG, &(level.monstre[level.currentLvl].mobHealth), getRectForCentenredCord(vw(50), vh(37), vh(50), vh(8)), (SDL_Color){255, 255, 255, 255}, "mobHealth");
    createUIText(&pageHolder.page[0],font,OR_MSG, &gold, (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255}, "playerGold");
    createUIText(&pageHolder.page[0],font,DMG_MSG, &damage_click, (SDL_Rect) {vw(80),vh(1), vh(30), vh(10)}, (SDL_Color){255, 255, 255, 255} , "playerDamage");
    createUIText(&pageHolder.page[0],font,MOB_MSG, &level.mobKilled, getRectForCentenredCord(vw(50), vh(10), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255}, "mobKilled");
    createUIText(&pageHolder.page[0],font,LVL_MSG, &level.currentLvl, getRectForCentenredCord(vw(50), vh(4), vh(20), vh(7)), (SDL_Color){255, 255, 255, 255}, "currentLvl");
    
    createButton(&pageHolder.page[0],getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, CLICK_MSG, NULL, 1.1, attack, 1, &damage_click);
    createButton(&pageHolder.page[0],getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, DMG_MSG, &(shop.nextPrice),0.5, upgradeButton, 3, &damage_click, &gold, &shop);
    createImgButton(&pageHolder.page[0],getRectForCentenredCord(vw(90), vh(90), 50, 50), "assets/ui/icons/settings.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, 1, &pageHolder.page[1]);
    refreshMobKilled();
}

char *fr_txt, *en_txt, *full_txt, *window_txt; 
void initSettingsPage(){
    createPage(&pageHolder.page[1]);

    pageHolder.page[1].container->nbTxt = 0;
    pageHolder.page[1].container->txt = NULL; 

    fr_txt = malloc(strlen("French") + 1);  
    sprintf(fr_txt, "French");
    en_txt = malloc(strlen("English") + 1);
    sprintf(en_txt, "English");
    full_txt = malloc(strlen("Fullscreen") + 1);
    sprintf(full_txt, "Fullscreen");
    window_txt = malloc(strlen("Window") + 1);
    sprintf(window_txt, "Window");

    createUIText(&pageHolder.page[1],font,SETTING_MSG,NULL, getRectForCentenredCord(vw(50), vh(5), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255}, "settingsTitle");
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(35), vh(20), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, FR_MSG, NULL, 1.05, SelectLanguage, 1, &fr_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(65), vh(20), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, EN_MSG, NULL, 1.05, SelectLanguage, 1, &en_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(35), vh(40), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, FULLSCREEN_MSG, NULL, 1.05, SelectScreen, 1, &full_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(65), vh(40), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, WINDOWED_MSG, NULL, 1.05, SelectScreen, 1, &window_txt);
    createImgButton(&pageHolder.page[1],getRectForCentenredCord(vw(90), vh(90), 50, 50), "assets/ui/icons/settings.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, 1, &pageHolder.page[0]);
}

//pageHolder.page[0] = main page 
///pageHolder.page[1] = settings page
void initPage(){
    pageHolder.pageNb = 2;
    pageHolder.page = malloc(sizeof(uiPage) * pageHolder.pageNb);
    initMainPage();
    initSettingsPage();
}

void destroyAllPages(){
    destroyPage(&pageHolder.page[0]);
    destroyPage(&pageHolder.page[1]);
    free(fr_txt);
    free(en_txt);
    free(full_txt);
    free(window_txt);
}


/*              NOTIFICATIONS              */
void initNotifList(){
    notifList.nbNotif = 0;
    notifList.notif = NULL;
}

char * createNotif(int tradId, int titleYOffset,float titleSize,char * imgBackground, int tapToClose, int duration, SDL_Rect dest,int messYOffset,float messSize, int nbLignes, ...){
    if (notifList.nbNotif >= MAX_NOTIF){
        printf("createNotif: Max Notif reached\n");
        return NULL;
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
            return NULL;
        }
        desc[i] = strdup(ligne);
    }
    va_end(args);

    notifList.notif[notifList.nbNotif].tradId = tradId;
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
    
    //Creation du label de la notif pour pouvoir l'attribuer au UITxt correspondant
    char notif_label[50];
    sprintf(notif_label, "%s%d", Traduction(tradId), notifList.nbNotif);
    notifList.notif[notifList.nbNotif].label = strdup(notif_label);
    
    // Créer l'UIText du titre avec le bon placement
    sprintf(notif_label, "title_%s%d", Traduction(tradId), notifList.nbNotif);
    SDL_Rect titleRect = getRectForCentenredCord(
        dest.x, 
        dest.y + notifList.notif[notifList.nbNotif].titleYOffset, 
        dest.w / 2 * notifList.notif[notifList.nbNotif].titleSize,
        dest.h * notifList.notif[notifList.nbNotif].titleSize
    );
    createUIText(currentpage, font, tradId, NULL, titleRect, (SDL_Color){255, 255, 255, 255}, strdup(notif_label));
    
    // Créer les UIText pour chaque ligne de description
    int lineHeight = 15;
    int startY = dest.y - ((nbLignes * lineHeight)/2) + notifList.notif[notifList.nbNotif].messYOffset;
    for (int i = 0; i < nbLignes; i++) {
        sprintf(notif_label, "desc_%s%d_%d", Traduction(tradId), notifList.nbNotif, i);
        SDL_Rect lineDest = getRectForCentenredCord(
            dest.x,
            startY + (i * lineHeight), 
            dest.w / 2 * notifList.notif[notifList.nbNotif].messSize, 
            lineHeight * notifList.notif[notifList.nbNotif].messSize
        );
        createUIText(currentpage, font, tradId, NULL, lineDest, (SDL_Color){255, 255, 255, 255}, strdup(notif_label));
    }
    
    notifList.nbNotif++;

    return notifList.notif[notifList.nbNotif-1].label;
}

void showNotif(Notif * notif){
    // if (notif == NULL){
    //     printf("showNotif: notif is NULL\n");
    //     return;
    // }

    // SDL_Texture* bgTexture = IMG_LoadTexture(renderer, notif->imgBackground);
    // if (bgTexture == NULL) {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", IMG_GetError());
    //     return;
    // }
    // SDL_Rect bg = getRectForCentenredCord(notif->dest.x, notif->dest.y, notif->dest.w, notif->dest.h);
    // SDL_RenderCopy(renderer, bgTexture, NULL, &bg);
    // SDL_DestroyTexture(bgTexture);
    
    // if (notif->title != NULL){
    //     affiche_txt(renderer, font, notif->title, getSizeForText(font, notif->title, getRectForCentenredCord(notif->dest.x, notif->dest.y+notif->titleYOffset, notif->dest.w / 2 * notif->titleSize,notif->dest.h * notif->titleSize)), (SDL_Color){255, 255, 255, 255});
    // } 
    // if (notif->desc != NULL){
    //     int lineHeight = 15;
    //     int startY = notif->dest.y - ((notif->nbLignes * lineHeight)/2) + notif->messYOffset;
    //     for (int i = 0; i < notif->nbLignes; i++) {
    //         SDL_Rect lineDest = getRectForCentenredCord(notif->dest.x,startY + (i * lineHeight), notif->dest.w / 2 * notif->messSize, lineHeight * notif->messSize);
    //         affiche_txt(renderer, font, notif->desc[i], getSizeForText(font, notif->desc[i], lineDest), (SDL_Color){255, 255, 255, 255});
    //     }
    // }
}

void markNotifForDeletion(int index) {
    if (index < 0 || index >= notifList.nbNotif) {
        return;
    }
    notifList.notif[index].duration = -1; // Marquer pour suppression
}

void deleteNotif(int index) {
    if (index < 0 || index >= notifList.nbNotif) {
        return;
    }
    
    // Libérer la mémoire pour les descriptions
    for (int i = 0; i < notifList.notif[index].nbLignes; i++) {
        free(notifList.notif[index].desc[i]);
    }
    free(notifList.notif[index].desc);
    
    // Libérer le label de la notification
    free(notifList.notif[index].label);
    
    // Rechercher et supprimer les UIText correspondants
    char title_label[60];
    sprintf(title_label, "title_%s%d", "notif", index); // Assurez-vous que "notif" correspond à votre valeur de "title"
    
    // Supprimer l'UIText du titre
    uiTxt *titleText = getTxtFromLabel(title_label);
    if (titleText) {
        // Trouver l'index de l'UIText à supprimer
        int txtIndex = titleText - currentpage->container->txt;
        if (txtIndex >= 0 && txtIndex < currentpage->container->nbTxt) {
            // Libérer les ressources
            SDL_DestroyTexture(currentpage->container->txt[txtIndex].texture);
            free(currentpage->container->txt[txtIndex].label);
            
            // Déplacer les UIText suivants pour combler le trou
            for (int i = txtIndex; i < currentpage->container->nbTxt - 1; i++) {
                currentpage->container->txt[i] = currentpage->container->txt[i + 1];
            }
            currentpage->container->nbTxt--;
        }
    }
    
    // Supprimer les UIText des descriptions
    for (int i = 0; i < notifList.notif[index].nbLignes; i++) {
        sprintf(title_label, "desc_%s%d_%d", "notif", index, i);
        uiTxt *descText = getTxtFromLabel(title_label);
        if (descText) {
            int txtIndex = descText - currentpage->container->txt;
            if (txtIndex >= 0 && txtIndex < currentpage->container->nbTxt) {
                // Libérer les ressources
                SDL_DestroyTexture(currentpage->container->txt[txtIndex].texture);
                free(currentpage->container->txt[txtIndex].label);
                
                // Déplacer les UIText suivants pour combler le trou
                for (int j = txtIndex; j < currentpage->container->nbTxt - 1; j++) {
                    currentpage->container->txt[j] = currentpage->container->txt[j + 1];
                }
                currentpage->container->nbTxt--;
            }
        }
    }
    
    // Déplacer les notifications suivantes pour combler le trou
    for (int i = index; i < notifList.nbNotif - 1; i++) {
        notifList.notif[i] = notifList.notif[i + 1];
    }
    
    // Décrémenter le nombre de notifications
    notifList.nbNotif--;
    
    // Redimensionner le tableau de notifications si nécessaire
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