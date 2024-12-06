#include <stdio.h>
#include "traduction/lang.h"

int main() {
    SelectLanguage(en);
    printf("%s\n", Traduction(BIENVENUE_MSG));
    printf("%s\n", Traduction(AUREVOIR_MSG));
    
    SelectLanguage(fr);
    printf("%s\n", Traduction(BIENVENUE_MSG));
    printf("%s\n", Traduction(AUREVOIR_MSG));
    
    return 0;
}