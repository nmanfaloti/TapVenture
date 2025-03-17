#include "../lib/player.h"
#include "../lib/heros.h"
#include "../lib/ui.h"
#include "../lib/lang.h"
#include "../lib/aff.h"
#include "../lib/chaine.h"
#include "../lib/ui.h"
#include <time.h>

unsigned long long int gold = 0;
unsigned long long int damage_click = 1;

void goldGainOffline(time_t lastSaveTime){
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, lastSaveTime);
    unsigned long long int goldBySec = herosGoldGenBySec();
    unsigned long long int goldEarned = (elapsedTime * goldBySec);
    addGold((goldEarned/10));
    if (goldEarned != 0){
        char * goldEarnedMsg = formatChaine("%t %w",GOLD_OFFLINE_DESC_MSG, goldEarned/10);
        //sprintf(goldEarnedMsg, "%s %lld",Traduction(GOLD_OFFLINE_DESC_MSG), goldEarned/10);
        createNotif(Traduction(GOLD_OFFLINE_MSG),0,1,"assets/ui/notif.png", 1, 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, goldEarnedMsg);
        free(goldEarnedMsg);
    }
}

void addGold(unsigned long long int goldToAdd){
    if(gold + goldToAdd >= LLD_MAX){
        gold = LLD_MAX;
    }
    else{
        gold += goldToAdd;
    }
    //Update l'affichage de l'or
    if (currentpage == &pageHolder.page[0]){
        uiTxt *txtToChange = getTxtFromLabel("playerGold");
        setUiText(txtToChange, formatChaine("%t: %w",OR_MSG, gold));
    }
    else if (currentpage == &pageHolder.page[3]){
        uiTxt *txtToChange = getTxtFromLabel("playerGoldHeroShop");
        setUiText(txtToChange, formatChaine("%t: %w",OR_MSG, gold));
        updateHeroShopPage();
    }
}

void setPlayerDamage(unsigned long long int damage){
    damage_click = damage;
    //Update l'affichage des dégats
    uiTxt *txtToChange = getTxtFromLabel("playerDamage");
    setUiText(txtToChange, formatChaine("%t: %w",DMG_MSG, damage_click));
}