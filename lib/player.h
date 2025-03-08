#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>

extern unsigned long long int gold;
extern unsigned long long int damage_click;
extern char username[50];

void goldGainOffline(time_t lastSaveTime);
void addGold(unsigned long long int goldToAdd);
void setPlayerDamage(unsigned long long int damage);

#endif // PLAYER_H