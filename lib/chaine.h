#ifndef CHAINE_H
#define CHAINE_H

/**
 * \file chaine.h
 * \brief Interface de gestion des chaînes de caractères dans TapVenture.
 *
 * Ce fichier définit les constantes, types et fonctions pour l'initialisation et la gestion
 * des chaînes de caractères utilisées dans TapVenture, incluant la concaténation, le formatage
 * et la conversion de nombres.
 * \author Lucas DUPONT
 * \date 6 mars 2025
 */


/**
 * \fn char * strCatMalloc(const char * srt1, const char * str2)
 * \brief Alloue dynamiquement et renvoie une chaîne résultant de la concaténation de srt1 et str2.
 * \param srt1 Chaîne de caractères source #1.
 * \param str2 Chaîne de caractères source #2.
 * \return Pointeur vers la nouvelle chaîne concaténée. L'appelant doit libérer cette mémoire.
 */
char * strCatMalloc(const char * srt1, const char * str2);

/**
 * \fn char * strCatMallocFree1(char * str1, char * str2)
 * \brief Concatène une chaînes et un caractère et libère la première après la concaténation.
 * \param str1 Chaîne de caractères dont la mémoire sera libérée.
 * \param str2 Chaîne de caractères à concaténer.
 * \return Pointeur vers la nouvelle chaîne concaténée. L'appelant doit libérer cette mémoire.
 */
char * strCatMallocFree1(char * str1, char * str2);

/**
 * \fn char * strCharMalloc(const char * srt1, const char * str2)
 * \brief Alloue dynamiquement et renvoie une chaîne qui combine srt1 et str2 (fonctionnalité similaire à strCatMalloc).
 * \param srt1 Chaîne de caractères source #1.
 * \param str2 Un caractères source #2.
 * \return Pointeur vers la nouvelle chaîne combinée. L'appelant doit libérer cette mémoire.
 */
char * strCharMalloc(const char * srt1, const char * str2);

/**
 * \fn char * strCharMallocFree1(char * str1, char * str2)
 * \brief Concatène une chaînes et un caractère et libère la première chaîne après la concaténation.
 * \param str1 Chaîne de caractères dont la mémoire sera libérée.
 * \param str2 Un caractères à concaténer.
 * \return Pointeur vers la nouvelle chaîne résultante. L'appelant doit libérer cette mémoire.
 */
char * strCharMallocFree1(char * str1, char * str2);

/**
 * \fn char * formatChaine(char * chaine, ...)
 * \brief Formate une chaîne de caractères en utilisant des arguments variables.
 * \param chaine Chaîne de caractères initiale.
 * \param ... Liste d'arguments variables pour le formatage.
 * \return Pointeur vers la chaîne formatée. L'appelant doit libérer cette mémoire.
 */
char * formatChaine(char * chaine, ...);

/**
 * \fn char * formatULLI(unsigned long long int nombre)
 * \brief Convertit un nombre unsigned long long int en une chaîne de caractères.
 * \param nombre Nombre à convertir.
 * \return Pointeur vers la chaîne représentant le nombre. L'appelant doit libérer cette mémoire.
 */
char * formatULLI(unsigned long long int nombre);

#endif // CHAINE_H