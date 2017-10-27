/**
 * \file pile.h
 * \brief header de la pile d'execution 
 * \author mishell
 * \version 1.0
 * \date 01/04/2016
 *
 * Header de la pile d'execution  
 *
 */
#ifndef _PILE_
#define _PILE_

// librairies
#include <stdbool.h>


#define LimiteHaute 10
typedef char** ListeElements[LimiteHaute];
typedef char** element;

/*
	definition d'une structure de pile qui prend un compte : 
	 - une liste de pointeur de tableau de chaines de caractère 
	 - un sommet
	 - une taille 
*/
typedef struct 
{
	ListeElements contenu; 
	int sommet;
	int longueur;

}pile; 

// prototypes
pile* init_pile();
bool isEmpty (pile*);
bool isFull (pile*);
int add(pile*, element);
char** pop(pile*);
int sizePile(pile*);


#endif
