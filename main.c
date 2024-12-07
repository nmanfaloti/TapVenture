#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "traduction/lang.h"
#include "option/json.h"
#include "affichage/aff.h"

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

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)){
        printf("Error SDL_Init\n");
        return 1;
    };
    
    printf("Test Width: \n");
    printf("Avant : %d\n",widthscreen);
    widthscreen = 640;
    printf("Apres : %d\n",widthscreen);


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


    int running = 1;
    SDL_Event event;

    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, x, y);
        SDL_RenderDrawLine(renderer, 640, 0, x, y);
        SDL_RenderDrawLine(renderer, 0, 480, x, y);
        SDL_RenderDrawLine(renderer, 640, 480, x, y);


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
