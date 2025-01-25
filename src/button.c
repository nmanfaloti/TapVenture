#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../lib/button.h"
#include "../lib/lang.h"
#include "../lib/input_user.h"

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

ListeButton *listeButton = NULL;
void initListButton() {
    listeButton = malloc(sizeof(ListeButton));
    if (listeButton != NULL) {
        listeButton->nbButton = 0;
        listeButton->buttons = NULL;
        printf("listeButton initialisé avec succès\n");
    } else {
        printf("Erreur d'allocation mémoire pour listeButton\n");
    }
}
ListeButtonImg *listeButtonImg = NULL;
void initListButtonImg() {
    listeButtonImg = malloc(sizeof(ListeButtonImg));
    if (listeButtonImg != NULL) {
        listeButtonImg->nbButton = 0;
        listeButtonImg->buttons = NULL;
        printf("listeButtonImg initialisé avec succès\n");
    } else {
        printf("Erreur d'allocation mémoire pour listeButtonImg\n");
    }
}

void createButton(SDL_Rect rect, SDL_Color color, int txtInd, int * info,float growEffect,SDL_Color colorHover,  int (*callFunction)(void **), int numArgs, ...) {
    if (listeButton == NULL) {
        initListButton();
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

    listeButton->buttons = realloc(listeButton->buttons, (listeButton->nbButton + 1) * sizeof(Button));
    if (listeButton->buttons != NULL) {
        listeButton->buttons[listeButton->nbButton] = newButton;
        listeButton->nbButton++;
    } else {
        free(newButton.args);
    }
}

void createImgButton(SDL_Rect rect, char *texture, char *background, int offsetLogoX, int offsetLogoY, int (*callFunction)(void **), int numArgs, ...) {
    if (listeButtonImg == NULL) {
        initListButtonImg();
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

    listeButtonImg->buttons = realloc(listeButtonImg->buttons, (listeButtonImg->nbButton + 1) * sizeof(ButtonImg));
    if (listeButtonImg->buttons != NULL) {
        listeButtonImg->buttons[listeButtonImg->nbButton] = newButton;
        listeButtonImg->nbButton++;
    } else {
        free(newButton.args);
    }
}


void ButtonHandle(SDL_Renderer *renderer, TTF_Font *font) {
    if (listeButton == NULL) {
        printf("Initialisation de listeButton\n");
        initListButton();
    }
    if (listeButtonImg == NULL) {
        printf("Initialisation de listeButtonImg\n");
        initListButtonImg();
    }
    if (listeButtonImg != NULL) {
        for (int i = 0; i < listeButtonImg->nbButton; i++) {
            draw_button_image(renderer, listeButtonImg->buttons[i].rect, listeButtonImg->buttons[i].background, listeButtonImg->buttons[i].texture, listeButtonImg->buttons[i].offsetLogoX , listeButtonImg->buttons[i].offsetLogoY);
        }
    } else {
        printf("Erreur: listeButtonImg est toujours NULL après initialisation\n");
    }
    if (listeButton != NULL) {
        for (int i = 0; i < listeButton->nbButton; i++) {
            char txt[50] = "";
            if (listeButton->buttons[i].info != NULL) {
                sprintf(txt, "%s %d", Traduction(listeButton->buttons[i].text), *listeButton->buttons[i].info);
            }   
            else{
                sprintf(txt, "%s", Traduction(listeButton->buttons[i].text));
            }
            draw_button(renderer, listeButton->buttons[i].rect, listeButton->buttons[i].color, txt, font);
            if (checkBoutton(listeButton->buttons[i].rect, mouseX, mouseY)) {
                listeButton->buttons[i].color = listeButton->buttons[i].colorHover;
                if (listeButton->buttons[i].growEffect != 0) {
                    listeButton->buttons[i].rect = (SDL_Rect){
                        listeButton->buttons[i].iniRect.x - (listeButton->buttons[i].iniRect.w * (listeButton->buttons[i].growEffect - 1) / 2),
                        listeButton->buttons[i].iniRect.y - (listeButton->buttons[i].iniRect.h * (listeButton->buttons[i].growEffect - 1) / 2),
                        listeButton->buttons[i].iniRect.w * listeButton->buttons[i].growEffect,
                        listeButton->buttons[i].iniRect.h * listeButton->buttons[i].growEffect
                    };
                }
            } else {
                listeButton->buttons[i].color = listeButton->buttons[i].iniColor;
                if (listeButton->buttons[i].growEffect) {
                    listeButton->buttons[i].rect = listeButton->buttons[i].iniRect;
                }
            }
        }
    } else {
        printf("Erreur: listeButton est toujours NULL après initialisation\n");
    }
}

void destroyButton() {
    for (int i = 0; i < listeButton->nbButton; i++) {
        free(listeButton->buttons[i].args);
    }
    free(listeButton->buttons);
    free(listeButton);
}

void destroyButtonImg() {
    for (int i = 0; i < listeButtonImg->nbButton; i++) {
        free(listeButtonImg->buttons[i].args);
    }
    free(listeButtonImg->buttons);
    free(listeButtonImg);
}