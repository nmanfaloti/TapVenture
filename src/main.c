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
#include "../lib/challenge.h"
#include "../lib/chaine.h"
#include "../lib/prestige.h"

int main() {
    if (init_SDL()){
        return 1;
    }

    initLevel(level.monstre);
    initShop();
    initLang("English");
    initPage();
    
    int running = 1;
    SDL_Event event;
    loadSave();
    


    while (running){
        while (SDL_PollEvent(&event)){
            running = input_event(event);
        }
        SDLScreenHandler();
        uiImgHandle();
        ButtonHandle();
        uiHandle();
        attackHeros();
        updateChallenge();
        displayTimers();

        SDL_RenderPresent(renderer);
    }
    if(challenge.active){
        resetChallenge();
    }
    destroyPrestigeList();
    destroyAllPages();
    makeSave();
    SDLExit();
    return 0;
}

