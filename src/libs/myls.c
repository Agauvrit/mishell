/**
 * \file myls.c
 * \brief commande LS
 * \author mishell
 * \version 1.0
 * \date 01/04/2016
 *
 * Execution de la commande ls
 *
 */

//Librairies
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

#include "myls.h"


/**
 * \fn int runls(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande LS.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runls(int argc, char *argv[]){

	// Cible du LS
	char *dest;

	// Pour les dossiers et fichiers
	DIR *dirp;
	struct dirent *dptr;
	struct stat sb;
	struct passwd *pw;
	struct group *gr;
	
	// Variables pour l'affichage
	char* temps;
	int taille;
	int tmp=0;
	int first=0;
	
	// Variable concernant les options
	extern char * optarg; 
	int c,aflag=0,lflag=0,errflag=0;
	
	//Vérification de la présence d'une cible
	if (argc>1){
		if(argv[argc-1][0]=='-')
			dest="./";
		else
			dest=argv[argc-1];
	}
	else
		dest="./";

	// Chargement des options
	while((c = getopt(argc, argv, "al")) != -1){
		switch (c)
		{
			case 'a':
				aflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case '?':
				errflag=1;
				break;
		}
	}
	
	// Gestion des options non admises
	if(errflag){
		printf("Erreur au niveau des options");
		exit(-1);
	}

	// Ouverture du dossier cible. Sortie si il y a eu une erreur.
	if((dirp=opendir(dest))==NULL){
		printf("Erreur lors de l'ouverture de dossier. Numero d'erreur : %d \n",errno);
		exit(-1);
	}
	
	// Parcours des éléments du dossier
	while((dptr=readdir(dirp))){
		if(lstat(dptr->d_name,&sb) != 1){
			// Gestion de l'affichage
			if (first){
				printf("\n");
			}
			else first=1;
			
			// Gestion des options
			if(aflag){
				if(lflag){
					// Si il s'agit d'un dossier
					if(S_ISDIR(sb.st_mode)){
						printf("d"); 
						tmp=1;
					}
					else if(S_ISLNK(sb.st_mode)) printf("l");
					else printf("-");
					//permissions
					printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
					printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
					printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
					printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
					printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
					printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
					printf( (sb.st_mode & S_IROTH) ? "r" : "-");
					printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
					printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
			    	printf(" %3ld", (long) sb.st_nlink); //nombre de lien
					if((pw=getpwuid(sb.st_uid))!=NULL) printf(" %8s", pw->pw_name); // utilisateur
					else printf(" %ld", (long) sb.st_uid);
					if((gr=getgrgid(sb.st_gid))!=NULL) printf(" %21s", gr->gr_name); // groupe
					else printf(" %ld", (long) sb.st_gid);
					if(sb.st_size < 1000) printf("%5lld", (long long) sb.st_size); // taille du fichier
					else printf("%4lldK", (long long) sb.st_size/1000);
					temps = ctime(&sb.st_mtime);
					taille = strlen(temps);
					temps[taille-9] = '\0';
					temps = &temps[4];
			    	printf(" %s ", temps); 

			    	// Gestion des couleurs pour les dossiers
			    	if(tmp) printf(BLUE "%s/ " RESET, dptr->d_name);
			   		else printf("%s", dptr->d_name);
					tmp=0;	
				}
				else{
					// Gestion des couleurs pour les dossiers
					if(tmp) printf(BLUE "%s/ " RESET, dptr->d_name);
			   		else printf("%s", dptr->d_name);
					tmp=0;	
				}
			}
			else if(lflag){
				if(dptr->d_name[0] != '.'){
					if(S_ISDIR(sb.st_mode)){
						printf("d"); 
						tmp=1;
					}
					//permissions
					else if(S_ISLNK(sb.st_mode)) printf("l");
					else printf("-");
					printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
					printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
					printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
					printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
					printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
					printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
					printf( (sb.st_mode & S_IROTH) ? "r" : "-");
					printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
					printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
			    	printf(" %3ld", (long) sb.st_nlink); //nombre de lien
					if((pw=getpwuid(sb.st_uid))!=NULL) printf(" %8s", pw->pw_name); // utilisateur
					else printf(" %ld", (long) sb.st_uid);
					if((gr=getgrgid(sb.st_gid))!=NULL) printf(" %21s", gr->gr_name); // groupe
					else printf(" %ld", (long) sb.st_gid);
					if(sb.st_size < 1000) printf("%5lld", (long long) sb.st_size); // taille du fichier
					else printf("%4lldK", (long long) sb.st_size/1000);
					temps = ctime(&sb.st_mtime);
					taille = strlen(temps);
					temps[taille-9] = '\0';
					temps = &temps[4];
			    	printf(" %s ", temps); 
			    	
					if(tmp) printf(BLUE "%s/ " RESET, dptr->d_name);
			   		else printf("%s", dptr->d_name);
					tmp=0;	
				}
				else first=0;	
			}
			else{
				if(dptr->d_name[0] != '.'){
					if(tmp) printf(BLUE "%s/ " RESET, dptr->d_name);
			   		else printf("%s", dptr->d_name);
					tmp=0;
				}
				else first = 0;
			}
		}
		else{
			printf("Erreur lors de la demande d'information du fichier : %s. Numero d'erreur : %d\n",dptr->d_name,errno);
		}
	}
	// Gestion de l'affichage
	if(first){
		printf("\n");
	}
	// Fermeture du répertoire
	closedir(dirp);

	// Si il n'y a pas eu de problème
	return 0;

}
