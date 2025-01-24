#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/save_json.h"
#include "../lib/player.h"
#include "../lib/heros.h"
#include "../lib/lang.h"

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

int makeSave(){
    char * nom_ficher = "save/save.json";
    system("rm save/save.json");
    
    createValueForKey("USERNAME", username, nom_ficher);
    createValueForKey("LANGUAGE", (char *)LanguageAct.Language, nom_ficher);
    //dataInt
    char value[30];
    sprintf(value, "%d", level);
    createValueForKey("LEVEL", value, nom_ficher);
    sprintf(value, "%d", gold);
    createValueForKey("GOLD", value, nom_ficher);
    sprintf(value, "%d", damage_click);
    createValueForKey("DAMAGE_CLICK", value, nom_ficher);

    //heros
    for(int i = 0; i < HEROS_COUNT; i++){
        char value[10];
        char key[20];
        sprintf(value, "%d", heros[i].level);
        sprintf(key, "HERO_LEVEL_%d", i);
        createValueForKey(key, value, nom_ficher);
    }
    time_t current_time = time(NULL);
    sprintf(value, "%ld", current_time);
    createValueForKey("SAVETIME", value, nom_ficher);
    printf("Save done\n");
    return 0;
}

int loadSave(){
    char * nom_ficher = "save/save.json";
    char * saveTime = getValueForKey("SAVETIME", "save/save.json");
    if (saveTime == NULL) {
        //No save file found
        initVariable();
        return 1;
    }
    //load save
    char * value;
    value = getValueForKey("USERNAME", "save/save.json");
    strcpy(username, value);
    value = getValueForKey("LANGUAGE", "save/save.json");
    SelectLanguage(value);
    //dataInt
    value = getValueForKey("LEVEL", nom_ficher);
    level = atoi(value);
    value = getValueForKey("GOLD", nom_ficher);
    gold = atoi(value);
    value = getValueForKey("DAMAGE_CLICK", nom_ficher);
    damage_click = atoi(value);
    //heros
    for(int i = 0; i < HEROS_COUNT; i++){
        char key[20];
        sprintf(key, "HERO_LEVEL_%d", i);
        char * level = getValueForKey(key, "save/save.json");
        upgradeHeroAtLevel(heros,i,atoi(level));
    }
    return 0;
}


int initVariable(){
    initHeros(heros);
    strcpy(username, "Default");
    SelectLanguage("English");
    level = 0;
    gold = 0;
    damage_click = 10;
    return 0;
}



