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
#include "../lib/sdl_init.h"
#include "../lib/player.h"

int main() {
    if (init_SDL()){
        return 1;
    }

    levelInfo level;
    level.timeToKill = -1.0;
    level.mobKilled = 0;
    level.mobToKill = 10;
    level.currentLvl = 0;
    damageShop shop;
    shop.damageLevel = 0;
    shop.lastPrice = 0;
    shop.nextPrice = getPriceForLevels(shop.damageLevel+1);
    damage_click = getDamageFromLevel(shop.damageLevel);
    
    loadSave();

    char health_txt[100], gold_txt[100], dmg_txt[100], remainingMob[100],currentLvl[100];

    createButton(getRectForCentenredCord(vw(50), vh(50), vw(30), vh(15)), (SDL_Color){0, 0, 255, 255}, CLICK_MSG, NULL, 1.1,(SDL_Color){255, 0, 0, 255}, attackButton, 5,&level.monstre[level.currentLvl], &gold, &damage_click, &level.mobKilled, &level.mobToKill);
    createButton(getRectForCentenredCord(vw(15), vh(50), vw(25), vh(15)), (SDL_Color){0, 150, 0, 255}, DMG_MSG, &(shop.nextPrice),1.05,(SDL_Color){100, 0, 0, 255}, upgradeButton, 3, &damage_click, &gold, &shop);
    
    createImgButton(getRectForCentenredCord(vw(50), vh(70), 100, 100), "assets/ui/buttons/Button_Blue_3Slides.png", "assets/ui/icons/Arrow_Down.png", 0, 5, upgradeButton, 3 , &damage_click, &gold, &shop);

    initLevel(level.monstre);

    void * argument[20];
    argument[0] = &level.monstre[level.currentLvl];
    argument[1] = &gold;
    argument[3] = &level.mobKilled;
    argument[4] = &level.mobToKill;
    
    int running = 1;
    int x, y;
    SDL_Event event;

    while (running){
        while (SDL_PollEvent(&event)){
            running = input_event(event);
        }
        
        attackHeros(heros, argument);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&x, &y);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, 0, x, y);
        SDL_RenderDrawLine(renderer, 0, heightscreen, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, heightscreen, x, y);

        sprintf(health_txt, "%s: %d", Traduction(VIE_MSG), level.monstre[level.currentLvl].mobHealth);
        sprintf(gold_txt, "%s: %d", Traduction(OR_MSG), gold);
        sprintf(dmg_txt, "%s %d", Traduction(DMG_MSG), damage_click);
        sprintf(remainingMob, "%s: %d/%d", Traduction(MOB_MSG), level.mobKilled, level.mobToKill);
        sprintf(currentLvl, "%s: %d", Traduction(LVL_MSG), level.currentLvl);

        if (isBoss(level.currentLvl)) {
            if (writeBossTimer(&level, renderer, font, (SDL_Rect){widthscreen/2,100,100,50}) == 1) {
                level.monstre[level.currentLvl].mobHealth = level.monstre[level.currentLvl].iniHealth;
                level.startTimer = SDL_GetTicks();
            }
        }else{
            if (level.mobToKill == 1){
                level.mobToKill = 10;
            }
        }

        ButtonHandle(renderer, font, x,y);
        affiche_txt(renderer, font, currentLvl, getRectForCentenredCord(widthscreen/2,30,100,50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(widthscreen/2, 30+50, 200, 50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, health_txt, getRectForCentenredCord(widthscreen/2, heightscreen/2-70, 200, 50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, gold_txt,(SDL_Rect){10,10,100,50}, (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, dmg_txt,(SDL_Rect){widthscreen-140,10,130,50}, (SDL_Color){255, 255, 255, 255});
        
        
        SDL_RenderPresent(renderer);

    }
    makeSave();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


/* TAILLE ECRAN */
/* sudo apt-get install libsdl2-image-dev
    Fullscreen 
SDL_GetRendererOutputSize(renderer, &widthscreen, &heightscreen);

    Windowed
SDL_GetWindowSize(window, &widthscreen, &heightscreen);
*/


/* TRADUCTION UTILISATION EXEMPLE */
/*
SelectLanguage("English");
printf("%s\n", Traduction(BIENVENUE_MSG));
printf("%s\n", Traduction(AUREVOIR_MSG));

SelectLanguage("French");
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

/*
LastAttack = os.time()
while (attacking) {
    
}

For ()
    LastAttack - os.time > Cooldown :
        Attack 
        LastAttack = os.time

50ms 
*/
