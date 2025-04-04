
#include "../lib/sdl.h"
#include "../lib/aff.h"
#include "../lib/heros.h"
#include "../lib/lang.h"
#include "../lib/button.h"
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/challenge.h"
#include "../lib/audio.h"
#include "../lib/inv.h"

int mouseX = 0;
int mouseY = 0;

int mouseXclickG = 0;
int mouseXclickD = 0;
int game_running = 1;


int input_event(SDL_Event event){
    if (quitGameValue == 1){
        return 0;
    }
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
                            break; //Pour ne pas cliquer sur plusieurs boutons en meme temps
                        }
                    }
                }
                for (int i = 0; i < currentpage->buttonsImgList->nbButton; i++) {
                    if (checkBoutton(currentpage->buttonsImgList->buttons[i].rect, mouseXclickG, mouseXclickD)) {
                        if (currentpage->buttonsImgList->buttons[i].callFunction) {
                            currentpage->buttonsImgList->buttons[i].callFunction(currentpage->buttonsImgList->buttons[i].args);
                            break; //Pour ne pas cliquer sur plusieurs boutons en meme temps
                        }
                    }
                }
            }
            break;
        case SDL_KEYDOWN:
            switch (SDL_GetKeyName(event.key.keysym.sym)[0]){
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
                    SDL_SetWindowFullscreen(window, 0);
                    SDL_GetWindowSize(window, &widthscreen, &heightscreen);
                    break;
                case 'Q':
                    printf("Souris Coords : %d %d ", mouseX, mouseY);
                    printf("Center Screen coord %d %d", widthscreen/2, heightscreen/2);
                    break;
                default:
                    // printf("Touche Inconnu: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                    break;
                case 'C':
                    launchChallenge(NULL);
                    break;
            }
        default:
            break;
    }
    if ( &(pageHolder.page[4]) == currentpage) {
        handle_inv_event(event);
    }
    SDL_GetMouseState(&mouseX, &mouseY);
    return 1;
}