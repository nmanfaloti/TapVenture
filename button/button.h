typedef enum {
    BUTTON_TEST1,
    BUTTON_TEST2,
    BUTTON_COUNT
} ButtonIndex;



typedef struct {
    SDL_Rect rect;
    SDL_Rect iniRect;
    SDL_Color color;
    char *text;
} Button;

void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font);
int checkBoutton(SDL_Rect rect, int mouse_x, int mouse_y);
