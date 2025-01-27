
typedef enum {
    USERNAME,
    LANGUAGE,
    DATA_COUNT_CHAR
} dataIndexChar;

typedef enum {
    LEVEL,
    GOLD,
    DAMAGE_CLICK,
    DATA_COUNT_INT
} dataIndexInt;



char * getValueForKey(char * key, char * nom_ficher);
int ChangeValueForKey(char * key, char * value, char * nom_ficher);
int createValueForKey(char * key, char * value, char * nom_ficher);

char * strCatMalloc(const char * srt1, const char * str2);
int isHereFile(char * nameFile);
int rmFile(char * nameFile);

int makeSave();
int loadSave();

int makeSavePlayer(char * save);
int loadSavePlayer(char * save);
int makeSaveHeros(char * save);
int loadSaveHeros(char * save);

int initPlayer();