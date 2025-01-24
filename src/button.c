#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../lib/button.h"

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

void draw_button_image(SDL_Renderer *renderer, SDL_Rect rect, char *pathText, char *pathBackground, int offsetLogo) {
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
    SDL_Rect rectOffest = {rect.x - offsetLogo, rect.y - offsetLogo, rect.w, rect.h};

    SDL_RenderCopy(renderer, textureBackground, NULL, &rect);
    SDL_RenderCopy(renderer, texture, NULL, &rectOffest);

    SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(texture);
}