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
    SDL_Window * window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
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

    Button listButton[10];
    listButton[0].rect = (SDL_Rect){200, 150, 240, 80};
    listButton[0].color = (SDL_Color){0, 0, 255, 255};
    listButton[0].text = "Click Me";
    listButton[1].rect = (SDL_Rect){15,250,100,50};
    listButton[1].color = (SDL_Color){255, 0, 0, 255};
    listButton[1].text = "Click Me";

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
                listButton[i].color = (SDL_Color){255, 255, 0, 255};
            } else {
                listButton[i].color = (SDL_Color){0, 0, 255, 255};
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