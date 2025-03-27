#ifndef LANG_H
#define LANG_H

/**
 * \file lang.h
 * \brief Déclarations pour la gestion du système de langues dans TapVenture.
 * 
 * Ce fichier contient les définitions et déclarations nécessaires pour la gestion des traductions 
 * dans le jeu TapVenture, incluant l'énumération des clés de messages, la structure Lang et les fonctions associées.
 * \author Noam MANFALOTI & Lucas DUPONT & Ibrahim SAPIEV
 * \date 6 décembre 2024
 */

/**
 * \enum CleMsg
 * \brief Énumération des clés de messages pour la traduction.
 *
 * Cette énumération regroupe toutes les clés de messages utilisés dans TapVenture.
 */
typedef enum {
    BIENVENUE_MSG,               /*!< Message de bienvenue. */
    AUREVOIR_MSG,                /*!< Message d'au revoir. */
    CLICK_MSG,                   /*!< Message d'action de clic. */
    VIE_MSG,                     /*!< Message relatif aux points de vie. */
    OR_MSG,                      /*!< Message relatif à l'or. */
    DMG_MSG,                     /*!< Message relatif aux dégâts. */
    UPG_MSG,                     /*!< Message pour l'amélioration des dégâts. */
    MOB_MSG,                     /*!< Message relatif aux monstres. */
    LVL_MSG,                     /*!< Message relatif aux niveaux. */
    SETTING_MSG,                 /*!< Message relatif aux paramètres de configuration. */
    FR_MSG,                      /*!< Libellé pour la langue française. */
    EN_MSG,                      /*!< Libellé pour la langue anglaise. */
    WELCOME_MSG,                 /*!< Message de bienvenue en anglais. */
    WELCOME_DESC_MSG,            /*!< Description de bienvenue. */
    GOLD_OFFLINE_MSG,            /*!< Message indiquant l'or gagné hors ligne. */
    GOLD_OFFLINE_DESC_MSG,       /*!< Description de l'or gagné hors ligne. */
    FULLSCREEN_MSG,              /*!< Message pour activer le mode plein écran. */
    WINDOWED_MSG,                /*!< Message pour passer en mode fenêtré. */
    CHALLENGE_MSG_WIN,           /*!< Message affiché lors de la réussite d'un challenge. */
    CHALLENGE_MSG_LOSE,          /*!< Message affiché lors de l'échec d'un challenge. */
    CHALLENGE_MSG_COOLDOWN,      /*!< Message indiquant le temps de recharge d'un challenge. */
    TIMER_MSG,                   /*!< Message relatif à l'affichage du timer. */
    NOTIF_TITLE,                 /*!< Titre utilisé pour les notifications. */
    PRESTIGE_MSG,                /*!< Message relatif au prestige. */
    PRESTIGE_COST,               /*!< Message indiquant le coût du prestige. */
    PRESTIGE_GOLD_DESC,          /*!< Description de l'augmentation d'or via le prestige. */
    PRESTIGE_DAMAGE_DESC,        /*!< Description de l'augmentation des dégâts via le prestige. */
    PRESTIGE_PRESTIGE_DESC,      /*!< Description de l'augmentation des points de prestige. */
    PRESTIGE_HERO_DESC,          /*!< Description concernant le maintien du héros lors du prestige. */
    PRESTIGE_HEROLVL_DESC,       /*!< Description concernant le maintien du niveau du héros lors du prestige. */
    PRESTIGE_CONGRATS,           /*!< Message de félicitations après un prestige. */
    HERO_SHOP_TITLE_MSG,         /*!< Titre de la boutique des héros. */
    PRICE_MSG,                   /*!< Message pour l'affichage du prix. */
    HERO_SHOP_MULTIPLICATOR_MSG, /*!< Message relatif au multiplicateur dans la boutique des héros. */
    DPS_MSG,                   /*!< Message pour l'affichage des dégâts par seconde. */
    MAX_MSG,                   /*!< Message indiquant la valeur maximale. */
    NB_MSG                     /*!< Nombre total de messages. */
} CleMsg;

/**
 * \struct Lang
 * \brief Structure représentant une langue et ses messages associés.
 *
 * Cette structure contient le nom de la langue et un tableau de chaînes de caractères correspondant 
 * aux différents messages traduits utilisés dans le jeu.
 */
typedef struct {
    const char *Language;         /*!< Nom de la langue. */
    const char *messages[NB_MSG]; /*!< Tableau contenant les messages traduits. */
} Lang;

/**
 * \brief Langue active utilisée dans TapVenture.
 */
extern Lang LanguageAct;

/**
 * \fn char* Traduction(CleMsg key)
 * \brief Récupère la traduction associée à une clé de message.
 *
 * Cette fonction renvoie la chaîne de caractères correspondant à la clé de message passée en paramètre selon la langue active.
 *
 * \param key Clé de message pour laquelle la traduction est demandée.
 * \return Une chaîne de caractères contenant la traduction correspondante.
 */
char* Traduction(CleMsg key);

/**
 * \fn int SelectLanguage(void * l[20])
 * \brief Sélectionne la langue à utiliser.
 *
 * Cette fonction permet de changer la langue active en fonction des paramètres fournis.
 * 
 * \param l Tableau de pointeurs utilisé pour passer les paramètres de sélection.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int SelectLanguage(void * l[20]);

/**
 * \fn void initLang(char * lang)
 * \brief Initialise la langue utilisée par le jeu.
 *
 * Cette fonction initialise la langue active en fonction du nom de la langue fourni.
 * Si la langue spécifiée n'est pas reconnue, la langue anglaise est sélectionnée par défaut.
 *
 * \param lang Nom de la langue à initialiser (par exemple, "French" ou "English").
 */
void initLang(char * lang);

#endif // LANG_H