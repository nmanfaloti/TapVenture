#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../lib/sdl.h"
#include "../lib/button.h"
#include "../lib/ui.h"
#include "../lib/boutique.h"
#include "../lib/lang.h"
#include "../lib/input_user.h"
#include "../lib/aff.h"

static int setButtonTexture(SDL_Texture **texture, char *path) {
    //Gestion de l'image en svg
    SDL_RWops* rw = SDL_RWFromFile(path, "rb");
    SDL_Surface *surface = IMG_LoadSVG_RW(rw);
    if (!surface) {
        SDL_FreeRW(rw);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.texture) -> %s: %s\n", path,IMG_GetError());
        return 0;
    }
    //Création de la texture
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!*texture) {
        SDL_FreeRW(rw);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.texture)-> %s : %s\n",path ,IMG_GetError());
        return 0;
    }
    //Libération de la mémoire
    SDL_FreeRW(rw);
    SDL_FreeSurface(surface);
    return 1;
}


void drawButton(Button *button) {
    if (!button) return;
    if (button->texture) {
        SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
    }else {
        SDL_SetRenderDrawColor(renderer, button->textColor.r, button->textColor.g, button->textColor.b, button->textColor.a);
        SDL_RenderFillRect(renderer, &button->rect);
    }
    if (button->textTexture) {
        SDL_RenderCopy(renderer, button->textTexture, NULL, &button->textRect);
    }
}

int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y) {
    return mouse_x >= rect.x && mouse_x <= (rect.x + rect.w) && mouse_y >= rect.y && mouse_y <= (rect.y + rect.h);
}

SDL_Rect getRectForCentenredCord(int x, int y, int w, int h) {
    return (SDL_Rect){x - w / 2, y - h / 2, w, h};
}

void drawButtonImg(ButtonImg *button) {
    if (!button) return;
    if (button->backgroundTexture) {
        SDL_RenderCopy(renderer, button->backgroundTexture, NULL, &button->rect);
    }
    if (button->imgTexture) {
        int icon_width, icon_height;
        SDL_QueryTexture(button->imgTexture, NULL, NULL, &icon_width, &icon_height);
        
        // Calcule la taille de l'image en fonction de la taille du bouton
        int img_width = button->rect.w * 0.6;  // 70% 
        int img_height = button->rect.h * 0.6; // 70%
        
        // Calcule la position de l'image pour la centrer
        SDL_Rect imgRect = {
            button->rect.x + (button->rect.w - img_width) / 2,
            button->rect.y + (button->rect.h - img_height) / 2,
            img_width,
            img_height
        };
        
        // Applique l'offset de l'image
        imgRect.x -= button->offsetLogoX;
        imgRect.y -= button->offsetLogoY;
        
        SDL_RenderCopy(renderer, button->imgTexture, NULL, &imgRect);
    }
}

void initListButton(ListeButton *listeButton) {
    listeButton = malloc(sizeof(ListeButton));
    if (listeButton != NULL) {
        listeButton->nbButton = 0;
        listeButton->buttons = NULL;
        printf("listeButton initialisé avec succès\n");
    } else {
        printf("Erreur d'allocation mémoire pour listeButton\n");
    }
}

void initListButtonImg(ListeButtonImg *listeButtonImg) {
    listeButtonImg = malloc(sizeof(ListeButtonImg));
    if (listeButtonImg != NULL) {
        listeButtonImg->nbButton = 0;
        listeButtonImg->buttons = NULL;
        printf("listeButtonImg initialisé avec succès\n");
    } else {
        printf("Erreur d'allocation mémoire pour listeButtonImg\n");
    }
}

void createButton(uiPage * page,SDL_Rect rect, char * pathImg, char *hoverPath,TTF_Font *Textfont,SDL_Color color, int txtInd, int * info,float growEffect,int (*callFunction)(void **), int numArgs, ...) {
    if (page->buttonsList == NULL) {
        initListButton(page->buttonsList);
    }
    va_list args;
    va_start(args, numArgs);
    void *params[numArgs];
    for (int i = 0; i < numArgs; i++) {
        params[i] = va_arg(args, void *);
    }
    va_end(args);
    
    // Création du bouton
    Button newButton;
    newButton.rect = rect;
    newButton.iniRect = rect;
    newButton.textColor = color;
    newButton.text = txtInd;
    newButton.info = info;
    newButton.growEffect = growEffect;
    newButton.callFunction = callFunction;
    newButton.args = malloc(numArgs * sizeof(void *));
    for (int i = 0; i < numArgs; i++) {
        newButton.args[i] = params[i]; // Assignation des arguments
    }

    //Création de la texture du bouton (arrire plan)
    if (!setButtonTexture(&newButton.texture, pathImg)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.texture): %s\n", IMG_GetError());
        return;
    }
    if (!setButtonTexture(&newButton.initialTexture, pathImg)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.initialTexture): %s\n", IMG_GetError());
        return;
    }
    newButton.font = Textfont ? Textfont : font;  // Si la police de caractère n'est pas spécifié on utilise la police par défaut

    // Création de la texture du hover (si spécifié)
    if (hoverPath){
        if (!setButtonTexture(&newButton.selectedTexture, hoverPath)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.selectedTexture): %s\n", IMG_GetError());
            return;
        }
    }
    // Création de la texture du texte avec l'info si spécifié
    SDL_Surface *textSurface;
    if (newButton.info == NULL) { // Gestion du cas ou le texte n'a pas besoin d'info
        textSurface = TTF_RenderText_Blended(newButton.font, Traduction(newButton.text), newButton.textColor);
    }else{
        // Gestion du cas ou le texte a besoin d'info
        char infoTxt[50];
        sprintf(infoTxt, "%d", *info);
        char *txt=malloc(sizeof(char) * strlen(Traduction(newButton.text)) + sizeof(char) * strlen(infoTxt) + 3);
        sprintf(txt, "%s: %d", Traduction(newButton.text), *info);
        textSurface = TTF_RenderText_Blended(newButton.font, txt, newButton.textColor);
        free(txt);
    }
    if (!textSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to render text: %s\n", TTF_GetError());
        return;
    }
    // Création de la texture du texte
    newButton.textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!newButton.textTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
    // Calcul de la position du texte pour le centrer
    float scale = 1.0;
    if (textSurface->w > newButton.rect.w) {
        scale = (float)newButton.rect.w / (float) textSurface->w;
    }
    if (textSurface->h > newButton.rect.h) {
        float scale_h = (float)newButton.rect.h / (float) textSurface->h;
        if (scale_h < scale) {
            scale = scale_h;
        }
    }
    // -2 sur le y pour centrer le texte car il y a un effet 3d sur le bouton
    newButton.textRect = (SDL_Rect) {newButton.rect.x + rect.w / 2 - textSurface->w * scale / 2, newButton.rect.y + newButton.rect.h / 2 - textSurface->h * scale / 2 -2, textSurface->w * scale, textSurface->h * scale};
    newButton.textIniRect = newButton.textRect;
    SDL_FreeSurface(textSurface);

    // Ajout du bouton à la page
    page->buttonsList->buttons = realloc(page->buttonsList->buttons, (page->buttonsList->nbButton + 1) * sizeof(Button));
    if (page->buttonsList->buttons != NULL) {
        page->buttonsList->buttons[page->buttonsList->nbButton] = newButton;
        page->buttonsList->nbButton++;
    } else {
        printf("Button: Erreur lors de l'ajout du bouton a la page\n");
        free(newButton.args);
        SDL_DestroyTexture(newButton.texture);
        SDL_DestroyTexture(newButton.initialTexture);
        SDL_DestroyTexture(newButton.selectedTexture);
        SDL_DestroyTexture(newButton.textTexture);
    }
}

void setButtonScale(Button *button, float scale) {
    if (!button) {
        SDL_Log("Button : setButtonScale : button is NULL");
        return;
    }
    button->rect = (SDL_Rect){
        button->iniRect.x - (button->iniRect.w * (scale - 1) / 2),
        button->iniRect.y - (button->iniRect.h * (scale - 1) / 2),
        button->iniRect.w * scale,
        button->iniRect.h * scale
    };
    if (button->textTexture) {
        button->textRect = (SDL_Rect){
            button->textIniRect.x - (button->textIniRect.w * (scale - 1) / 2),
            button->textIniRect.y - (button->textIniRect.h * (scale - 1) / 2),
            button->textIniRect.w * scale,
            button->textIniRect.h * scale
        };
    }
}

void setButtonText(Button *button, const char *text){
    if (!button) {
        SDL_Log("Button : setButtonText : button is NULL");
        return;
    }
    if (!text) {
        SDL_Log("Button : setButtonText : text is NULL");
        return;
    }
    //Détruit l'ancienne texture
    if (button->textTexture) {
        SDL_DestroyTexture(button->textTexture);
        button->textTexture = NULL;
    }
    //Création de la nouvelle texture du texte
    SDL_Surface *textSurface = TTF_RenderText_Blended(button->font, text, button->textColor);
    if (!textSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to render text: %s\n", TTF_GetError());
        return;
    }
    button->textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!button->textTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
}

void refreshButtonShop(){
    if (pageHolder.page == NULL ||  pageHolder.page[0].buttonsList == NULL){
        return;
    }
    //Update du bouton pour afficher le nouveau prix (1 est son indice dans la liste des boutons)
    char txt[50] = "";
    sprintf(txt, "%s: %d", Traduction(pageHolder.page[0].buttonsList->buttons[1].text), shop.nextPrice);
    setButtonText(&pageHolder.page[0].buttonsList->buttons[1], txt);
}

void refreshButtonLanguage(){
    if (pageHolder.page == NULL || pageHolder.page[0].buttonsList == NULL || pageHolder.page[1].buttonsList == NULL){
        return;
    }
    for (int i = 0; i < pageHolder.page[0].buttonsList->nbButton; i++){
        if (pageHolder.page[0].buttonsList->buttons[i].info != NULL){
            char txt[50] = "";
            sprintf(txt, "%s: %d", Traduction(pageHolder.page[0].buttonsList->buttons[i].text), *pageHolder.page[0].buttonsList->buttons[i].info);
            setButtonText(&pageHolder.page[0].buttonsList->buttons[i], txt);
        } else {
            setButtonText(&pageHolder.page[0].buttonsList->buttons[i], Traduction(pageHolder.page[0].buttonsList->buttons[i].text));
        }
    }
    for (int i=0; i<pageHolder.page[1].buttonsList->nbButton; i++){
        if (pageHolder.page[1].buttonsList->buttons[i].info != NULL){
            char txt[50] = "";
            sprintf(txt, "%s: %d", Traduction(pageHolder.page[1].buttonsList->buttons[i].text), *pageHolder.page[1].buttonsList->buttons[i].info);
            setButtonText(&pageHolder.page[1].buttonsList->buttons[i], txt);
        } else {
            setButtonText(&pageHolder.page[1].buttonsList->buttons[i], Traduction(pageHolder.page[1].buttonsList->buttons[i].text));
        }
    }
}

void createImgButton(uiPage * page,SDL_Rect rect, char *pathImg, char *pathBackground, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...) {
    if (page->buttonsImgList == NULL) {
        initListButtonImg(page->buttonsImgList);
    }
    va_list args;
    va_start(args, numArgs);
    void *params[numArgs];
    for (int i = 0; i < numArgs; i++) {
        params[i] = va_arg(args, void *);
    }
    va_end(args);

    ButtonImg newButton;
    newButton.rect = rect;
    newButton.offsetLogoX = offsetLogoX;
    newButton.offsetLogoY = offsetLogoY;
    newButton.callFunction = callFunction;
    newButton.args = malloc(numArgs * sizeof(void *));
    for (int i = 0; i < numArgs; i++) {
        newButton.args[i] = params[i];
    }
    if (!setButtonTexture(&newButton.imgTexture, pathImg)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.imgTexture)->%s : %s\n",pathImg,IMG_GetError());
        return;
    }
    if (!setButtonTexture(&newButton.backgroundTexture, pathBackground)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Button : Failed to load image (newButton.backgroundTexture) ->%s : %s\n", pathBackground,IMG_GetError());
        return;
    }

    page->buttonsImgList->buttons = realloc(page->buttonsImgList->buttons, (page->buttonsImgList->nbButton + 1) * sizeof(ButtonImg));
    if (page->buttonsImgList->buttons != NULL) {
        page->buttonsImgList->buttons[page->buttonsImgList->nbButton] = newButton;
        page->buttonsImgList->nbButton++;
    } else {
        printf("Button: Erreur lors de l'ajout du boutonImg a la page\n");
        free(newButton.args);
        SDL_DestroyTexture(newButton.imgTexture);
        SDL_DestroyTexture(newButton.backgroundTexture);
    }
}

void ButtonHandle() {
    if (currentpage->buttonsList == NULL) {
        printf("Initialisation de listeButton\n");
        initListButton(currentpage->buttonsList);
    }
    if (currentpage->buttonsImgList == NULL) {
        printf("Initialisation de listeButtonImg\n");
        initListButtonImg(currentpage->buttonsImgList);
    }
    if (currentpage->buttonsImgList != NULL) {
        for (int i = 0; i < currentpage->buttonsImgList->nbButton; i++) 
            drawButtonImg(&currentpage->buttonsImgList->buttons[i]);
    } else {
        printf("Erreur: listeButtonImg est toujours NULL après initialisation\n");
    }
    if (currentpage->buttonsList != NULL) {
        for (int i = 0; i < currentpage->buttonsList->nbButton; i++) {
            drawButton(&currentpage->buttonsList->buttons[i]);
            if (checkBoutton(currentpage->buttonsList->buttons[i].rect, mouseX, mouseY)) {
                if (currentpage->buttonsList->buttons[i].selectedTexture) {
                    currentpage->buttonsList->buttons[i].texture = currentpage->buttonsList->buttons[i].selectedTexture;
                }
                if (currentpage->buttonsList->buttons[i].growEffect > 1) {
                    setButtonScale(&currentpage->buttonsList->buttons[i], currentpage->buttonsList->buttons[i].growEffect);
                }
            } else {
                if (currentpage->buttonsList->buttons[i].selectedTexture) {
                    currentpage->buttonsList->buttons[i].texture = currentpage->buttonsList->buttons[i].initialTexture;
                }
                if (currentpage->buttonsList->buttons[i].growEffect > 1) {
                    setButtonScale(&currentpage->buttonsList->buttons[i], 1);
                }
            }
        }
    } else {
        printf("Erreur: listeButton est toujours NULL après initialisation\n");
    }
} 

void destroyAllButtonFromPage(ListeButton *listeButton) {
    if (!listeButton) return;
    
    for (int i = 0; i < listeButton->nbButton; i++) {
        if (listeButton->buttons[i].args) {
            free(listeButton->buttons[i].args);
            listeButton->buttons[i].args = NULL;
        }
        
        SDL_Texture *textureToFree = listeButton->buttons[i].texture;
        SDL_Texture *initialTextureToFree = listeButton->buttons[i].initialTexture;
        SDL_Texture *selectedTextureToFree = listeButton->buttons[i].selectedTexture;
        
        // Libérer texture principale si elle existe
        if (textureToFree) {
            SDL_DestroyTexture(textureToFree);
            listeButton->buttons[i].texture = NULL;
        }
        
        // Libérer initialTexture seulement si elle est différente de texture
        if (initialTextureToFree && initialTextureToFree != textureToFree) {
            SDL_DestroyTexture(initialTextureToFree);
        }
        listeButton->buttons[i].initialTexture = NULL;
        
        // Libérer selectedTexture seulement si elle existe et est différente des autres
        if (selectedTextureToFree && selectedTextureToFree != textureToFree && selectedTextureToFree != initialTextureToFree) {
            SDL_DestroyTexture(selectedTextureToFree);
        }
        listeButton->buttons[i].selectedTexture = NULL;
        
        // Libérer textTexture
        if (listeButton->buttons[i].textTexture) {
            SDL_DestroyTexture(listeButton->buttons[i].textTexture);
            listeButton->buttons[i].textTexture = NULL;
        }
    }
    
    if (listeButton->buttons) {
        free(listeButton->buttons);
        listeButton->buttons = NULL;
    }
    
    free(listeButton);
}

void destroyAllButtonImgFromPage(ListeButtonImg *listeButtonImg) {
    for (int i = 0; i < listeButtonImg->nbButton; i++) {
        free(listeButtonImg->buttons[i].args);
        listeButtonImg->buttons[i].args = NULL;
        SDL_DestroyTexture(listeButtonImg->buttons[i].imgTexture);
        listeButtonImg->buttons[i].imgTexture = NULL;
        SDL_DestroyTexture(listeButtonImg->buttons[i].backgroundTexture);
        listeButtonImg->buttons[i].backgroundTexture = NULL;
    }
    free(listeButtonImg->buttons);
    listeButtonImg->buttons = NULL;
    free(listeButtonImg);
}