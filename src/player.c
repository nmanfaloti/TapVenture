#include "../lib/player.h"
#include "../lib/heros.h"
#include "../lib/ui.h"
#include "../lib/lang.h"
#include <time.h>

int gold = 0;
int damage_click = 1;
char username[50];

void goldGainOffline(time_t lastSaveTime){
    char goldEarnedMsg[100];
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, lastSaveTime);
    int goldBySec = herosGoldGenBySec();
    int goldEarned = (elapsedTime * goldBySec);
    gold += (int)(goldEarned/10);
    sprintf(goldEarnedMsg, "%d", goldEarned/10);
    createNotif(Traduction(GOLD_OFFLINE_MSG),35,1,"assets/ui/notif.png", 1, 5, (SDL_Rect){vw(50), vh(24), vw(40), vh(30)},30, 1.7,2, Traduction(GOLD_OFFLINE_DESC_MSG), goldEarnedMsg);
}