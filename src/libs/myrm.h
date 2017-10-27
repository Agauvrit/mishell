/**
 * \file myrm.h
 * \brief Header commande RM
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Header de la commande rm
 *
 */

 //Prototype 
int demandeChar(char *path);
int delDirectoryEmpty(char *dir, int vflag,int fflag);
bool estLienDur(char *file);
bool estLien(mode_t mode);
bool estRep(mode_t mode);
int delFile(char *path, int vflag, int fflag);
int delDirectory(char *path,int vflag, int fflag);
int delElem(char *dir, char* name,int vflag, int fflag);
int runrm(int argc, char *argv[]);


