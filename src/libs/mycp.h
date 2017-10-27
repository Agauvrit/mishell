/**
 * \file mycp.h
 * \brief Header commande CP
 * \author mishell
 * \version 1.0
 * \date 10/04/2016
 *
 * Header de la commande cp
 *
 */

// Prototypes
bool estRep3(mode_t mode);
void newRep(char *lien);
int File_to_file(char *src, char *dest);
int File_to_rep(char *src_file, char *dest_dir);
int Files_to_rep(int argc, char *argv[]);
int Rep_to_rep(char *dir_path_src, char *dir_path_dest);
int Reps_to_rep(int argc, char *argv[]);
int runcp(int argc, char *argv[]);
