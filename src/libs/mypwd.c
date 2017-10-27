/**
 * \file mypwd.c
 * \brief Commande PWD
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Programme d'éxecution de la commande PWD
 *
 */

// Librairies
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "mypwd.h"

/**
 * \fn void runpwd(int argc, char *argv[])
 * \brief Fonction lancement commande PWD
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxécution du programme.
 */
int runpwd(int argc, char *argv[]){

	// Affichage du contenu de la variable d'environnement "PWD"
	printf("%s \n",getenv("PWD"));
	return 0;

}

