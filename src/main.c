#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../lib/lang.h"
#include "../lib/save_json.h"
#include "../lib/aff.h"
#include "../lib/button.h"
#include "../lib/combat.h"
#include "../lib/boutique.h"
#include "../lib/heros.h"
#include "../lib/input_user.h"
#include "../lib/sdl.h"
#include "../lib/player.h"
#include "../lib/ui.h"

int main() {
    if (init_SDL()){
        return 1;
    }

    
    initLevel(level.monstre);
    initShop();
    initMainPage();

    void * argument[20];
    argument[0] = &gold;
    argument[2] = &level.mobKilled;
    argument[3] = &level.mobToKill;
    
    int running = 1;
    SDL_Event event;
    loadSave();

    while (running){
        while (SDL_PollEvent(&event)){
            running = input_event(event);
        }
        SDLScreenHandler();
        ButtonHandle(renderer, font);
        uiHandle();
        attackHeros(heros, argument);
        
        SDL_RenderPresent(renderer);
    }
    destroyPage(currentpage);
    makeSave();
    SDLExit();
    return 0;
}

