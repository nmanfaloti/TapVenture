#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/combat.h"
#include "../lib/lang.h"

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
        // createNotif("Challenge",32,1,"assets/ui/notif.png", 1, 3, (SDL_Rect){vw(50), vh(24), vw(40), vh(30)},30, 1.8,1, Traduction(CHALLENGE_DESC_MSG_LOSE));
        resetChallenge();
        return;
    }

    if (level.mobKilled >= challengeTarget) {
        char goldEarnedMsg[100];
        if (level.currentLvl != 0) lvl=level.currentLvl;
        addGold(challengeReward * lvl);
        sprintf(goldEarnedMsg, "%d", challengeReward * lvl);
        // createNotif("Challenge",32,1,"assets/ui/notif.png", 1, 3, (SDL_Rect){vw(50), vh(24), vw(40), vh(30)},30, 1.8,2, Traduction(CHALLENGE_DESC_MSG_WIN), goldEarnedMsg);
        resetChallenge();
        return;
    }
}

void displayChallengeTimer(TTF_Font* font) {
    unsigned int currentTime = SDL_GetTicks();
            unsigned int elapsedTimeChallenge = (currentTime - challengeStartTime) / 1000;
            unsigned int remainingTime = challengeDuration - elapsedTimeChallenge;

            char timerText[50];
            sprintf(timerText, "Temps restant: %d secondes", remainingTime);
            // affiche_txt(renderer, font, timerText, getRectForCentenredCord(vw(50), vh(20), vh(40), vh(7)), (SDL_Color){255, 255, 255, 255});
}