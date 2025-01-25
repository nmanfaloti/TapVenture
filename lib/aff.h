#ifndef AFF_H
#define AFF_H
#include <SDL2/SDL_ttf.h>

typedef struct uiTxt_s{
    int tradID;
    int * info; 
    SDL_Rect dest;
    SDL_Color color;
}uiTxt;

typedef struct uiContainer_s{
    int nbTxt;
    uiTxt *txt;
}uiContainer;

extern int widthscreen;
extern int heightscreen;

void affiche_txt(SDL_Renderer* pRenderer, TTF_Font* font, char * txt, SDL_Rect dest, SDL_Color color);
SDL_Rect getSizeForText(TTF_Font* font, char * txt, SDL_Rect dest);
int vw(float percent);
int vh(float percent);

void createUIText(int tradID, int * info, SDL_Rect dest, SDL_Color color);
void uiHandle();
void initUItxt();

#endif // AFF_H