#include "../lib/player.h"
#include "../lib/heros.h"
#include <time.h>

int gold = 0;
int damage_click = 1;
char username[50];

void goldHorsLigne(time_t lastSaveTime){
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, lastSaveTime);
    int goldParSec = herosGoldGenParSec();
    int goldEarned = (int)(elapsedTime * goldParSec);
    gold += (int)(goldEarned/10);
}