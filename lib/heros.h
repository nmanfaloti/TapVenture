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


int initHeros(hero heros[HEROS_COUNT]);
int attackHeros(hero heros[HEROS_COUNT], void * args[20]);
int upgradeHero(hero heros[HEROS_COUNT], int HeroIndex, int * gold);

extern hero heros[HEROS_COUNT];

#endif // HEROS_H