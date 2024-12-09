typedef enum {
    BIENVENUE_MSG,
    AUREVOIR_MSG,
    NB_MSG
} CleMsg;

typedef struct {
    const char *Language;
    const char *messages[NB_MSG];
} Lang;

extern Lang en;
extern Lang fr;
extern Lang LanguageAct;

char* Traduction(CleMsg key);

void SelectLanguage(Lang lang);
