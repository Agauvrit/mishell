/**
 * \file myecho.c
 * \brief commande Echo
 * \author mishell
 * \version 1.0
 * \date 02/01/2016
 *
 * Execution de la commande echo
 *
 */

// Librairies
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#include "myecho.h"

/**
 * \fn int runecho(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande Echo.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runecho(int argc, char *argv[]){

	// Déclaration des variables d'options
	int c,nflag=0,eflag=0,errflag=0;
	int nbfile=argc-1;

	// Chargement des options
	while((c = getopt(argc, argv, "ne")) != -1){
		switch (c)
		{
			case 'e':
				eflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
			case '?':
				errflag=1;
				break;
		}
	}

	if(!eflag){
		for(int i=1;i<=nbfile;i++){
			printf("%s ", argv[i]);
		}
		if(!nflag)
			printf("\n");
	}
	
	// Si il y a une option non permise
	if(errflag){
		printf("Option non permises \n");
		exit(-1);
	}

	return 0;
}

