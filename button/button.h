typedef enum {
    BUTTON_CLICK,
    BUTTON_DMG,
    BUTTON_COUNT
} ButtonIndex;


typedef enum {
    BUTTON_IMG,
    BUTTON_IMG_COUNT
} ButtonImgIndex;

typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    SDL_Color color;
    char *text;
    int (*callFunction)(void **); //Pointeur sur la fonction executé lors du click
    void *args[20]; //Arguments de la fonction (en void pour n'importe quel type)
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


void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font);
int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y);
SDL_Rect getRectForCentenredCord(int x, int y, int w, int h);

void draw_button_image(SDL_Renderer *renderer, SDL_Rect rect, char *texture, char *background, int offsetLogo);