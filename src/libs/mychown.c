/**
 * \file mychown.c
 * \brief commande CHOWN
 * \author mishell
 * \version 1.0
 * \date 05/04/2016
 *
 * Execution de la commande chown
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
#include "mychown.h"

/**
 * \fn recur(char * fichier, int uid, int gid, int fflag, int vflag, int user, char * newGrpUser)
 * \brief Fonction pour la recursivité de la commande chown.
 *
 * \param fichier : nom du fichier ou dossier.
 * \param uid : l'IUD du dossier/fichier
 * \param gid : le GID du dossier/fichier
 * \param fflag : valeur du flag -f
 * \param vflag: valeur du flag -v
 * \param user: entier pour savoir si on change l'utilisateur ou le groupe
 * \param newGrpUser : nouveau nom de l'utilisateur ou du groupe
 */
void recur(char * fichier, int uid, int gid, int fflag, int vflag, int user, char * newGrpUser){
	char *userGrp, str[50];
	
	if(user) userGrp = "utilisateur";
	else userGrp = "groupe";

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
      					printf("chown: %s correctement change: \"%s\" \n",userGrp, newGrpUser);
		   			}else{
						printf("chown: %s incorrect: \"%s\"\n",userGrp, newGrpUser);
						exit(-1);
					}
				}else if(!vflag && !fflag){
					if (chown(fichier, uid, gid) == -1) {
						printf("chown: %s incorrect: \"%s\"\n",userGrp, newGrpUser);
						exit(-1);
					}
				}

				//Vérification du type du dossier
				if(ent->d_type == DT_DIR){
					recur(str, uid, gid, fflag, vflag, user, newGrpUser);
				}
        	}
        	closedir(dirp);
    	}

}

/**
 * \fn int runchown(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande chown.
 *
 * \param argc Nombre d'élements contenus dans argv.
 * \param argv élements dans la ligne de commande. Il peut s'agir du nom de la commande, d'options ou de destinations.
 * \return valeur suivant la bonne éxecution ou non de la fonction.
 */
int runchown(int argc, char *argv[]){

	// variable pour gérer les options
	int c,errflag=0,fflag=0,vflag=0,rflag=0,opt=2,i;
	
	// Variable pour récupérer les informations d'un dossier
	uid_t uid;
	gid_t gid;
  	struct passwd *pw;
   	char *ptr, *user, *grp, *str;
	int groupe=0, utilisateur=0;
	
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

	// Si on change l'utilisateur ou le groupe
	if(strstr(str, ":") != NULL){
		groupe = 1;
		user = strtok(str, ":");
		grp = strtok (NULL, " ,");
	}else{
		utilisateur = 1;
		user = str;
	}
	
	uid = strtol(user, &ptr, 10);  

  	if (*ptr != '\0') {   
		     
   		pw = getpwnam(user);  
		if(pw == NULL){
			printf("L'utilisateur %s n'existe pas\n", user);
			exit(-1);
		}
      	uid = pw->pw_uid;
  	}
	// Gestion des options en regardant si on change de groupe ou d'utilisateur
	for(i = 1; i<(argc-opt)+1;i++){
		printf("fichier : %s\n", argv[argc-i]);
		if(fflag){
			if(utilisateur){
				
				chown(argv[argc-i], uid, -1);
			}else if(groupe){
	  			gid = strtol(grp, &ptr, 10);

				if (*ptr != '\0') {         
	   				pw = getpwnam(user);
					if(pw == NULL){
						exit(-1);
						printf("L'utilisateur %s n'existe pas\n", user);
					}
		  		}
		  		gid = pw->pw_gid;
	  			
				chown(argv[argc-i], uid, gid);
			}
		}else if(vflag && !fflag){
			if(utilisateur){
				
				if (chown(argv[argc-i], uid, -1) == 0) {
		  			printf("chown: utilisateur correctement change: \"%s\"\n", user);
	   			}else{
					printf("chown: utilisateur incorrect: \"%s\"\n", user);
					exit(-1);
				}

			}else if(groupe){
	  			gid = strtol(grp, &ptr, 10);

				if (*ptr != '\0') {         
	   				pw = getpwnam(user); 
					if(pw == NULL) exit(-1);
		  			gid = pw->pw_gid;
	  			}
				if (chown(argv[argc-i], uid, gid) == 0) {
		  			printf("chown: groupe correctement change: \"%s\"\n", grp);
	   			}else{
					printf("chown: groupe incorrect: \"%s\"\n", grp);
					exit(-1);
				}
			}

		}else{
			if(utilisateur){
				if (chown(argv[argc-i], uid, -1) == -1) {
					printf("chown: utilisateur incorrect: \"%s\"\n", user);
					exit(-1);
				}

			}else if(groupe){
	  			gid = strtol(grp, &ptr, 10);

				if (*ptr != '\0') {         
	   				pw = getpwnam(user);
					if(pw == NULL) exit(-1);
		  			gid = pw->pw_gid;
	  			}
				if (chown(argv[argc-i], uid, gid) == -1) {
					printf("chown: groupe incorrect: \"%s\"\n", grp);
					exit(-1);
				}
			}
		}

		// Si il y a l'option -r, on applique la commande sur tout les sous-fichier/sous-dossier
		if(rflag){
			if(utilisateur) recur(argv[argc-i], uid, -1, fflag, vflag, 1, str);
			else if(groupe) recur(argv[argc-i], uid, gid, fflag, vflag, 0, str);
		}
	}
    
 	return 0;
}

