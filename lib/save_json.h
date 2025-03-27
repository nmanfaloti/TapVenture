#ifndef SAVE_JSON_H
#define SAVE_JSON_H

/**
 * \file save_json.h
 * \brief Déclarations pour la sauvegarde et le chargement des données de TapVenture.
 * 
 * Ce fichier contient les déclarations des fonctions permettant de lire, écrire et modifier
 * les sauvegardes au format JSON pour le jeu TapVenture, incluant la gestion des sauvegardes
 * du joueur, des héros et du prestige.
 * 
 * \author Lucas DUPONT & Ibrahim SAPIEV & Noam MANFALOTI
 * \date 7 décembre 2024
 */

/**
 * \fn char * getValueForKey(char * key, char * nom_ficher)
 * \brief Récupère la valeur associée à une clé dans un fichier de sauvegarde.
 *
 * Cette fonction parcourt le fichier de sauvegarde à la recherche de la clé spécifiée et renvoie
 * la valeur qui lui est associée. La chaîne renvoyée est allouée dynamiquement et doit être libérée par l'appelant.
 *
 * \param key La clé pour laquelle récupérer la valeur.
 * \param nom_ficher Le chemin du fichier de sauvegarde.
 * \return Une chaîne de caractères contenant la valeur associée à la clé, ou NULL en cas d'erreur.
 */
char * getValueForKey(char * key, char * nom_ficher);

/**
 * \fn int ChangeValueForKey(char * key, char * value, char * nom_ficher)
 * \brief Modifie la valeur associée à une clé dans un fichier de sauvegarde.
 *
 * Cette fonction modifie la valeur existante pour la clé spécifiée dans le fichier de sauvegarde.
 *
 * \param key La clé dont la valeur doit être modifiée.
 * \param value La nouvelle valeur à associer à la clé.
 * \param nom_ficher Le chemin du fichier de sauvegarde.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int ChangeValueForKey(char * key, char * value, char * nom_ficher);

/**
 * \fn int createValueForKey(char * key, char * value, char * nom_ficher)
 * \brief Crée une nouvelle paire clé/valeur dans un fichier de sauvegarde.
 *
 * Cette fonction ajoute une nouvelle entrée sous forme de paire clé/valeur dans le fichier de sauvegarde.
 *
 * \param key La clé à créer.
 * \param value La valeur à associer.
 * \param nom_ficher Le chemin du fichier de sauvegarde.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int createValueForKey(char * key, char * value, char * nom_ficher);

/**
 * \fn int isHereFile(char * nameFile)
 * \brief Vérifie l'existence d'un fichier.
 *
 * \param nameFile Le nom du fichier à vérifier.
 * \return 1 si le fichier existe, 0 sinon.
 */
int isHereFile(char * nameFile);

/**
 * \fn int rmFile(char * nameFile)
 * \brief Supprime un fichier.
 *
 * \param nameFile Le nom du fichier à supprimer.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int rmFile(char * nameFile);

/**
 * \fn int makeSave()
 * \brief Crée une sauvegarde globale des données du jeu.
 *
 * Cette fonction génère et enregistre les sauvegardes pour le joueur, les héros et le prestige.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int makeSave();

/**
 * \fn int loadSave()
 * \brief Charge la sauvegarde globale des données du jeu.
 *
 * Cette fonction lit et restaure les sauvegardes pour le joueur, les héros et le prestige.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int loadSave();

/**
 * \fn int loadSavePlayer(char * save)
 * \brief Charge la sauvegarde des données du joueur.
 *
 * \param save Le chemin du fichier de sauvegarde du joueur.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int loadSavePlayer(char * save);

/**
 * \fn int makeSavePlayer(char * save)
 * \brief Crée la sauvegarde des données du joueur.
 *
 * \param save Le chemin du fichier de sauvegarde du joueur.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int makeSavePlayer(char * save);

/**
 * \fn int loadSaveHeros(char * save)
 * \brief Charge la sauvegarde des héros.
 *
 * \param save Le chemin du fichier de sauvegarde des héros.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int loadSaveHeros(char * save);

/**
 * \fn int makeSaveHeros(char * save)
 * \brief Crée la sauvegarde des données des héros.
 *
 * \param save Le chemin du fichier de sauvegarde des héros.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int makeSaveHeros(char * save);

/**
 * \fn int loadSavePrestige(char * save)
 * \brief Charge la sauvegarde des données de prestige.
 *
 * \param save Le chemin du fichier de sauvegarde du prestige.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int loadSavePrestige(char * save);

/**
 * \fn int makeSavePrestige(char * save)
 * \brief Crée la sauvegarde des données de prestige.
 *
 * \param save Le chemin du fichier de sauvegarde du prestige.
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int makeSavePrestige(char * save);

/**
 * \fn int initPlayer()
 * \brief Initialise les données par défaut du joueur.
 *
 * Cette fonction initialise les paramètres de base du joueur, notamment la langue, le niveau, l'or
 * et les dégâts pour un clic, afin de démarrer ou réinitialiser une partie.
 *
 * \return 0 en cas de succès, une valeur non nulle sinon.
 */
int initPlayer();

#endif // SAVE_JSON_H