
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
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void *args[20]; //Arguments de la fonction (en void pour n'importe quel type)
    int offsetLogo;
} ButtonImg;


typedef struct {
    int nbButton;
    Button *buttons;
} ListeButton;

extern ListeButton *listeButton;

void ButtonHandle(SDL_Renderer *renderer, TTF_Font *font, int x, int y);
void initListButton();

void createButton(SDL_Rect rect, SDL_Color color, int txtInd, int * info,float growEffect,SDL_Color colorHover,  int (*callFunction)(void **), int numArgs, ...);
void destroyButton();
void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font);
int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y);
SDL_Rect getRectForCentenredCord(int x, int y, int w, int h);
void draw_button_image(SDL_Renderer *renderer, SDL_Rect rect, char *texture, char *background, int offsetLogo);