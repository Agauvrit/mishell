/**
 * \file mymore.c
 * \brief commande More
 * \author mishell
 * \version 1.0
 * \date 10/04/2016
 *
 * Execution de la commande more
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

#include "mymore.h"

/**
 * \fn int demandeChar2()
 * \brief Fonction demande de saut de ligne ou quitter.
 *
 * \return valeur pour savoir si l'utilisateur veut passer a la ligne suivante ou quitter
 */
int demandeChar2(){
	
	char c;

	c=getchar();

	// Si l'utilisateur veut passer a la ligne suivante
	if(c=='\n')
		return 2;
	// Sinon si il veut quitter
	else if(c=='q')
		return 1;
	
	else
		return demandeChar2();
	
	
}

/**
 * \fn int runmore(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande More.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runmore(int argc, char *argv[]){

	// Déclaration des variables d'options
	int c,errflag=0,first=0,cpt,yes,fin=0;

	// Fichier et buffer pour la lecture du fichier
	FILE *stream=NULL;
	char chaine[1024];

	// Chargement des options
	while((c = getopt(argc, argv, "")) != -1){
		switch (c)
		{
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
		printf("Il n'y a pas de fichier a lire\n");
		exit(-1);
	}

	cpt=0;

	// Application de la méthode sur chacun des fichiers
	for(int i=1;i<argc;i++){
		
		stream=fopen(argv[i],"r");

		// Gestion de l'affichage
		if(!first){
			printf("Fichier [%s]\n\n",argv[i]);
			first = 1;
		}
		else
			printf("\n----------------------------------------------------------------------------\nFichier [%s] \n\n",argv[i]);

		// Si le fichier est bien ouvert
		if(stream){
			// Récupération ligne par ligne du texte contenu dans le fichier
			while(fgets(chaine, sizeof chaine, stream)){
				if(cpt<22){
					printf("%s",chaine);
				}
				else{
					// Recuperation de la demande utilisateur
					yes=demandeChar2();
					// Si il souhaite passer a la ligne suivante
					if(yes==2){
						if(chaine[0]!='\n')
							printf("%s",chaine);
					}
					// Si l'utilisateur veut quitter
					else if(yes==1){
						fin=1;
						break;
					}
					else
						printf("Erreur lors de la reception demande ce charactère ! \n");
				}
				
				cpt++;
			}
			// Fermeture du fichier
			fclose(stream);
			if(fin)
				break;
		}
		else
			printf("Erreur lors de l'ouverture du fichier \"%s\" \n",argv[i]);
	}
	printf("\n");
	return 0;
}



