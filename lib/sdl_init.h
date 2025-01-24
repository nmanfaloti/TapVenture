#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

int init_SDL();

SDL_Window * window;
SDL_Renderer * renderer;
TTF_Font *font;

#endif // SDL_INIT_H