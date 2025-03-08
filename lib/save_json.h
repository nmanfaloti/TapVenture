#ifndef SAVE_JSON_H
#define SAVE_JSON_H


char * getValueForKey(char * key, char * nom_ficher);
int ChangeValueForKey(char * key, char * value, char * nom_ficher);
int createValueForKey(char * key, char * value, char * nom_ficher);

int isHereFile(char * nameFile);
int rmFile(char * nameFile);

int makeSave();
int loadSave();

int loadSavePlayer(char * save);
int makeSavePlayer(char * save);
int loadSaveHeros(char * save);
int makeSaveHeros(char * save);


int initPlayer();

#endif // SAVE_JSON_H