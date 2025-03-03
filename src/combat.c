#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/combat.h"
#include "../lib/aff.h"
#include "../lib/sdl.h"
#include "../lib/player.h"
#include "../lib/lang.h"
#include "../lib/ui.h"

levelInfo level;

int initLevel(monstreInfo monstre[]) {
    level.timeToKill = -1.0;
    level.mobKilled = 0;
    level.mobToKill = 10;
    level.currentLvl = 0;
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

int attack(void * args[20]) {
    int * damage= args[0];
    char txt[50] = "";
    uiTxt * txtHolder;
    monstreInfo * currentMonstre = &level.monstre[level.currentLvl];
    currentMonstre->mobHealth -=  *damage;
    if (currentMonstre->mobHealth <= 0) {
        level.mobKilled += 1;
        addGold(currentMonstre->coinMin + rand() % (currentMonstre->coinMax - currentMonstre->coinMin + 1));
        currentMonstre->mobHealth = currentMonstre->iniHealth;
        if (level.mobKilled >= level.mobToKill) {
            level.currentLvl += 1;
            level.mobKilled = 0;
            mobHandler();
            refreshCurrentLvl();
        }
        refreshMobKilled();
    }
    //Actualisation de la vie du monstre (A la fin pour eviter d'avoir la vie du monstre a 0 ou -)
    refreshMobHealth();
    return 1;
}

int initBoss(int difficultyTime) {
    level.mobToKill = 1;
    level.timeToKill = difficultyTime;
    level.startTimer = SDL_GetTicks();

    return 1;
}

int isBoss(int currentLvl) {
    return currentLvl % 5 == 0;
}

int writeBossTimer(SDL_Rect dest) {
    if (level.currentLvl % 5 != 0 || level.timeToKill == -1) {
        return 0;
    }
    int timeLeft = level.timeToKill - (SDL_GetTicks() - level.startTimer) / 1000;
    char timeLeftTxt[100];
    sprintf(timeLeftTxt, "%d", timeLeft);
    // affiche_txt(renderer, font, timeLeftTxt, getSizeForText(font,timeLeftTxt, dest), (SDL_Color){255, 255, 255, 255});
    if (timeLeft <= 0) {
        return 1;
    }
    return 0;
}

void mobHandler() {
    initBoss(30);
    if (isBoss(level.currentLvl)) {
        if (writeBossTimer((SDL_Rect){vh(50),100,100,50}) == 1) {
            level.monstre[level.currentLvl].mobHealth = level.monstre[level.currentLvl].iniHealth;
            level.startTimer = SDL_GetTicks();
        }
    }else{
        if (level.mobToKill == 1){
            level.mobToKill = 10;
        }
    }
}