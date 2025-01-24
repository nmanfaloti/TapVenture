typedef struct {
    int damageLevel;
    int lastPrice;
    int nextPrice;
} damageShop;

int upgradeButton(void * args[20]);
int getDamageFromLevel(int level);
double getPriceForLevels(int levels);
double getPriceKnowLevel(double previousPrice, int level);