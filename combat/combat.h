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

int attackButton(void * args[20]);
int initLevel(monstreInfo monstre[]);
int InitBoss(levelInfo *level, int diffTime);
int isBoss(int currentLvl);
int writeBossTimer(levelInfo *level, SDL_Renderer* pRenderer, TTF_Font* font, SDL_Rect dest);
int setLevel(levelInfo *level, int lvl);