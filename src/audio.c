#include "../lib/audio.h"

Mix_Chunk * musics[MUSIC_COUNT];

//Mix_AllocateChannels(2); // Allouer 2 cannaux 
//Mix_Volume(0, MIX_MAX_VOLUME); // Mets le son a 100% en volume pour le premier cannaux
//Mix_Volume(1, MIX_MAX_VOLUME / 2); // Mets le son a 50% en volume pour le deuxièmme cannaux 

int initMusic(){
    loadMusic(MUSIC_GAME, "assets/audio/game.mp3");
    loadMusic(MUSIC_BUZZ, "assets/audio/buzz.mp3");
    musics[MUSIC_COUNT] = NULL;
    //Mix_AllocateChannels(2);
    Mix_Volume(CANAL_MUSIC_BACK, MIX_MAX_VOLUME);
    Mix_Volume(CANAL_EFFECT, MIX_MAX_VOLUME / 8);
    return 0;
}

bool loadMusic(musicIndex musicIndex, char * path){
    musics[musicIndex] = Mix_LoadWAV(path);
    if (musics[musicIndex] == NULL){
        printf("Erreur lors du chargement de la musique %d\n", musicIndex);
        return true;
    }
    return false;
}

void playMusic(int musicIndex, canalsIndex canalsIndex, int nbLoops){ // nbLoops a 0 pour une lecture en boucle
    if (musics[musicIndex] != NULL && musicIndex < MUSIC_COUNT && musicIndex >= 0 && canalsIndex < CANAL_COUNT && canalsIndex >= 0 && nbLoops >= 0){
        Mix_PlayChannel(canalsIndex, musics[musicIndex], nbLoops-1);
    }
    else{
        printf("Erreur lors de la lecture de la musique %d\n", musicIndex);
    }
}

void freeAllMusic(){
    for (int i = MUSIC_GAME; i < MUSIC_COUNT; i++){
        if (musics[i] != NULL){
            Mix_FreeChunk(musics[i]);
            musics[i] = NULL;
        }
    }
}