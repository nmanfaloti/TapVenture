#ifndef AFF_H
#define AFF_H
#include <SDL2/SDL_ttf.h>

extern int widthscreen;
extern int heightscreen;

void affiche_txt(SDL_Renderer* pRenderer, TTF_Font* font, char * txt, SDL_Rect dest, SDL_Color color);
SDL_Rect getSizeForText(TTF_Font* font, char * txt, SDL_Rect dest);
int vw(float percent);
int vh(float percent);


#endif // AFF_H