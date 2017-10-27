/**
 * \file mycp.c
 * \brief commande CP
 * \author mishell
 * \version 1.0
 * \date 10/04/2016
 *
 * Execution de la commande cp
 *
 */

// Librairies
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>

#include "mycp.h"

// Variables de fichier
int indent = 0;
const int BUFFER_SIZE = 1024;
const int FOLDER_RIGHT = 0775;


/**
 * \fn bool estRep3(mode_t mode) 
 * \brief Vérification qu'il s'agit d'un repertoire
 *
 * \param mode Mode du fichier testé
 * \return valeur correspondant au test si le fichier est un repertoire ou non
 */
bool estRep3(mode_t mode) {
    return S_ISDIR(mode);
}



/**
 * \fn void newRep(char *lien)
 * \brief Création d'un nouveau repertoire
 *
 * \param  lien nouveau répertoire
 */
void newRep(char *lien) {

    struct stat st;

    if (stat(lien, &st) == -1) {
        if (mkdir(lien, FOLDER_RIGHT) == -1) {
            perror("Création répertoire impossible \n");
            exit(-1);
        }
    }
}

/**
 * \fn int File_to_file(char *src, char *dest)
 * \brief Copie le contenu d'un fichier dans un autre
 * 
 * \param src fichier source
 * \param dest fichier destination
 * \return bonne éxecution de la fonction
 */
int File_to_file(char *src, char *dest) {

    ssize_t taille;
    char buf[BUFFER_SIZE];

    FILE *file1 = fopen(src, "r");
    FILE *file2 = fopen(dest, "w");

    if (file1 == NULL) {
        printf("Impossible d'ouvrir \"%s\" \n", src);
        exit(-1);
    }
    else if (file2 == NULL) {
        printf("Impossible d'ouvrir \"%s\" \n", dest);
        exit(-1);
    }


    while ((taille = read(fileno(file1), buf, BUFFER_SIZE)) > 0 ) {
        ssize_t new = 0;
        do {
            new = write(fileno(file2), buf + new, taille - new);
            if (new == -1) {
                printf("Erreur d'écriture \n");
                break;
            }
        } while (new != taille);
    }

    if (taille == 1) {
        printf("Erreur de lecture\n");
    }

    fclose(file1);
    fclose(file2);
    return 0;
}

/**
 * \fn int File_to_rep(char *src_file, char *dest_dir)
 * \brief Copie un fichier dans un répertoire
 * 
 * \param src_file fichier source
 * \param dest_dir repertoire destination
 * \return bonne éxecution de la fonction
 */
int File_to_rep(char *src_file, char *dest_dir) {

    FILE *file;

    // au cas ou l'utilisateur a entré le chemin sans / à la fin
    char *dir = dest_dir;

    //On crée le path vers le fichier destination
    char *fileName = basename(src_file);
    char path_file[strlen(dir) + strlen(fileName)];
    strcpy(path_file, dir);
    strcpy(path_file + strlen(dir), fileName);

    if ((file = fopen(path_file, "w")) == NULL) { // création du fichier
        printf("Ouverture impossible du fichier \"%s\" \n", path_file);
        exit(-1);
    }

    fclose(file);

    //On copie le fichier dans le nouveau fichier créé
    return File_to_file(src_file, path_file);
}

/**
 * \fn int Files_to_rep(int argc, char *argv[])
 * \brief Copie plusieurs fichiers en entrée dans un répertoire
 * 
 * \param argc Le nombre d'argument
 * \param argv Les arguments
 * \return bonne éxecution de la fonctions
 */
int Files_to_rep(int argc, char *argv[]) {

    struct stat st;
    int cpt;

    for (cpt = 1; cpt < argc-1; cpt++){

        //On vérifie ce n'est pas un dossier que l'on essaye de copier
        if (stat(argv[cpt], &st) == -1) {
            printf("Erreur obtention d'information \"%s\" \n",argv[cpt]);
            exit(-1);
        }
        if (estRep3(st.st_mode)) { //si c'est un répertoire, message erreur
            printf("cp: Impossible de copier le répertoire \"%s\" \n", argv[cpt]);
            exit(-1);
        }

        File_to_rep(argv[cpt], argv[argc-1]);
    }
    return 0;
}

/**
 * \fn int Rep_to_rep(char *dir_path_src, char *dir_path_dest)
 * \brief Copie le contenu d'un répertoire dans un répertoire 
 *
 * \param dir_path_src Le chemin du répertoire à copier
 * \param dir_path_dest Le chemin du répertoire de destination
 * \return bonne éxecution de la fonction
 */
int Rep_to_rep(char *dir_path_src, char *dir_path_dest) {

    DIR *dirp_src;
    struct dirent *dptr_src;
    struct stat st;

    //ajout d'un / à la source
    char dir_path_src_tmp[strlen(dir_path_src) + 100];
    strcpy(dir_path_src_tmp, dir_path_src);
    if(dir_path_src[strlen(dir_path_src) -1]!='/')
        strcat(dir_path_src_tmp, "/");

    //ajout d'un / à la destination
    char dir_path_dest_tmp[strlen(dir_path_dest) + 2];
    strcpy(dir_path_dest_tmp, dir_path_dest);
    if(dir_path_dest[strlen(dir_path_dest) -1]!='/')
        strcat(dir_path_dest_tmp, "/");
    newRep(dir_path_dest_tmp);


    if ((dirp_src = opendir(dir_path_src_tmp)) == NULL) {
        printf("Impossible d'ouvrir le répertoire \"%s\", erreur : %s \n", dir_path_src_tmp, strerror(errno));
        exit(-1);
    }

    while ((dptr_src = readdir(dirp_src)) != NULL) { //tous les éléments de la source

        if ((strcmp(dptr_src->d_name, "..") != 0) && (strcmp(dptr_src->d_name,".") != 0)) {

            char path[strlen(dir_path_src_tmp) + strlen(dptr_src->d_name) + 2];
            strcpy(path, dir_path_src);
            strcat(path, "/");
            strcat(path, dptr_src->d_name);

            //distinction fichiers et dossiers
            if (stat(path, &st) == -1) {
                printf("Erreur obtention d'information \n");
                exit(-1);
            }
            if (S_ISDIR(st.st_mode)) { //répertoire
                //construction de la destination
                char destDir[strlen(dir_path_dest_tmp) + strlen(dptr_src->d_name) + 2];
                strcpy(destDir, dir_path_dest_tmp);
                strcat(destDir, dptr_src->d_name);
                strcat(destDir, "/");
                newRep(dir_path_dest_tmp);

                indent++;
                Rep_to_rep(path, destDir);
                indent--;
            } else { //fichier
                File_to_rep(path, dir_path_dest_tmp);
            }
        }
    }

    if(closedir(dirp_src) != 0) {
        perror("Closedir error");
        exit(1);
    }

    return 0;
}

/**
 * \fn int Reps_to_rep(int argc, char *argv[])
 * \brief Copie plusieurs répertoires en entrée dans un repertoire
 *
 * \param argc Le nombre d'argument
 * \param argv Les arguments
 */
int Reps_to_rep(int argc, char *argv[]) {
    int cpt;
    for (cpt = 2; cpt < argc-1; cpt++){
        Rep_to_rep(argv[cpt], argv[argc-1]);
    }
    return 0;
}

/**
 * \fn int runcp(int argc, char *argv[])
 * \brief Fonction d'éxecution de la commande cp
 *
 * \param argc Le nombre d'argument
 * \param argv Les arguments
 * \return bonne éxecution de la fonction
 */
int runcp(int argc, char *argv[]) {

    struct stat st;
    char c;
    int rflag=0,errflag=0;

    while((c = getopt(argc, argv, "r")) != -1){
        switch (c)
        {
            case 'r':
                rflag = 1;
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

    if(rflag){
        Reps_to_rep(argc,argv);
    }
    else{

        // Si la destination est un dossier 
        if (stat(argv[argc-1], &st) != -1 && estRep3(st.st_mode)){
            return Files_to_rep(argc,argv);
        }
        else {

            // COpie impossible de plusieurs element dans un seul fichier
            if (argc > 3) {
                printf("cp: impossible \"%s\" n'est pas un dossier\n", argv[argc-1]);
                exit(-1);
            }
            return File_to_file(argv[1], argv[2]);
        }
    }

    return 0;
}
