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
#include "../lib/boutique.h"
#include "../lib/ui.h"
#include "../lib/button.h"
#include "../lib/chaine.h"
#include "../lib/challenge.h"
#include "../lib/prestige.h"
#include "../lib/inv.h"



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


int isHereFile(char * nameFile){
    //RETURN 1 if file exist, else 0
    char * srtPressance = strCatMalloc("test -e ", nameFile);
    if(srtPressance == NULL){
        printf("Erreur lors de la verification de l'existance du fichier %s(Probleme d'allocation memoire)\n", nameFile);
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


int loadSave(){
    loadSaveHeros("save/heros.json");
    loadSavePrestige("save/prestige.json");
    loadSavePlayer("save/player.json");
    if (level.currentLvl!=0)
        mobHandler();
    refreshMobHealth();
    refreshMobKilled();
    refreshMobLabel();
    refreshMobTexture();
    return 0;
}

int makeSave(){
    if(!isHereFile("save")){
        system("mkdir save");
    }
    makeSavePlayer("save/player.json");
    makeSaveHeros("save/heros.json");
    makeSavePrestige("save/prestige.json");
    makeSaveInventaire();
    return 0;
}

int loadSavePlayer(char * save){
    if(!isHereFile(save)){
        initPlayer();
        initShop();
        refreshMobKilled();
        return 1;
    }

    char * value;

    value = getValueForKey("LANGUAGE", save);
    if(value == NULL){
        printf("Save : can't load  LANGUAGE\n");
    }
    else{
        initLang(value);
        free(value);
    }

    //dataInt
    value = getValueForKey("MAX_LEVEL", save);
    if(value == NULL){
        printf("Save : can't load  MAX_LEVEL\n");
    }
    else{
        level.currentLvl = atoi(value);
        level.maxLevel = atoi(value);
        refreshCurrentLvl();
        free(value);
    }

    value = getValueForKey("MOB_KILLED", save);
    if(value == NULL){
        printf("Save : can't load  MOB_KILLED\n");
    }
    else{
        level.mobKilled = atoi(value);
        refreshMobKilled();
        free(value);
    }

    value = getValueForKey("GOLD", save);
    if(value == NULL){
        printf("Save : can't load  GOLD\n");
    }
    else{
        sscanf(value,"%lld", &gold);
        free(value);
    }

    unsigned long long int tempo;
    value = getValueForKey("DAMAGE_CLICK", save);
    if(value == NULL){
        printf("Save : can't load  DAMAGE_CLICK\n");
    }
    else{
        sscanf(value,"%lld", &tempo);
        setPlayerDamage(tempo);
        free(value);
    }

    value = getValueForKey("SHOP", save);
    if(value == NULL){
        printf("Save : can't load  SHOP\n");
    }
    else{
        shop.damageLevel = atoi(value);
        shop.nextPrice=getPriceForLevels(shop.damageLevel+1);
        refreshButtonShop();
        free(value);
    }

    value = getValueForKey("LAST_CHALLENGE", save);
    if(value == NULL){
        printf("Save : can't load  LAST_CHALLENGE\n");
    }
    else{   
        challenge.lastTime = atol(value);
        free(value);
    }

    // Calculer l'or gagné en fonction du temps écoulé depuis la dernière sauvegarde
    value = getValueForKey("TIME", save);
    if(value == NULL){
        printf("Save : can't load  TIME\n");
    }
    else{
        time_t lastSaveTime = atol(value);
        free(value);
        goldGainOffline(lastSaveTime);
    }
    return 0;
}
int makeSavePlayer(char * save){
    rmFile(save);

    createValueForKey("LANGUAGE", (char *)LanguageAct.Language, save);
    //dataInt
    char value[30];
    sprintf(value, "%d", level.maxLevel);
    createValueForKey("MAX_LEVEL", value, save);
    sprintf(value, "%d", level.mobKilled);
    createValueForKey("MOB_KILLED", value, save);
    sprintf(value, "%lld", gold);
    createValueForKey("GOLD", value, save);
    sprintf(value, "%lld", damage_click);
    createValueForKey("DAMAGE_CLICK", value, save);
    sprintf(value, "%d", shop.damageLevel);
    createValueForKey("SHOP", value, save);
    sprintf(value, "%ld",challenge.lastTime);
    createValueForKey("LAST_CHALLENGE", value, save);



    time_t temps = time(NULL);
    sprintf(value, "%ld", temps);
    createValueForKey("TIME", value, save);

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
        if (level == NULL) {
            printf("Save : can't load  HERO_%d_LEVEL\n", i);
        }
        else{
            levelH = atoi(level);
            free(level);
        }

        makeHeroAtLevel(i,levelH);
        levelH = 0;
    }
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

int loadSavePrestige(char * save){
    if(!isHereFile(save)){
        return 1;
    }

    char * value;
    int maxIndex;

    value = getValueForKey("POINTS", save);
    if(value == NULL){
        printf("Save : can't load  POINTS\n");
    }
    else{   
        prestigePoints = atoi(value);
        free(value);
    }
    value = getValueForKey("SKILL_TREE_GOLD", save);
    if(value == NULL){
        printf("Save : can't load  SKILL_TREE_GOLD\n");
    }
    else{   
        maxIndex = atoi(value);
        free(value);
        for (int i=0;i<maxIndex;i++){
            buyPrestigeItem("Gold", i, 0);
        }
    }
    value = getValueForKey("SKILL_TREE_DAMAGE", save);
    if(value == NULL){
        printf("Save : can't load  SKILL_TREE_DAMAGE\n");
    }
    else{   
        maxIndex = atoi(value);
        free(value);
        for (int i=0;i<maxIndex;i++){
            buyPrestigeItem("Damage", i, 0);
        }
    }
    value = getValueForKey("SKILL_TREE_PRESTIGE", save);
    if(value == NULL){
        printf("Save : can't load  SKILL_TREE_PRESTIGE\n");
    }
    else{   
        maxIndex = atoi(value);
        free(value);
        for (int i=0;i<maxIndex;i++){
            buyPrestigeItem("Prestige", i, 0);
        }
    }
    refreshPrestigePage();
    return 0;
}

int makeSavePrestige(char * save){
    rmFile(save);

    char value[30];

    sprintf(value, "%d", prestigePoints);
    createValueForKey("POINTS", value, save);
    sprintf(value, "%d", getMaxPrestigeItems("Gold"));
    createValueForKey("SKILL_TREE_GOLD", value, save);
    sprintf(value, "%d", getMaxPrestigeItems("Damage"));
    createValueForKey("SKILL_TREE_DAMAGE", value, save);
    sprintf(value, "%d", getMaxPrestigeItems("Prestige"));
    createValueForKey("SKILL_TREE_PRESTIGE", value, save);
    return 0;
}

int initPlayer(){
    initLang("English");
    level.currentLvl = 0;
    gold = 0;
    damage_click = 10;
    challenge.lastTime = 0;
    createNotif(Traduction(WELCOME_MSG),0,1,"assets/ui/notif.png", 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, Traduction(WELCOME_DESC_MSG));
    return 0;
}


