#ifndef BOUTIQUE_H
#define BOUTIQUE_H

/**
 * @file boutique.h
 * @brief Interface de gestion de la boutique de mise à niveau des dégâts dans TapVenture.
 *
 * Ce fichier définit les constantes, structures et fonctions pour l'initialisation et la gestion
 * de la boutique de mise à niveau, incluant l'augmentation des niveaux de dégâts et le calcul des prix.
 * @author Noam MANFALOTI
 * @date 20 décembre 2024
 */

/**
 * @struct damageShop
 * @brief Structure représentant le système de mise à niveau des dégâts dans la boutique.
 *
 * Cette structure contient le niveau actuel de dégâts et les prix associés aux mises à niveau.
 */
typedef struct {
    int damageLevel;                        /**< Niveau actuel de la mise à niveau des dégâts */
    unsigned long long int lastPrice;         /**< Prix payé pour la dernière mise à niveau */
    unsigned long long int nextPrice;         /**< Prix calculé pour la prochaine mise à niveau */
} damageShop;

extern damageShop shop;

/**
 * @fn void initShop()
 * @brief Initialise la boutique de mise à niveau des dégâts.
 *
 * Configure la boutique en initialisant le niveau de dégâts et en calculant le prix initial des mises à niveau.
 */
void initShop();

/**
 * @fn int upgradeButton()
 * @brief Gère le processus de mise à niveau lors de l'appui sur le bouton d'upgrade.
 *
 * Effectue la mise à niveau en déduisant le prix correspondant, en incrémentant le niveau de mise à niveau,
 * et en mettant à jour les prix de la boutique.
 *
 * @return 0 si la mise à niveau est effectuée avec succès, une valeur non nulle sinon.
 */
int upgradeButton();

/**
 * @fn int getDamageFromLevel(int level)
 * @brief Calcule les dégâts en fonction d'un niveau donné.
 *
 * Applique une formule fixe pour multiplier les dégâts de base selon le niveau spécifié.
 *
 * @param level Le niveau actuel de la mise à niveau.
 *
 * @return La valeur des dégâts calculés.
 */
int getDamageFromLevel(int level);

/**
 * @fn double getPriceForLevels(int levels)
 * @brief Calcule le prix de mise à niveau pour un nombre spécifié de niveaux.
 *
 * Parcourt de manière itérative en partant d'une valeur de base, en appliquant un multiplicateur
 * et en ajustant incrémentalement le prix.
 *
 * @param levels Le nombre de niveaux pour lesquels le prix doit être calculé.
 *
 * @return Le prix calculé sous forme de double.
 */
double getPriceForLevels(int levels);

/**
 * @fn double getPriceKnowLevel(double previousPrice, int level)
 * @brief Calcule le prix de la mise à niveau suivante en fonction du prix précédent et du niveau.
 *
 * Utilise le prix précédent et intègre le niveau à venir dans le calcul d'un nouveau prix en appliquant
 * un multiplicateur et un ajustement epsilon.
 *
 * @param previousPrice Le prix de la mise à niveau précédente.
 * @param level Le niveau à venir pour la mise à niveau.
 *
 * @return Le nouveau prix de mise à niveau calculé.
 */
double getPriceKnowLevel(double previousPrice, int level);

#endif // BOUTIQUE_H