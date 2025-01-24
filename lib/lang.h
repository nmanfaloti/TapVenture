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
    NB_MSG
} CleMsg;

typedef struct {
    const char *Language;
    const char *messages[NB_MSG];
} Lang;

extern Lang LanguageAct;

char* Traduction(CleMsg key);

void SelectLanguage(const char *lang);
