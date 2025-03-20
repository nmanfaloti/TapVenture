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
#include "../lib/inv.h"
#include "../lib/audio.h"


int main() {
    if (init_SDL()){
        return 1;
    }

    initLevel(level.monstre);
    initShop();
    init_inv_main();
    initLang("English");
    initPage();
    initMusic();
    playMusic(MUSIC_GAME, CANAL_MUSIC_BACK, 0); // 0 pour une lecture en boucle
    
    int running = 1;
    SDL_Event event;
    loadSave();
    
    const int FPS = 6000;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    int frameCount = 0;
    Uint32 lastTime = SDL_GetTicks();

    while (running){
        frameStart = SDL_GetTicks();

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

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        frameCount++;
        if (SDL_GetTicks() - lastTime >= 1000) {
            // system("clear");
            // printf("TapVenture\n");
            // printf("FPS: %d\n", frameCount);
            frameCount = 0;
            lastTime = SDL_GetTicks();
        }
    }
    if(challenge.active){
        resetChallenge();
    }
    makeSave();
    freeAllMusic();
    dest_all_inventaires() ;
    destroyPrestigeList();
    destroyAllPages();
    SDLExit();
    return 0;
}
