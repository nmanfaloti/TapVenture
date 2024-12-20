#include <SDL2/SDL.h>   
#include <stdlib.h>   
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "aff.h"

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
    SDL_DestroyTexture(texture);
}
