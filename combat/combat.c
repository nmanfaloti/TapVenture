#include <stdio.h>
#include <stdlib.h>
#include "combat.h"


int initLevel(monstreInfo monstre[]) {
    monstre[0].mobHealth = 100;
    monstre[0].iniHealth = 100;
    monstre[0].coinMin = 5;
    monstre[0].coinMax = 15;
    //Initialisation des monstres (En fonction du niveau)
    for (int i = 1; i < 50; i++) {
        monstre[i].mobHealth = monstre[i-1].mobHealth * 1.5;
        monstre[i].iniHealth = monstre[i].mobHealth;
        monstre[i].coinMin = monstre[i-1].coinMin * 1.25;
        monstre[i].coinMax = monstre[i-1].coinMax * 1.25;
    }
    //Initialisation des Boss apres afin de pas fausser les prochains monstres
    for (int i = 1; i < 50; i++) {
        if (i % 5 == 0) {
            monstre[i].mobHealth = monstre[i-1].mobHealth * 5;
            monstre[i].iniHealth = monstre[i].mobHealth;
            monstre[i].coinMin = monstre[i-1].coinMin * 3.5;
            monstre[i].coinMax = monstre[i-1].coinMax * 3.5;
        }
    }
    return 0;
}

int attackButton(void * args[20]) {
    levelInfo * level = args[0];
    int * gold = args[1];
    int * damage = args[2];
    int * mobKilled = args[3];
    int * mobToKill = args[4];

    monstreInfo * currentMonstre = &level->monstre[level->currentLvl];
    //Gestion des combats de boss (Le combat commence des que le joueur attaque le boss)
    if (level->currentLvl % 5 ==0){
        level->timeToKill = 30.0;
        level->mobToKill = 1;
    }else{
        level->timeToKill = -1.0;
        level->mobToKill = 10;
    }
    currentMonstre->mobHealth -= *damage;

    if (currentMonstre->mobHealth <= 0) {
        *mobKilled += 1;
        *gold += currentMonstre->coinMin + rand() % (currentMonstre->coinMax - currentMonstre->coinMin + 1);
        currentMonstre->mobHealth = currentMonstre->iniHealth;

        if (*mobKilled >= *mobToKill) {
            level->currentLvl += 1;
            *mobKilled = 0;
        }
    }
    return 1;
}