/**
 * \file myrm.c
 * \brief commande RM
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Execution de la commande rm
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
#include <errno.h>
#include <stdbool.h>

#include "myrm.h"


/**
 * \fn int demandeChar(char *path)
 * \brief Fonction demande de suppression.
 *
 * \param path Lien du fichier en cours pour la suppression
 * \return valeur pour savoir si l'utilisateur veut supprimer ou non le fichier
 */
int demandeChar(char *path){

	char c;

	printf("Voulez vous vraiment supprimer le fichier/dossier \"%s\" ? [Y,O/N] \n", path);
	c=getchar();
	while(getchar()!='\n');
	if(c=='y'|| c=='Y'|| c=='o'|| c=='O'){
		return 1;
	}
	else if(c=='n' || c=='N'){
		return 0;
	}
	else
		return demandeChar(path);
}

/**
 * \fn int delDirectoryEmpty(char *dir, int vflag,int fflag)
 * \brief Fonction de suppression d'un repertoire vide
 *
 * \param dir Nom du répertoire visé
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \return valeur concernant la bonne éxecution ou non de la fonction
 */
int delDirectoryEmpty(char *dir, int vflag,int fflag){

	// variable de réception de la demande de suppression
	int yes;

	if(fflag){
		// Vérification de la suppression du dossier
		if( (rmdir(dir)) == -1 ){
			if(errno==ENOTEMPTY)
				printf("rm: impossible de supprimer \"%s\": Le dossier n'est pas vide\n",dir);
			else
				printf("Erreur de suppression d'un dossier vide\n");
			return -1;
		}
		else
			printf("Le dossier %s a bien été supprimé \n",dir);
	}
	else{
		// Demande de la suppression
		yes=demandeChar(dir);
		if(yes==1){
			if((rmdir(dir)) == -1 ){
				if(errno==ENOTEMPTY)
					printf("rm: impossible de supprimer \"%s\": Le dossier n'est pas vide\n",dir);
				else
					printf("Erreur de suppression d'un dossier vide code erreur : %d \n",errno);
				return -1;
			}
			else
				printf("Le dossier %s a bien été supprimé \n",dir);
		}
		else if(yes==-1){
			printf("Problème lors de la demande de suppression \n");
			exit(-8);
		}
	}
	return 0;
}


/**
 * \fn bool estLienDur(char *file)
 * \brief Vérification que le paramètre est un lien dur.
 *
 * \param file Nom du fichier à tester
 * \return valeur correspondant au test si le fichier est un lien dur ou non
 */
bool estLienDur(char *file) {
    return strcmp(file, ".") == 0 || strcmp(file, "..") == 0;
}

/**
 * \fn bool estLien(mode_t mode)
 * \brief Vérification qu'il s'agit d'un lien
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un lien ou non
 */
bool estLien(mode_t mode) {
    return S_ISLNK(mode);
}

/**
 * \fn bool estRep(mode_t mode) 
 * \brief Vérification qu'il s'agit d'un repertoire
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un repertoire ou non
 */
bool estRep(mode_t mode) {
    return S_ISDIR(mode);
}


/**
 * \fn int delFile(char *path, int vflag, int fflag)
 * \brief Fonction de suppression d'un fichier
 *
 * \param path Lien du fichier
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \return valeur concernant la bonne éxecution de la fonction
 */
int delFile(char *path, int vflag, int fflag){

    int yes;

	struct stat Stat;

	// Obtention d'information sur le fichier
	if(lstat(path, &Stat) == 0){
		if(fflag){
			if(unlink(path)!=-1){
				if(vflag)
					printf("Le fichier %s à bien été supprimé \n",path);
			}
			else if(errno==EISDIR)
				printf("rm: Suppression impossible le fichier est un dossier\n");
			else
				printf("Erreur suppression d'un fichier et l'erreur est : %d \n",errno);
		}
		else{
			// Demande de suppression à l'utilisateur
			yes=demandeChar(path);
			if(yes==1){
				if(unlink(path)!=-1){
					if(vflag)
						printf("Le fichier %s à bien été supprimé \n",path);
				}
				else if(errno==EISDIR)
					printf("rm: Suppression impossible le fichier est un dossier \n");
				else
					printf("Erreur suppression d'un fichier et l'erreur est : %d \n",errno);
			}
			else if(yes==-1){
				printf("Problème lors de la demande de suppression \n");
				exit(-8);
			}
		}
	}
	// Si le fichier n'est pas présent
	else{
		printf("rm: erreur impossible de trouver le fichier : %s \n",path);
		exit(-1);
	}

	return 0;
}


/**
 * \fn int delDirectory(char *path, int vflag, int fflag)
 * \brief Suppression d'un repertoire lié à l'option -r
 *
 * \param path Lien du dossier
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int delDirectory(char *path, int vflag, int fflag){

	// Déclaration des structures
	DIR *dirp;
    struct dirent *dptr;
    int yes;

    // Ouverture du dossier
    if ((dirp = opendir(path)) == NULL) {
        printf("Erreur lors de l'ouverture du dossier %s \n",path);
        exit(-1);
    }
    // Parcours du dossier en supprimant chacun des éléments
    while ((dptr = readdir(dirp))) {
    	
        delElem(path,dptr->d_name,vflag,fflag);
    }
    // Fermeture du dossier
    closedir(dirp);

    // Début de suppression du répertoire vide
    if(fflag){
	    if((rmdir(path))==0){
	    	if(vflag)
	    		printf("Le dossier %s à été supprimé \n",path);
	    }
	    else{
	    	if(errno==ENOTEMPTY)
	    		printf("Erreur lors de la suppression du dossier \n");
	    	else
	    		printf("Erreur suppression d'un fichier et l'erreur est : %d \n",errno);
	    }
	}
	else{
		// Demande de confirmation à l'utilisateur
		yes=demandeChar(path);
		if(yes==1){
			if(rmdir(path)!=-1){
		    	if(vflag){
		    		printf("Le dossier %s à été supprimé \n",path);
		    	}
		    }
		    else{
		    	if(errno==ENOTEMPTY)
		    		printf("Erreur lors de la suppression du dossier \n");
		    	else
		    		printf("Erreur suppression d'un fichier et l'erreur est : %d \n",errno);
		    }
		}
		else if(yes==-1){
			printf("Problème lors de la demande de suppression \n");
			exit(-8);
		}
	}

	return 0;
}

/**
 * \fn int runrm(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande RM.
 *
 * \param dir Nom du dossier ou est présent le fichier
 * \param name Nom de l'élément à supprimer
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int delElem(char *dir, char* name,int vflag, int fflag){

	//Création du nom de l'élement
    char path[PATH_MAX];
    sprintf(path, "%s%s", dir, name);

    struct stat Stat;

    // Vérification de la présence du fichier/dossier
    if (lstat(path, &Stat) == 0) {

        // Si il s'agit d'un répertoire
        if (!estLienDur(name) && estRep(Stat.st_mode)) {
            char* c = "/";
            delDirectory(strcat(path,c),vflag,fflag); 
        }    
        // Si c'est un fichier on le supprime
        else if(!(estLienDur(name) || estLien(Stat.st_mode))) {
            delFile(path,vflag,fflag);
        }
    }
    else {
        printf("Le fichier %s/%s n'est pas présent \n",dir,name);
        exit(-1);
    }

    return 0;
}

/**
 * \fn int runrm(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande RM.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runrm(int argc, char *argv[]){

	// Variables pour l'affichage
	int opt=0;	
	
	// Variable concernant les options
	extern char * optarg; 
	int c,dflag=0,rflag=0,vflag=0,fflag=0,errflag=0;
	

	// Chargement des options
	while((c = getopt(argc, argv, "rfdv")) != -1){
		switch (c)
		{
			case 'r':
				opt++;
				rflag = 1;
				break;
			case 'f':
				opt++;
				fflag = 1;
				break;
			case 'v':
				opt++;
				vflag = 1;
				break;
			case 'd':
				opt++;
				dflag = 1;
				break;
			case '?':
				opt++;
				errflag=1;
				break;
		}
	}
	
	// Gestion des options non permises
	if(errflag){
		printf("Erreur au niveau des options \n");
		exit(-1);
	}

	// l'option r override l'option d
	if(rflag){
		dflag=0;
	}

	// Application de la fonction RM pour chacune des destinations
	for(int i=opt+1;i<argc;i++){

		// S'il s'agit juste de supprimer un répertoire vide
		if(dflag){
			delDirectoryEmpty(argv[i],vflag,fflag);
		}

		// Si l'on souhaite supprimer recursivement un repertoire
		if(rflag){
			delElem(argv[i],"",vflag,fflag);
		}
		// Si l'on souhaite supprimer un fichier
		else{
			delFile(argv[i],vflag,fflag);
		}
	}

	return 0;

}
	
