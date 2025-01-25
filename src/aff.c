#include <SDL2/SDL.h>   
#include <stdlib.h>   
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/aff.h"
#include "../lib/lang.h"
#include "../lib/sdl.h"
#include "../lib/combat.h"
#include "../lib/button.h"
#include "../lib/player.h"

uiContainer container;
int widthscreen = 800;
int heightscreen = 500;

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

void createUIText(int tradID, int * info, SDL_Rect dest, SDL_Color color){
    if (container.txt == NULL) {
        container.txt = malloc(sizeof(uiTxt));
    } else {
        container.txt = realloc(container.txt, sizeof(uiTxt) * (container.nbTxt + 1));
    }
    container.txt[container.nbTxt].tradID = tradID;
    container.txt[container.nbTxt].info = info;
    container.txt[container.nbTxt].dest = dest;
    container.txt[container.nbTxt].color = color;
    container.nbTxt++;
}

void uiHandle(){
    char *txt;
    char info[50];
    for (int i = 0; i < container.nbTxt; i++){
        sprintf(info, "%d", *container.txt[i].info);
        if (container.txt[i].info != NULL){
            const char *trad = Traduction(container.txt[i].tradID);
            size_t txt_size = strlen(trad) + strlen(info) + 3; // +3 pour : et \0
            txt = malloc(txt_size);
            if (txt == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            snprintf(txt, txt_size, "%s: %s", trad, info);
        } else {
            const char *trad = Traduction(container.txt[i].tradID);
            size_t txt_size = strlen(trad) + 1;  // +1 pour \0
            txt = malloc(txt_size);
            if (txt == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            snprintf(txt, txt_size, "%s", trad);
        }
        affiche_txt(renderer, font, txt, container.txt[i].dest, container.txt[i].color);
        free(txt);
    }
    char remainingMob[10];
    sprintf(remainingMob, "/%d", level.mobToKill);
    affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(vw(64.5), vh(10), vh(6), vh(6.2)) , (SDL_Color){255, 255, 255, 255});
}

void initUItxt(){
    container.nbTxt = 0;
    container.txt = NULL;

    createUIText(VIE_MSG, &(level.monstre[level.currentLvl].mobHealth), getRectForCentenredCord(vw(50), vh(37), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255});
    createUIText(OR_MSG, &gold, (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255});
    // createUIText(DMG_MSG, &damage, (SDL_Rect) {vw(1),vh(1), vh(15), vh(10)}, (SDL_Color){255, 255, 255, 255});
    createUIText(MOB_MSG, &level.mobKilled, getRectForCentenredCord(vw(50), vh(10), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255});
    createUIText(LVL_MSG, &level.currentLvl, getRectForCentenredCord(vw(50), vh(4), vh(20), vh(7)), (SDL_Color){255, 255, 255, 255});

}

void destroyUItxt(){
    free(container.txt);
}