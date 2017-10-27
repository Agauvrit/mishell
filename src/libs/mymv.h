/**
 * \file mymv.h
 * \brief Header commande MV
 * \author mishell
 * \version 1.0
 * \date 03/04/2016
 *
 * Header de la commande mv
 *
 */

//Prototype 
int replaceFile(char *src, char *dest, int vflag, int nflag, int iflag, int fflag);
char* ajoutChaine(const char *src, char d);
char* mystrcat(char *src, char* dest);
int demandeChar1(char *path);
bool estLienDur2(char *file);
bool estRep2(mode_t mode);
int replaceFile(char *src, char *dest, int vflag, int nflag, int iflag, int fflag);
int replaceRep(char *src, char *dest, int vflag, int nflag, int iflag, int fflag);
int replaceElem(char *src, char *dest, int vflag, int nflag, int iflag, int fflag);
int runmv(int argc, char *argv[]);


