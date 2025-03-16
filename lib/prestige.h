#ifndef PRESTIGE_H
#define PRESTIGE_H

#define MAX_PRESTIGE_ITEMS 8
#define NB_PRESTIGE_ITEMS_PER_PAGE 4

typedef struct prestigeItem_s{
    char *name;
    char *description;
    int cost;
    int owned;
    float value;
    void (*effect)(float);
    int index;
    SDL_Rect pos;
} prestigeItem;

typedef struct prestigeList_s{
    prestigeItem *items;
    int count;
} prestigeList;

typedef struct prestigeTree_s{
    prestigeList *Gold;
    prestigeList *Damage;
    prestigeList *Prestige;
}prestigeTree_t;

void initPrestige();
void addPrestigeItem(prestigeList *list, char *name, char *description, int cost, void (*effect)(float), float value);
void goldPrestige(float value);
void damagePrestige(float value);
void prestigePrestige(float value);
void printTree();
void buyPrestigeItem(char *type, int index);
void checkDisplayPrestigeItemText();
void destroyPrestigeList();
void refreshPrestigePage();

void doPrestige();
int canBuy(char *type, int index);
#endif