
#include "../lib/sdl_init.h"
#include "../lib/aff.h"
#include "../lib/heros.h"
#include "../lib/lang.h"
#include "../lib/button.h"
#include "../lib/player.h"

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
                    for (int i = 0; i < listeButton->nbButton; i++) {
                        if (checkBoutton(listeButton->buttons[i].rect, mouseXclickG, mouseXclickD)) {
                            if (listeButton->buttons[i].callFunction) {
                                listeButton->buttons[i].callFunction(listeButton->buttons[i].args);
                            }
                        }
                    }
                    for (int i = 0; i < listeButtonImg->nbButton; i++) {
                        if (checkBoutton(listeButtonImg->buttons[i].rect, mouseXclickG, mouseXclickD)) {
                            if (listeButtonImg->buttons[i].callFunction) {
                                listeButtonImg->buttons[i].callFunction(listeButtonImg->buttons[i].args);
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
                        SelectLanguage("English");
                        break;
                    case 'R':
                        SelectLanguage("French");
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
                        upgradeHero(heros, 0, &gold);
                        printf("Upgrade Hero 0\n");
                        break;
                    case '1':
                        upgradeHero(heros, 1, &gold);
                        printf("Upgrade Hero 1\n");
                        break;
                }
            default:
                break;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        return 1;
}