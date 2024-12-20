typedef struct {
    int mobHealth;
    int iniHealth;
    int coinMax;
    int coinMin;
} monstreInfo;

typedef struct {
    monstreInfo monstre[50];
    float timeToKill;
    int mobKilled;
    int mobToKill;
    int currentLvl;
} levelInfo;

int attackButton(void * args[20]);

int initLevel(monstreInfo monstre[]);
