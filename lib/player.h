#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>

extern int gold;
extern int damage_click;
extern char username[50];

void goldGainOffline(time_t lastSaveTime);

#endif // PLAYER_H