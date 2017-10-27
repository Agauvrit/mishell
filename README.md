Projet : Mishell 

Auteurs :   - Aurélien Gauvrit
            - Arthur Dailland
            - Thomas Picard
            - Jérémy Bernier

Arboresence du projet :

Dossier "src" contenant les fichiers sources dans "src/libs" et les executables des commandes dans "src/exe". Les sources des différents shells
étant à la racine du dossier "src".

Dossier "bin" contenant les compilés des différents sources ainsi que la librairie statique.

Compilation : 
Se placer a la racine du projet et compiler le makefile via la commande : make
Les shells disponibles sont :   1) Shell qui utilise des executables
                                2) Shell qui utilise une librairie statique

Execution des shells : 1) ./bin/mishell
                       2) ./bin/mishell_statique

Attention ! 
Une erreur peut se produire lors du lancement des différentes commandes lorsque le shell est lancé, résultant d'un core dumped.
Pour corriger ceci il faut modifier les valeurs de MAXLINE et MAXARGS des shells respectivement dans les 
fichiers "src/mishell.c" et "src/mishell2.c" en les abaissant ou les augmentant.
                       
Les commandes disponibles et options associés : - cat, options : -b -n
                                                - chgrp, options : -f -v -r
                                                - cp, options : -r
                                                - du, options : -c -s
                                                - echo, options : -n
                                                - ls, options : -a -l
                                                - mkdir, options : -m -v
                                                - more
                                                - mv, options : -v -f -n -i
                                                - pwd
                                                - rm, options : -f -v -d -r
                                                - chmod, options : -v -f -r
                                                - chown, options : -v -f -r
                                                
Le shell gère aussi les opérateurs suivant : &, |, &&, ||, <<, <, >, >>

