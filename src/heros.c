#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "../lib/heros.h"
#include "../lib/combat.h"
#include "../lib/player.h"
#include "../lib/ui.h"
#include "../lib/aff.h"
#include "../lib/lang.h"
#include "../lib/sdl.h"
#include "../lib/chaine.h"
#include "../lib/prestige.h"
#include "../lib/inv.h"

#define DEGATS_BASE 10
#define PRIX_BASE 100
#define DEFAULT_COOLDOWN 10000
//UP correspond a la différence entre les heros 
#define DEGAT_UP 1.15
#define PRIX_UP 1.30
#define COOLDOWN_UP 0.8
//UPGRADE pour les amelioration de heros
#define DEGAT_UPGRADE 1.15
#define PRIX_UPGRADE 1.40

#define MAX_MULTIPLICATOR 1000

hero heros[HEROS_COUNT];
int multiplicator = 1;

int initHeros(){
    heros[HERO0].degat = DEGATS_BASE;
    heros[HERO0].prix = PRIX_BASE;
    heros[HERO0].level = 0;
    heros[HERO0].cooldown = DEFAULT_COOLDOWN; //en millisecondes
    heros[HERO0].lastAttack = 0;
    for (int i = HERO1; i < HEROS_COUNT; i++){
        heros[i].degat = heros[i-1].degat * DEGAT_UP;
        heros[i].prix = heros[i-1].prix * PRIX_UP;
        heros[i].level = 0;
        heros[i].cooldown = heros[i-1].cooldown * COOLDOWN_UP;
        heros[i].lastAttack = 0;
    }
    return 0;
}

int attackHeros(){
    void * args[20];
    for (int i = HERO0; i < HEROS_COUNT; i++){
        gestion_stat_heros(i);
        if( heros[i].level > 0 && heros[i].lastAttack + heros[i].cooldown/ stat_item_temps <= SDL_GetTicks()){
            unsigned long long int degat_attaque_booster ;
            heros[i].lastAttack = SDL_GetTicks();
            if ( heros[i].degat * stat_item_degat < LLD_MAX)
                degat_attaque_booster = heros[i].degat * stat_item_degat;
            else degat_attaque_booster = LLD_MAX ;
            args[0] = &(degat_attaque_booster);
            bool joueur = false;
            args[1] = &joueur;
            attack(args); 
        }
    }
    return 0;
}

int upgradeHero(int heroIndex, bool pay){
    if(heroIndex >= HEROS_COUNT || heroIndex < 0){
        printf("Erreur lors de l'upgrade du hero %d\n", heroIndex);
        return 1;
    }
    if (!pay && (heroIndex < HEROS_COUNT)){
        if (((heros[heroIndex].prix * PRIX_UPGRADE) < LLD_MAX) && ((heros[heroIndex].prix * PRIX_UPGRADE) > 0) && ((heros[heroIndex].degat * DEGAT_UPGRADE) < LLD_MAX) && ((heros[heroIndex].degat * DEGAT_UPGRADE) > 0)){
            heros[heroIndex].prix *= PRIX_UPGRADE;
            heros[heroIndex].degat *= DEGAT_UPGRADE;
            heros[heroIndex].level += 1;
            return 0;
        }
        else{
            return 1;
        }
    }
    else if ((heroIndex < HEROS_COUNT) && (heros[heroIndex].prix <= gold)){
        if (((heros[heroIndex].prix * PRIX_UPGRADE) < LLD_MAX) && ((heros[heroIndex].prix * PRIX_UPGRADE) > 0) && ((heros[heroIndex].degat * DEGAT_UPGRADE) < LLD_MAX) && ((heros[heroIndex].degat * DEGAT_UPGRADE) > 0)){
            addGold(-heros[heroIndex].prix);
            heros[heroIndex].prix *= PRIX_UPGRADE;
            heros[heroIndex].degat *= DEGAT_UPGRADE;
            heros[heroIndex].level += 1;
            if(currentpage == &pageHolder.page[3]){
                updateHeroShopPage();
            }
            return 0;
        }
        else{
            return 1;
        }
    }
    return 1;
}

int upgradeHeroCB(void * args[20]){
    int * heroIndex = (int *)args;
    if (getHeroPriceByMultiplicator(*heroIndex) <= gold){
        for (int i = 0; i < multiplicator; i++){
            if (upgradeHero(*heroIndex, true)){
                return 1;
            }
        }
    }
    else if (multiplicator == MAX_MULTIPLICATOR){
        for (int i = 0; i < multiplicator; i++){
            if (upgradeHero(*heroIndex, true)){
                return 0;
            }
        }
    }
    return 0;
}

int makeHeroAtLevel(int heroIndex, int levelH){
    if (heroIndex < HEROS_COUNT){
        while (heros[heroIndex].level < levelH){
            if (upgradeHero(heroIndex, false)){
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

unsigned long long int getHeroPriceByMultiplicator(int heroIndex){
    unsigned long long int price = getHeroPrice(heroIndex);
    unsigned long long int sommePrice = 0;
        
    for (int i = 0; i < multiplicator; i++){
        if ((sommePrice + price) > LLD_MAX){
            return LLD_MAX;
        }
        else if(multiplicator == MAX_MULTIPLICATOR && (sommePrice + price) > gold){//calcul du prix le maximum en fonction du gold
            return sommePrice;
        }
        sommePrice += price;
        price = price * PRIX_UPGRADE;
    }
    return sommePrice;
}

unsigned long long int getHeroPrice(int heroIndex){
    if (heroIndex < HEROS_COUNT){
        return heros[heroIndex].prix;
    }
    printf("Erreur lors de la recuperation du prix du hero %d\n", heroIndex);
    return 0;
}

unsigned long long int herosDPS(int indiceHero){
    if (indiceHero < HEROS_COUNT && heros[indiceHero].level > 0){
        if( heros[indiceHero].cooldown > 1000){
            return heros[indiceHero].degat * damageModifier/ (heros[indiceHero].cooldown /1000 ); 
        }
        else{
            return heros[indiceHero].degat * damageModifier;
        }
    }
    return 0;
}

unsigned long long int herosAllDPS(){
    unsigned long long int somme = 0;
    for (int i = HERO0; i < HEROS_COUNT; i++) {
        if (heros[i].level > 0){
            if((somme + herosDPS(i)) > LLD_MAX){
                return LLD_MAX;
            }
            somme += herosDPS(i);
        }
    }
    return somme; 
}

unsigned long long int herosGoldGenBySec(){
    monstreInfo * currentMonstre = &level.monstre[level.currentLvl];
    int coinMoy = (currentMonstre->coinMin + currentMonstre->coinMax) / 2;
    float nbMonstresMortParSecondes = herosAllDPS() * 1.0 / currentMonstre->iniHealth * 1.0;
    return nbMonstresMortParSecondes * coinMoy;

}

void initHerosPage(){
    createPage(&pageHolder.page[3]);

    pageHolder.page[3].container->nbTxt = 0;
    pageHolder.page[3].container->txt = NULL; 

    createUIImg(&pageHolder.page[3], "assets/ui/background/heroShopbg.png", (SDL_Rect){0, 0, vw(100), vh(100)}, "heroShopBackground");

    createButton(&pageHolder.page[3],getRectForCentenredCord(vw(92), vh(5), vw(15), vh(8)),"assets/ui/buttons/green/button_rectangle_depth_gloss.svg", "assets/ui/buttons/green/button_rectangle_depth_flat.svg", font, (SDL_Color){0, 0, 0, 200}, HERO_SHOP_MULTIPLICATOR_MSG, &multiplicator ,0.5, changeMultiplicator, 0);

    createUIText(&pageHolder.page[3], fontBig, formatChaine("%t", HERO_SHOP_TITLE_MSG), getRectForCentenredCord(vw(50), vh(5), vw(40), vh(10)), (SDL_Color){255, 255, 255, 255}, "heroShopTitle");
    createUIText(&pageHolder.page[3],fontBig,formatChaine("%t: %w",OR_MSG, gold), (SDL_Rect) {vw(1),vh(1), vw(15), vh(10)}, (SDL_Color){255, 255, 255, 255}, "playerGoldHeroShop");
    createUIText(&pageHolder.page[3],fontBig,formatChaine("%t: %w",DPS_MSG, herosAllDPS()), getRectForCentenredCord(vw(50), vh(92), vw(22), vh(8)), (SDL_Color){255, 255, 255, 255}, "DPSHeroShop");

    createImgButton(&pageHolder.page[3],getRectForCentenredCord(vw(95), vh(90), 50, 50), "assets/ui/icons/others/return.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage, "returnButton",1, &pageHolder.page[0]);

    loadHerosMatrice();
}

void loadHerosMatrice(){
    int vectorHerosW = vw(80) / NB_HEROS_PER_PAGE_W;
    int vectorHerosH = vh(90) / NB_HEROS_PER_PAGE_H;
    int textOffsetW = vh(9);
    int textOffsetH = vh(5);
    for (int i = 0; i < 13 / NB_HEROS_PER_PAGE_H; i++){
        for (int j = 0; j < NB_HEROS_PER_PAGE_W-1; j++){
            int heroIndex = i+j*NB_HEROS_PER_PAGE_W;
            if(heroIndex < HEROS_COUNT){
                char imgHero[50],heroLabel[15];
                sprintf(imgHero, "assets/ui/heros/hero%d.png", heroIndex);
                sprintf(heroLabel, "hero%d", heroIndex);
                createImgButton(&pageHolder.page[3],getRectForCentenredCord((vw(15)+(i * vectorHerosW)), (vh(15)+(j * vectorHerosH)), vw(10), vh(15)), NULL, imgHero, 0, 0, upgradeHeroCB, heroLabel,1, heroIndex);
                char *degat = formatChaine("%t: %w",DMG_MSG, heros[heroIndex].degat);
                char degatLabel[50];
                sprintf(degatLabel, "heroDegat%d", heroIndex);
                createUIText(&pageHolder.page[3], fontBig, degat, getSizeForText(fontBig, degat, (SDL_Rect){(vw(15)+(i * vectorHerosW)+textOffsetW), (vh(15)+(j * vectorHerosH)+textOffsetH), vw(8), vh(8)}), (SDL_Color){255, 255, 255, 255}, degatLabel);
                char *cost = formatChaine("%t: %w",PRICE_MSG, getHeroPriceByMultiplicator(heroIndex));
                char costLabel[50];
                sprintf(costLabel, "heroCost%d", heroIndex);
                createUIText(&pageHolder.page[3], fontBig, cost, getSizeForText(fontBig, cost, (SDL_Rect){(vw(15)+(i * vectorHerosW)+textOffsetW), (vh(15)+(j * vectorHerosH)), vw(8), vh(8)}), (SDL_Color){255, 255, 255, 255}, costLabel);
                char *level = formatChaine("%t: %w",LVL_MSG, heros[heroIndex].level);
                char levelLabel[50];
                sprintf(levelLabel, "heroLevel%d", heroIndex);
                createUIText(&pageHolder.page[3], fontBig, level, getSizeForText(fontBig, level, (SDL_Rect){(vw(15)+(i * vectorHerosW)+textOffsetW), (vh(15)+(j * vectorHerosH)-textOffsetH), vw(8), vh(8)}), (SDL_Color){255, 255, 255, 255}, levelLabel);
            }
        }
    }
} 


void updateHeroShopPage(){
    for (int i = HERO0; i < HEROS_COUNT; i++){
        updateHeroIInShopPage(i);
    }
}

void updateHeroIInShopPage(int heroIndex){
    if (heroIndex < HEROS_COUNT){
        char heroLabel[50];
        sprintf(heroLabel, "heroDegat%d", heroIndex);
        uiTxt *txtToChange = getTxtFromLabel(heroLabel);
        setUiText(txtToChange, formatChaine("%t: %w",PRICE_MSG, getHeroPriceByMultiplicator(heroIndex)));
        sprintf(heroLabel, "heroCost%d", heroIndex);
        uiTxt *txtToChange2 = getTxtFromLabel(heroLabel);
        setUiText(txtToChange2, formatChaine("%t: %w",DMG_MSG, heros[heroIndex].degat));
        sprintf(heroLabel, "heroLevel%d", heroIndex);
        uiTxt *txtToChange3 = getTxtFromLabel(heroLabel);
        setUiText(txtToChange3, formatChaine("%t: %w",LVL_MSG, heros[heroIndex].level));
        uiTxt *txtToChange4 = getTxtFromLabel("DPSHeroShop");
        setUiText(txtToChange4, formatChaine("%t: %w",DPS_MSG, herosAllDPS()));
    }
}

int changeMultiplicator(){
    if (multiplicator == 1){
        multiplicator = 10;
    }
    else if (multiplicator == 10){
        multiplicator = 100;
    }
    else if (multiplicator == 100){
        multiplicator = MAX_MULTIPLICATOR;
    }
    else{
        multiplicator = 1;
    }
    if (pageHolder.page == NULL ||  pageHolder.page[3].buttonsList == NULL){
        return 1;
    }
    //Update du bouton pour afficher le nouveau multiplicator (0 est son indice dans la liste des boutons)
    char * txt; 
    if(multiplicator == MAX_MULTIPLICATOR){
        txt = formatChaine("%t: %t", pageHolder.page[3].buttonsList->buttons[0].text, MAX_MSG);
    }
    else{
        txt = formatChaine("%t: %w", pageHolder.page[3].buttonsList->buttons[0].text, multiplicator);
    }
    setButtonText(&pageHolder.page[3].buttonsList->buttons[0], txt);
    free(txt);
    updateHeroShopPage();
    return 0;
}