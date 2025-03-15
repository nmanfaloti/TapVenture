typedef struct {
    int damageLevel;
    unsigned long long int lastPrice;
    unsigned long long int nextPrice;
} damageShop;

extern damageShop shop;

void initShop();
int upgradeButton();
int getDamageFromLevel(int level);
double getPriceForLevels(int levels);
double getPriceKnowLevel(double previousPrice, int level);
