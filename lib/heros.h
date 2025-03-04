#ifndef HEROS_H
#define HEROS_H

typedef enum {
    HERO0,
    HERO1,
    HEROS_COUNT
} herosIndex;

typedef struct {
    int degat;
    int prix;
    int level;
    int cooldown;
    unsigned int lastAttack;
}hero;

int initHeros();
int attackHeros();
int upgradeHero(int HeroIndex, int * gold);
int makeHeroAtLevel(int heroIndex, int level);

float herosDPS(int indice);
int herosAllDPS();
int herosGoldGenBySec();

extern hero heros[HEROS_COUNT];

#endif // HEROS_H