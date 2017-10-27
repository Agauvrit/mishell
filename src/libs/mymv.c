/**
 * \file mymv.c
 * \brief commande MV
 * \author mishell
 * \version 1.0
 * \date 03/04/2016
 *
 * Execution de la commande mv
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
#include <libgen.h>

#include "mymv.h"

/**
 * \fn char* ajoutChaine(const char *src, char d)
 * \brief Ajout d'un caractère à une chaine de caractère
 *
 * \param src chaine source qui va se voir fixé un nouveau carctère
 * \param d caractère à ajouté à la chaine
 * \return nouvelle chaine de caractère
 */
char* ajoutChaine(const char *src, char d) {

	// Calcul de la taille
    int taille = strlen(src);

    // Demande d'allocation pour la nouvelle chaine
    char *nouvelleChaine = (char *) malloc((taille + 2) * sizeof(char));
    // Copie de la source dans la nouvelle chaine
    strcpy(nouvelleChaine, src);

    // Si le malloc a échoué
    if (!nouvelleChaine) {
        printf("Malloc error");
        exit(-1);
    }

    // Ajout du caractère en fin de chaine
    nouvelleChaine[taille] = d;
    nouvelleChaine[taille+1] = '\0';

    return nouvelleChaine;
}

/**
 * \fn char* char* mystrcat(char *src, char* dest){
 * \brief concaténation de deux chaines
 *
 * \param src chaine source a qui on va concaténé l'autre chaine
 * \param dest chaine de caractère à concaténé
 * \return les deux chaines concaténés
 */
char* mystrcat(char *src, char* dest){

	// Calcul de la taille
	int taille = strlen(src)+strlen(dest);

	// Demande d'allocation de mémoire
	char *nouvelleChaine = (char *) malloc((taille+2) * sizeof(char));
	// Copie de la chaine
    strcpy(nouvelleChaine, src);

    // Parcours de la seconde chaine
	int tailleDest = strlen(dest);
	for(int i=0;i<tailleDest;i++){
		nouvelleChaine = ajoutChaine(nouvelleChaine,dest[i]);
	}

	return nouvelleChaine;
}

/**
 * \fn int demandeChar1(char *path)
 * \brief Fonction demande de suppression.
 *
 * \param path Lien du fichier en cours pour la suppression
 * \return valeur pour savoir si l'utilisateur veut supprimer ou non le fichier
 */
int demandeChar1(char *path){

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
		return demandeChar1(path);
}

/**
 * \fn bool estLienDur2(char *file)
 * \brief Vérification que le paramètre est un lien dur.
 *
 * \param file Nom du fichier à tester
 * \return valeur correspondant au test si le fichier est un lien dur ou non
 */
bool estLienDur2(char *file) {
    return strcmp(file, ".") == 0 || strcmp(file, "..") == 0;
}

/**
 * \fn bool estRep2(mode_t mode) 
 * \brief Vérification qu'il s'agit d'un repertoire
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un repertoire ou non
 */
bool estRep2(mode_t mode) {
    return S_ISDIR(mode);
}

/**
 * \fn int replaceFile(char *src, char *dest, int vflag, int nflag, int iflag, int fflag)
 * \brief Remplacement du fichier source dans le dossier destination
 *
 * \param src fichier source
 * \param dest dossier destination
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \param nflag valeur déterminant la présence ou non de l'option n
 * \param iflag valeur déterminant la présence ou non de l'option i
 * \return bon fonctionnement ou non de la méthode
 */
int replaceFile(char *src, char *dest, int vflag, int nflag, int iflag, int fflag){

	int yes;
	struct stat Stat;
		
	// Si l'element destination existe
	if(lstat(dest, &Stat) == 0){
		// Si l'élément destination n'est pas un dossier
		if(!estRep2(Stat.st_mode)){
			// Gestion des flags
			if(!nflag){
				if(iflag){
					// Demande de confirmation
					yes=demandeChar1(dest);
					if(yes){
						// Supression du fichier existant
						if(unlink(dest)==0){
							// Renomage du fichier
							if(rename(src,dest)==0){
								if(vflag)
									printf("Le fichier à bien été changé d'emplacement et de nom \n");
							}
							else
								printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
						}
						else
							printf("Problème lors de la suppression du fichier destination. Numéro d'erreur : %d \n",errno);
					}	
				}
				else{
					// Suppression du fichier existant
					if(unlink(dest)==0){
						// Renomage du fichier
						if(rename(src,dest)==0){
							if(vflag)
								printf("Le fichier à bien été changé d'emplacement et de nom \n");
						}
						else
							printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
					}
					else
						printf("Problème lors de la suppression du fichier destination. Numéro d'erreur : %d \n",errno);
				}
			}
		}
		else{
			if(!nflag){
				if(iflag){
					// Demande de confirmation
					yes=demandeChar1(dest);
					if(yes){
						if(dest[strlen(dest)]!='/'){
							if(rename(src,(mystrcat(mystrcat(dest,"/"),src)))==0){
								if(vflag)
									printf("Le fichier à bien été changé d'emplacement et de nom \n");
							}
							else
								printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
						}
						else{
							if(rename(src,mystrcat(dest,src))==0){
								if(vflag)
									printf("Le fichier à bien été changé d'emplacement et de nom \n");
							}
							else
								printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
						}
					}
				}
				else{
					if(dest[strlen(dest)]!='/'){
						if(rename(src,(mystrcat(mystrcat(dest,"/"),src)))==0){
							if(vflag)
								printf("Le fichier à bien été changé d'emplacement et de nom \n");
						}
						else
							printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
					}
					else{
						if(rename(src,mystrcat(dest,src))==0){
							if(vflag)
								printf("Le fichier à bien été changé d'emplacement et de nom \n");
						}
						else
							printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
					}
				}
			}
		}
	} 
	// Sinon directement renomé en fichier destination
	else{
		// verification du bon fonctionnement du rename
		if(rename(src,dest)==0){
			if(vflag)
				printf("Le fichier à bien été changé d'emplacement et de nom \n");
		}
		else
			printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
	}

	return 0;
}


/**
 * \fn int replaceRep(char *src, char *dest, int vflag, int nflag, int iflag, int fflag)
 * \brief Remplacement du dossier source dans le dossier destination
 *
 * \param src fichier source
 * \param dest dossier destination
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \param nflag valeur déterminant la présence ou non de l'option n
 * \param iflag valeur déterminant la présence ou non de l'option i
 * \return bon fonctionnement ou non de la méthode
 */
int replaceRep(char *src, char *dest, int vflag, int nflag, int iflag, int fflag){

	int yes;
	char tmp[100];
	struct stat Stat;
	struct stat Stat2;

	strcpy(tmp,dest);
	if(lstat(dirname(tmp), &Stat) == 0){
		if(lstat(dest, &Stat2) == 0){
			if(estRep2(Stat2.st_mode)){
				if(rmdir(dest)==0){
					if(!nflag){
						if(iflag){
							yes=demandeChar1(dest);
							if(yes){
								if(rename(src,dest)==0){
									if(vflag)
										printf("Le dossier à bien été changé d'emplacement et de nom \n");
								}
								else
									printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
							}
						}
						else{
							if(rename(src,dest)==0){
								if(vflag)
									printf("Le fichier à bien été changé d'emplacement et de nom \n");
							}
							else
								printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);
						}
					}
				}
				else
					printf("mv: impossible le dossier cible n'est pas vide \n");
			}
			else{
				if(!estLienDur2(dest)){
					printf("mv: impossible %s n'est pas un dossier \n",dest);
					exit(-1);
				}
				else{
					printf("Il n'est pas possible d'overwrite un lien dur \n");
					exit(-1);
				}
			}
		}
		else{
			if(rename(src,dest)==0){
				if(vflag)
					printf("Le fichier à bien été changé d'emplacement et de nom \n");
			}
			else
				printf("Problème lors du rename. Numéro d'erreur : %d \n",errno);	
		}
	} 
	else{
		printf("Le path : %s n'existe pas \n",dirname(dest));
		exit(-1);
	}
	return 0;
}

/**
 * \fn int replaceElem(char *src, char *dest, int vflag, int nflag, int iflag, int fflag)
 * \brief Remplacement de l'element source dans le dossier destination
 *
 * \param src fichier source
 * \param dest dossier destination
 * \param vflag valeur déterminant la présence ou non de l'option v
 * \param fflag valeur déterminant la présence ou non de l'option f
 * \param nflag valeur déterminant la présence ou non de l'option n
 * \param iflag valeur déterminant la présence ou non de l'option i
 * \return bon fonctionnement ou non de la méthode
 */
int replaceElem(char *src, char *dest, int vflag, int nflag, int iflag, int fflag){

	struct stat Stat;

	if(lstat(src, &Stat) == 0){
		if (!estLienDur2(src) && estRep2(Stat.st_mode))
			replaceRep(src,dest,vflag,nflag,iflag,fflag);
		else if(!estRep2(Stat.st_mode))
			replaceFile(src,dest,vflag,nflag,iflag,fflag);
	}
	else{
		printf("Fichier ou dossier source inconnu \n");
		exit(-1);
	}

	return 0;
}


/**
 * \fn int runmv(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande MV.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runmv(int argc, char *argv[]){

	
	// Variable concernant les options
	extern char * optarg; 
	int c,fflag=0,vflag=0,iflag=0,nflag=0,errflag=0,opt=0;
	

	// Chargement des options
	while((c = getopt(argc, argv, "vifn")) != -1){
		switch (c)
		{
			case 'v':
				opt++;
				vflag = 1;
				break;
			case 'f':
				opt++;
				fflag = 1;
				break;
			case 'i':
				opt++;
				iflag = 1;
				break;
			case 'n':
				opt++;
				nflag = 1;
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

	// Gestion des flags plus important que les autres
	if(nflag){
		fflag=0;
		iflag=0;
	}
	else if(iflag)
		fflag=0;

	// Si le nombre d'argument est correct
	if (argc==opt+3){
		replaceElem(argv[argc-2],argv[argc-1],vflag,nflag,iflag,fflag);
	}
	else if(argc==opt+2){
		printf("mv: impossible veuilez spécifier une destination \n");
		exit(-1);
	}
	else if(argc==opt+1){
		printf("mv: impossible il faut une source et une destination \n");
		exit(-1);
	}
	else{
		printf("Problème au niveau des paramètres \n");
		exit(-1);
	}

	return 0;

}
	

