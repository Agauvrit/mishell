/**
 * \file mydu.c
 * \brief commande DU
 * \author mishell
 * \version 1.0
 * \date 02/04/2016
 *
 * Execution de la commande du
 *
 */

// Librairies
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>
#include <stdbool.h>

#include "mydu.h"


/**
 * \fn bool estLienDur1(char *file)
 * \brief Vérification que le paramètre est un lien dur.
 *
 * \param file Nom du fichier à tester
 * \return valeur correspondant au test si le fichier est un lien dur ou non
 */
bool estLienDur1(char *file) {
    return strcmp(file, ".") == 0 || strcmp(file, "..") == 0;
}

/**
 * \fn bool estLien1(mode_t mode)
 * \brief Vérification qu'il s'agit d'un lien
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un lien ou non
 */
bool estLien1(mode_t mode) {
    return S_ISLNK(mode);
}

/**
 * \fn bool estRep1(mode_t mode) 
 * \brief Vérification qu'il s'agit d'un repertoire
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un repertoire ou non
 */
bool estRep1(mode_t mode) {
    return S_ISDIR(mode);
}

/**
 * \fn int sizeElement(char *dir, char *name, int sflag)
 * \brief Calcul de la taille d'un élément
 *
 * \param dir Dossier dont la taille doit être calculé
 * \param name Fichier dont la taille doit être calculé
 * \param sflag valeur déterminant la présence ou non de l'option s
 * \return taille de l'élément
 */
int sizeElement(char *dir, char *name, int sflag) {

    // Initialisation de la taille
    int size = 0;

    // Création du chemin
    char path[PATH_MAX];
    sprintf(path, "%s%s", dir, name);

    struct stat Stat;

    // Lecture des propriétés de l'élément
    if (lstat(path, &Stat) == 0) {

        // Si il s'agit d'un répertoire
        if (!estLienDur1(name) && estRep1(Stat.st_mode)) {
            char c = '/';
            size = (int) Stat.st_size + sizeDirContent(strcat(path, &c),sflag);

        // Si il s'agit d'un lien dur ou lien symbolique
        } else if (estLienDur1(name) || estLien1(Stat.st_mode)) {
            size = 0;

        // Si c'est un fichier renvoyer sa taille
        } else {
            size = (int) Stat.st_size;
        }

    } 
    // Si le fichier n'a pas été trouvé
    else {
        size = 0;
    }

    // Gestion de l'affichage
    if (size > 0)
        if(!sflag)
            printf("%d\t%s\n", size, path);
    
    return size;
}

/**
 * \fn int sizeDirContent(char *path,int sflag)
 * \brief Calcul de la taille d'un dossier
 *
 * \param path Lien du dossier dont la taille doit être calculé
 * \param sflag valeur déterminant la présence ou non de l'option s
 * \return taille du dossier
 */
int sizeDirContent(char *path,int sflag) {

    // Initialisation de la taille
    int size = 0;

    // Déclaration des structures
    DIR *dirp;
    struct dirent *dptr;

    // Ouverture du dossier
    if ((dirp = opendir(path)) == NULL) {
        printf("Error");
        return -1;
    }

    // Parcours du dossier
    while ((dptr = readdir(dirp))) {
        // Calcul de la taille de chaque élément
        size += sizeElement(path, dptr->d_name,sflag);
    }

    // Fermeture du dossier
    closedir(dirp);

    return size;
}

int rundu(int argc, char *argv[]){

    // Variable concernant les options
    extern char * optarg; 
    int c,cflag=0,sflag=0,errflag=0,opt=0;

    // Variable de résultat et d'affichage
    int result=0,tmp=0;
    
    
    // Chargement des options
    while((c = getopt(argc, argv, "bcs")) != -1){
        switch (c)
        {
            case 'c':
                opt++;
                cflag = 1;
                break;
            case 's':
                opt++;
                sflag = 1;
                break;
            case '?':
                opt++;
                errflag=1;
                break;
        }
    }
    
    // Gestion des options non permises
    if(errflag){
        printf("Erreur au niveau des options");
        exit(-1);
    }

    // Si des cibles ont été spécifiés
    if(argc > 1+opt){
        // Pour chaque cible on calcul la taille
        for(int i=opt+1;i<argc;i++){
            tmp = sizeElement(argv[i], "", sflag);
            if(sflag)
                printf("%d\t%s \n",tmp,argv[i]);
            result += tmp;
        }
    }
    // Si aucune cible n'est spécifié
    else{
        result = sizeElement(".","",sflag);
        if(sflag)
            printf("%d\t .\n",result);
    }

    // Gestion de l'affichage suivant l'option -c
    if(cflag)
        printf("%d\ttotal\n",result);

    return 0;
}

