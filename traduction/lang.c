#include <string.h>
#include "lang.h"

Lang en = {
    .Language = "English",
    .messages = {
        [BIENVENUE_MSG] = "Welcome to RPG CLICKER",
        [AUREVOIR_MSG] = "Are you sure you want to quit ?",
        [CLICK_MSG] = "Click to attack",
        [VIE_MSG] = "Life remaining",
        [OR_MSG] = "Gold",
        [DMG_MSG] = "Damage :",
        [UPG_MSG] = "Damage Upgrade",
        [MOB_MSG] = "Remaining Monsters",
        [LVL_MSG] = "Level",
        
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
    }
};

Lang LanguageAct;

char* Traduction(CleMsg key) {
    return (char*)LanguageAct.messages[key];
}

void SelectLanguage(const char *lang) {
    if (strcmp(lang, "French") == 0) {
        LanguageAct = fr;
    } else if (strcmp(lang, "English") == 0) {
        LanguageAct = en;
    } else {
        LanguageAct = en;
    }
}
