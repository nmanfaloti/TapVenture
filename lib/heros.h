#ifndef HEROS_H
#define HEROS_H

#include <stdbool.h>

typedef enum {
    HERO0,
    HERO1,
    HEROS_COUNT
} herosIndex;

typedef struct {
    unsigned long long int degat;
    unsigned long long int prix;
    int level;
    int cooldown;
    unsigned int lastAttack;
}hero;

int initHeros();
int attackHeros();
int upgradeHero(int HeroIndex, bool pay);
int makeHeroAtLevel(int heroIndex, int level);

unsigned long long int herosDPS(int indiceHero);
unsigned long long int herosAllDPS();
unsigned long long int herosGoldGenBySec();

void initHerosPage();

extern hero heros[HEROS_COUNT];

#endif // HEROS_H