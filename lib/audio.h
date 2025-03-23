#ifndef AUDIO_H
#define AUDIO_H 

#include <stdbool.h>
#include "../lib/sdl.h"

typedef enum {
    MUSIC_GAME,
    MUSIC_CLICK,
    MUSIC_LEVEL_UP,
    MUSIC_LEVEL_UP_BOSS,
    MUSIC_COIN,
    MUSIC_ATTACK,
    MUSIC_COUNT
} musicIndex;

typedef enum {
    CANAL_MUSIC_BACK,
    CANAL_EFFECT,
    CANAL_COUNT
} canalsIndex;


int initMusic();
bool loadMusic(musicIndex musicIndex, char * path);
void playMusic(int musicIndex, canalsIndex canalsIndex, int nbLoops);
void stopAllMusic();
void freeAllMusic();


#endif // AUDIO_H