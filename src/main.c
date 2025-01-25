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

int main() {
    if (init_SDL()){
        return 1;
    }

    damageShop shop;
    shop.damageLevel = 0;
    shop.lastPrice = 0;
    shop.nextPrice = getPriceForLevels(shop.damageLevel+1);
    damage_click = getDamageFromLevel(shop.damageLevel);
    
    loadSave();
    initUItxt();
    createButton(getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)), (SDL_Color){0, 0, 255, 255}, CLICK_MSG, NULL, 1.1,(SDL_Color){255, 0, 0, 255}, attackButton, 4, &gold, &damage_click, &level.mobKilled, &level.mobToKill);
    createButton(getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, DMG_MSG, &(shop.nextPrice),1.05,(SDL_Color){100, 0, 0, 255}, upgradeButton, 3, &damage_click, &gold, &shop);
    createImgButton(getRectForCentenredCord(vw(50), vh(70), 100, 100), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Arrow_Down.png", 0, 5, attackButton, 4, &gold, &damage_click, &level.mobKilled, &level.mobToKill);

    initLevel(level.monstre);

    void * argument[20];
    argument[0] = &level.monstre[level.currentLvl];
    argument[1] = &gold;
    argument[3] = &level.mobKilled;
    argument[4] = &level.mobToKill;
    
    int running = 1;
    SDL_Event event;

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
    destroyButton();
    destroyButtonImg();
    destroyUItxt();
    makeSave();
    SDLExit();
    return 0;
}

