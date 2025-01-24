
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

int makeSave();
int loadSave();
int initVariableGlobal();