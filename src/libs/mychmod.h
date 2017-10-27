/**
 * \file mychmod.h
 * \brief Header commande CHMOD
 * \author mishell
 * \version 1.0
 * \date 05/04/2016
 *
 * Fichier header de la commande CHMOD
 *
 */

//Prototypes
int octal_decimal(int n);
void recur2(char * nom, int mode, int fflag, int vflag, char *perm);
int runchmod(int argc, char *argv[]);
