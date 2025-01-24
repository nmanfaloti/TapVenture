#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "lib/combat.h"
#include "lib/aff.h"

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
    currentMonstre->mobHealth -= *damage;

    if (currentMonstre->mobHealth <= 0) {
        *mobKilled += 1;
        *gold += currentMonstre->coinMin + rand() % (currentMonstre->coinMax - currentMonstre->coinMin + 1);
        currentMonstre->mobHealth = currentMonstre->iniHealth;

        if (*mobKilled >= *mobToKill) {
            level->currentLvl += 1;
            *mobKilled = 0;

            if (level->currentLvl % 5 == 0) {
                InitBoss(level, 30);
            }
        }
    }
    return 1;
}

int InitBoss(levelInfo *level, int difficultyTime) {
    level->mobToKill = 1;
    level->timeToKill = difficultyTime;
    level->startTimer = SDL_GetTicks();

    return 1;
}

int isBoss(int currentLvl) {
    return currentLvl % 5 == 0;
}

int writeBossTimer(levelInfo *level, SDL_Renderer* pRenderer, TTF_Font* font, SDL_Rect dest) {
    if (level->currentLvl % 5 != 0 || level->timeToKill == -1) {
        return 0;
    }

    int timeLeft = level->timeToKill - (SDL_GetTicks() - level->startTimer) / 1000;
    char timeLeftTxt[100];
    sprintf(timeLeftTxt, "%d", timeLeft);
    affiche_txt(pRenderer, font, timeLeftTxt, getSizeForText(font,timeLeftTxt, dest), (SDL_Color){255, 255, 255, 255});
    if (timeLeft <= 0) {
        return 1;
    }
    return 0;
}

