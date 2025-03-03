#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "../lib/prestige.h"

prestigeTree_t prestigeTree;

int prestigePoints=1;
float goldModifier=1;
float damageModifier=1;
float prestigeModifier=1;

void initPrestige(){
    prestigeTree.Gold = malloc(sizeof(prestigeList));
    prestigeTree.Damage = malloc(sizeof(prestigeList));
    prestigeTree.Prestige = malloc(sizeof(prestigeList));
    prestigeTree.Gold->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    prestigeTree.Damage->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    prestigeTree.Prestige->items = malloc(sizeof(prestigeItem) * MAX_PRESTIGE_ITEMS);
    prestigeTree.Gold->count = 0;
    prestigeTree.Damage->count = 0;
    prestigeTree.Prestige->count = 0;

    //Arbre Gold
    addPrestigeItem(prestigeTree.Gold, "Gold 1", "Gain 10% more gold", 1, goldPrestige, 0.1);
    addPrestigeItem(prestigeTree.Gold, "Gold 2", "Gain 20% more gold", 2, goldPrestige, 0.2);
    //Arbre Degats
    addPrestigeItem(prestigeTree.Damage, "Damage 1", "Deal 10% more damage", 1, damagePrestige, 0.1);
    //Arbre Prestige
    addPrestigeItem(prestigeTree.Prestige, "Prestige 1", "Gain 10% more prestige", 1, prestigePrestige, 0.1);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 2", "Gain 20% more prestige", 2, prestigePrestige, 0.2);
    addPrestigeItem(prestigeTree.Prestige, "Prestige 3", "Gain 30% more prestige", 3, prestigePrestige, 0.3);
}

void addPrestigeItem(prestigeList *list, char *name, char *description, int cost, void (*effect)(float), float value){
    prestigeItem item;
    item.name = malloc(sizeof(char) * strlen(name));
    strcpy(item.name, name);
    item.description = malloc(sizeof(char) * strlen(description));
    strcpy(item.description, description);
    item.cost = cost;
    item.effect = effect;
    item.owned = 0;
    item.value = value;
    list->items[list->count] = item;
    list->count++;
}

void goldPrestige(float value){
    goldModifier += value;
}

void damagePrestige(float value){
    damageModifier += value;
}

void prestigePrestige(float value){
    prestigeModifier += value;
}

void printTree(){
    printf("--------------------------------\n");
    printf("Prestige Tree:\n");
    printf("Gold:\n");
    for (int i = 0; i < prestigeTree.Gold->count; i++){
        printf("Name: %s\n", prestigeTree.Gold->items[i].name);
        printf("Description: %s\n", prestigeTree.Gold->items[i].description);
        printf("Cost: %d\n", prestigeTree.Gold->items[i].cost);
    }
    printf("################################\n");
    printf("Damage:\n");
    for (int i = 0; i < prestigeTree.Damage->count; i++){
        printf("Name: %s\n", prestigeTree.Damage->items[i].name);
        printf("Description: %s\n", prestigeTree.Damage->items[i].description);
        printf("Cost: %d\n", prestigeTree.Damage->items[i].cost);
    }
    printf("################################\n");
    printf("Prestige:\n");
    for (int i = 0; i < prestigeTree.Prestige->count; i++){
        printf("Name: %s\n", prestigeTree.Prestige->items[i].name);
        printf("Description: %s\n", prestigeTree.Prestige->items[i].description);
        printf("Cost: %d\n", prestigeTree.Prestige->items[i].cost);
    }
    printf("--------------------------------\n");
    printf("Owned:\n");
    for (int i = 0; i < prestigeTree.Gold->count; i++){
        if (prestigeTree.Gold->items[i].owned){
            printf("Name: %s\n", prestigeTree.Gold->items[i].name);
        }
    }
    for (int i = 0; i < prestigeTree.Damage->count; i++){
        if (prestigeTree.Damage->items[i].owned){
            printf("Name: %s\n", prestigeTree.Damage->items[i].name);
        }
    }
    for (int i = 0; i < prestigeTree.Prestige->count; i++){
        if (prestigeTree.Prestige->items[i].owned){
            printf("Name: %s\n", prestigeTree.Prestige->items[i].name);
        }
    }
    printf("--------------------------------\n");
}

int canBuy(char *selectedTree, int index){
    if (index == 0){
        return 1;
    }
    if (strcmp(selectedTree, "Gold") == 0){
        for (int i = 1; i < index; i++){
            //Verifie si l'item precedent est achete
            if (prestigeTree.Gold->items[i].owned == 0){ 
                return 0;
            }
            //Verifie si le joueur a assez de prestige points
            printf("prestigePoints: %d | %d \n", prestigePoints, prestigeTree.Gold->items[index].cost);
            return prestigeTree.Gold->items[index].cost <= prestigePoints;
        }
    }else if (strcmp(selectedTree, "Damage") == 0){
        for (int i = 1; i < index; i++){
            if (prestigeTree.Damage->items[i].owned == 0){
                return 0;
            }
            return prestigeTree.Damage->items[index].cost <= prestigePoints;
        }
    }else if (strcmp(selectedTree, "Prestige") == 0){
      for (int i = 1; i < index; i++){
            if (prestigeTree.Prestige->items[i].owned == 0){
                return 0;
            }
            return prestigeTree.Prestige->items[index].cost <= prestigePoints;
        }
    }
    return 0;
}

void buyPrestigeItem(char *selectedTree, int index){
    if (strcmp(selectedTree, "Gold") == 0){
        if (canBuy(selectedTree, index)){
            prestigePoints -= prestigeTree.Gold->items[index].cost;
            prestigeTree.Gold->items[index].effect(prestigeTree.Gold->items[index].value);
            prestigeTree.Gold->items[index].owned = 1;
        }else{
            printf("Can't buy this item Gold\n");
        }
    }else if (strcmp(selectedTree, "Damage") == 0){
        if (canBuy(selectedTree, index)){
            prestigePoints -= prestigeTree.Damage->items[index].cost;
            prestigeTree.Damage->items[index].effect(prestigeTree.Damage->items[index].value);
            prestigeTree.Damage->items[index].owned = 1;
        }else{
            printf("Can't buy this item\n");
        }
    }else if (strcmp(selectedTree, "Prestige") == 0){
        if (canBuy(selectedTree, index)){
            prestigePoints -= prestigeTree.Prestige->items[index].cost;
            prestigeTree.Prestige->items[index].effect(prestigeTree.Prestige->items[index].value);
            prestigeTree.Prestige->items[index].owned = 1;
        }else{
            printf("Can't buy this item\n");
        }
    }
}