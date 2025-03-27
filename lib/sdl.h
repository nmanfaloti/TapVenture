#ifndef sdl_H
#define sdl_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#define LINK_FONT "assets/fonts/ChakraPetch-SemiBold.ttf"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern TTF_Font *fontBig;
extern SDL_Cursor *cursor;
extern SDL_Cursor *classicCursor;
extern SDL_Cursor *swordCursor;
extern SDL_Cursor *handCursor;

int init_SDL();
void SDLExit();
void SDLScreenHandler();
#endif // sdl_H