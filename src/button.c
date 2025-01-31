#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../lib/button.h"
#include "../lib/ui.h"

#include "../lib/lang.h"
#include "../lib/input_user.h"
#include "../lib/aff.h"

void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    float scale = 1.0;
    if (surface->w > rect.w) {
        scale = (float)rect.w / (float)surface->w;
    }
    if (surface->h > rect.h) {
        float scale_h = (float)rect.h / (float)surface->h;
        if (scale_h < scale) {
            scale = scale_h;
        }
    }
    SDL_Rect textRect = {rect.x + rect.w / 2 - surface->w * scale / 2, rect.y + rect.h / 2 - surface->h * scale / 2, surface->w * scale, surface->h * scale};
    

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y) {
    return mouse_x >= rect.x && mouse_x <= (rect.x + rect.w) && mouse_y >= rect.y && mouse_y <= (rect.y + rect.h);
}

SDL_Rect getRectForCentenredCord(int x, int y, int w, int h) {
    return (SDL_Rect){x - w / 2, y - h / 2, w, h};
}

void draw_button_image(SDL_Renderer *renderer, SDL_Rect rect, char *pathText, char *pathBackground, int offsetLogoX, int offsetLogoY) {
    SDL_Surface *surface = IMG_Load(pathText);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Surface *surfaceBackground = IMG_Load(pathBackground);
    if (!surfaceBackground) {
        printf("Failed to load image: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Texture *textureBackground = SDL_CreateTextureFromSurface(renderer, surfaceBackground);
    if (!textureBackground) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        SDL_FreeSurface(surfaceBackground);
        return;
    }

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surfaceBackground);

    int icon_width, icon_height;
    SDL_QueryTexture(texture, NULL, NULL, &icon_width, &icon_height);
    SDL_Rect rectOffest = {rect.x - offsetLogoX, rect.y - offsetLogoY, rect.w, rect.h};

    SDL_RenderCopy(renderer, textureBackground, NULL, &rect);
    SDL_RenderCopy(renderer, texture, NULL, &rectOffest);

    SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(texture);
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


void createButton(uiPage * page,SDL_Rect rect, SDL_Color color, int txtInd, int * info,float growEffect,SDL_Color colorHover,  int (*callFunction)(void **), int numArgs, ...) {
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

    Button newButton;
    newButton.rect = rect;
    newButton.iniRect = rect;
    newButton.color = color;
    newButton.iniColor = color;
    newButton.text = txtInd;
    newButton.info = info;
    newButton.growEffect = growEffect;
    newButton.colorHover = colorHover;
    newButton.callFunction = callFunction;
    newButton.args = malloc(numArgs * sizeof(void *));
    for (int i = 0; i < numArgs; i++) {
        newButton.args[i] = params[i];
    }

    page->buttonsList->buttons = realloc(page->buttonsList->buttons, (page->buttonsList->nbButton + 1) * sizeof(Button));
    if (page->buttonsList->buttons != NULL) {
        page->buttonsList->buttons[page->buttonsList->nbButton] = newButton;
        page->buttonsList->nbButton++;
    } else {
        free(newButton.args);
    }
}

void createImgButton(uiPage * page,SDL_Rect rect, char *texture, char *background, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...) {
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
    newButton.texture = texture;
    newButton.background = background;
    newButton.offsetLogoX = offsetLogoX;
    newButton.offsetLogoY = offsetLogoY;
    newButton.callFunction = callFunction;
    newButton.args = malloc(numArgs * sizeof(void *));
    for (int i = 0; i < numArgs; i++) {
        newButton.args[i] = params[i];
    }

    page->buttonsImgList->buttons = realloc(page->buttonsImgList->buttons, (page->buttonsImgList->nbButton + 1) * sizeof(ButtonImg));
    if (page->buttonsImgList->buttons != NULL) {
        page->buttonsImgList->buttons[page->buttonsImgList->nbButton] = newButton;
        page->buttonsImgList->nbButton++;
    } else {
        free(newButton.args);
    }
}

void ButtonHandle(SDL_Renderer *renderer, TTF_Font *font) {
    if (currentpage->buttonsList == NULL) {
        printf("Initialisation de listeButton\n");
        initListButton(currentpage->buttonsList);
    }
    if (currentpage->buttonsImgList == NULL) {
        printf("Initialisation de listeButtonImg\n");
        initListButtonImg(currentpage->buttonsImgList);
    }
    if (currentpage->buttonsImgList != NULL) {
        for (int i = 0; i < currentpage->buttonsImgList->nbButton; i++) {
            draw_button_image(renderer, currentpage->buttonsImgList->buttons[i].rect, currentpage->buttonsImgList->buttons[i].background, currentpage->buttonsImgList->buttons[i].texture, currentpage->buttonsImgList->buttons[i].offsetLogoX , currentpage->buttonsImgList->buttons[i].offsetLogoY);
        }
    } else {
        printf("Erreur: listeButtonImg est toujours NULL après initialisation\n");
    }
    if (currentpage->buttonsList != NULL) {
        for (int i = 0; i < currentpage->buttonsList->nbButton; i++) {
            char txt[50] = "";
            if (currentpage->buttonsList->buttons[i].info != NULL) {
                sprintf(txt, "%s %d", Traduction(currentpage->buttonsList->buttons[i].text), *currentpage->buttonsList->buttons[i].info);
            }   
            else{
                sprintf(txt, "%s", Traduction(currentpage->buttonsList->buttons[i].text));
            }
            draw_button(renderer, currentpage->buttonsList->buttons[i].rect, currentpage->buttonsList->buttons[i].color, txt, font);
            if (checkBoutton(currentpage->buttonsList->buttons[i].rect, mouseX, mouseY)) {
                currentpage->buttonsList->buttons[i].color = currentpage->buttonsList->buttons[i].colorHover;
                if (currentpage->buttonsList->buttons[i].growEffect != 0) {
                    currentpage->buttonsList->buttons[i].rect = (SDL_Rect){
                        currentpage->buttonsList->buttons[i].iniRect.x - (currentpage->buttonsList->buttons[i].iniRect.w * (currentpage->buttonsList->buttons[i].growEffect - 1) / 2),
                        currentpage->buttonsList->buttons[i].iniRect.y - (currentpage->buttonsList->buttons[i].iniRect.h * (currentpage->buttonsList->buttons[i].growEffect - 1) / 2),
                        currentpage->buttonsList->buttons[i].iniRect.w * currentpage->buttonsList->buttons[i].growEffect,
                        currentpage->buttonsList->buttons[i].iniRect.h * currentpage->buttonsList->buttons[i].growEffect
                    };
                }
            } else {
                currentpage->buttonsList->buttons[i].color = currentpage->buttonsList->buttons[i].iniColor;
                if (currentpage->buttonsList->buttons[i].growEffect) {
                    currentpage->buttonsList->buttons[i].rect = currentpage->buttonsList->buttons[i].iniRect;
                }
            }
        }
    } else {
        printf("Erreur: listeButton est toujours NULL après initialisation\n");
    }
}


void destroyButton(ListeButton *listeButton) {
    for (int i = 0; i < listeButton->nbButton; i++) {
        free(listeButton->buttons[i].args);
        listeButton->buttons[i].args = NULL;
    }
    free(listeButton->buttons);
    listeButton->buttons = NULL;
    free(listeButton);
}

void destroyButtonImg(ListeButtonImg *listeButtonImg) {
    for (int i = 0; i < listeButtonImg->nbButton; i++) {
        free(listeButtonImg->buttons[i].args);
        listeButtonImg->buttons[i].args = NULL;
    }
    free(listeButtonImg->buttons);
    listeButtonImg->buttons = NULL;
    free(listeButtonImg);
}