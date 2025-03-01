#ifndef LANG_H
#define LANG_H
typedef enum {
    BIENVENUE_MSG,
    AUREVOIR_MSG,
    CLICK_MSG,
    VIE_MSG,
    OR_MSG,
    DMG_MSG,
    UPG_MSG,
    MOB_MSG,
    LVL_MSG,
    SETTING_MSG,
    FR_MSG,
    EN_MSG,
    WELCOME_MSG,
    WELCOME_DESC_MSG,
    GOLD_OFFLINE_MSG,
    GOLD_OFFLINE_DESC_MSG,
    FULLSCREEN_MSG,
    WINDOWED_MSG,
    CHALLENGE_DESC_MSG_WIN,
    CHALLENGE_DESC_MSG_LOSE,
    CHALLENGE_DESC_MSG_COOLDOWN,
    NB_MSG
} CleMsg;

typedef struct {
    const char *Language;
    const char *messages[NB_MSG];
} Lang;

extern Lang LanguageAct;

char* Traduction(CleMsg key);

int SelectLanguage(void * l[20]);

void initLang(char * lang);
#endif //LANG_H