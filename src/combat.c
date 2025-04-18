#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/combat.h"
#include "../lib/aff.h"
#include "../lib/sdl.h"
#include "../lib/player.h"
#include "../lib/lang.h"
#include "../lib/ui.h"
#include "../lib/challenge.h"
#include "../lib/prestige.h"
#include "../lib/audio.h"
#include "../lib/inv.h"

levelInfo level = {
    .monstre = {{0}},
    .timeToKill = -1,
    .startTimer = 0,
    .mobKilled = 0,
    .mobToKill = 10,
    .currentLvl = 0,
    .label = {
        "Limul",
        "SmallFoot",
        "Morvus",
        "TasDeRok",
        "Noeil"
    },
};
Uint32 lastAttackAnimTime = 0;
Uint32 attackAnimDuration = 250;

char * getCurrentMobLabel(){
    return level.label[(level.currentLvl-1)/10];
}

char *getCurrentMobImgPath() {
    if (level.currentLvl < 0 || level.currentLvl > 50) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "getCurrentMobImgPath: Invalid level.currentLvl -> %d", level.currentLvl);
        return NULL;
    }

    const char *label = level.label[(level.currentLvl - 1) / 10];
    if (label == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "getCurrentMobImgPath: Invalid label for level -> %d", level.currentLvl);
        return NULL;
    }

    size_t pathLen = strlen("assets/ui/monsters/") + strlen(label) + strlen(".png") + 1;
    char *path = malloc(pathLen);
    if (path == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "getCurrentMobImgPath: Failed to allocate memory for path");
        return NULL;
    }

    snprintf(path, pathLen, "assets/ui/monsters/%s.png", label);
    return path;
}

static void playDamageAnimation() {
    ButtonImg *button = getImgButtonFromLabel("mobImg");
    if (button == NULL) {
        return;
    }

    char *damagePath = malloc(strlen("assets/ui/monsters/") + strlen(getCurrentMobLabel()) + strlen("_damage.png") + 1);
    sprintf(damagePath, "assets/ui/monsters/%s_damage.png", getCurrentMobLabel());

    setImgButtonTexture(button, NULL, damagePath);
    lastAttackAnimTime = SDL_GetTicks();

    free(damagePath);
}

static void checkDamageAnimation(){
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastAttackAnimTime >= attackAnimDuration) {
        refreshMobTexture();
        lastAttackAnimTime = 0;
    }
}

int initLevel(monstreInfo monstre[]) {
    level.timeToKill = -1.0;
    level.mobKilled = 0;
    level.mobToKill = 10;
    level.currentLvl = 0;
    level.maxLevel = 0;
    monstre[0].mobHealth = 100;
    monstre[0].iniHealth = 100;
    monstre[0].coinMin = 5;
    monstre[0].coinMax = 15;
    //Initialisation des monstres (En fonction du niveau)
    for (int i = 1; i <= 50; i++) {
        monstre[i].mobHealth = monstre[i-1].mobHealth * 1.5;
        monstre[i].iniHealth = monstre[i].mobHealth;
        monstre[i].coinMin = monstre[i-1].coinMin * 1.25;
        monstre[i].coinMax = monstre[i-1].coinMax * 1.25;
    }
    //Initialisation des Boss apres afin de pas fausser les prochains monstres
    for (int i = 1; i <= 50; i++) {
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
    unsigned long long int * damage = args[0];
    bool * joueur = args[1];
    if(joueur != NULL && *joueur == true){
         playMusic(MUSIC_ATTACK, CANAL_EFFECT, 1); 
         playDamageAnimation();
    }
    monstreInfo * currentMonstre = &level.monstre[level.currentLvl];
    if (currentMonstre->mobHealth <= *damage * damageModifier) {
        level.mobKilled += 1;
        
        addGold((currentMonstre->coinMin + rand() % (currentMonstre->coinMax - currentMonstre->coinMin + 1)));
        currentMonstre->mobHealth = currentMonstre->iniHealth;
        if (level.mobKilled >= level.mobToKill) {
            if (level.currentLvl < 50) {
                if (level.currentLvl % 5 == 0) {
                    playMusic(MUSIC_LEVEL_UP_BOSS, CANAL_EFFECT, 1);
                } 
                else {
                    playMusic(MUSIC_LEVEL_UP, CANAL_EFFECT, 1);
                }
                level.currentLvl++;
                mobHandler();
                if(level.currentLvl > level.maxLevel){
                    level.maxLevel = level.currentLvl;
                }
            }else doPrestige();
            level.mobKilled = 0;
            refreshCurrentLvl();
            refreshMobLabel();
            refreshMobTexture();
        }
        refreshMobKilled();
        drop_item();
    }
    else{
        currentMonstre->mobHealth -= floor(*damage * damageModifier);
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
    if (currentLvl == 0) return 0;
    return currentLvl % 5 == 0;
}

void mobAnimationHandler(){
    if (lastAttackAnimTime == 0) {
        return;
    }
    checkDamageAnimation();
}

void mobHandler() {
    initBoss(30);
    checkDamageAnimation();
    if (isBoss(level.currentLvl)) {
        level.monstre[level.currentLvl].mobHealth = level.monstre[level.currentLvl].iniHealth;
        level.startTimer = SDL_GetTicks();
    }else{
        if (level.mobToKill == 1){
            level.mobToKill = 10;
        }
    }
}
int challengeUI = 0;
int bossUI = 0;

void displayTimers() {

    // Pour le Challenge
    if (challenge.active) {
        int elapsed = (SDL_GetTicks() - challenge.startTime) / 1000;
        int remaining = challenge.duration - elapsed;
        if (remaining <= 0) remaining = 0;
        char *timerStr=malloc(strlen(Traduction(TIMER_MSG)) + 10);
        sprintf(timerStr, "%s %d s",Traduction(TIMER_MSG), remaining);
        if (challengeUI == 0) {
            createUIText(&pageHolder.page[0], font, timerStr, getRectForCentenredCord(vw(50), vh(20), vh(50), vh(8)), (SDL_Color){0, 0, 0, 255}, "challengeTimer");
            challengeUI = 1;
        } else {
            if (currentpage == &pageHolder.page[0] && challenge.active) {
                uiTxt * txtHolder = getTxtFromLabel("challengeTimer");
                if (txtHolder != NULL)
                    setUiText(txtHolder, timerStr);
            }
        }
    } else {
        // Si le challenge n'est plus actif, détruire l'UI texte s'il existe
        if (challengeUI == 1 ) {
            uiTxt * txtHolder = getTxtFromLabel("challengeTimer");
            if (txtHolder != NULL)
                destroyUITxt(txtHolder, &pageHolder.page[0]);
            challengeUI = 0;
        }
    }

    // Pour le Boss
    if (isBoss(level.currentLvl) && level.timeToKill > 0 && level.currentLvl != 0) {
        int elapsed = (SDL_GetTicks() - level.startTimer) / 1000;
        int remaining = level.timeToKill - elapsed;
        if (remaining <= 0){
            remaining = 0;
            level.monstre[level.currentLvl].mobHealth = level.monstre[level.currentLvl].iniHealth;
            level.startTimer = SDL_GetTicks();
            refreshMobHealth();
        }
        char *timerStr=malloc(strlen(Traduction(TIMER_MSG)) + 10);
        sprintf(timerStr, "%s %d s", Traduction(TIMER_MSG),remaining);
        if (bossUI == 0) {
            createUIText(&pageHolder.page[0], font, timerStr, getRectForCentenredCord(vw(50), vh(20), vh(50), vh(8)), (SDL_Color){0, 0, 0, 255}, "bossTimer");
            bossUI = 1;
        } else {
            if (currentpage == &pageHolder.page[0] && isBoss(level.currentLvl)) {
                uiTxt * txtHolder = getTxtFromLabel("bossTimer");
                if (txtHolder != NULL)
                    setUiText(txtHolder, timerStr);
            }
        }
    } else {
        // Si le timer du boss n'est plus nécessaire, détruire l'UI texte correspondant
        if (bossUI == 1) {
            uiTxt * txtHolder = getTxtFromLabel("bossTimer");
            if (txtHolder != NULL)
                destroyUITxt(txtHolder, &pageHolder.page[0]);
            bossUI = 0;
        }
    }
}

//si le wantedLevel est a 1 alors on monte de niveau sinon on descend
int changeLevel(void * l[20]) {
    if (challenge.active) return 0;

    if (l[0] == NULL){
        printf("ChangePrestigePage Error: No args\n");
        return 1;
    }
    int * wantedLevel = (int*) l;
    if (*wantedLevel==1 && (level.currentLvl < level.maxLevel)) level.currentLvl++;
    else if (*wantedLevel==2 && (level.currentLvl>0)) level.currentLvl--;
    level.mobKilled = 0;
    level.monstre[level.currentLvl].mobHealth = level.monstre[level.currentLvl].iniHealth;
    if (level.currentLvl != 0) 
        mobHandler();
    refreshMobHealth();
    refreshMobKilled();
    refreshCurrentLvl();
    refreshMobLabel();
    refreshMobTexture();
    return 1;
}
