#ifndef sdl_H
#define sdl_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define LINK_FONT "assets/fonts/ChakraPetch-SemiBold.ttf"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern TTF_Font *fontBig;

int init_SDL();
void SDLExit();
void SDLScreenHandler();
#endif // sdl_H