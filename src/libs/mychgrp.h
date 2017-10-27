/**
 * \file mychgrp.h
 * \brief Header commande CHGRP
 * \author mishell
 * \version 1.0
 * \date 10/04/2016
 *
 * Fichier header de la commande CHGRP
 *
 */

//Prototypes
void recur1(char * fichier, int uid, int gid, int fflag, int vflag, char * newGrp);
int runchgrp(int argc, char *argv[]);
