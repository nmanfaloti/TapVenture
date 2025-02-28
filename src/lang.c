#include <string.h>
#include <stdio.h>
#include "../lib/lang.h"

Lang en = {
    .Language = "English",
    .messages = {
        [BIENVENUE_MSG] = "Welcome to RPG CLICKER",
        [AUREVOIR_MSG] = "Are you sure you want to quit ?",
        [CLICK_MSG] = "Click to attack",
        [VIE_MSG] = "Life remaining",
        [OR_MSG] = "Gold",
        [DMG_MSG] = "Damage",
        [UPG_MSG] = "Damage Upgrade",
        [MOB_MSG] = "Remaining Monsters",
        [LVL_MSG] = "Level",
        [SETTING_MSG] = "Settings",
        [FR_MSG] = "French",
        [EN_MSG] = "English",
        [WELCOME_MSG] = "Welcome",
        [WELCOME_DESC_MSG] = "Welcome to TapVenture",
        [GOLD_OFFLINE_MSG] = "Offline Gold",
        [GOLD_OFFLINE_DESC_MSG] = "You earned gold while you were away:",
        [FULLSCREEN_MSG] = "Fullscreen",
        [WINDOWED_MSG] = "Windowed",
        [CHALLENGE_DESC_MSG_WIN] = "You earned gold by completing the challenge: ",
        [CHALLENGE_DESC_MSG_LOSE] = "You failed the challenge"
        
    }
};

Lang fr = {
    .Language = "French",
    .messages = {
        [BIENVENUE_MSG] = "Bienvenue dans RPG CLICKER",
        [AUREVOIR_MSG] = "Etes-vous sûr de vouloir quitter ?",
        [CLICK_MSG] = "Cliquez pour attaquer",
        [VIE_MSG] = "Vie restante",
        [OR_MSG] = "Or",
        [DMG_MSG] = "Degats :",
        [UPG_MSG] = "Amelioration Degats",
        [MOB_MSG] = "Monstre Restants",
        [LVL_MSG] = "Niveau",
        [SETTING_MSG] = "Parametres",
        [FR_MSG] = "Francais",
        [EN_MSG] = "Anglais",
        [WELCOME_MSG] = "Bienvenue",
        [WELCOME_DESC_MSG] = "Bienvenue dans TapVenture",
        [GOLD_OFFLINE_MSG] = "Or Hors Ligne",
        [GOLD_OFFLINE_DESC_MSG] = "Vous avez gagne de l'or pendant votre absence: ",
        [FULLSCREEN_MSG] = "Plein Ecran",
        [WINDOWED_MSG] = "Mode Fenetre",
        [CHALLENGE_DESC_MSG_WIN] = "Vous avez gagner de l'or en reussissant le challenge: ",
        [CHALLENGE_DESC_MSG_LOSE] = "Vous avez echouer le challenge"
    }
};

Lang LanguageAct;

char* Traduction(CleMsg key) {
    return (char*)LanguageAct.messages[key];
}

int SelectLanguage(void * l[20]) {
    char **lang = (char **)l[0];
    if (strcmp(*lang, "French") == 0) {
        LanguageAct = fr;
    } else if (strcmp(*lang, "English") == 0) {
        LanguageAct = en;
    } else {
        printf("Language not found %s\n", *lang);
        LanguageAct = en;
    }
    return 0;
}

void initLang(char * lang){
    if (strcmp(lang, "French") == 0) {
        LanguageAct = fr;
    } else if (strcmp(lang, "English") == 0) {
        LanguageAct = en;
    } else {
        LanguageAct = en;
    }
}