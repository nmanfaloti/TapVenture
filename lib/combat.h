#include <SDL2/SDL.h>

typedef struct {
    unsigned long long int mobHealth;
    unsigned long long int iniHealth;
    unsigned long long int coinMax;
    unsigned long long int coinMin;
} monstreInfo;


typedef struct {
    monstreInfo monstre[51];
    int timeToKill;
    unsigned int startTimer; 
    int mobKilled;
    int mobToKill;
    int currentLvl;
    char * label[5];
    char * img[5];
} levelInfo;


extern levelInfo level;

int attack(void * args[20]);
int initLevel(monstreInfo monstre[]);
int InitBoss(int diffTime);
int isBoss(int currentLvl);
int writeBossTimer(SDL_Rect dest);
int setLevel(levelInfo *level, int lvl);
void mobHandler();
void displayTimers();