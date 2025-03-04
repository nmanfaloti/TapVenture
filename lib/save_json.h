#ifndef SAVE_JSON_H
#define SAVE_JSON_H

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

int isHereFile(char * nameFile);
int rmFile(char * nameFile);

int makeSave();
int loadSave();

int makeSavePlayer(char * save);
int loadSavePlayer(char * save);
int makeSaveHeros(char * save);
int loadSaveHeros(char * save);

int initPlayer();

#endif // SAVE_JSON_H