/**
 * \file mychmod.c
 * \brief commande CHMOD
 * \author mishell
 * \version 1.0
 * \date 05/04/2016
 *
 * Execution de la commande chmod
 *
 */

//Librairies
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "mychmod.h"


/**
 * \fn recur2(char * nom, int mode, int fflag, int vflag, char *perm)
 * \brief Fonction pour la recursivité de la commande chmod.
 *
 * \param nom : nom du fichier ou dossier.
 * \param mode : valeur en octal des nouveaux droits du fichier/dossier
 * \param fflag : valeur du flag -f
 * \param vflag : valeur du flag -v
 * \param perm : valeur en decimal des nouveaux droits du fichier/dossier pour l'affichage
 */
void recur2(char * nom, int mode, int fflag, int vflag, char *perm){
	char src[50];

	//Ouverture du fichier/dossier
	DIR *dirp = opendir(nom);
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
				strcpy(src, nom);
				strcat(src, "/");
				strcat(src, ent->d_name);

				//Gestion des options
				if(fflag){
					chmod(src, mode);
				}else if(vflag && !fflag){
					if(chmod(src, mode)==0){
						printf("Le mode de \"%s\" a été modifié en %s\n", ent->d_name, perm);	
					}else{
				 		printf("mode incorrect : \"%s\"\n", perm);
						exit(-1);
					}
				}else{
					if(chmod(src, mode)==-1){
 						printf("mode incorrect : \"%s\"\n", perm);
						exit(-1);
					}
				}

				//Vérification du type du dossier
				if(ent->d_type == DT_DIR){
					recur2(src, mode, fflag, vflag, perm);
				}
        	}
        	closedir(dirp);
    	}else{
			printf("Erreur lors de l'ouverture de dossier. Numero d'erreur : %d \n",errno);
			exit(-1);
		}

}

/**
 * \fn int octal_decimal(int n)
 * \brief Fonction qui met une valeur décimal en octal.
 *
 * \param n valeur décimal.
 * \return valeur décimal en octal.
 */
int octal_decimal(int n){
    int decimal=0, i=0, rem;
    while (n!=0){
        rem = n%10;
        n/=10;
        decimal += rem*pow(8,i);
        ++i;
    }
    return decimal;
}

/**
 * \fn int runchmod(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande chmod.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runchmod(int argc, char *argv[]){

	// variable pour gérer les options
	int c,errflag=0,fflag=0,vflag=0,rflag=0,opt=1,i;

	// variable pour gérer la récupération du mode et de la mettre dans la forme souhaité
	long arg;
	int mode;
	char *ptr;
	
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

	// Récupération de l'argument en octal
	arg = strtol(argv[opt], &ptr, 10);
	mode = octal_decimal(arg);
	
	printf("mode : %d\n", mode);
	// Gestion des options
	for(i = 1; i<(argc-opt);i++){
		if(fflag){
			chmod(argv[argc-i], mode);
		}else if(vflag && !fflag){
			if(chmod(argv[argc-i], mode)==0){
				printf("Le mode de \"%s\" a été modifié en %s\n", argv[argc-i], argv[opt]);	
			}else{
				printf("mode incorrect : \"%s\"\n", argv[opt]);	
				exit(-1);
			}
		}else{
			if(chmod(argv[argc-i], mode)==-1){
				printf("mode incorrect : \"%s\"\n", argv[opt]);
				exit(-1);
			}
		}
		// Si il y a l'option -r, on applique la commande sur tout les sous-fichier/sous-dossier
		if(rflag) recur2(argv[argc-i], mode, fflag, vflag, argv[opt]);
	}

	return 0;
}
