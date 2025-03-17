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
#include "../lib/chaine.h"
#include "../lib/heros.h"
#include "../lib/prestige.h"

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
    if (currentpage->container == NULL || currentpage->container->txt == NULL || currentpage->container->nbTxt == 0){
        printf("No txt in page\n");
        return NULL;
    }
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        if (strcmp(currentpage->container->txt[i].label, label) == 0){
            return &currentpage->container->txt[i];
        }
    }
    return NULL;
}

uiImg * getImgFromLabel(char * label){
    if (currentpage->container == NULL || currentpage->container->img == NULL || currentpage->container->nbImg == 0){
        printf("No img in page\n");
        return NULL;
    }
    for (int i = 0; i < currentpage->container->nbImg; i++){
        if (strcmp(currentpage->container->img[i].label, label) == 0){
            return &currentpage->container->img[i];
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
    setUiText(txtHolder, formatChaine("%t: %w",VIE_MSG, level.monstre[level.currentLvl].mobHealth));
}
void refreshCurrentLvl(){
    uiTxt * txtHolder = getTxtFromLabel("currentLvl");
    if (txtHolder == NULL){
        return;
    }
    setUiText(txtHolder, formatChaine("%t: %d",LVL_MSG, level.currentLvl));
}

void refreshMobKilled(){
    uiTxt * txtHolder = getTxtFromLabel("mobKilled");
    if (txtHolder == NULL){
        return;
    }
    if (challenge.active) setUiText(txtHolder, formatChaine("%t: %d/%d",MOB_MSG, level.mobKilled, challenge.target));
    else setUiText(txtHolder, formatChaine("%t: %d/%d",MOB_MSG, level.mobKilled, level.mobToKill));
}

void createUIText(uiPage * page,TTF_Font* font, char * chaine, SDL_Rect dest, SDL_Color color, char * label){
    if (page->container->txt == NULL) {
        page->container->txt = malloc(sizeof(uiTxt));
    } else {
        page->container->txt = realloc(page->container->txt, sizeof(uiTxt) * (page->container->nbTxt + 1));
    }
    page->container->txt[page->container->nbTxt].chaine = chaine;
    page->container->txt[page->container->nbTxt].dest = dest;
    page->container->txt[page->container->nbTxt].color = color;
    page->container->txt[page->container->nbTxt].label = malloc(strlen(label) + 1);
    strcpy(page->container->txt[page->container->nbTxt].label, label);
    
    // Création de la texture du texte avec l'info si spécifié
    SDL_Surface *textSurface;
    textSurface = TTF_RenderText_Blended(font, chaine, color);
    
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
void createUIImg(uiPage * page, char * path, SDL_Rect dest, char * label){
    if (page->container->img == NULL) {
        page->container->img = malloc(sizeof(uiImg));
    } else {
        page->container->img = realloc(page->container->img, sizeof(uiImg) * (page->container->nbImg + 1));
    }
    page->container->img[page->container->nbImg].label = malloc(strlen(label) + 1);
    strcpy(page->container->img[page->container->nbImg].label, label);
    page->container->img[page->container->nbImg].dest = dest;
    page->container->img[page->container->nbImg].texture = IMG_LoadTexture(renderer, path);
    if (!page->container->img[page->container->nbImg].texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image: %s\n", IMG_GetError());
        return;
    }
    page->container->nbImg++;
}
void uiImgHandle(){
    for (int i=0;i< currentpage->container->nbImg; i++){
        SDL_RenderCopy(renderer, currentpage->container->img[i].texture, NULL, &currentpage->container->img[i].dest);
    }
}

void uiHandle(){
    for (int i = 0; i < currentpage->container->nbTxt; i++){
        affiche_txt(&currentpage->container->txt[i]);
    }

    uiNotifHandle();
    checkDisplayPrestigeItemText();
}

void destroyUITxt(uiTxt * txt, uiPage * page){
    if (!txt) {
        SDL_Log("destroyUITxt: txt is NULL");
        return;
    }
    if (!page) {
        SDL_Log("destroyUITxt: page is NULL");
        return;
    }
    if (txt->texture) {
        SDL_DestroyTexture(txt->texture);
        txt->texture = NULL;
    }
    if (txt->label) {
        free(txt->label);
        txt->label = NULL;
    }
    if (txt->chaine) {
        free(txt->chaine);
        txt->chaine = NULL;
    }
    if (page->container->nbTxt > 0) {
        page->container->nbTxt--;
    }
}

void destroyUIImg(uiImg * img, uiPage * page){
    if (!img) {
        SDL_Log("destroyUIImg: img is NULL");
        return;
    }
    if (!page) {
        SDL_Log("destroyUIImg: page is NULL");
        return;
    }
    if (img->texture) {
        SDL_DestroyTexture(img->texture);
        img->texture = NULL;
    }
    if (img->label) {
        free(img->label);
        img->label = NULL;
    }
    if (page->container->nbImg > 0) {
        page->container->nbImg--;
    }
}

void setUiText(uiTxt *txt, char *text) {
    if (!txt) {
        SDL_Log("setUiText: txt is NULL");
        return;
    }
    if (!text) {
        SDL_Log("setUiText: text is NULL");
        return;
    }
    if (txt->texture) {
        SDL_DestroyTexture(txt->texture);
        txt->texture = NULL;
    }
    
    if (txt->chaine) {
        free(txt->chaine);
    }
    txt->chaine = text;

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

void refreshUI(){
    if (pageHolder.page == NULL) {
        return;
    }
    destroyPages();
    initPage();
}


/*              PAGES              */    

void createPage(uiPage * page){
    page->container = malloc(sizeof(uiContainer));
    page->container->nbTxt = 0;
    page->container->txt = NULL;
    page->container->nbImg = 0;
    page->container->img = NULL;
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
            for (int i = page->container->nbTxt - 1; i >= 0; i--) {
                destroyUITxt(&page->container->txt[i], page);
            }
            free(page->container->txt);
            page->container->txt = NULL;
        }
        if (page->container->img != NULL) {
            for (int i = page->container->nbImg - 1; i >= 0; i--) {
                destroyUIImg(&page->container->img[i], page);
            }
            free(page->container->img);
            page->container->img = NULL;
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
        if(currentpage == &pageHolder.page[3]){ //hero shop 
            updateHeroShopPage();
            addGold(0); //Pour mettre a jour l'affichage de l'or et des heros
        }
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
        refreshUI();
        return 1;
    } else if (strcmp(*lang, "Fullscreen") == 0) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetRendererOutputSize(renderer, &widthscreen, &heightscreen);
        refreshUI();
        return 1;
    }
    return 0;
}
void initMainPage(){
    createPage(&pageHolder.page[0]);
    currentpage = &pageHolder.page[0];

    //pageHolder.page[0].container->nbTxt = 0;
    //pageHolder.page[0].container->txt = NULL;

    createUIText(&pageHolder.page[0],font,formatChaine("%t: %w",VIE_MSG, level.monstre[level.currentLvl].mobHealth), getRectForCentenredCord(vw(50), vh(37), vh(50), vh(8)), (SDL_Color){255, 255, 255, 255}, "mobHealth");
    createUIText(&pageHolder.page[0],font,formatChaine("%t: %w",OR_MSG, gold), (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255}, "playerGold");
    createUIText(&pageHolder.page[0],font,formatChaine("%t: %w",DMG_MSG, damage_click), (SDL_Rect) {vw(80),vh(1), vh(30), vh(10)}, (SDL_Color){255, 255, 255, 255} , "playerDamage");
    createUIText(&pageHolder.page[0],font,formatChaine("%t: %d/10",MOB_MSG, level.mobKilled), getRectForCentenredCord(vw(50), vh(10), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255}, "mobKilled");
    createUIText(&pageHolder.page[0],font,formatChaine("%t: %d",LVL_MSG, level.currentLvl), getRectForCentenredCord(vw(50), vh(4), vh(20), vh(7)), (SDL_Color){255, 255, 255, 255}, "currentLvl");

    createButton(&pageHolder.page[0],getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, CLICK_MSG, NULL, 1.1, attack, 1, &damage_click);
    createButton(&pageHolder.page[0],getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, DMG_MSG, (int*)&(shop.nextPrice),0.5, upgradeButton, 0);
    createImgButton(&pageHolder.page[0],getRectForCentenredCord(vw(90), vh(90), 50, 50), "assets/ui/icons/others/settings.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, 1, &pageHolder.page[1]);
    createImgButton(&pageHolder.page[0],getRectForCentenredCord(vw(90), vh(80), 50, 50), "assets/ui/icons/prestige/pprestige7.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 0, changePage, 1, &pageHolder.page[2]);
    createImgButton(&pageHolder.page[0],getRectForCentenredCord(vw(10), vh(90), 50, 50), "assets/ui/icons/others/heros.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, 1, &pageHolder.page[3]);
    createImgButton(&pageHolder.page[0],getRectForCentenredCord(vw(90), vh(20), 50, 50), "assets/ui/icons/others/challenge.svg", "assets/ui/buttons/extra/button_rectangle_depth_line.svg", 0, 2, launchChallenge,0);
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

    createUIText(&pageHolder.page[1],font,formatChaine("%t",SETTING_MSG), getRectForCentenredCord(vw(50), vh(5), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255}, "settingsTitle");
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(35), vh(20), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, FR_MSG, NULL, 1.05, SelectLanguage, 1, &fr_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(65), vh(20), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, EN_MSG, NULL, 1.05, SelectLanguage, 1, &en_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(35), vh(40), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, FULLSCREEN_MSG, NULL, 1.05, SelectScreen, 1, &full_txt);
    createButton(&pageHolder.page[1],getRectForCentenredCord(vw(65), vh(40), vw(15), vh(10)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, WINDOWED_MSG, NULL, 1.05, SelectScreen, 1, &window_txt);
    createImgButton(&pageHolder.page[1],getRectForCentenredCord(vw(90), vh(90), 50, 50), "assets/ui/icons/others/settings.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, 1, &pageHolder.page[0]);
}

//pageHolder.page[0] = main page 
///pageHolder.page[1] = settings page
void initPage(){
    pageHolder.pageNb = 4;
    pageHolder.page = malloc(sizeof(uiPage) * pageHolder.pageNb);
    initMainPage();
    initSettingsPage();
    initPrestige();
    initHerosPage();
}

void destroyPages(){
    for (int i = 0; i < pageHolder.pageNb; i++) {
        if (pageHolder.page[i].container != NULL) {
            destroyPage(&pageHolder.page[i]);
        }
    }
    free(pageHolder.page);

	free(fr_txt);
    free(en_txt);
    free(full_txt);
    free(window_txt);
}

void destroyAllPages(){
    for (int i = 0; i < pageHolder.pageNb; i++) {
        if (pageHolder.page[i].container != NULL) {
            destroyPage(&pageHolder.page[i]);
        }
    }
    if (fr_txt) free(fr_txt);
    if (en_txt) free(en_txt);
    if (full_txt) free(full_txt);
    if (window_txt) free(window_txt);
    if (pageHolder.page) free(pageHolder.page);
}


/*              NOTIFICATIONS              */

void initNotifList(){
    notifList.nbNotif = 0;
    notifList.notif = NULL;
}
void createNotif(char * title, int titleYOffset, float titleSize, char * imgBackground, int tapToClose, int duration, SDL_Rect dest, int messYOffset, float messSize, char *notifMess) {
    if (notifList.nbNotif >= MAX_NOTIF) {
        printf("createNotif: Max Notif reached\n");
        return;
    }
    if (notifList.notif == NULL) {
        notifList.notif = malloc(sizeof(Notif));
    } else {
        notifList.notif = realloc(notifList.notif, sizeof(Notif) * (notifList.nbNotif + 1));
    }
    
    if (notifList.notif == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Memory allocation failed\n");
        return;
    }

    notifList.notif[notifList.nbNotif].tapToClose = tapToClose;
    notifList.notif[notifList.nbNotif].duration = duration;
    notifList.notif[notifList.nbNotif].dest = dest;
    notifList.notif[notifList.nbNotif].messYOffset = messYOffset;
    notifList.notif[notifList.nbNotif].messSize = messSize;
    notifList.notif[notifList.nbNotif].titleYOffset = titleYOffset;
    notifList.notif[notifList.nbNotif].titleSize = titleSize;

    // Initialisation des textures
    notifList.notif[notifList.nbNotif].imgTexture = NULL;
    notifList.notif[notifList.nbNotif].titleTexture = NULL;
    notifList.notif[notifList.nbNotif].descTexture = NULL;

    // Creation de l'image de fond de la notification
    notifList.notif[notifList.nbNotif].imgTexture = IMG_LoadTexture(renderer, imgBackground);
    if (!notifList.notif[notifList.nbNotif].imgTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Failed to load image: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect titleRect = getSizeForText(font, title, (SDL_Rect){
        dest.x,
        dest.y + notifList.notif[notifList.nbNotif].titleYOffset + dest.h / 4,
        dest.w / 2 * titleSize,
        dest.h / 2 * titleSize
    });

    // Ajuster la position pour centrer le texte
    titleRect.x = dest.x + (dest.w - titleRect.w) / 2;
    notifList.notif[notifList.nbNotif].titleRect = titleRect;

    // Créer la texture du titre
    SDL_Surface *surface = TTF_RenderText_Blended(font, title, (SDL_Color){255, 255, 255, 255});
    if (!surface) {
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].imgTexture);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Failed to render title text: %s\n", TTF_GetError());
        return;
    }
    
    notifList.notif[notifList.nbNotif].titleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!notifList.notif[notifList.nbNotif].titleTexture) {
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].imgTexture);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Failed to create title texture: %s\n", SDL_GetError());
        return;
    }
    
    // Créer la texture de la description
    surface = TTF_RenderUTF8_Blended_Wrapped(font, notifMess, (SDL_Color){255, 255, 255, 255}, dest.w - 40);
    if (!surface) {
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].imgTexture);
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].titleTexture);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Failed to render description text: %s\n", TTF_GetError());
        return;
    }
    
    // Récupérer la taille de la surface
    int textWidth = surface->w * messSize;
    int textHeight = surface->h * messSize;
    
    // Créer la texture de la description
    notifList.notif[notifList.nbNotif].descRect = (SDL_Rect){
        dest.x + 20,
        dest.y + notifList.notif[notifList.nbNotif].messYOffset + (dest.h * titleSize / 2) + 10,
        textWidth,
        textHeight
    };
    
    notifList.notif[notifList.nbNotif].descTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!notifList.notif[notifList.nbNotif].descTexture) {
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].imgTexture);
        SDL_DestroyTexture(notifList.notif[notifList.nbNotif].titleTexture);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createNotif: Failed to create description texture: %s\n", SDL_GetError());
        return;
    }

    notifList.nbNotif++;
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

    // Destruction de l'image de fon de la notification
    SDL_DestroyTexture(notifList.notif[index].imgTexture);
    // Destruction du titre de la notification
    SDL_DestroyTexture(notifList.notif[index].titleTexture);
    // Destruction de la description de la notification
    SDL_DestroyTexture(notifList.notif[index].descTexture);
    // Décaler les notifications suivantes
    for (int i = index; i < notifList.nbNotif - 1; i++) {
        notifList.notif[i] = notifList.notif[i + 1];
    }
    notifList.nbNotif--;

    // Réallouer la mémoire pour la liste des notifications
    if (notifList.nbNotif > 0) {
        notifList.notif = realloc(notifList.notif, sizeof(Notif) * notifList.nbNotif);
    } else {
        free(notifList.notif);
        notifList.notif = NULL;
    }
}

void showNotif(Notif * notif) {
    if (notif == NULL) {
        return;
    }
    // Affichage de l'image de fond de la notification
    SDL_RenderCopy(renderer, notif->imgTexture, NULL, &notif->dest);
    // Affichage du titre
    SDL_RenderCopy(renderer, notif->titleTexture, NULL, &notif->titleRect);
    // Affichage de la description
    SDL_RenderCopy(renderer, notif->descTexture, NULL, &notif->descRect);
}

int lastNotifUpdate[MAX_NOTIF];
void uiNotifHandle() {
    if (notifList.notif == NULL || notifList.nbNotif == 0) {
        return;
    }
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < notifList.nbNotif; i++) {
        // Affichage de la description
        if (notifList.notif[i].descTexture){
            showNotif(&notifList.notif[i]);
        }
        // Gestion du temps d'affichage de la notification 
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