#include "../lib/sdl.h"
#include "../lib/aff.h"
#include "../lib/input_user.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

int init_SDL(){
    if (SDL_Init(SDL_INIT_VIDEO)){
        printf("Error SDL_Init\n");
        return 1;
    };
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == -1) {
        printf("IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, widthscreen, heightscreen, SDL_WINDOW_SHOWN);
    if (window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    font = TTF_OpenFont(LINK_FONT, 20);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    return 0;
}

void SDLExit(){
    if (font != NULL){
        TTF_CloseFont(font);
        font = NULL;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDLScreenHandler() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, mouseX, mouseY);
        SDL_RenderDrawLine(renderer, widthscreen, 0, mouseX, mouseY);
        SDL_RenderDrawLine(renderer, 0, heightscreen, mouseX, mouseY);
        SDL_RenderDrawLine(renderer, widthscreen, heightscreen, mouseX, mouseY);

        SDL_GetWindowSize(window, &widthscreen, &heightscreen);
}