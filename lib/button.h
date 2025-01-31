#ifndef BUTTON_H
#define BUTTON_H 

typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    SDL_Color color;
    SDL_Color iniColor;
    SDL_Color colorHover;
    float growEffect;
    int * info;
    int text;
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void **args; // Pointeur pour les arguments de la fonction
} Button;

typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    char *texture;
    char *background;
    int offsetLogoX;
    int offsetLogoY;
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void **args; // Pointeur pour les arguments de la fonction
} ButtonImg;

typedef struct {
    int nbButton;
    Button *buttons;
} ListeButton;

typedef struct {
    int nbButton;
    ButtonImg *buttons;
} ListeButtonImg;

void ButtonHandle(SDL_Renderer *renderer, TTF_Font *font);
void initListButton(ListeButton *listeButton);
void initListButtonImg(ListeButtonImg *listeButtonImg);


void destroyButton(ListeButton *listeButton);
void destroyButtonImg(ListeButtonImg *listeButton);
void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font);
int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y);
SDL_Rect getRectForCentenredCord(int x, int y, int w, int h);
void draw_button_image(SDL_Renderer *renderer, SDL_Rect rect, char *texture, char *background, int offsetLogoX, int offsetLogoY);

#endif // BUTTON_H