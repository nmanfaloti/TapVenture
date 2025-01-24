#include "boutique.h"
#include <math.h>

double getPriceForLevels(int levels) {
    double base_price = 70; 
    double multiplier = 1.2; 
    double epsilon = 0.01; 
    double price = base_price; 

    for (int level = 0; level < levels; level++) {
        price *= (multiplier + (epsilon * level));
    }

    return price;
}

double getPriceKnowLevel(double previousPrice, int level) {
    double multiplier = 1.2; 
    double epsilon = 0.01; 

    return previousPrice * (multiplier + (epsilon * level));
}

int getDamageFromLevel(int level) {
    return 10 * pow(1.25, level);
}

int upgradeButton(void * args[20]) {
    int * damage = args[0];
    int * gold = args[1];
    damageShop * shop = args[2];

    if (*gold >= shop->nextPrice) {
        *gold -= shop->nextPrice;
        shop->damageLevel += 1;
        shop->lastPrice = shop->nextPrice;
        shop->nextPrice = getPriceKnowLevel(shop->lastPrice, shop->damageLevel + 1);
        *damage = *damage * 1.25;
    }

    return 0;
}
