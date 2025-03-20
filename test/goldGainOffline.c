#include "goldGainOffline.h"

/* Variables globales utilisées pour les tests */
unsigned long long int testGold = 0;
unsigned long long int notifGold = 0;
int notifCalled = 0;
unsigned long long int testGoldRate = 100; // Valeur de retour simulée pour herosGoldGenBySec

/* Temps actuel faux pour surcharger time() */
time_t fakeCurrentTime = 0;
time_t time(time_t *t) {
    if(t) *t = fakeCurrentTime;
    return fakeCurrentTime;
}

/* Stub pour herosGoldGenBySec */
unsigned long long int herosGoldGenBySec(void){
    return testGoldRate;
}

/* Stub pour addGold : incrémente la valeur globale de l'or */
void addGold(unsigned long long int gold){
    testGold += gold;
}

/* Stub pour formatChaine : alloue et retourne une chaîne formatée */
char * formatChaine(const char *fmt, const char *str, unsigned long long int val){
    char * buffer = malloc(100);
    snprintf(buffer, 100, fmt, str, val);
    return buffer;
}

/* Stub factice pour getRectForCentenredCord */
void * getRectForCentenredCord(int x, int y, int width, int height){
    return NULL;
}

/* Stubs factices pour vw et vh */
int vw(int percent){
    return percent;
}

int vh(int percent){
    return percent;
}

/* Stub pour createNotif : enregistre qu'une notification a été créée ainsi que son paramètre or */
void createNotif(const char *title, int a, int b, const char *imgPath, int c, int d, void *rect, int e, int f, const char *msg){
    notifCalled = 1;
    // Extraction de la quantité d'or depuis msg
    // On suppose que msg contient "Test <valeur>" donc on le parse.
    const char *space = strchr(msg, ' ');
    if (space)
        notifGold = strtoull(space+1, NULL, 10);
}

/* La fonction à tester */
void goldGainOffline(time_t lastSaveTime){
    time_t currentTime = time(NULL);
    if (lastSaveTime <= 0 || currentTime < lastSaveTime){
        return;
    }
    double elapsedTime = difftime(currentTime, lastSaveTime);
    unsigned long long int goldBySec = herosGoldGenBySec();
    unsigned long long int goldEarned = (elapsedTime * goldBySec);
    addGold((goldEarned/10));
    if (goldEarned != 0){
        char * goldEarnedMsg = formatChaine("%s %llu","Test", goldEarned/10);
        createNotif("test",0,1,"assets/ui/notif.png", 1, 3,
                    getRectForCentenredCord(vw(50), vh(30), vw(40), vh(40)),
                    0, 1, goldEarnedMsg);
        free(goldEarnedMsg);
    }
}