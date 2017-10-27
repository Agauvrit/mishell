/**
 * \file mychgrp.c
 * \brief commande CHGRP
 * \author mishell
 * \version 1.0
 * \date 10/04/2016
 *
 * Execution de la commande chgrp
 *
 */

//Librairies
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>

#include "mychgrp.h"

/**
 * \fn recur1(char * fichier, int uid, int gid, int fflag, int vflag, char * newGrp)
 * \brief Fonction pour la recursivité de la commande chown.
 *
 * \param fichier : nom du fichier ou dossier.
 * \param uid : l'IUD du dossier/fichier
 * \param gid : le nouveau GID
 * \param fflag : valeur du flag -f
 * \param vflag: valeur du flag -v
 * \param newGrp : nouveau nom du groupe
 */
void recur1(char * fichier, int uid, int gid, int fflag, int vflag, char * newGrp){
	char str[50];

	//Ouverture du fichier/dossier
	DIR *dirp = opendir(fichier);
		if (dirp != NULL){
        	struct dirent * ent;
			struct stat file_stat;

			//Parcours des éléments du dossier/fichier
			while ((ent = readdir(dirp)) != NULL){
				stat(ent->d_name, &file_stat);

				//On ne prend pas en compte "." et ".."
				if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
            		continue;
        		}

				//Stockage du chemin du sous-fichier/sous-dossier
				strcpy(str, fichier);
				strcat(str, "/");
				strcat(str, ent->d_name);

				//Gestion des options
				if(fflag){
					chown(fichier, uid, gid);
				}else if(vflag && !fflag){
					if (chown(fichier, uid, gid) == 0) {
      					printf("chgrp: groupe correctement change: \"%s\" \n",newGrp);
		   			}else{
						printf("chgrp: groupe incorrect: \"%s\"\n",newGrp);
					}
				}else if(!vflag && !fflag){
					if (chown(fichier, uid, gid) == -1) {
						printf("chgrp: groupe incorrect: \"%s\"\n",newGrp);
					}
				}

				
				//Vérification du type du dossier
				if(ent->d_type == DT_DIR){
					recur1(str, uid, gid, fflag, vflag, newGrp);
				}
        	}
        	closedir(dirp);
    	}
}

/**
 * \fn int runchgrp(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande chgrp.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runchgrp(int argc, char *argv[]){

	// variable pour gérer les options
	int c,errflag=0,fflag=0,vflag=0,rflag=0,opt=2,i;
	
	// Variable pour récupérer les informations d'un dossier
	DIR *dirp;
	uid_t uid;
	gid_t gid;
  	struct passwd *pw, *p;
	struct dirent *dptr;
	struct stat sb;
   	char *str;

	// Chargement des options
	while((c = getopt(argc, argv, "fvr")) != -1){
		switch (c)
		{
			case 'f':
				fflag = 1;
				opt++;
				break;
			case 'v':
				vflag = 1;
				opt++;
				break;
			case 'r':
				rflag = 1;
				opt++;
				break;
			case '?':
				errflag=1;
				break;
		}
	}
	
	// Gestion des options non admises
	if(errflag){
		printf("Erreur au niveau des options\n");
		exit(-1);
	}

	// Si il n'y a aucun fichier a lire
	if((argc-opt) == 0){
		printf("Il n'y a pas de fichier a lire\n");
		exit(-1);
	}
	
	str = argv[opt-1];

	for(i = 1; i<(argc-opt)+1;i++){
		printf("fichier : %s\n", argv[argc-i]);
		//Ouverture du fichier/dossier
		dirp = opendir(argv[argc-i]);
		if (dirp != NULL){
			dptr=readdir(dirp);
			stat(dptr->d_name, &sb);
		
			// Récupérer l'uid du fichier/dossier 
			if((pw=getpwuid(sb.st_uid))!=NULL) uid = pw->pw_uid;
			else exit(-1);
			
			// Récupération du nouveau gid
			if((p = getpwnam(str))!=NULL) gid = p->pw_gid;
			else exit(-1);

			// Gestion des options
			if(fflag){
				chown(argv[argc-i], uid, gid);
			}else if(vflag && !fflag){
				
				if (chown(argv[argc-i], uid, gid) == 0) {
		 			printf("chgrp: groupe correctement change: \"%s\"\n", str);
				}else{
					printf("chgrp: groupe incorrect: \"%s\"\n", str);
				}
			}else{
				if (chown(argv[argc-i], uid, gid) == -1) {
					printf("chgrp: groupe incorrect: \"%s\"\n", str);
				}
			}
	
			// Si il y a l'option -r, on applique la commande sur tout les sous-fichier/sous-dossier
			if(rflag){
				recur1(argv[argc-i], uid, gid, fflag, vflag, str);
			}	
		}
	}
    
 	return 0;
}


