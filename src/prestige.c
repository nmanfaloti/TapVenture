#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "../lib/sdl.h"
#include "../lib/prestige.h"
#include "../lib/ui.h"
#include "../lib/aff.h"
#include "../lib/lang.h"
#include "../lib/chaine.h"
#include "../lib/input_user.h"
#include "../lib/player.h"
#include "../lib/combat.h"
#include "../lib/heros.h"
#include "../lib/boutique.h"

prestigeTree_t prestigeTree = {NULL, NULL, NULL};

int prestigePoints=0;
int prestigePageIndex=0;
int prestigePointsReward=1;

// Hauteur d'un élément de prestige en fonction du nombre total d'éléments par page
int vectorPrestige;

// Modificateurs des stats liés au prestige
float goldModifier=1;
float damageModifier=1;
float prestigeModifier=1;
// Améliorations de prestige lié aux héros
int heroKeepUpgrade=0;
int heroKeepLevel=0;

// Prestige le joueur
void doPrestige(){
    prestigePoints += prestigePointsReward;
    
    refreshPrestigePage();
    createNotif(Traduction(PRESTIGE_MSG), 0, 1, "assets/ui/notif.png", 5, getRectForCentenredCord(vw(50), vh(30), vw(30), vh(50)), 0, 1, Traduction(PRESTIGE_CONGRATS));
    gold = 0;
    level.currentLvl = 0;
    level.maxLevel = 0;
    level.mobKilled = 0;
    initShop();
    addGold(0);
    setPlayerDamage(10);
    initHeros();
    if (heroKeepUpgrade > 0){
        for (int i = 0; i < heroKeepUpgrade; i++){
            makeHeroAtLevel(i, heroKeepLevel ? heroKeepLevel : 1);
        }
    }
    refreshCurrentLvl();
    refreshButtonShop();
    mobHandler();
}

static void initPrestigeState(){
    if (prestigeTree.Gold != NULL || prestigeTree.Damage != NULL || prestigeTree.Prestige != NULL){
        return;
    }
    prestigeTree.Gold = malloc(sizeof(prestigeList));
    
    if (!prestigeTree.Gold) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Gold\n");
        exit(EXIT_FAILURE);
    }
    prestigeTree.Damage = malloc(sizeof(prestigeList));
    if (!prestigeTree.Damage) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Damage\n");
        exit(EXIT_FAILURE);
    }
    prestigeTree.Prestige = malloc(sizeof(prestigeList));
    if (!prestigeTree.Prestige) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Prestige\n");
        exit(EXIT_FAILURE);
    }
    prestigeTree.Gold->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    if (!prestigeTree.Gold->items) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Gold->items\n");
        exit(EXIT_FAILURE);
    }
    prestigeTree.Damage->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    if (!prestigeTree.Damage->items) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Damage->items\n");
        exit(EXIT_FAILURE);
    }
    prestigeTree.Prestige->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    if (!prestigeTree.Prestige->items) {
        fprintf(stderr, "Failed to allocate memory for prestigeTree.Prestige->items\n");
        exit(EXIT_FAILURE);
    }

    prestigeTree.Gold->count = 0;
    prestigeTree.Damage->count = 0;
    prestigeTree.Prestige->count = 0;

    //Arbre Gold
    char *desc = formatChaine("%t 10%%", PRESTIGE_GOLD_DESC); 
    addPrestigeItem(prestigeTree.Gold, "Gold 1", desc, 1, goldPrestige, 0.1);
    free(desc);
    desc = formatChaine("%t 20%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 2", desc, 2, goldPrestige, 0.2);
    free(desc);
    desc = formatChaine("%t 30%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 3", desc, 3, goldPrestige, 0.3);
    free(desc);
    desc = formatChaine("%t 40%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 4", desc, 4, goldPrestige, 0.4);
    free(desc);
    desc = formatChaine("%t 50%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 5", desc, 5, goldPrestige, 0.5);
    free(desc);
    desc = formatChaine("%t 60%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 6", desc, 6, goldPrestige, 0.6);
    free(desc);
    desc = formatChaine("%t 70%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 7", desc, 7, goldPrestige, 0.7);
    free(desc);
    desc = formatChaine("%t 80%%", PRESTIGE_GOLD_DESC);
    addPrestigeItem(prestigeTree.Gold, "Gold 8", desc, 8, goldPrestige, 0.8);
    free(desc);

    // Arbre Degats
    desc = formatChaine("%t 10%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 1", desc, 1, damagePrestige, 0.1);
    free(desc);
    desc = formatChaine("%t 20%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 2", desc, 2, damagePrestige, 0.2);
    free(desc);
    desc = formatChaine("%t 30%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 3", desc, 3, damagePrestige, 0.3);
    free(desc);
    desc = formatChaine("%t 40%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 4", desc, 4, damagePrestige, 0.4);
    free(desc);
    desc = formatChaine("%t 50%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 5", desc, 5, damagePrestige, 0.5);
    free(desc);
    desc = formatChaine("%t 60%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 6", desc, 6, damagePrestige, 0.6);
    free(desc);
    desc = formatChaine("%t 70%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 7", desc, 7, damagePrestige, 0.7);
    free(desc);
    desc = formatChaine("%t 80%%", PRESTIGE_DAMAGE_DESC);
    addPrestigeItem(prestigeTree.Damage, "Damage 8", desc, 8, damagePrestige, 0.8);
    free(desc);

    // Arbre Prestige
    desc = formatChaine("%t 1", PRESTIGE_PRESTIGE_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 1", desc, 1, prestigePrestige, 1);
    free(desc);
    desc = formatChaine("%t 1", PRESTIGE_HERO_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 2", desc, 2, prestigeKeepHero, 1);
    free(desc);
    desc = formatChaine("%t 5", PRESTIGE_HEROLVL_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 3", desc, 3, prestigeKeepHeroLevel, 5);
    free(desc);
    desc = formatChaine("%t 2", PRESTIGE_PRESTIGE_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 4", desc, 4, prestigePrestige, 2);
    free(desc);
    desc = formatChaine("%t 10", PRESTIGE_HEROLVL_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 5", desc, 5, prestigeKeepHeroLevel, 5);
    free(desc);
    desc = formatChaine("%t 2", PRESTIGE_HERO_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 6", desc, 6, prestigeKeepHero, 1);
    free(desc);
    desc = formatChaine("%t 5", PRESTIGE_PRESTIGE_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 7", desc, 7, prestigePrestige, 5);
    free(desc);
    desc = formatChaine("%t 20", PRESTIGE_HEROLVL_DESC);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 8", desc, 8, prestigeKeepHeroLevel, 10);
    free(desc);

}

//Acheter un objet de prestige
int buyPrestigeButton(void *args[20]){
    if (args[0] == NULL || args[1] == NULL){
        printf("buyPrestigeButton Error: No args\n");
        return 1;
    }

    char *selectedTree = (char*) args[0];
    prestigeItem *item = (prestigeItem*) args[1];
    int index = item->index;

    buyPrestigeItem(selectedTree, index , 1);
    return 0;
}

// Créer l'affichage de l'arbre de prestige
static void loadPrestigeTree(){
    for (int j = 0; j < NB_PRESTIGE_ITEMS_PER_PAGE; j++){
        int i = j;
        if (prestigePageIndex) {
            i+=NB_PRESTIGE_ITEMS_PER_PAGE*prestigePageIndex;
        }
        // Gold
        char imgName[50],buttonLabel[25];
        sprintf(imgName, "assets/ui/icons/prestige/pcoin%d.svg", i+1);
        sprintf(buttonLabel, "Gold %d", i+1);
        if (prestigeTree.Gold->items[i].owned){
            createImgButton(&pageHolder.page[2], prestigeTree.Gold->items[i].pos, imgName, "assets/ui/buttons/yellow/button_square_gradient.svg", 0, 0, buyPrestigeButton,buttonLabel, 2,"Gold", &prestigeTree.Gold->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigelinegold.png", (SDL_Rect){prestigeTree.Gold->items[i].pos.x + prestigeTree.Gold->items[i].pos.w  / 4 , prestigeTree.Gold->items[i].pos.y + prestigeTree.Gold->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Gold->items[i].pos.h}, "prestigeLine");
        }else{
            createImgButton(&pageHolder.page[2], prestigeTree.Gold->items[i].pos, imgName, "assets/ui/buttons/grey/button_square_gradient.svg", 0, 0, buyPrestigeButton, buttonLabel, 2, "Gold", &prestigeTree.Gold->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigelinelocked.png", (SDL_Rect){prestigeTree.Gold->items[i].pos.x + prestigeTree.Gold->items[i].pos.w  / 4 , prestigeTree.Gold->items[i].pos.y + prestigeTree.Gold->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Gold->items[i].pos.h}, "prestigeLine");       
        }
        // Prestige
        sprintf(imgName, "assets/ui/icons/prestige/pprestige%d.svg", i+1);
        sprintf(buttonLabel, "Prestige %d", i+1);
        if (prestigeTree.Prestige->items[i].owned){
            createImgButton(&pageHolder.page[2], prestigeTree.Prestige->items[i].pos, imgName, "assets/ui/buttons/blue/button_square_gradient.svg", 0, 0, buyPrestigeButton, buttonLabel, 2, "Prestige", &prestigeTree.Prestige->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigeline.png", (SDL_Rect){prestigeTree.Prestige->items[i].pos.x + prestigeTree.Prestige->items[i].pos.w  / 4 , prestigeTree.Prestige->items[i].pos.y + prestigeTree.Prestige->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Prestige->items[i].pos.h}, "prestigeLine");
        }else{
            createImgButton(&pageHolder.page[2], prestigeTree.Prestige->items[i].pos, imgName, "assets/ui/buttons/grey/button_square_gradient.svg", 0, 0, buyPrestigeButton, buttonLabel, 2, "Prestige", &prestigeTree.Prestige->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigelinelocked.png", (SDL_Rect){prestigeTree.Prestige->items[i].pos.x + prestigeTree.Prestige->items[i].pos.w  / 4 , prestigeTree.Prestige->items[i].pos.y + prestigeTree.Prestige->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Prestige->items[i].pos.h}, "prestigeLine");       
        }
        // Damage
        sprintf(imgName, "assets/ui/icons/prestige/pdamage%d.svg", i+1);
        sprintf(buttonLabel, "Damage %d", i+1);
        if (prestigeTree.Damage->items[i].owned){
            createImgButton(&pageHolder.page[2], prestigeTree.Damage->items[i].pos, imgName, "assets/ui/buttons/red/button_square_gradient.svg", 0, 0, buyPrestigeButton, buttonLabel, 2, "Damage", &prestigeTree.Damage->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigelinedamage.png", (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Damage->items[i].pos.w  / 4 , prestigeTree.Damage->items[i].pos.y + prestigeTree.Damage->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Damage->items[i].pos.h}, "prestigeLine");
        }else{
            createImgButton(&pageHolder.page[2], prestigeTree.Damage->items[i].pos, imgName, "assets/ui/buttons/grey/button_square_gradient.svg", 0, 0, buyPrestigeButton, buttonLabel, 2, "Damage", &prestigeTree.Damage->items[i]);
            if (i%NB_PRESTIGE_ITEMS_PER_PAGE != 0) createUIImg(&pageHolder.page[2], "assets/ui/icons/prestige/prestigelinelocked.png", (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Damage->items[i].pos.w  / 4 , prestigeTree.Damage->items[i].pos.y + prestigeTree.Damage->items[i].pos.h , vw(5), vectorPrestige - prestigeTree.Damage->items[i].pos.h}, "prestigeLine");
        }
    }
}

static int ChangePrestigePage(void *args[20]){
    if (args[0] == NULL){
        printf("ChangePrestigePage Error: No args\n");
        return 1;
    }
    int * up = (int *) args;

    if (*up == 2 && prestigePageIndex < MAX_PRESTIGE_ITEMS / NB_PRESTIGE_ITEMS_PER_PAGE - 1){
        prestigePageIndex++;
    } else if (*up == 1 && prestigePageIndex > 0){
        prestigePageIndex--;
    } else {
        return 1;
    }

    refreshPrestigePage();
    return 0; 
}

static void initPrestigePage(){
    createPage(&pageHolder.page[2]);

    pageHolder.page[2].container->nbTxt = 0;
    pageHolder.page[2].container->txt = NULL;
    pageHolder.page[2].container->nbImg = 0;
    pageHolder.page[2].container->img = NULL;

    createUIImg(&pageHolder.page[2], "assets/ui/background/prestigebg1.png", (SDL_Rect){0, 0, vw(100), vh(100)}, "prestigeBackground1");
    createUIImg(&pageHolder.page[2], "assets/ui/background/prestigebg2.png", (SDL_Rect){vw(17.5), vh(17.5), vw(65), vh(65)}, "prestigeBackground2");
    createUIText(&pageHolder.page[2], fontBig, formatChaine("%t", PRESTIGE_MSG), getRectForCentenredCord(vw(50), vh(5), vh(40), vh(10)), (SDL_Color){255, 255, 255, 255}, "prestigeTitle");
    char * desc = formatChaine("POINTS: %d" , prestigePoints);
    createUIText(&pageHolder.page[2], font, desc, getSizeForText(font, desc, (SDL_Rect){vw(1), vh(1), vw(10), vh(5)}), (SDL_Color){255, 255, 255, 255}, "prestigePoints");
    createImgButton(&pageHolder.page[2],getRectForCentenredCord(vw(95), vh(90), 50, 50), "assets/ui/icons/others/return.svg", "assets/ui/buttons/extra/button_round_depth_line.svg", 0, 2, changePage,"returnButton", 1, &pageHolder.page[0]);
    
    createImgButton(&pageHolder.page[2],getRectForCentenredCord(vw(90), vh(5), vw(5), vw(5)), "assets/ui/buttons/blue/arrow_basic_n.svg", "assets/ui/buttons/extra/button_square_line.svg", 0, 0, ChangePrestigePage, "arrow1", 1, 2);
    createImgButton(&pageHolder.page[2],getRectForCentenredCord(vw(96), vh(5), vw(5), vw(5)), "assets/ui/buttons/blue/arrow_basic_s.svg", "assets/ui/buttons/extra/button_square_line.svg", 0, 0, ChangePrestigePage, "arrow2",1, 1);
}

void refreshPrestigeTreeSize(){
    //Refresh les positions des items de prestige celon la taille de la fenetre
    for (int i = 0; i < prestigeTree.Gold->count; i++) {
        prestigeTree.Gold->items[i].pos = getRectForCentenredCord(vw(15), 
            vh(90) - (i % NB_PRESTIGE_ITEMS_PER_PAGE) * vh(100) / NB_PRESTIGE_ITEMS_PER_PAGE, 
            vw(10), vh(10));
    }
    for (int i = 0; i < prestigeTree.Damage->count; i++) {
        prestigeTree.Damage->items[i].pos = getRectForCentenredCord(vw(50), 
            vh(90) - (i % NB_PRESTIGE_ITEMS_PER_PAGE) * vh(100) / NB_PRESTIGE_ITEMS_PER_PAGE, 
            vw(10), vh(10));
    }
    for (int i = 0; i < prestigeTree.Prestige->count; i++) {
        prestigeTree.Prestige->items[i].pos = getRectForCentenredCord(vw(85), 
            vh(90) - (i % NB_PRESTIGE_ITEMS_PER_PAGE) * vh(100) / NB_PRESTIGE_ITEMS_PER_PAGE, 
            vw(10), vh(10));
    }
    refreshPrestigePage();
}

void refreshPrestigePage(){
    destroyPage(&pageHolder.page[2]);
    
    initPrestigePage();
    loadPrestigeTree();
}

void initPrestige(){
    // Calcul une fois pour eviter de le recalculer à chaque fois
    vectorPrestige = vh(100) / NB_PRESTIGE_ITEMS_PER_PAGE;

    initPrestigeState();
    initPrestigePage();
    loadPrestigeTree();
}

int getMaxPrestigeItems(char * selectedTree){
    int max = 0;
    if (strcmp(selectedTree, "Gold") == 0){
        for (int i = 0; i < prestigeTree.Gold->count; i++) {
            if (prestigeTree.Gold->items[i].owned){
                max++;
            }else{
                return max; // Renvoie l'indice du dernier item acheté
            }
        }
    } else if (strcmp(selectedTree, "Damage") == 0){
        for (int i = 0; i < prestigeTree.Damage->count; i++) {
            if (prestigeTree.Damage->items[i].owned){
                max++;
            }else{
                return max; // Renvoie l'indice du dernier item acheté
            }
        }
    } else if (strcmp(selectedTree, "Prestige") == 0){
        for (int i = 0; i < prestigeTree.Prestige->count; i++) {
            if (prestigeTree.Prestige->items[i].owned){
                max++;
            }else{
                return max; // Renvoie l'indice du dernier item acheté
            }
        }
    }
    return max;
}


int created[3] = {-1, -1, -1}; //Pour savoir si un texte a été créé ou pas dans une catégorie

//Affiche les information de l'amélioration de prestige lorsqu'on passe la souris dessus
void checkDisplayPrestigeItemText(){
    if (currentpage != &pageHolder.page[2]){
        return;
    }
    uiTxt *existingText = getTxtFromLabel("prestigeDescription");
    uiTxt *existingCost = getTxtFromLabel("prestigeCost");
    
    for (int j = 0; j < NB_PRESTIGE_ITEMS_PER_PAGE; j++){
        int i = j;
        if (prestigePageIndex) {
            i+=NB_PRESTIGE_ITEMS_PER_PAGE*prestigePageIndex;
        }
        int hover = checkButton(prestigeTree.Gold->items[i].pos, mouseX, mouseY);
        if (hover && created[0] == -1) {
            if (prestigeTree.Gold->items[i].description) {
                char *desc = malloc(strlen(prestigeTree.Gold->items[i].description) + 1);
                strcpy(desc, prestigeTree.Gold->items[i].description);
                createUIText(&pageHolder.page[2], font, desc, getSizeForText(font, desc, (SDL_Rect){prestigeTree.Gold->items[i].pos.x + prestigeTree.Gold->items[i].pos.w * 1.25, prestigeTree.Gold->items[i].pos.y + vh(5), vw(20), vh(30)}), (SDL_Color){255, 255, 255, 255}, "prestigeDescription");

                char *cost = formatChaine("%t %d", PRESTIGE_COST, prestigeTree.Gold->items[i].cost);
                createUIText(&pageHolder.page[2], font, cost, getSizeForText(font, cost, (SDL_Rect){prestigeTree.Gold->items[i].pos.x + prestigeTree.Gold->items[i].pos.w * 1.25, prestigeTree.Gold->items[i].pos.y, vw(7), vh(4)}), (SDL_Color){255, 255, 255, 255}, "prestigeCost");
                existingText = getTxtFromLabel("prestigeDescription");
                existingCost = getTxtFromLabel("prestigeCost");
                created[0] = i;
            }
        } else if (created[0] == i && !hover) {
            destroyUITxt(existingText, &pageHolder.page[2]);
            destroyUITxt(existingCost, &pageHolder.page[2]);
            created[0] = -1;
        }

        hover = checkButton(prestigeTree.Damage->items[i].pos, mouseX, mouseY);
        if (hover && created[2] == -1) {
            if (prestigeTree.Damage->items[i].description) {
                char *desc = malloc(strlen(prestigeTree.Damage->items[i].description) + 1);
                strcpy(desc, prestigeTree.Damage->items[i].description);
                createUIText(&pageHolder.page[2], font, desc, getSizeForText(font, prestigeTree.Damage->items[i].description, (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Damage->items[i].pos.w * 1.25, prestigeTree.Damage->items[i].pos.y + vh(5), vw(20), vh(10)}), (SDL_Color){255, 255, 255, 255}, "prestigeDescription");
                char *cost = formatChaine("%t %d", PRESTIGE_COST, prestigeTree.Damage->items[i].cost);
                createUIText(&pageHolder.page[2], font, cost, getSizeForText(font, cost, (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Damage->items[i].pos.w * 1.25, prestigeTree.Damage->items[i].pos.y, vw(7), vh(4)}), (SDL_Color){255, 255, 255, 255}, "prestigeCost");
                existingText = getTxtFromLabel("prestigeDescription");
                existingCost = getTxtFromLabel("prestigeCost");
                created[2] = i;
            }
        } else if (created[2] == i && !hover) {
            destroyUITxt(existingText, &pageHolder.page[2]);
            destroyUITxt(existingCost, &pageHolder.page[2]);
            created[2] = -1;
        }

        hover = checkButton(prestigeTree.Prestige->items[i].pos, mouseX, mouseY);  
        if (hover && created[1] == -1) {
            if (prestigeTree.Prestige->items[i].description) {
                char *desc = malloc(strlen(prestigeTree.Prestige->items[i].description) + 1);
                strcpy(desc, prestigeTree.Prestige->items[i].description);
                // Utilisation de la position x de Damage pour avoir le texte à gauche du bouton et donc ne pas sortir de l'écran
                createUIText(&pageHolder.page[2], font, desc, getSizeForText(font, prestigeTree.Prestige->items[i].description, (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Prestige->items[i].pos.w * 1.25, prestigeTree.Prestige->items[i].pos.y + vh(5), vw(20), vh(10)}), (SDL_Color){255, 255, 255, 255}, "prestigeDescription");
                char *cost = formatChaine("%t %d", PRESTIGE_COST, prestigeTree.Prestige->items[i].cost);
                createUIText(&pageHolder.page[2], font, cost, getSizeForText(font, cost, (SDL_Rect){prestigeTree.Damage->items[i].pos.x + prestigeTree.Prestige->items[i].pos.w * 1.25, prestigeTree.Prestige->items[i].pos.y, vw(7), vh(4)}), (SDL_Color){255, 255, 255, 255}, "prestigeCost");
                existingText = getTxtFromLabel("prestigeDescription");
                existingCost = getTxtFromLabel("prestigeCost");
                created[1] = i;
            }
        } else if (created[1] == i && !hover) {
            destroyUITxt(existingText, &pageHolder.page[2]);
            destroyUITxt(existingCost, &pageHolder.page[2]);
            created[1] = -1;
        }
    }
}

//Ajouter un item de prestige à la liste
void addPrestigeItem(prestigeList *list, char *name, char *description, int cost, void (*effect)(float), float value) {
    if (list->count >= MAX_PRESTIGE_ITEMS) {
        printf("Error: Max Prestige Item\n");
        return;
    }
    prestigeItem item;
    
    // +1 pour le caractère nul à la fin de la chaîne
    item.name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(item.name, name);

    item.description = malloc(sizeof(char) * (strlen(description) + 1));
    strcpy(item.description, description);
    
    item.cost = cost;
    item.effect = effect;
    item.owned = 0;
    item.value = value;
    
    if (list == prestigeTree.Gold) {
        item.pos = getRectForCentenredCord(vw(15), vh(90)-(((prestigeTree.Gold->count % NB_PRESTIGE_ITEMS_PER_PAGE)) * vectorPrestige), vw(10), vh(10));
        item.index = prestigeTree.Gold->count;
    } else if (list == prestigeTree.Damage) {
        item.pos = getRectForCentenredCord(vw(50), vh(90)-(((prestigeTree.Damage->count % NB_PRESTIGE_ITEMS_PER_PAGE)) * vectorPrestige), vw(10), vh(10));
        item.index = prestigeTree.Damage->count;
    } else if (list == prestigeTree.Prestige) {
        item.pos = getRectForCentenredCord(vw(85), vh(90)-(((prestigeTree.Prestige->count % NB_PRESTIGE_ITEMS_PER_PAGE)) * vectorPrestige), vw(10), vh(10));
        item.index = prestigeTree.Prestige->count;
    } else {
        free(item.name);
        free(item.description);
        printf("Error: Invalid prestige list\n");
        return;
    }
    list->items[list->count] = item;
    list->count++;
}

//Destruction d'un item de prestige
static void destroyPrestigeItem(prestigeItem *item){
    free(item->name);
    free(item->description);
}
// Destruction de la liste de prestige
void destroyPrestigeList(){
    if (prestigeTree.Gold != NULL) {
        for (int i = 0; i < prestigeTree.Gold->count; i++) {
            destroyPrestigeItem(&prestigeTree.Gold->items[i]);
        }
        free(prestigeTree.Gold->items);
        free(prestigeTree.Gold);
    }
    
    if (prestigeTree.Damage != NULL) {
        for (int i = 0; i < prestigeTree.Damage->count; i++) {
            destroyPrestigeItem(&prestigeTree.Damage->items[i]);
        }
        free(prestigeTree.Damage->items);
        free(prestigeTree.Damage);
    }
    
    if (prestigeTree.Prestige != NULL) {
        for (int i = 0; i < prestigeTree.Prestige->count; i++) {
            destroyPrestigeItem(&prestigeTree.Prestige->items[i]);
        }
        free(prestigeTree.Prestige->items);
        free(prestigeTree.Prestige);
    }

    prestigeTree.Gold = NULL;
    prestigeTree.Damage = NULL;
    prestigeTree.Prestige = NULL;
}

void goldPrestige(float value){
    goldModifier += value;
}

void damagePrestige(float value){
    damageModifier += value;
}

void prestigePrestige(float value){
    prestigePointsReward += value;
}

void prestigeKeepHero(float value){
    heroKeepUpgrade += value;
}
void prestigeKeepHeroLevel(float value){
    heroKeepLevel += value;
}

// Vérifie si l'item peut être acheté
int canBuy(char *selectedTree, int index){
    if (strcmp(selectedTree, "Gold") == 0){
        if (prestigeTree.Gold->items[index].owned) return 0;
        if (index == 0 && prestigeTree.Gold->items[index].cost <= prestigePoints) return 1;
        for (int i = 0; i < index; i++){
            // Vérifie si l'item précédent est acheté
            if (prestigeTree.Gold->items[i].owned == 0){ 
                return 0;
            }
        }
        // Vérifie si le joueur a assez de prestige points
        return prestigeTree.Gold->items[index].cost <= prestigePoints;
    } else if (strcmp(selectedTree, "Damage") == 0){
        if (prestigeTree.Damage->items[index].owned) return 0;
        if (index == 0 && prestigeTree.Damage->items[index].cost <= prestigePoints) return 1;
        for (int i = 0; i < index; i++){
            if (prestigeTree.Damage->items[i].owned == 0){
                return 0;
            }
        }
        return prestigeTree.Damage->items[index].cost <= prestigePoints;
    } else if (strcmp(selectedTree, "Prestige") == 0){
        if (prestigeTree.Prestige->items[index].owned) return 0;
        if (index == 0 && prestigeTree.Prestige->items[index].cost <= prestigePoints) return 1;
        for (int i = 0; i < index; i++){
            if (prestigeTree.Prestige->items[i].owned == 0){
                return 0;
            }
        }
        return prestigeTree.Prestige->items[index].cost <= prestigePoints;
    }
    return 0;
}

// Acheter un item de prestige 
void buyPrestigeItem(char *selectedTree, int index, int pay){
    if (strcmp(selectedTree, "Gold") == 0){
        if (!pay || canBuy(selectedTree, index)){
            if (pay) {
                prestigePoints -= prestigeTree.Gold->items[index].cost;
            }
            prestigeTree.Gold->items[index].effect(prestigeTree.Gold->items[index].value);
            prestigeTree.Gold->items[index].owned = 1;
            refreshPrestigePage();
        }
    }else if (strcmp(selectedTree, "Damage") == 0){
        if (!pay || canBuy(selectedTree, index)){
            if (pay) {
                prestigePoints -= prestigeTree.Damage->items[index].cost;
            }
            prestigeTree.Damage->items[index].effect(prestigeTree.Damage->items[index].value);
            prestigeTree.Damage->items[index].owned = 1;
            refreshPrestigePage();
        }
    }else if (strcmp(selectedTree, "Prestige") == 0){
        if (!pay || canBuy(selectedTree, index)){
            if (pay) {
                prestigePoints -= prestigeTree.Prestige->items[index].cost;
            }
            prestigeTree.Prestige->items[index].effect(prestigeTree.Prestige->items[index].value);
            prestigeTree.Prestige->items[index].owned = 1;
            refreshPrestigePage();
        }
    }
}