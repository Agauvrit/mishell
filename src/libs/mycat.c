/**
 * \file mycat.c
 * \brief commande Cat
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Execution de la commande cat
 *
 */

// Librairies
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "mycat.h"

int lecture_stdin(){

	int res;
	char c;
	res=read(STDIN_FILENO,&c,1);
	while(res > 0){
		printf("%c",c); 
		res = read(STDIN_FILENO,&c,1);
	}
	return 0;
}

/**
 * \fn int runcat(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande Cat.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runcat(int argc, char *argv[]){

	// Déclaration des variables d'options
	int c,nflag=0,errflag=0,opt=0,cpt=0,first=0,bflag=0;

	// Fichier et buffer pour la lecture du fichier
	FILE *stream=NULL;
	char chaine[1024];

	// Chargement des options
	while((c = getopt(argc, argv, "bn")) != -1){
		switch (c)
		{
			case 'n':
				opt++;
				nflag = 1;
				break;
			case'b':
				opt++;
				bflag = 1;
			case '?':
				errflag=1;
				break;
		}
	}

	// Gestion des options non permises
	if(errflag){
		printf(" Options non permises \n");
		exit(-1);
	}

	// Si il n'y a aucun fichier a lire
	if(argc<2){
		lecture_stdin();
	}

	// L'option -b override l'option n
	if(bflag)
		nflag=0;

	// Application de la méthode sur chacun des fichiers
	for(int i=opt+1;i<argc;i++){
		stream=fopen(argv[i],"r");

		// Gestion de l'affichage
		if(!first)
			first = 1;
		else
			printf("\n----------------------------------------------------------------------------\n");

		// Si le fichier est bien ouvert
		if(stream){
			// Récupération ligne par ligne du texte contenu dans le fichier
			while(fgets(chaine, sizeof chaine, stream)){
				if(nflag){
					printf("%d  ",cpt);
					printf("%s ",chaine);
					cpt++;
				}
				else if (bflag){
					if(chaine[0]=='\n');
						//printf("\n");
					else{
						printf("%d  ",cpt);
						printf("%s ",chaine);
						cpt++;
					}
				}
				else{
					if(chaine[0]!='\n')
						printf("%s \n",chaine);
					else
						printf("\n");
				}
			}
			cpt=0;
			// Fermeture du fichier
			fclose(stream);
		}
		else
			printf("Erreur lors de l'ouverture du fichier\n");
	}
	return 0;
}
