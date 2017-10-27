/**
 * \file mychown.h
 * \brief Header commande CHOWN
 * \author mishell
 * \version 1.0
 * \date 05/04/2016
 *
 * Fichier header de la commande CHOWN
 *
 */

//Prototypes
void recur(char * fichier, int uid, int gid, int fflag, int vflag, int user, char * newGrpUser);
int runchown(int argc, char *argv[]);

