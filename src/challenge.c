#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/combat.h"

int challengeActive = 0;
unsigned int challengeStartTime;

unsigned int challengeDuration = 30; // Durée du challenge en secondes
int challengeTarget = 15; // Nombre de monstres à tuer pour réussir le challenge
int challengeReward = 500; // Récompense pour avoir réussi le challenge * le lvl actuel

int lvl = 1;

int launchChallenge() {
    if (challengeActive) {
        printf("Un challenge est déjà en cours !\n");
        return 1;
    }
    if (isBoss(level.currentLvl) && level.currentLvl != 0) {
        printf("Impossible de lancer un challenge sur un boss !\n");
        return 1;
    }

    printf("Challenge lancé !\n");
    challengeActive = 1;
    challengeStartTime = SDL_GetTicks();
    level.mobKilled = 0; // Réinitialiser le compteur de monstres tués pour le challenge
    level.mobToKill = challengeTarget + 1; 
    return 0;
}

void resetChallenge() {
    challengeActive = 0;
    level.mobToKill = 10;
    level.mobKilled = 0;
}

void updateChallenge() {
    if (!challengeActive) return;

    unsigned int currentTime = SDL_GetTicks();
    unsigned int elapsedTimeChallenge = (currentTime - challengeStartTime) / 1000;

    if (elapsedTimeChallenge >= challengeDuration) {
        printf("Challenge échoué !\n");
        resetChallenge();
        return;
    }

    if (level.mobKilled >= challengeTarget) {
        if (level.currentLvl != 0) lvl=level.currentLvl;
        gold += challengeReward * lvl; 
        printf("Challenge réussi ! Vous avez gagné %d pièces d'or.\n", challengeReward * lvl);
        resetChallenge();
        return;
    }
}

void displayChallengeTimer(SDL_Renderer* renderer, TTF_Font* font) {
    unsigned int currentTime = SDL_GetTicks();
            unsigned int elapsedTimeChallenge = (currentTime - challengeStartTime) / 1000;
            unsigned int remainingTime = challengeDuration - elapsedTimeChallenge;

            char timerText[50];
            sprintf(timerText, "Temps restant: %d secondes", remainingTime);
            affiche_txt(renderer, font, timerText, getRectForCentenredCord(vw(50), vh(20), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255});
}