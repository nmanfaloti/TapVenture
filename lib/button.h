#ifndef BUTTON_H
#define BUTTON_H 

#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    char * pathImg;
    float growEffect;
    int * info;
    int text;
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void **args; // Pointeur pour les arguments de la fonction
    SDL_Texture *texture;  //Texture de l'image de fond du bouton
    SDL_Texture *initialTexture; //Sauvegarde de la texture initial
    SDL_Texture *selectedTexture; //Texture de l'image de fond du bouton lors du hover
    SDL_Texture *textTexture; //Texture du texte du bouton
    SDL_Rect textRect; //Position du texte
    SDL_Rect textIniRect; //Position initial du texte
    SDL_Color textColor; 
    TTF_Font *font;
} Button;

typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    char * pathImg;
    char * pathBackground;
    int offsetLogoX;
    int offsetLogoY;
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void **args; // Pointeur pour les arguments de la fonction
    SDL_Texture *imgTexture;  //Texture de l'image de fond du bouton
    SDL_Texture *backgroundTexture; //Texture de l'image de fond du bouton
    char * label;
} ButtonImg;

typedef struct {
    int nbButton;
    Button *buttons;
} ListeButton;

typedef struct {
    int nbButton;
    ButtonImg *buttons;
} ListeButtonImg;

void ButtonHandle();
void initListButton(ListeButton *listeButton);
void initListButtonImg(ListeButtonImg *listeButtonImg);
void refreshButtonShop();
void refreshButtonLanguage();

void destroyAllButtonFromPage(ListeButton *listeButton);
void destroyAllButtonImgFromPage(ListeButtonImg *listeButton);
void drawButton(Button *button);
int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y);
SDL_Rect getRectForCentenredCord(int x, int y, int w, int h);
void drawButtonImg(ButtonImg *button);

#endif // BUTTON_H