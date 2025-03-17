#ifndef HEROS_H
#define HEROS_H

#include <stdbool.h>

#define NB_HEROS_PER_PAGE_H 3
#define NB_HEROS_PER_PAGE_W 4

typedef enum {
    HERO0,
    HERO1,
    HERO2,
    HERO3,
    HERO4,
    HERO5,
    HERO6,
    HERO7,
    HERO8,
    HERO9,
    HERO10,
    HERO11,
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
int upgradeHeroCB(void * args[20]);
int makeHeroAtLevel(int heroIndex, int level);

unsigned long long int getHeroPriceByMultiplicator(int heroIndex);
unsigned long long int getHeroPrice(int heroIndex);

unsigned long long int herosDPS(int indiceHero);
unsigned long long int herosAllDPS();
unsigned long long int herosGoldGenBySec();

void initHerosPage();
void loadHerosMactrice();
void updateHeroShopPage();
void updateHeroIInShopPage(int heroIndex);
int changeMultiplicator();

extern hero heros[HEROS_COUNT];

#endif // HEROS_H