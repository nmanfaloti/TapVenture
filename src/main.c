/**
 * @file main.c
 * @brief Point d'entrée principal pour le jeu TapVenture.
 *
 * Ce fichier contient la fonction principale qui initialise les ressources,
 * gère la boucle principale du jeu et libère les ressources à la fin.
 *
 * @details
 * - Initialise SDL et d'autres modules nécessaires.
 * - Gère les événements utilisateur et les mises à jour de l'interface.
 * - Maintient une boucle de rendu avec un contrôle de la fréquence d'images.
 * - Sauvegarde les données et libère les ressources avant de quitter.
 *
 * @author Votre Nom
 * @date 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../lib/lang.h"
#include "../lib/save_json.h"
#include "../lib/combat.h"
#include "../lib/boutique.h"
#include "../lib/heros.h"
#include "../lib/input_user.h"
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
    srand( time( NULL ) );

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
        mobAnimationHandler();
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
