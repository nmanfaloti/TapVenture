#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "../lib/heros.h"
#include "../lib/combat.h"
#include "../lib/player.h"

#define DEGATS_BASE 10
#define PRIX_BASE 100
#define DEFAULT_COOLDOWN 1000
//UP correspond a la différence entre les heros 
#define DEGAT_UP 1.20
#define PRIX_UP 1.15
#define COOLDOWN_UP 1.1
//UPGRADE pour les amelioration de heros
#define DEGAT_UPGRADE 1.20
#define PRIX_UPGRADE 1.15

hero heros[HEROS_COUNT];

int initHeros(){
    heros[HERO0].degat = DEGATS_BASE;
    heros[HERO0].prix = PRIX_BASE;
    heros[HERO0].level = 0;
    heros[HERO0].cooldown = DEFAULT_COOLDOWN; //en millisecondes
    heros[HERO0].lastAttack = 0;
    for (int i = HERO1; i < HEROS_COUNT; i++) {
        heros[i].degat = heros[i-1].degat * DEGAT_UP;
        heros[i].prix = heros[i-1].prix * PRIX_UP;
        heros[i].level = 0;
        heros[i].cooldown = heros[i-1].cooldown * COOLDOWN_UP;
        heros[i].lastAttack = 0;
    }
    return 0;
}

int attackHeros(){
    void * args[20];
    for (int i = HERO0; i < HEROS_COUNT; i++) {
        if( heros[i].level > 0 && heros[i].lastAttack + heros[i].cooldown <= SDL_GetTicks() ){
            heros[i].lastAttack = SDL_GetTicks();
            args[0] = &heros[i].degat;
            attack(args); 
        }
    }
    return 0;
}

int upgradeHero(int heroIndex, int * gold) {
    if (gold == NULL){
        heros[heroIndex].level += 1;
        heros[heroIndex].prix *= PRIX_UPGRADE;
        heros[heroIndex].degat *= DEGAT_UPGRADE;
        return 0;
    }
    if (heroIndex < HEROS_COUNT && heros[heroIndex].prix <= *gold) {
        heros[heroIndex].level += 1;
        *gold -= heros[heroIndex].prix;
        heros[heroIndex].prix *= PRIX_UPGRADE;
        heros[heroIndex].degat *= DEGAT_UPGRADE;
        return 0;
    }
    return 1;
}

int makeHeroAtLevel(int heroIndex, int levelH) {
    if (heroIndex < HEROS_COUNT) {
        while (heros[heroIndex].level < levelH) {
            upgradeHero(heroIndex, NULL);
        }
        return 0;
    }
    return 1;
}

int herosDPS(){
    int somme = 0;
    for (int i = HERO0; i < HEROS_COUNT; i++) {
        if (heros[i].level > 0){
            somme += heros[i].degat / (heros[i].cooldown /1000 );
        }
    }
    return somme; 
}

int herosGoldGenParSec(){
    monstreInfo * currentMonstre = &level.monstre[level.currentLvl];
    int coinMoy = (currentMonstre->coinMin + currentMonstre->coinMax) / 2;
    float nbMonstresMortParSecondes = herosDPS() * 1.0 / currentMonstre->iniHealth * 1.0;
    return nbMonstresMortParSecondes * coinMoy;

}