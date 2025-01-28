#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "../lib/heros.h"
#include "../lib/combat.h"

hero heros[HEROS_COUNT];

int initHeros(hero heros[HEROS_COUNT]){
    heros[HERO0].degat = 10;
    heros[HERO0].prix = 100;
    heros[HERO0].level = 0;
    heros[HERO0].cooldown = 1000; //en millisecondes
    heros[HERO0].lastAttack = 0;
    for (int i = HERO1; i < HEROS_COUNT; i++) {
        heros[i].degat = heros[i-1].degat * 1.5;
        heros[i].prix = heros[i-1].prix * 1.5;
        heros[i].level = 0;
        heros[i].cooldown = heros[i-1].cooldown * 1.1;
        heros[i].lastAttack = 0;
    }
    return 0;
}

int attackHeros(hero heros[HEROS_COUNT], void * args[20]) {
    for (int i = HERO0; i < HEROS_COUNT; i++) {
        if( heros[i].level > 0 && heros[i].lastAttack + heros[i].cooldown <= SDL_GetTicks() ){
            heros[i].lastAttack = SDL_GetTicks();
            args[1] = &heros[i].degat;
            attackButton(args); 
        }
    }
    return 0;
}

int upgradeHero(hero heros[HEROS_COUNT], int HeroIndex, int * gold) {
    if (HeroIndex < HEROS_COUNT && heros[HeroIndex].prix <= *gold) {
        heros[HeroIndex].level += 1;
        *gold -= heros[HeroIndex].prix;
        heros[HeroIndex].prix *= 1.5;
        heros[HeroIndex].degat *= 1.5;
        return 0;
    }
    return 1;
}
