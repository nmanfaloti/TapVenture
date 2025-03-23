#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>

#define LLD_MAX 802379605485813759

extern unsigned long long int gold;
extern unsigned long long int damage_click;

void goldGainOffline(time_t lastSaveTime);
void addGold(long long int goldToAdd);
void setPlayerDamage(unsigned long long int damage);

#endif // PLAYER_H