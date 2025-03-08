#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/challenge.h"
#include "../lib/player.h"
#include "../lib/combat.h"
#include "../lib/lang.h"
#include "../lib/ui.h"
#include "../lib/aff.h"
#include "../lib/sdl.h"

#define DURATION 30 // Durée du challenge en secondes
#define TARGET 15 // Nombre de monstres à tuer pour réussir le challenge
#define REWARD 500 // Récompense pour avoir réussi le challenge * le lvl actuel
#define COOLDOWN 60*30 // Cooldown en minutes entre chaque challenge


Challenge_t challenge = {
    .active = 0,
    .startTime = 0,
    .duration = DURATION,
    .target = TARGET,
    .reward = REWARD,
    .cooldown = COOLDOWN,
    .lastTime = 0
};
char timeToWaitMsg[100];


int lvl = 1;

int launchChallenge() {
    if (challenge.active) {
        printf("Un challenge est déjà en cours !\n");
        return 1;
    }
    if (isBoss(level.currentLvl) && level.currentLvl != 0) {
        printf("Impossible de lancer un challenge sur un boss !\n");
        return 1;
    }
    if (time(NULL) - challenge.lastTime < challenge.cooldown) {
        sprintf(timeToWaitMsg, "%s %ld %s",Traduction(CHALLENGE_MSG_COOLDOWN), 30 - (time(NULL) - challenge.lastTime) / 60,"minutes");
        createNotif("Challenge",0,1,"assets/ui/notif.png", 1, 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, timeToWaitMsg);
        return 1;
    }
    printf("Challenge lancé !\n");
    challenge.active = 1;
    challenge.startTime = SDL_GetTicks();
    level.mobKilled = 0; // Réinitialiser le compteur de monstres tués pour le challenge
    level.mobToKill = challenge.target + 1; 
    refreshMobKilled();
    challenge.lastTime = time(NULL);
    //createUIText(&pageHolder.page[0],font,formatChaine("%t: %d",CHALLENGE_MSG_TIMER), getRectForCentenredCord(vw(50), vh(20), vh(50), vh(7)), (SDL_Color){255, 255, 255, 255}, "TimerChallenge");
    return 0;
}

void resetChallenge() {
    challenge.active = 0;
    level.mobToKill = 10;
    level.mobKilled = 0;
    refreshMobKilled();
    destroyUItxt(getTxtFromLabel("TimerChallenge"), &pageHolder.page[0]);
}

/*
void displayChallengeTimer(int elapsedTimeChallenge) {
    int remainingTime = challenge.duration - elapsedTimeChallenge;
    uiTxt * txtHolder = getTxtFromLabel("TimerChallenge");
    if (txtHolder == NULL){
        return;
    }
    char timerText[50];
    //setUiText(txtHolder, formatChaine("%t: %d",CHALLENGE_MSG_TIMER, remainingTime));
}
*/
void updateChallenge() {
    if (!challenge.active) return;
    int currentTime = SDL_GetTicks();
    int elapsedTimeChallenge = (currentTime - challenge.startTime) / 1000;
    if (elapsedTimeChallenge >= challenge.duration) {
        createNotif("Challenge",0,1,"assets/ui/notif.png", 1, 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, Traduction(CHALLENGE_MSG_LOSE));
        resetChallenge();
        return;
    }
    
    //displayChallengeTimer(elapsedTimeChallenge);
    if (level.mobKilled >= challenge.target) {
        char goldEarnedMsg[100];
        if (level.currentLvl != 0) lvl=level.currentLvl;
        addGold((unsigned long long int)challenge.reward * lvl);
        sprintf(goldEarnedMsg, "%s %d",Traduction(CHALLENGE_MSG_WIN), challenge.reward * lvl);
        createNotif("Challenge",0,1,"assets/ui/notif.png", 1, 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, goldEarnedMsg);
        resetChallenge();
        return;
    }
}


