#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../lib/chaine.h"
#include "../lib/lang.h"

//concatene 2 chaine et malloc un espace pour les stocker
char * strCatMalloc(const char * srt1, const char * str2){
    if (srt1 == NULL || str2 == NULL) {
        printf("Erreur lors de la concatenation de chaine NULL dans strCatMalloc\n");
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

char * strCatMallocFree1(char * str1, char * str2){ //identique a strCatMalloc mais free() le 1er char *
    char * chaine = strCatMalloc(str1,str2);
    free(str1);
    return chaine;
}

//concatene 1 chaine et 1 caractère puis malloc un espace pour les stocker
char * strCharMalloc(const char * srt1, const char * str2){
    if (srt1 == NULL || str2 == NULL) {
        printf("Erreur lors de la concatenation de chaine NULL dans strCharMalloc\n");
        return NULL;
    }
    char * result = malloc(strlen(srt1) + 1 + 1);
    if (result == NULL) {
        printf("Erreur lors de l'allocation de mémoire dans strCharMalloc %s + %c\n", srt1, *str2);
        return NULL;
    }
    sprintf(result, "%s%c", srt1, *str2);
    return result;
}

char * strCharMallocFree1(char * str1, char * str2){ //identique a strCharMalloc mais free() le 1er char *
    char * chaine = strCharMalloc(str1,str2);
    free(str1);
    return chaine;
}

char * formatChaine(char *chaine,...){ //fonction qui renvoie la chaine formatée avec un espace memoire malloc
    //%t pour mettre une traduction 
    //%w pour les unsigned long long int avec le formatage 
    va_list ap; 
    char * retChaine = malloc(sizeof(char));
    strcpy(retChaine,"");
    char *p = NULL, *s = NULL;
    char buffer[20];
    int ival;
    unsigned long long int lluval;
    double dval;

    va_start(ap, chaine);
    for(p = chaine; *p; p++) {
        if(*p != '%') {
            retChaine = strCharMallocFree1(retChaine,p);
        }
        else{
            switch (*++p){
                case 'd':
                    ival = va_arg(ap, int);
                    sprintf(buffer,"%d",ival);
                    retChaine = strCatMallocFree1(retChaine,buffer);
                    break;
                case 'f':
                    dval  = va_arg(ap, double);
                    sprintf(buffer,"%.2f",dval);
                    retChaine = strCatMallocFree1(retChaine,buffer);
                    break;
                case 's':
                    s = va_arg(ap, char *);
                    retChaine = strCatMallocFree1(retChaine,s);
                    break;
                case 't':
                    s = Traduction(va_arg(ap, int));
                    if(s == NULL){
                        printf("Erreur lors de la traduction dans formatChaine pour \"%s\"\n",chaine);
                    }
                    else{
                        retChaine = strCatMallocFree1(retChaine,s);
                    }
                    break;
                case 'w':
                    lluval = va_arg(ap, unsigned long long int);
                    s = formatULLI(lluval);
                    retChaine = strCatMallocFree1(retChaine,s);
                    free(s);
                    s = NULL;
                    break;
                default:
                    retChaine = strCharMallocFree1(retChaine,p);
            }
        }
    }
    va_end(ap);
    return retChaine;
}


char * formatULLI(unsigned long long int nombre){
    char * buffer = malloc(20);
    if(buffer == NULL){
        printf("Erreur lors de l'allocation de mémoire dans formatULLI\n");
        return NULL;
    }

    if (nombre <= 999 ){
        sprintf(buffer,"%llu",nombre);
        return buffer;
    }

    char letter = 'Q';
    unsigned long long int diviseur = 100000000000000000; //max 20 chiffres pour un unsigned long long int
    for (int i = 0; i < 19; i++){
        if (nombre /100 >= diviseur){
            break;
        }
        letter--;
        diviseur = diviseur / 10;
    }
    nombre = nombre / diviseur;
    sprintf(buffer,"%llu%c",nombre,letter);
    return buffer;
}
