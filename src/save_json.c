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
    char * global_save = "save/save.json";
    char * save_heros = "save/save_heros.json";
    system("rm save/save.json");
    system("rm save/save_heros.json");
    
    createValueForKey("USERNAME", username, global_save);
    createValueForKey("LANGUAGE", (char *)LanguageAct.Language, global_save);
    //dataInt
    char value[30];
    sprintf(value, "%d", level);
    createValueForKey("LEVEL", value, global_save);
    sprintf(value, "%d", gold);
    createValueForKey("GOLD", value, global_save);
    sprintf(value, "%d", damage_click);
    createValueForKey("DAMAGE_CLICK", value, global_save);

    //heros
    for(int i = 0; i < HEROS_COUNT; i++){
        char value[10];
        char key[20];
        sprintf(key, "HERO_%d_LEVEL", i);
        sprintf(value, "%d", heros[i].level);
        createValueForKey(key, value, save_heros);

        sprintf(key, "HERO_%d_DEGAT", i);
        sprintf(value, "%d", heros[i].degat);
        createValueForKey(key, value, save_heros);

        sprintf(key, "HERO_%d_PRIX", i);
        sprintf(value, "%d", heros[i].prix);
        createValueForKey(key, value, save_heros);

        sprintf(key, "HERO_%d_COOLDOWN", i);
        sprintf(value, "%d", heros[i].cooldown);
        createValueForKey(key, value, save_heros);

    }
    return 0;
}

int loadSave(){
    char * global_save = "save/save.json";
    char * save_heros = "save/save_heros.json";
    if(system("test -e save/save.json") != 0){
        //No global save file found
        initVariableGlobal();
    }
    else{
        //load global save
        char * value;
        value = getValueForKey("USERNAME", "save/save.json");
        strcpy(username, value);
        free(value);

        value = getValueForKey("LANGUAGE", "save/save.json");
        SelectLanguage(value);
        free(value);
        //dataInt
        value = getValueForKey("LEVEL", global_save);
        level = atoi(value);
        free(value);
        value = getValueForKey("GOLD", global_save);
        gold = atoi(value);
        free(value);
        value = getValueForKey("DAMAGE_CLICK", global_save);
        damage_click = atoi(value);
        free(value);
    }
    //heros
    if(system("test -e save/save_heros.json") != 0){
        //No heros save file found
        initHeros(heros);
    }
    else{
        for(int i = 0; i < HEROS_COUNT; i++){
            char key[20];
            sprintf(key, "HERO_%d_LEVEL", i);
            char * level = getValueForKey(key, save_heros);
            heros[i].level = atoi(level);
            free(level);

            sprintf(key, "HERO_%d_DEGAT", i);
            char * degat = getValueForKey(key, save_heros);
            heros[i].degat = atoi(degat);
            free(degat);

            sprintf(key, "HERO_%d_PRIX", i);
            char * prix = getValueForKey(key, save_heros);
            heros[i].prix = atoi(prix);
            free(prix);

            sprintf(key, "HERO_%d_COOLDOWN", i);
            char * cooldown = getValueForKey(key, save_heros);
            heros[i].cooldown = atoi(cooldown);
            free(cooldown);

            heros[i].lastAttack = 0;
        }
    }
    return 0;
}


int initVariableGlobal(){
    strcpy(username, "Default");
    SelectLanguage("English");
    level = 0;
    gold = 0;
    damage_click = 10;
    return 0;
}


