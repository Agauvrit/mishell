/**
 * \file mymkdir.c
 * \brief commande Mkdir
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Execution de la commande ls
 *
 */

// Librairies 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "mymkdir.h"

/**
 * \fn int octal_decimal1(int n)
 * \brief Fonction transformation otcal en décimal
 *
 * \param n Nombre en octal à convertir en décimal
 * \return valeur du nombre octal en décimal
 */
int octal_decimal1(int n) 
{
    int decimal=0, i=0, rem;
    while (n!=0)
    {
        rem = n%10;
        n/=10;
        decimal += rem*pow(8,i);
        ++i;
    }
    return decimal;
}


/**
 * \fn int runmkdir(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande Mkdir.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runmkdir(int argc, char *argv[]){


	// Variable concernant les options
	extern char * optarg; 
	char *mvalue = NULL;
	int c,mflag=0,vflag=0,errflag=0,permission=0000,opt=0;
	
	// Chargement des options
	while((c = getopt(argc, argv, "m:v")) != -1){
		switch (c)
		{
			case 'm':
				opt++;
				mflag = 1;
				mvalue = optarg;
				break;
			case 'v':
				opt++;
				vflag = 1;
				break;
			case '?':
				errflag=1;
				break;
		}
	}
	
	// gestion d'option non acceptables
	if(errflag){
		printf("Options non permises \n");
		exit(-1);
	}

	for(int i=opt+1;i<argc;i++){
		// Si les permissions sont données
		if (mflag){
			permission = atoi(mvalue);
			// Transformation des permissions
			octal_decimal1(permission);
			if(vflag){
				if ((mkdir(argv[i],permission)) == 0)
					printf("Le dossier \"%s\" à bien été créé \n",argv[i]);
				else{
					if(errno!=ENOENT)
						printf("Erreur lors de la création du répertoire. Numéro d'erreur : %d \n",errno);
					else
						printf("mkdir: impossible de créer le répertoire \"%s\" : Aucun fichier ou dossier de ce type ",argv[i]);
				}
			}
			else{
				if ((mkdir(argv[i],permission)) == -1)
					printf("Erreur lors de la création du répertoire. Numéro d'erreur : %d \n",errno);
				else {
					if(errno!=ENOENT)
						printf("Erreur lors de la création du répertoire. Numéro d'erreur : %d \n",errno);
					else
						printf("mkdir: impossible de créer le répertoire \"%s\" : Aucun fichier ou dossier de ce type ",argv[i]);
				}
			}	
		}
		else{
			if(vflag){
				// par défaut on met les permissions à 0755
				if ((mkdir(argv[i],0755)) == 0)
					printf("Le dossier \"%s\" à bien été créé \n",argv[i]);
				else {
					if(errno!=ENOENT)
						printf("Erreur lors de la création du répertoire. Numéro d'erreur : %d \n",errno);
					else
						printf("mkdir: impossible de créer le répertoire \"%s\" : Aucun fichier ou dossier de ce type ",argv[i]);
				}
			}
			else{
				if ((mkdir(argv[i],0755)) == -1){
					if(errno==EEXIST)
						printf("Le dossier existe déja dans ce répertoire\n");
					else{
						if(errno!=ENOENT)
							printf("Erreur lors de la création du répertoire. Numéro d'erreur : %d \n",errno);
						else
							printf("mkdir: impossible de créer le répertoire \"%s\" : Aucun fichier ou dossier de ce type ",argv[i]);
					}
				}
			}
		}
	}
	return 0;
}
