#include <string.h>
#include <stdio.h>
#include "../lib/lang.h"
#include "../lib/button.h"
#include "../lib/aff.h"

Lang en = {
    .Language = "English",
    .messages = {
        [BIENVENUE_MSG] = "Welcome to RPG CLICKER",
        [AUREVOIR_MSG] = "Are you sure you want to quit ?",
        [CLICK_MSG] = "Click to attack",
        [VIE_MSG] = "HP",
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
        [CHALLENGE_MSG_WIN] = "You earned gold by completing the challenge: ",
        [CHALLENGE_MSG_LOSE] = "You failed the challenge",
        [CHALLENGE_MSG_COOLDOWN] = "You must wait before starting a new challenge:", 
        [TIMER_MSG] = "Time remaining: ",
        [NOTIF_TITLE] = "Notification",
        [PRESTIGE_MSG] = "Prestige",
        [PRESTIGE_COST] = "Cost: ",
        [PRESTIGE_GOLD_DESC] = "Increase the gold earned by:",
        [PRESTIGE_DAMAGE_DESC] = "Increase damage by: ",
        [PRESTIGE_PRESTIGE_DESC] = "Increase prestige points by:",
        [PRESTIGE_HERO_DESC] = "Keep the hero:",
        [PRESTIGE_HEROLVL_DESC] = "Keep the hero level:",
        [PRESTIGE_CONGRATS] = "Congratulations for your Prestige, do not forgot to use it in the Prestige Tree !",
        [HERO_SHOP_TITLE_MSG] = "Hero Shop",
        [PRICE_MSG] = "Price",
        [HERO_SHOP_MULTIPLICATOR_MSG] = "Buy x",
        [DPS_MSG] = "Degats per second",
        [MAX_MSG] = "Max"
    }
};

Lang fr = {
    .Language = "French",
    .messages = {
        [BIENVENUE_MSG] = "Bienvenue dans RPG CLICKER",
        [AUREVOIR_MSG] = "Etes-vous sûr de vouloir quitter ?",
        [CLICK_MSG] = "Cliquez pour attaquer",
        [VIE_MSG] = "PV",
        [OR_MSG] = "Or",
        [DMG_MSG] = "Degats",
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
        [CHALLENGE_MSG_WIN] = "Vous avez gagner de l'or en reussissant le challenge: ",
        [CHALLENGE_MSG_LOSE] = "Vous avez echouer le challenge",
        [CHALLENGE_MSG_COOLDOWN] = "Vous devez attendre avant de commencer un nouveau challenge: ",
        [TIMER_MSG] = "Temps restant : ",
        [NOTIF_TITLE] = "Notification",
        [PRESTIGE_MSG] = "Prestige",
        [PRESTIGE_COST] = "Cout:",
        [PRESTIGE_GOLD_DESC] = "Augmente la quantité d'or gagnée par:",
        [PRESTIGE_DAMAGE_DESC] = "Augmente les dégats par:",
        [PRESTIGE_PRESTIGE_DESC] = "Augmente les points de prestige par:",
        [PRESTIGE_HERO_DESC] = "Garde le hero:",
        [PRESTIGE_HEROLVL_DESC] = "Garde le niveau du hero:",
        [PRESTIGE_CONGRATS] = "Felicitation pour votre Prestige, n'oubliez pas de l'utiliser dans l'Arbre de Prestige !",
        [HERO_SHOP_TITLE_MSG] = "Boutique de Heros",
        [PRICE_MSG] = "Prix",
        [HERO_SHOP_MULTIPLICATOR_MSG] = "Acheter x",
        [DPS_MSG] = "Degats par secondes",
        [MAX_MSG] = "Max"
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
    refreshUI();
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
    refreshUI();
}