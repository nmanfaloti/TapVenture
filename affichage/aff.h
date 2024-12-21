#include <SDL2/SDL_ttf.h>

void affiche_txt(SDL_Renderer* pRenderer, TTF_Font* font, char * txt, SDL_Rect dest, SDL_Color color);
SDL_Rect getSizeForText(TTF_Font* font, char * txt, SDL_Rect dest);

extern int widthscreen;
extern int heightscreen;