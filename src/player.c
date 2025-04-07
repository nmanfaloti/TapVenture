#include "../lib/player.h"
#include "../lib/heros.h"
#include "../lib/ui.h"
#include "../lib/lang.h"
#include "../lib/aff.h"
#include "../lib/chaine.h"
#include "../lib/ui.h"
#include "../lib/prestige.h"
#include "../lib/audio.h"
#include "../lib/inv.h"
#include <time.h>
#include <math.h>

unsigned long long int gold = 0;
unsigned long long int damage_click = 1;
int quitGameValue = 0;

void goldGainOffline(time_t lastSaveTime){
    time_t currentTime = time(NULL);
    if (lastSaveTime <= 0 || currentTime < lastSaveTime){
        return;
    }
    double elapsedTime = difftime(currentTime, lastSaveTime);
    unsigned long long int goldBySec = herosGoldGenBySec();
    unsigned long long int goldEarned = (elapsedTime * goldBySec);
    addGold((goldEarned/10));
    if (goldEarned != 0){
        char * goldEarnedMsg = formatChaine("%t %w",GOLD_OFFLINE_DESC_MSG, goldEarned/10);
        //sprintf(goldEarnedMsg, "%s %lld",Traduction(GOLD_OFFLINE_DESC_MSG), goldEarned/10);
        createNotif(Traduction(GOLD_OFFLINE_MSG),0,1,"assets/ui/notif.png", 3,getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),0, 1, goldEarnedMsg);
        free(goldEarnedMsg);
    }
}
    
void addGold(long long int goldToAdd){
    if(gold + goldToAdd * goldModifier * boost_gold()>= LLD_MAX && goldToAdd != 0){
        gold = LLD_MAX;
    }
    else{
        gold += goldToAdd * goldModifier; // Application du multiplicateur de l'or (Prestige)
    }
    if(goldToAdd > 0){
        playMusic(MUSIC_COIN, CANAL_EFFECT, 1);
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
    // Update l'affichage des dégats
    uiTxt *txtToChange = getTxtFromLabel("playerDamage");

    if (damage_click * damageModifier >= LLD_MAX){
        setUiText(txtToChange, formatChaine("%t: %w", DMG_MSG, LLD_MAX));
        return;
    }
    setUiText(txtToChange, formatChaine("%t: %w", DMG_MSG, (unsigned long long int)(damage_click * damageModifier)));
}

int quitGame(void * args[20]){
    args[0] = NULL;
    quitGameValue = 1;
    return 0;
}