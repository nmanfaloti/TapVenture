#include "../heros/heros.h"

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

int makeSave(char * nom_ficher, char dataStr[DATA_COUNT_CHAR][50], int dataInt[DATA_COUNT_INT],hero heros[HEROS_COUNT]);
int initVariable(char dataStr[DATA_COUNT_CHAR][50], int dataInt[DATA_COUNT_INT], hero heros[HEROS_COUNT]);
