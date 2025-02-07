#include <SDL2/SDL.h>

typedef struct {
    int mobHealth;
    int iniHealth;
    int coinMax;
    int coinMin;
} monstreInfo;

typedef struct {
    monstreInfo monstre[50];
    int timeToKill;
    unsigned int startTimer; 
    int mobKilled;
    int mobToKill;
    int currentLvl;
} levelInfo;

extern levelInfo level;

int attack(void * args[20]);
int initLevel(monstreInfo monstre[]);
int InitBoss(int diffTime);
int isBoss(int currentLvl);
int writeBossTimer(SDL_Rect dest);
int setLevel(levelInfo *level, int lvl);
void mobHandler();