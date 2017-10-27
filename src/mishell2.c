/**
 * \file mishell2.c
 * \brief shell qui utilise des librairies statiques  
 * \author mishell
 * \version 1.0
 * \date 01/04/2016
 *
 * shell qui utilise des librairies statiques  
 *
 */

// librairies
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "pile.h"

// librairies statiques
#include "libs/myls.h"
#include "libs/mycat.h"
#include "libs/mycp.h"
#include "libs/mydu.h"
#include "libs/myecho.h"
#include "libs/mymkdir.h"
#include "libs/mymore.h"
#include "libs/mymv.h"
#include "libs/mypwd.h"
#include "libs/myrm.h"
#include "libs/mychgrp.h"
#include "libs/mychmod.h"
#include "libs/mychown.h"

// variable statique 
#define MAXLINE 256
#define MAXARGS 123

extern char ** environ;


/**
 * \fn monExecve(int argc, char* argv[],int (*exec) (int, char **))
 * \brief Fonction qui executer une ligne de commande 
 *
 * \param argc Nombre d'arguments dans argv 
 * \param argv liste des diffférents arguments de la commande
 * \param exec pointeur de fonction de la commande
 * \return l'état à la fin de l'execution
 */
int monExecve(int argc, char* argv[],int (*exec) (int, char **)){
    return exec(argc,argv);
}

/**
 * \fn int parse_command_line(char command_line[], char* argv[])
 * \brief Fonction qui découpe la ligne de commande par mot 
 *
 * \param command_line chaine de caractère saisie par l'utilisateur 
 * \param argv liste des diffférents arguments de la commande
 */
int parse_command_line(char command_line[], char* argv[]){
	char *arg =(char*)malloc(124*sizeof(char)); 
	int indexArg = 0;
	int indexArgV = 0;
	for(int index=0;index < strlen(command_line);index++){   
		if(' ' == command_line[index] && indexArg !=0 ){
			arg[indexArg] = '\0';
			indexArg = 0;  
			argv[indexArgV] = arg; 
			indexArgV ++; 
			arg =(char*)malloc(124*sizeof(char));
		}
		else if(command_line[index] != 10) {
			arg[indexArg]=command_line[index]; 
			indexArg ++; 
		}
	}
    if(indexArg > 0){
        arg[indexArg] = '\0';
        argv[indexArgV] = arg; 
        indexArgV ++; 
    } 
	argv[indexArgV] = NULL; 
	return 0; 
}
`
/**
 * \fn void stack_cmd(pile *pile, char *argv[])
 * \brief Fonction qui séparare les lignes de commande et les empile dans la pile d'execution
 *
 * \param pile pile d'execution du shell 
 * \param argv liste des diffférents arguments de la commande
 */
void stack_cmd(pile *pile, char *argv[]){
    int index = 0; 
    int indexCmd = 0; 
    int indexp = 0; 
    char **command = (char**)malloc(MAXARGS*sizeof(char)); 
    while(argv[index] != NULL){
        if(strcmp(argv[index],"|") && strcmp(argv[index],"<") && strcmp(argv[index],"<<") && 
            strcmp(argv[index],">") && strcmp(argv[index],">>") && strcmp(argv[index],"||") && 
             strcmp(argv[index],"&&") && strcmp(argv[index],"&")){
            command[indexCmd]=argv[index];
            indexCmd ++;
        }
        else{
        	command[indexCmd] = NULL;
            indexCmd ++; 
            add(pile,command);
           	add(pile,&argv[index]);
           	indexp += 2; 
            command = (char**)malloc(MAXARGS*sizeof(char));     
            indexCmd =0;   
        } 
        index ++; 
    }
    if(indexCmd > 0){
	   add(pile,command);
    }
}

/**
 * \fn int transform_args(char* argv[])
 * \brief fonction qui range les différents argument selon les options puis les chemins de fichier
 *
 * \param argv liste des diffférents arguments de la commande
 * \return le nombre d'argument de argv 
 */
int transform_args(char* argv[]){
    int nbargs = 0;
    int tmp_cpt =0;
    while(argv[tmp_cpt] != NULL){
        nbargs++;
        tmp_cpt++;
    }

    //tableaux et compteurs temporaires
    char* args[MAXARGS];
    char* other_vars[MAXARGS];
    int other_vars_cpt=0;
    int args_cpt=0;

    for(int index=1; index < nbargs;index++){
        //Si l'argument est un paramètre de la commande (commence par un '-')
        if((strncmp(argv[index],"-",sizeof(char))==0)){
            //Si l'argument contient plus de 1 paramètre (ex: -al)
            if(strlen(argv[index]) > 2){
                //On copie la string de base
                char* base_argv_value = malloc(MAXARGS*sizeof(char));
                strcpy(base_argv_value,argv[index]);
                //On sépare chaque argument
                for (int j = 1; j < strlen(argv[index]); j++){
                    base_argv_value = base_argv_value + 1; // On décale de 1 caractère
                    char tmp_arg[2];
                    strncpy(tmp_arg,base_argv_value,sizeof(char));
                    char* tmp_final_arg = malloc(3*sizeof(char));
                    tmp_final_arg[0] = '-';
                    strcat(tmp_final_arg,tmp_arg);
                    //On ajoute aux arguments  
                    args[args_cpt] = tmp_final_arg;
                    args_cpt++;
                }
            }
            //Sinon, on ajoute l'argument directement
            else{
                args[args_cpt] = argv[index];
                args_cpt++;
            }
        }
        //Sinon, il ne s'agit pas d'un argument, on l'ajoute à la liste des paramètres
        else{
            other_vars[other_vars_cpt] = argv[index];
            other_vars_cpt++;
        }
    }

    //On ajoute d'abord les arguments séparés après la commande
    for (int i = 0; i < args_cpt; i++)
    {
        argv[i+1] = args[i];
    }

    //Puis on ajoute les paramètres de la commande
    for (int i = 0; i < other_vars_cpt; i++)
    {
        argv[i+args_cpt+1]=other_vars[i];
    }

    /* Vérification du bon fonctionnment
    for (int i = 0; i < args_cpt+other_vars_cpt+1; ++i)
    {
        printf("argument %d : %s\n",i,argv[i]);
    }
    */
    return nbargs;
}

/**
 * \fn int runCd(int argc, char **argv)
 * \brief Fonction qui séparare les lignes de commande et les empile dans la pile d'execution
 *
 * \param argc Nombre d'élements contenus dans argv 
 * \param argv liste des diffférents arguments de la commande
 * \return l'état d'execution de la commande 
 */
int runCd(int argc, char **argv){
	char* dest = NULL;
    char* tmp=NULL;
    char* cwd;
    char buff[PATH_MAX + 1];
    switch (argc) {
    case 1:
        dest = getenv("HOME");
        tmp = getenv("PWD");
        chdir(dest);
        setenv("OLDPWD",tmp,1);
        setenv("PWD",dest,1);
        break;
    case 2:
        if (strcmp(argv[1], "~")==0){
            dest = getenv("HOME");
            tmp = getenv("PWD");
            chdir(dest);
            setenv("OLDPWD",tmp,1);
            setenv("PWD",dest,1);
        }
        else{
            tmp=getenv("PWD");  
        	chdir(argv[1]);
            cwd = getcwd( buff, PATH_MAX + 1 );
            setenv("OLDPWD",tmp,1);
            setenv("PWD",cwd,1);
        }
        break;
    default:
        fprintf(stderr, "Problème lors du changement de répertoire\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void eval(char command_line[], const char *PATH);

int main(void)
{
	const char *PATH = getenv("PWD");

    char command_line[MAXLINE];
    while(1)
    {
		  //On affiche le répertoire courant du shell
        printf("%s > ",getenv("PWD"));
		  //On récupère la chaîne de caractères tapée sur l'entrée standard
        fgets(command_line, MAXLINE, stdin);
		  //Si on a bien un caractère de fin de fichier (touche entrée)

        if (feof(stdin))
        {
            exit(0);
        } 
        else 
        {
            eval(command_line,PATH);
        }
    }
}

/**
 * \fn void eval(char command_line[],const char *PATH)
 * \brief Fonction qui interprète la commande et execute les commande 
 *
 * \param command_line ligne de commande saisie par l'utilisateur
 * \param PATH chemin absolu du dossier executer par le shell 
 */
void eval(char command_line[],const char *PATH){
    pid_t pid;
    char *argv[MAXARGS];
   	parse_command_line(command_line, argv);
    pile *pileCmd; 

    pileCmd = init_pile(); 
    stack_cmd(pileCmd,argv); 
    char ** arg[MAXARGS]; 
    char **operateur[MAXARGS]; 
	operateur[0][0] = "\0"; 


	char isCd[] = "cd";
	if(strcmp(argv[0],isCd) == 0){
		int nbargs =  transform_args(argv);
		runCd(nbargs,argv);
	}

	// j'isole la ligne de commande que je vais executer
    pid = fork();

    if(pid == 0){
        int status = -2;
        int pipefd[2]; 

        if(pipe(pipefd) == -1){
        	perror("Erreur lors de la creation du tube"); 
        	exit(1); 
        }

        // tant que la pile d'execution n'est pas vide ou qu'il n'y a pas de condition d'arrêt, je dépile
        while(!isEmpty(pileCmd) && status != -1){ 
        	
        	pid_t pidLogique = 1; 
        	if(sizePile(pileCmd) == 2){
        		*operateur = pop(pileCmd);  
        		status = 0; 
        	}
            *arg = pop(pileCmd); 
            if(sizePile(pileCmd) > 0){
                *operateur = pop(pileCmd);  
            }

            // Si je possède l'operateur | 
            if(strcmp(operateur[0][0],"|") == 0){
            	if(sizePile(pileCmd) > 0){
            		int pidPipe = fork(); 
            		if(pidPipe == 0){
            			// config de l'entrée du tube
                        close(pipefd[1]); 
                        int tmpIn = dup(STDIN_FILENO);
                        if( tmpIn == -1){
                            fprintf(stderr, "%s : %s\n","dup", strerror(errno));
                            exit(-1);
                        }
                        if(dup2(pipefd[0],STDIN_FILENO) == -1){
                            fprintf(stderr, "%s : %s\n","dup2", strerror(errno));
                            exit(-1);
                        }
                        if(close(tmpIn) == -1){
                            fprintf(stderr, "%s : %s\n","close", strerror(errno));
                            exit(-1);
                        }
                        close(pipefd[0]);
                        int retourfils; 
       
	             		pidPipe = waitpid(pidPipe,&retourfils,0);
	             		// je regarde le status de la valeur de retour, le fils est NULL
	             		// Je ne passe pas à la commande suivante
	        			if(WEXITSTATUS(retourfils) == 0) {
	                    	status = -1;
	        			} 
	        			// sinon je passe à la commande suivante
	        			else {
	            			status = 0;
	        			} 
            		}
            	}
            	else{
                    int pidPipe = fork(); 
                    if(pidPipe == 0){
                    	// config de la sortie du tube 
                		close(pipefd[0]); 
            			int tmpOut = dup(STDOUT_FILENO);
            			if( tmpOut == -1){
            				fprintf(stderr, "%s : %s\n","dup", strerror(errno));
							exit(-1);
            			}
            			if(dup2(pipefd[1],STDOUT_FILENO) == -1){
            				fprintf(stderr, "%s : %s\n","dup2", strerror(errno));
							exit(-1);
            			}
            			if(close(tmpOut) == -1){
            				fprintf(stderr, "%s : %s\n","close", strerror(errno));
							exit(-1);
            			}
            			close(pipefd[1]);
                        status = 0; 
                    }
                    else{
                        close(pipefd[0]);
                        close(pipefd[1]);

                        int retourfils; 
                        pidPipe = waitpid(pidPipe,&retourfils,0);

                        // je regarde le status de la valeur de retour, le fils est NULL
                        // Je ne passe pas à la commande suivante
                        if(WEXITSTATUS(retourfils) == 0) {
                            status = -1;
                        } 
                        // sinon je passe à la commande suivante
                        else {
                            status = 0;
                        } 
                    }
                    
            	}
            }
            // si je possède l'operateur || 
            else if(strcmp(operateur[0][0],"||") == 0){
            	// si je possède plus d'un argument dans ma pile je crée un nouveau fils 
            	if(sizePile(pileCmd) > 0){
                	pidLogique = fork(); 
                	if(pidLogique == 0){
                	} 
                	// le parent attend la fin de l'execution du fils 
                    else{
                    	int retourfils; 
	             		pidLogique = waitpid(pidLogique,&retourfils,0);

	             		// je regarde le status de la valeur de retour, le fils est NULL
	             		// Je ne passe pas à la commande suivante
	        			if(WEXITSTATUS(retourfils) == 0) {
	            			/*printf("%d retourne status %d\n", 
	                    	pid, WEXITSTATUS(status)); */ 
	                    	status = -1;
	        			} 
	        			// sinon je passe à la commande suivante
	        			else {
	            			//printf("%d : terminaison anormale\n", pid);
	            			status = 0;
	        			} 
	        		} 
                } 
                else {
                	// j'execute dans tout les cas la dernière commande de la pile
                	status = 0;
                } 
            }
            // si je possède l'operateur >> : redirection à la fin du fichier 
            else if(strcmp(operateur[0][0],">>") == 0){
                status = 0; 
                int fd;
                int tmpfd;
                // creation du fichier ou ouverture du fichier
                fd = open(arg[0][0], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
                if (fd == -1){
                    fprintf(stderr, "open - %s\n", strerror(errno));
                    exit(-1);
                }

                //on duplique le descripteur 
                tmpfd = dup(STDOUT_FILENO);
                if (tmpfd == -1 && errno != EBADF){ //EBADF : pas de desc. a dupliquer 
                    fprintf(stderr, "dup - %s\n", strerror(errno));
                    exit(-1);
                }

                // on pointe l'ancien descripteur vers le fichier de sortie
                if (dup2(fd, STDOUT_FILENO) == -1){
                    fprintf(stderr, "dup2 - %s\n", strerror(errno));
                    exit(-1);
                }

                //maintenant on peut fermer l'ancien fichier 
                if (close(tmpfd) == -1){
                    fprintf(stderr, "close - %s\n", strerror(errno));
                    exit(-1);
                }
                 *arg = pop(pileCmd); 
            }
             // si je possède l'operateur > : redirection vers un fichier 
            else if(strcmp(operateur[0][0],">") == 0){
                status = 0;
                int fd;
                int tmpfd;
                // creation du fichier
                fd = open(arg[0][0], O_WRONLY | O_CREAT, S_IRWXU);
                if (fd == -1){
                    fprintf(stderr, "open - %s\n", strerror(errno));
                    exit(-1);
                }

                // on duplique le descripteur 
                tmpfd = dup(STDOUT_FILENO);
                if (tmpfd == -1 && errno != EBADF){ //EBADF : pas de desc. a dupliquer  
                    fprintf(stderr, "dup - %s\n", strerror(errno));
                    exit(-1);
                }

                // on remplace le descripteur de sortie par le fichier ouvert
                if (dup2(fd, STDOUT_FILENO) == -1){
                    fprintf(stderr, "dup2 - %s\n", strerror(errno));
                    exit(-1);
                }

                // maintenant on peut fermer l'ancien fichier 
                if (close(tmpfd) == -1){
                    fprintf(stderr, "close - %s\n", strerror(errno));
                    exit(-1);
                }
                *arg = pop(pileCmd); 
            }
            // si je possède l'operateur << : redirection, saisie clavier entree standard
            else if(strcmp(operateur[0][0],"<<") == 0){ 
            	// init du tube 
            	int pipefd[2]; 

            	// creation d'un tube
	            if(pipe(pipefd) == -1){
	            	perror("Erreur lors de la creation du tube"); 
	            	exit(1); 
	            }
            	
            	// fork pour séparer la commande executer et le texte saisie
            	int pidRedir = fork(); 
            	// pour le fils, on regarde le texte saisie jusqu'au mot de fin 
            	if(pidRedir == 0){
            		close(pipefd[0]);
            		char input[MAXLINE];
            		char cmp[strlen(arg[0][0])]; 
            		cmp[strlen(arg[0][0])] = '\0'; 
            		int index; 
            		int endOfFile = 0; 

            		// tant que je n'ai pas le mot de fin je continue de demander une saisie clavier. 
            		while(!endOfFile){
                		printf("> ");
						fgets(input, MAXLINE, stdin); 
                		for(index=0;index<strlen(arg[0][0]);index++){
                			cmp[index] = input[index];
                		}
                		// je compare la saisie avec le mot de fin 
                		if(strcmp(cmp,arg[0][0]) == 0){
                			endOfFile =1; 
                		}
                		else {
                			// j'écris la sortie dans le tube 
            				write(pipefd[1],input,strlen(input));
                		}
            		}
            		exit(0); 
            	}
          		// le parent
            	else {
            		// je change le descripteur de fichier de l'entree standard avec le tube 
            		close(pipefd[1]); 
            		int tmpIn = dup(STDIN_FILENO);
                    if( tmpIn == -1){
                        fprintf(stderr, "%s : %s\n","dup", strerror(errno));
                        exit(-1);
                    }
                    if(dup2(pipefd[0],STDIN_FILENO) == -1){
                        fprintf(stderr, "%s : %s\n","dup2", strerror(errno));
                        exit(-1);
                    }
                    if(close(tmpIn) == -1){
                        fprintf(stderr, "%s : %s\n","close", strerror(errno));
                        exit(-1);
                    }
                    close(pipefd[0]);

                    // j'attend que la saisie soit terminé 
                    int retourfils; 
   
             		pidRedir = waitpid(pidRedir,&retourfils,0);
             		// je regarde le status de la valeur de retour, le fils est NULL
             		// Je ne passe pas à la commande suivante
        			if(WEXITSTATUS(retourfils) == 0) {
                    	status = 0;
        			} 
        			// sinon je passe à la commande suivante
        			else {
            			status = -1;
        			} 
            	}
            	*arg = pop(pileCmd); 
            }
            // si je possède l'operateur < : redirection d'un fichier dans l'entree standard
            else if(strcmp(operateur[0][0],"<") == 0){
            	status = 0;
                int fd;
                int tmpfd;
                // ouverture en lecture du fichier 
                fd = open(arg[0][0], O_RDONLY);
                if (fd == -1){
                    fprintf(stderr, "open - %s\n", strerror(errno));
                    exit(-1);
                }

                // on duplique le descripteur 
                tmpfd = dup(STDIN_FILENO);
                if (tmpfd == -1 && errno != EBADF){ //EBADF : pas de desc. a dupliquer  
                    fprintf(stderr, "dup - %s\n", strerror(errno));
                    exit(-1);
                }

                // je pointe l'ancien descripteur d'entre vers le nouveau fichier
                if (dup2(fd, STDIN_FILENO) == -1){
                    fprintf(stderr, "dup2 - %s\n", strerror(errno));
                    exit(-1);
                }

                // maintenant on peut fermer l'ancien fichier 
                if (close(tmpfd) == -1){
                    fprintf(stderr, "close - %s\n", strerror(errno));
                    exit(-1);
                }
                *arg = pop(pileCmd); 
            }
            // si je possède l'operateur && 
            else if(strcmp(operateur[0][0],"&&") == 0){
                // si je possède plus d'un argument dans ma pile je crée un nouveau fils 
          		if(sizePile(pileCmd) > 0){
                	pidLogique = fork(); 
                	if(pidLogique == 0){
                	}
                	// le parent attend la fin de l'execution du fils 
                    else{
                    	int retourfils; 
	             		pidLogique = waitpid(pidLogique,&retourfils,0);

	             		// je regarde le status de la valeur de retour, le fils est NULL
	             		// Je passe à la commande suivante
	        			if(WEXITSTATUS(retourfils) == 0) {
	            			/*printf("%d retourne status %d\n", 
	                    	pidLogique, WEXITSTATUS(status));*/
	                    	status = 0; 
	        			} 
	        			// sinon j'arrête l'execution de la commande
	        			else {
	            			//printf("%d : terminaison anormale\n", pid);
	            			status = -1; 
	        			} 
	        		} 
                } 
                else {
                	// j'execute dans tout les cas la dernière commande de la pile
                	status = 0;
                }                       
            }
            else{
            	status = 0;
            }

        	// si le status est NULL, je peux executer la commande 
        	if(status == 0){
        		// je realise un fork pour executer la commande dans son propre processus 
            	pid_t pidproc = fork(); 
                
            	// pour le fils 
                if(pidproc == 0){
                    // je transform la commande pour qu'elle soit executable
                    int nb=transform_args(arg[0]); // permet aussi de compter le nombre d'arguments
                    // je construit le constructeur de l'executable
                    int (*pointeurFonction)(int,char**)=NULL;
                    if(strcmp(argv[0],"ls") == 0){
                        pointeurFonction=runls;
                    }
                    else if(strcmp(argv[0],"mkdir") == 0){
                        pointeurFonction=runmkdir; 
                    }
                    else if(strcmp(argv[0],"cd") == 0){
                        exit(0);
                    }
                    else if(strcmp(argv[0],"pwd") == 0){
                        pointeurFonction=runpwd;
                    }
                    else if(strcmp(argv[0],"cat") == 0){
                        pointeurFonction=runcat; 
                    }
                    else if(strcmp(argv[0],"more") == 0){
                        pointeurFonction=runmore;
                    }
                    else if(strcmp(argv[0],"mv") == 0){
                        pointeurFonction=runmv;
                    }
                    else if(strcmp(argv[0],"cp") == 0){
                        pointeurFonction=runcp;
                    }
                    else if(strcmp(argv[0],"rm") == 0){
                        pointeurFonction=runrm;
                    }
                    else if(strcmp(argv[0],"du") == 0){
                        pointeurFonction=rundu;
                    }
                    else if(strcmp(argv[0],"chmod") == 0){
                        pointeurFonction=runchmod;
                    }
                    else if(strcmp(argv[0],"chown") == 0){
                        pointeurFonction=runchown;
                    }
                    else if(strcmp(argv[0],"chgrp") == 0){
                        pointeurFonction=runchgrp; 
                    }
                    else if(strcmp(argv[0],"echo") == 0){
                        pointeurFonction=runecho;
                    }
                    
                    if(pointeurFonction){
                    	// execution 

                        if(monExecve(nb,arg[0],pointeurFonction) < 0){
                        	fprintf(stderr,"%s\n", strerror(errno));
                            exit(-1); // exit pour le pidproc  
                        }
                    }
                    else {
                    	exit(0);
                    }
                }
                // pour le père
                else {
                	// j'attend que le fils de pidproc
                	// je regarde si la commande est executé en arrière plan
                	if(strcmp(operateur[0][0],"&") !=0){
	                    int retour;
	                    if (waitpid(pidproc, &retour, 0) < 0){
	                        fprintf(stderr, "%s\n", strerror(errno));
	                    }
	                    if(WEXITSTATUS(retour) == 0){
	                    	exit(0); // exit pour le pidLogique
	                    }
	                    else { 	
	                    	exit(-1); // exit pour le pidLogique
	                    }
                 	}   
                }
            }
        }
        exit(0); // ferme 
    }
    else{ // processus parent, attend la fin d'execution de la commande

        int status;
        if (waitpid(pid, &status, 0) < 0)
        {
            fprintf(stderr, "%s\n", strerror(errno));
        }
    }
    
}

