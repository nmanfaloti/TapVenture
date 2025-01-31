#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/save_json.h"
#include "../lib/player.h"
#include "../lib/heros.h"
#include "../lib/lang.h"
#include "../lib/combat.h"
#include "../lib/aff.h"



char * getValueForKey(char * key, char * nom_ficher) {
    FILE * f = fopen(nom_ficher, "r");
    if (f == NULL) {
        return NULL;
    }
    char line[256];
    char * result = NULL;

    while (fgets(line, sizeof(line), f) != NULL) {
        char * found = strstr(line, key);
        if (found) {
            char * start = strchr(found, ':');
            if (start) {
                start++; 
                while (*start == ' ' || *start == '\"') start++; 

  
                char * end = strchr(start, '\"');
                if (end) {
                    *end = '\0';
                    result = strdup(start);// equivalent a un malloc() donc nessaisaire de free()
                    break;
                }
            }
        }
    }
    fclose(f);
    return result;
}

int ChangeValueForKey(char * key, char * value, char * nom_ficher) {
    FILE * f = fopen(nom_ficher, "r");
    if (f == NULL) {
        return 1;
    }
    
    FILE * temp = tmpfile();
    if (temp == NULL) {
        fclose(f);
        return 1;
    }

    char line[256];
    int key_found = 0;
    

    while (fgets(line, sizeof(line), f) != NULL) {
        char * found = strstr(line, key);
        if (found) {
            char * start = strchr(found, ':');
            if (start) {
                start++;
                while (*start == ' ' || *start == '\"') start++;

                char * end = strchr(start, '\"');
                if (end) {
                    char * ptr = end + 1;
                    while (*ptr == ' ') ptr++; 
                    int virgule = (*ptr == ',');

                    *start = '\0';
                    // Écrire la clé dans le fichier temporaire
                    fputs(line, temp);

                    //Vérifie si il y a une virgule pour l'ajouter a la fin
                    if (virgule) {
                        fprintf(temp,"%s\",\n",value);
                    } else {
                        fprintf(temp,"%s\"\n",value);
                    }

                    key_found = 1;
                    continue;
                }
            }
        }
        fputs(line, temp);
    }

    fclose(f);

    if (!key_found) {
        printf("Key existe pas \n");
        return 1;
    }

    f = fopen(nom_ficher, "w");
    if (f == NULL) {
        return 1;
    }

    rewind(temp);

    while (fgets(line, sizeof(line), temp) != NULL) {
        fputs(line, f);
    }

    fclose(f);

    return 0;
}


int createValueForKey(char * key, char * value, char * nom_ficher){
    FILE * f = fopen(nom_ficher, "r+");
    if (f == NULL) {
        // create file if not exist
        f = fopen(nom_ficher, "w");
        if (f == NULL) {
            return 1;
        }
        fprintf(f, "{\n\t\"%s\": \"%s\"\n}", key, value);
        fclose(f);
        return 0;
    }
    int returnCode = fseek(f, -2, SEEK_END);
    if(returnCode != 0){
        printf("Erreur lors du deplacement dans le fichier\n");
        return 2;
    }
    fprintf(f, ",\n\t\"%s\": \"%s\"\n}", key, value);
    fclose(f);
    return 0;
}

//concatene 2 chaine et malloc un espace pour les stocker
char * strCatMalloc(const char * srt1, const char * str2){
    char * result = malloc(strlen(srt1) + strlen(str2) + 1);
    if (result == NULL) {
        printf("Erreur lors de l'allocation de mémoire dans strCatMalloc\n");
        return NULL;
    }
    sprintf(result, "%s%s", srt1, str2);
    return result;
}

int isHereFile(char * nameFile){
    //RETURN 1 if file exist, else 0
    char * srtPressance = strCatMalloc("test -e ", nameFile);
    if(srtPressance == NULL){
        printf("Erreur lors de la verification de l'existance du fichier %s\n", nameFile);
        return 0;
    }
    if (system(srtPressance)){
        free(srtPressance);
        return 0;
    }
    free(srtPressance);
    return 1;
}
int rmFile(char * nameFile){
    char * strRm = strCatMalloc("rm ", nameFile);
    if(strRm == NULL){
        return 1;
    }
    if(isHereFile(nameFile)){
        system(strRm);
    }
    free(strRm);
    return 0;
}

char *dataIndexStrings[] = {
    "USERNAME",
    "LANGUAGE"
};
char *dataIndexInts[] = {
    "LEVEL",
    "GOLD",
    "DAMAGE_CLICK"
};

int loadSave(){
    loadSavePlayer("save/player.json");
    loadSaveHeros("save/heros.json");
    refreshMobHealth();
    return 0;
}

int makeSave(){
    if(!isHereFile("save")){
        system("mkdir save");
    }
    makeSavePlayer("save/player.json");
    makeSaveHeros("save/heros.json");
    return 0;
}
    
int makeSavePlayer(char * save){
    rmFile(save);

    createValueForKey("USERNAME", username, save);
    createValueForKey("LANGUAGE", (char *)LanguageAct.Language, save);
    //dataInt
    char value[30];
    sprintf(value, "%d", level.currentLvl);
    createValueForKey("LEVEL", value, save);
    sprintf(value, "%d", gold);
    createValueForKey("GOLD", value, save);
    sprintf(value, "%d", damage_click);
    createValueForKey("DAMAGE_CLICK", value, save);


    time_t temps = time(NULL);
    sprintf(value, "%ld", temps);
    createValueForKey("TIME", value, save);

    return 0;
}

int loadSavePlayer(char * save){
    if(!isHereFile(save)){
        initPlayer();
        return 1;
    }

    /*En cas de fichier avec des valeurs manqantes, ne pas free le pointeur*/
    char * value;
    value = getValueForKey("USERNAME", save);
    strcpy(username, value);
    free(value);

    value = getValueForKey("LANGUAGE", save);
    SelectLanguage(value);
    free(value);
    //dataInt
    value = getValueForKey("LEVEL", save);
    level.currentLvl = atoi(value);
    free(value);

    value = getValueForKey("GOLD", save);
    gold = atoi(value);
    free(value);

    value = getValueForKey("DAMAGE_CLICK", save);
    damage_click = atoi(value);
    free(value);

    // Calculer l'or gagné en fonction du temps écoulé
    value = getValueForKey("TIME", save);
    time_t lastSaveTime = atol(value);
    free(value);
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, lastSaveTime);

    // Calculer l'or gagné 0.1 or par seconde par niveau
    int goldEarned = (int)((elapsedTime * level.currentLvl)/10);
    gold += goldEarned;
    return 0;
}


int makeSaveHeros(char * save){
    rmFile(save);

    for(int i = 0; i < HEROS_COUNT; i++){
        char value[10];
        char key[20];
        sprintf(key, "HERO_%d_LEVEL", i);
        sprintf(value, "%d", heros[i].level);
        createValueForKey(key, value, save);
    }
    return 0;
}

int loadSaveHeros(char * save){
    if(!isHereFile(save)){
        initHeros();
        return 1;
    }
    initHeros();
    int levelH =0;
    for(int i = 0; i < HEROS_COUNT; i++){
        char key[20];
        sprintf(key, "HERO_%d_LEVEL", i);
        char * level = getValueForKey(key, save);
        levelH = atoi(level);
        free(level);

        makeHeroAtLevel(i,levelH);
        levelH = 0;
    }
    return 0;
}


int initPlayer(){
    strcpy(username, "Default");
    SelectLanguage("English");
    level.currentLvl = 0;
    gold = 0;
    damage_click = 10;
    return 0;
}



