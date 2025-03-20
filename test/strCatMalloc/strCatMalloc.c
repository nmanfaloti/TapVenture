#include "strCatMalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//concatene 2 chaine et malloc un espace pour les stocker
char * strCatMalloc(const char * srt1, const char * str2){
    if (srt1 == NULL || str2 == NULL) {
        printf("Erreur lors de la concatenation de chaine NULL\n");
        return NULL;
    }
    char * result = malloc(strlen(srt1) + strlen(str2) + 1);
    if (result == NULL) {
        printf("Erreur lors de l'allocation de mémoire dans strCatMalloc %s + %s\n", srt1, str2);
        return NULL;
    }
    sprintf(result, "%s%s", srt1, str2);
    return result;
}