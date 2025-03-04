#ifndef CHAINE_H
#define CHAINE_H

char * strCatMalloc(const char * srt1, const char * str2);
char * strCatMallocFree1(char * str1, char * str2);

char * strCharMalloc(const char * srt1, const char * str2);
char * strCharMallocFree1(char * str1, char * str2);

char * formatChaine(char *chaine,...);

char * formatULLI(unsigned long long int nombre);

#endif // CHAINE_H