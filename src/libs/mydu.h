/**
 * \file mydu.h
 * \brief Header commande DU
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Header de la commande du
 *
 */

// Prototypes 
bool estLienDur1(char *file);
bool estLien1(mode_t mode);
bool estRep1(mode_t mode);
int sizeElement(char *dir, char *name, int sflag);
int sizeDirContent(char *path, int sflag);
int rundu(int argc, char *argv[]);