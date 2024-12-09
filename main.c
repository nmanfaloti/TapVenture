#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "traduction/lang.h"
#include "option/json.h"
#include "affichage/aff.h"
#include "button/button.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)){
        printf("Error SDL_Init\n");
        return 1;
    };
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Window * window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, widthscreen, heightscreen, SDL_WINDOW_SHOWN);
    if (window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    TTF_Font *font = TTF_OpenFont("assets/fonts/Planes_ValMore.ttf", 20);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SelectLanguage(en);
    Button listButton[BUTTON_COUNT];
    int growEffect[BUTTON_COUNT];
    listButton[BUTTON_TEST1].rect = (SDL_Rect){widthscreen/8,heightscreen/8, 240, 80};
    listButton[BUTTON_TEST1].iniRect = listButton[BUTTON_TEST1].rect;
    listButton[BUTTON_TEST1].color = (SDL_Color){0, 0, 255, 255};
    listButton[BUTTON_TEST1].text = Traduction(BIENVENUE_MSG);
    growEffect[BUTTON_TEST1] = 1;
    SelectLanguage(fr);
    listButton[BUTTON_TEST2].rect = (SDL_Rect){widthscreen/2,heightscreen/2,100,50};
    listButton[BUTTON_TEST2].iniRect = listButton[BUTTON_TEST2].rect;
    listButton[BUTTON_TEST2].color = (SDL_Color){255, 0, 0, 255};
    listButton[BUTTON_TEST2].text = Traduction(BIENVENUE_MSG);

    int running = 1;
    int x, y;
    SDL_Event event;
    while (running){
        while (SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        for (int i = 0; i < 2; i++) {
                            if (checkBoutton(listButton[i].rect, mouseX, mouseY)) {
                                printf("Button %d clicked\n", i);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&x, &y);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, 0, x, y);
        SDL_RenderDrawLine(renderer, 0, heightscreen, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, heightscreen, x, y);

       for (int i = 0; i < 2; i++) {
            draw_button(renderer, listButton[i].rect, listButton[i].color, listButton[i].text, font);
            if (checkBoutton(listButton[i].rect, x, y)) {
                listButton[i].color = (SDL_Color){125, 125, 0, 255};
                if (growEffect[i]){
                    float scaleFactor = 1.1;
                    listButton[i].rect = (SDL_Rect){
                        listButton[i].iniRect.x - (listButton[i].iniRect.w * (scaleFactor - 1) / 2),
                        listButton[i].iniRect.y - (listButton[i].iniRect.h * (scaleFactor - 1) / 2),
                        listButton[i].iniRect.w * scaleFactor,
                        listButton[i].iniRect.h * scaleFactor
                    };
                }
            } else {
                listButton[i].color = (SDL_Color){0, 0, 255, 255};
                if (growEffect[i]){
                    listButton[i].rect = listButton[i].iniRect;
                }
            }
        }

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/* TRADUCTION UTILISATION EXEMPLE */
/*
SelectLanguage(en);
printf("%s\n", Traduction(BIENVENUE_MSG));
printf("%s\n", Traduction(AUREVOIR_MSG));

SelectLanguage(fr);
printf("%s\n", Traduction(BIENVENUE_MSG));
printf("%s\n", Traduction(AUREVOIR_MSG));
*/

/* JSON UTILISATION EXEMPLE */
/*
char * username = getValueForKey("username", "option/test.json");
if (username) {
    printf("username: %s\n", username);
    free(username);
}
char * newusername = "dross";
if (ChangeValueForKey("username", newusername, "option/test.json")){
    printf("Erreur durant la modification %s : %s\n", "username", newusername);
}

username = getValueForKey("username", "option/test.json");
if (username) {
    printf("username: %s\n", username);
    free(username);
}
if (ChangeValueForKey("language","en","option/test.json")){
    printf("Erreur durant la modification %s : %s\n", "language", "en");
}
*/

/* Text Exemple */
/* 
TTF_Font *font = TTF_OpenFont("assets/fonts/Planes_ValMore.ttf", 20);
if (font == NULL) {
    printf("Failed to load font: %s\n", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 1;
}
affiche_txt(renderer, font, "Hello World", (SDL_Rect){widthscreen/4, heightscreen/4, widthscreen/2, heightscreen/2}, (SDL_Color){255, 255, 255, 255});
*/

/* Button Exemple */
/*  
Button listButton[10];
listButton[0].rect = (SDL_Rect){200, 150, 240, 80};
listButton[0].color = (SDL_Color){0, 0, 255, 255};
listButton[0].text = "Click Me";

for (int i = 0; i < 1; i++) {
    draw_button(renderer, listButton[i].rect, listButton[i].color, listButton[i].text, font);
    if (checkBoutton(listButton[i].rect, x, y)) {
        listButton[i].color = (SDL_Color){255, 255, 0, 255};
    } else {
        listButton[i].color = (SDL_Color){0, 0, 255, 255};
    }
}
*/