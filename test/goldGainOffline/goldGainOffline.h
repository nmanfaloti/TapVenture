#ifndef GOLDGAINOFFLINE_H
#define GOLDGAINOFFLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Variables globales utilisées pour les tests */
extern unsigned long long int testGold;
extern unsigned long long int notifGold;
extern int notifCalled;
extern unsigned long long int testGoldRate;

/* Temps actuel faux pour surcharger time() */
extern time_t fakeCurrentTime;
time_t time(time_t *t);

/* Stub pour herosGoldGenBySec */
unsigned long long int herosGoldGenBySec(void);

/* Stub pour addGold : incrémente la valeur globale de l'or */
void addGold(unsigned long long int gold);

/* Stub pour formatChaine : alloue et retourne une chaîne formatée */
char * formatChaine(const char *fmt, const char *str, unsigned long long int val);

/* Stub factice pour getRectForCentenredCord */
void * getRectForCentenredCord(int x, int y, int width, int height);

/* Stubs factices pour vw et vh */
int vw(int percent);
int vh(int percent);

/* Stub pour createNotif : enregistre qu'une notification a été créée ainsi que son paramètre or */
void createNotif(const char *title, int a, int b, const char *imgPath, int c, int d, void *rect, int e, int f, const char *msg);

/* La fonction à tester */
void goldGainOffline(time_t lastSaveTime);

#endif /* GOLDGAINOFFLINE_H */