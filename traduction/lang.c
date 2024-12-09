#include "lang.h"

Lang en = {
    .Language = "English",
    .messages = {
        [BIENVENUE_MSG] = "Welcome to RPG CLICKER",
        [AUREVOIR_MSG] = "Are you sure you want to quit ?",
    }
};

Lang fr = {
    .Language = "French",
    .messages = {
        [BIENVENUE_MSG] = "Bienvenue dans RPG CLICKER",
        [AUREVOIR_MSG] = "Etes-vous sûr de vouloir quitter ?"
    }
};

Lang LanguageAct;

char* Traduction(CleMsg key) {
    return (char*)LanguageAct.messages[key];
}

void SelectLanguage(Lang lang) {
    LanguageAct = lang;
}
