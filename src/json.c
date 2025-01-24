#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib/json.h"

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
                    result = strdup(start);
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

char *dataIndexStrings[] = {
    "USERNAME",
    "LANGUAGE"
};
char *dataIndexInts[] = {
    "LEVEL",
    "GOLD",
    "DAMAGE_CLICK"
};

int makeSave(char * nom_ficher, char dataStr[DATA_COUNT_CHAR][50], int dataInt[DATA_COUNT_INT],hero heros[HEROS_COUNT]){
    system("rm save/save.json");
    //dataSrt
    for(int i = 0; i < DATA_COUNT_CHAR; i++){
        createValueForKey(dataIndexStrings[i], dataStr[i], nom_ficher);
    }
    //dataInt
    for(int i = 0; i < DATA_COUNT_INT; i++){
        char value[10];
        sprintf(value, "%d", dataInt[i]);
        createValueForKey(dataIndexInts[i], value, nom_ficher);
    }
    //heros
    for(int i = 0; i < HEROS_COUNT; i++){
        char value[10];
        char key[20];
        sprintf(value, "%d", heros[i].level);
        sprintf(key, "HERO_LEVEL_%d", i);
        createValueForKey(key, value, nom_ficher);
    }
    time_t current_time = time(NULL);
    char value[10];
    sprintf(value, "%ld", current_time);
    createValueForKey("SAVETIME", value, nom_ficher);

    return 0;
}



int initVariable(char dataStr[DATA_COUNT_CHAR][50], int dataInt[DATA_COUNT_INT], hero heros[HEROS_COUNT]){
    time_t current_time = time(NULL);
    char * saveTime = getValueForKey("SAVETIME", "save/save.json");
    if(saveTime && atoi(saveTime) <= current_time){
        //dataSrt
        for (int i = 0; i < DATA_COUNT_CHAR; i++){
            char * value = getValueForKey(dataIndexStrings[i], "save/save.json");
            strcpy(dataStr[i], value);
        }
        //dataInt
        for (int i = 0; i < DATA_COUNT_INT; i++){
            char * value = getValueForKey(dataIndexInts[i], "save/save.json");
            dataInt[i] = atoi(value);
        }
        //heros
        for(int i = 0; i < HEROS_COUNT; i++){
            char key[20];
            sprintf(key, "HERO_LEVEL_%d", i);
            char * level = getValueForKey(key, "save/save.json");
            upgradeHeroAtLevel(heros,i,atoi(level));
        }
        return 0;
    }
    printf("No save file found or save file in future\n");
    initHeros(heros);
    strcpy(dataStr[USERNAME], "Default");
    strcpy(dataStr[LANGUAGE], "English");
    dataInt[LEVEL] = 0;
    dataInt[GOLD] = 0;
    dataInt[DAMAGE_CLICK] = 10;
    
    return 0;
}



