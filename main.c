#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "traduction/lang.h"
#include "option/json.h"
#include "affichage/aff.h"
#include "button/button.h"
#include "combat/combat.h"
#include "boutique/boutique.h"
#include "heros/heros.h"

void refreshButton(Button listButton[]) {
    listButton[BUTTON_CLICK].rect = getRectForCentenredCord(widthscreen/2, heightscreen/2, 240, 80);
    listButton[BUTTON_CLICK].iniRect = listButton[BUTTON_CLICK].rect;
    listButton[BUTTON_DMG].rect = getRectForCentenredCord(110, heightscreen/2, 160, 80);
    listButton[BUTTON_DMG].iniRect = listButton[BUTTON_DMG].rect;
    listButton[BUTTON_DMG].text = Traduction(UPG_MSG);
    listButton[BUTTON_CLICK].text = Traduction(CLICK_MSG);
}

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
    if (IMG_Init(IMG_INIT_PNG) == -1) {
        printf("IMG_Init: %s\n", IMG_GetError());
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

    levelInfo level;
    level.timeToKill = -1.0;
    level.mobKilled = 0;
    level.mobToKill = 10;
    level.currentLvl = 0;
    int gold = 0;
    damageShop shop;
    shop.damageLevel = 0;
    shop.lastPrice = 0;
    shop.nextPrice = getPriceForLevels(shop.damageLevel+1);
    int damage = getDamageFromLevel(shop.damageLevel);
    SelectLanguage(en);


    char damagebutton[100],health_txt[100], gold_txt[100], dmg_txt[100], remainingMob[100],currentLvl[100];
    sprintf(damagebutton, "%s: %d", Traduction(UPG_MSG), shop.nextPrice);
    Button listButton[BUTTON_COUNT];
    float growEffect[BUTTON_COUNT] = {0};
    listButton[BUTTON_CLICK].rect = getRectForCentenredCord(widthscreen/2, heightscreen/2, 240, 80);
    listButton[BUTTON_CLICK].iniRect = listButton[BUTTON_CLICK].rect;
    listButton[BUTTON_CLICK].color = (SDL_Color){0, 0, 255, 255};
    listButton[BUTTON_CLICK].text = Traduction(CLICK_MSG);
    listButton[BUTTON_CLICK].callFunction = attackButton;
    listButton[BUTTON_CLICK].args[0] = &level.monstre[level.currentLvl];
    listButton[BUTTON_CLICK].args[1] = &gold;
    listButton[BUTTON_CLICK].args[2] = &damage;
    listButton[BUTTON_CLICK].args[3] = &level.mobKilled;
    listButton[BUTTON_CLICK].args[4] = &level.mobToKill;
    growEffect[BUTTON_CLICK] = 1.1;
    listButton[BUTTON_DMG].rect = getRectForCentenredCord(110, heightscreen/2, 160, 80);
    listButton[BUTTON_DMG].iniRect = listButton[BUTTON_DMG].rect;
    listButton[BUTTON_DMG].color = (SDL_Color){150, 0, 0, 255};
    listButton[BUTTON_DMG].text = damagebutton;
    listButton[BUTTON_DMG].callFunction = upgradeButton;
    listButton[BUTTON_DMG].args[0] = &damage;
    listButton[BUTTON_DMG].args[1] = &gold;
    listButton[BUTTON_DMG].args[2] = &shop;
    
    ButtonImg listButtonImg[BUTTON_IMG_COUNT];
    listButtonImg[BUTTON_IMG].rect = getRectForCentenredCord(widthscreen/2, heightscreen/2, 240, 80);
    listButtonImg[BUTTON_IMG].iniRect = listButtonImg[BUTTON_IMG].rect;
    listButtonImg[BUTTON_IMG].background = "assets/ui/buttons/Button_Blue.png";
    listButtonImg[BUTTON_IMG].texture = "assets/ui/icons/Arrow_Left.png";
    listButtonImg[BUTTON_IMG].offsetLogo = 10;

    initLevel(level.monstre);

    hero listHeros[HEROS_COUNT];
    initHeros(listHeros);
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
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        for (int i = 0; i < BUTTON_COUNT; i++) {
                            if (checkBoutton(listButton[i].rect, mouseX, mouseY)) {
                                if (listButton[i].callFunction) {
                                    listButton[i].callFunction(listButton[i].args);
                                }
                            }
                        }
                        // for (int i = 0; i < BUTTON_IMG_COUNT; i++) {
                        //     if (checkBoutton(listButtonImg[i].rect, mouseX, mouseY)) {
                        //         if (listButtonImg[i].callFunction) {
                        //             listButtonImg[i].callFunction(listButtonImg[i].args);
                        //         }
                        //     }
                        // }
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (SDL_GetKeyName(event.key.keysym.sym)[0]){
                        case 'F':
                            if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                                SDL_SetWindowFullscreen(window, 0);
                                SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                            } else {
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                                SDL_GetRendererOutputSize(renderer, &widthscreen, &heightscreen);
                            }
                            refreshButton(listButton);
                            break;
                        case 'H':
                            SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                            refreshButton(listButton);
                            break;
                        case 'E': 
                            SelectLanguage(en);
                            refreshButton(listButton);
                            break;
                        case 'R':
                            SelectLanguage(fr);
                            refreshButton(listButton);
                            break;
                        case 'W':
                            printf("Windowed\n");
                            SDL_SetWindowFullscreen(window, 0);
                            SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                            break;
                        case 'Q':
                            printf("Souris Coords : %d %d ", x, y);
                            printf("Center Screen coord %d %d", widthscreen/2, heightscreen/2);
                            break;
                        case '0':
                            upgradeHero(listHeros,0,&gold);
                            printf("tentative Upgrade Hero 0, nouveaux prix: %d\n", listHeros[0].prix);
                            break;
                        case '1':
                            upgradeHero(listHeros,1,&gold);
                            printf("tentative Upgrade Hero 1, nouveaux prix: %d\n",listHeros[1].prix);
                            break;
                        default:
                            printf("Touche Inconnu: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                            break;
                    }
                default:
                    break;
            }
        }
        
        attackHeros(listHeros, argument);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&x, &y);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, 0, x, y);
        SDL_RenderDrawLine(renderer, 0, heightscreen, x, y);
        SDL_RenderDrawLine(renderer, widthscreen, heightscreen, x, y);

        sprintf(damagebutton, "%s: %d", Traduction(UPG_MSG), shop.nextPrice);
        sprintf(health_txt, "%s: %d", Traduction(VIE_MSG), level.monstre[level.currentLvl].mobHealth);
        sprintf(gold_txt, "%s: %d", Traduction(OR_MSG), gold);
        sprintf(dmg_txt, "%s %d", Traduction(DMG_MSG), damage);
        sprintf(remainingMob, "%s: %d/%d", Traduction(MOB_MSG), level.mobKilled, level.mobToKill);
        sprintf(currentLvl, "%s: %d", Traduction(LVL_MSG), level.currentLvl);

        affiche_txt(renderer, font, currentLvl, getRectForCentenredCord(widthscreen/2,30,100,50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, remainingMob, getRectForCentenredCord(widthscreen/2, 30+50, 200, 50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, health_txt, getRectForCentenredCord(widthscreen/2, heightscreen/2-70, 200, 50), (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, gold_txt,(SDL_Rect){10,10,100,50}, (SDL_Color){255, 255, 255, 255});
        affiche_txt(renderer, font, dmg_txt,(SDL_Rect){widthscreen-140,10,130,50}, (SDL_Color){255, 255, 255, 255});
        for (int i = 0; i < BUTTON_COUNT; i++) {
            draw_button(renderer, listButton[i].rect, listButton[i].color, listButton[i].text, font);
            if (checkBoutton(listButton[i].rect, x, y)) {
                listButton[i].color = (SDL_Color){125, 125, 0, 255};
                if (growEffect[i] != 0) {
                    listButton[i].rect = (SDL_Rect){
                        listButton[i].iniRect.x - (listButton[i].iniRect.w * (growEffect[i] - 1) / 2),
                        listButton[i].iniRect.y - (listButton[i].iniRect.h * (growEffect[i] - 1) / 2),
                        listButton[i].iniRect.w * growEffect[i],
                        listButton[i].iniRect.h * growEffect[i]
                    };
                }
            } else {
                listButton[i].color = (SDL_Color){0, 0, 255, 255};
                if (growEffect[i]){
                    listButton[i].rect = listButton[i].iniRect;
                }
            }
        }
        for (int i = 0; i < BUTTON_IMG_COUNT; i++) {
            // draw_button_image(renderer, listButtonImg[i].rect, listButtonImg[i].texture, listButtonImg[i].background, listButtonImg[i].offsetLogo);
            if (checkBoutton(listButtonImg[i].rect, x, y)) {
                listButtonImg[i].rect = (SDL_Rect){
                    listButtonImg[i].iniRect.x - (listButtonImg[i].iniRect.w * 0.1 / 2),
                    listButtonImg[i].iniRect.y - (listButtonImg[i].iniRect.h * 0.1 / 2),
                    listButtonImg[i].iniRect.w * 1.1,
                    listButtonImg[i].iniRect.h * 1.1
                };
            } else {
                listButtonImg[i].rect = listButtonImg[i].iniRect;
            }
        }

        SDL_RenderPresent(renderer);

    }

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
