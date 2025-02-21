
#include "../lib/sdl.h"
#include "../lib/aff.h"
#include "../lib/heros.h"
#include "../lib/lang.h"
#include "../lib/button.h"
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/challenge.h"

int mouseX = 0;
int mouseY = 0;

int mouseXclickG = 0;
int mouseXclickD = 0;
int game_running = 1;


int input_event(SDL_Event event){
    switch (event.type) {
        case SDL_QUIT:
            return 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseXclickG = event.button.x;
                mouseXclickD = event.button.y;
                /*appelle de la fonction pour check si on click sur un bouton */
                for (int i = 0; i < currentpage->buttonsList->nbButton; i++) {
                    if (checkBoutton(currentpage->buttonsList->buttons[i].rect, mouseXclickG, mouseXclickD)) {
                        if (currentpage->buttonsList->buttons[i].callFunction) {
                            currentpage->buttonsList->buttons[i].callFunction(currentpage->buttonsList->buttons[i].args);
                        }
                    }
                }
                for (int i = 0; i < currentpage->buttonsImgList->nbButton; i++) {
                    if (checkBoutton(currentpage->buttonsImgList->buttons[i].rect, mouseXclickG, mouseXclickD)) {
                        if (currentpage->buttonsImgList->buttons[i].callFunction) {
                            currentpage->buttonsImgList->buttons[i].callFunction(currentpage->buttonsImgList->buttons[i].args);
                        }
                    }
                }
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
                    break;
                case 'H':
                    SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                    break;
                case 'E': 
                    // SelectLanguage("English");
                    break;
                case 'R':
                    // SelectLanguage("French");
                    break;
                case 'W':
                    printf("Windowed\n");
                    SDL_SetWindowFullscreen(window, 0);
                    SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                    break;
                case 'Q':
                    printf("Souris Coords : %d %d ", mouseX, mouseY);
                    printf("Center Screen coord %d %d", widthscreen/2, heightscreen/2);
                    break;
                default:
                    printf("Touche Inconnu: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                    break;
                case '0':
                    upgradeHero(0, &gold);
                    printf("hero 0, level : %d, degat : %d, prix : %d\n", heros[0].level, heros[0].degat,heros[0].prix );
                    break;
                case '1':
                    upgradeHero(1, &gold);
                    printf("hero 1, level : %d, degat : %d, prix : %d\n", heros[1].level, heros[1].degat,heros[1].prix );
                    break;
                case '2':
                    printf("Degat générer par les heros /s %d\n",herosDPS());
                    break;
                case '3':
                    printf("Gold générer par les heros /s %d\n",herosGoldGenBySec());
                    break;
                case 'C':
                    launchChallenge(NULL);
                    break;
            }
        default:
            break;
    }

    SDL_GetMouseState(&mouseX, &mouseY);
    return 1;
}