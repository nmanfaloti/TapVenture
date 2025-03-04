#ifndef AFF_H
#define AFF_H
#include <SDL2/SDL_ttf.h>

typedef struct uiTxt_s{
    char * chaine;
    char * label;
    SDL_Rect dest;
    SDL_Color color;
    SDL_Texture * texture;
}uiTxt;

typedef struct uiImg_s{
    char * label;
    SDL_Rect dest;
    SDL_Texture * texture;
}uiImg;

typedef struct uiContainer_s{
    int nbTxt;
    uiTxt *txt;
    int nbImg;
    uiImg *img;
}uiContainer;


extern int widthscreen;
extern int heightscreen;

void affiche_txt(uiTxt * txt);
SDL_Rect getSizeForText(TTF_Font* font, char * txt, SDL_Rect dest);
int vw(float percent);
int vh(float percent);

void uiHandle();
void initUItxt();
void destroyUItxt();
void refreshMobHealth();
void refreshUILanguage();
void refreshMobKilled();
void refreshCurrentLvl();

void setUiText(uiTxt *txt, const char *text);

#endif // AFF_H