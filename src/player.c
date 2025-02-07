#include "../lib/player.h"
#include "../lib/heros.h"
#include <time.h>

int gold = 0;
int damage_click = 1;
char username[50];

void goldGainOffline(time_t lastSaveTime){
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, lastSaveTime);
    int goldBySec = herosGoldGenBySec();
    int goldEarned = (elapsedTime * goldBySec);
    gold += (int)(goldEarned/10);
}