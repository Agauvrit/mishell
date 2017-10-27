/**
 * \file pile.c
 * \brief pile d'execution 
 * \author mishell
 * \version 1.0
 * \date 01/04/2016
 *
 * Pile d'execution
 *
 */

// librairies
#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

/**
 * \fn  init_pile()
 * \brief Fonction qui permet de creer la pile d'execution 
 *
 */ 
pile* init_pile()
{	
	pile *p = (pile*) malloc(sizeof(pile)); 

	p->longueur = 10;
	p->sommet = 0; 
	return p; 
}

/**
 * \fn isEmpty(pile *p)
 * \brief Fonction qui indique si la pile est vide ou non 
 *
 * \param p la pile
 * \return vraie si la pile est vide
 */
bool isEmpty(pile *p)
{	
	bool verif; 
	if (p->sommet == 0){
		verif= true;
	}	
	else {
		verif= false;
	}
	return verif;
}

/**
 * \fn isFull(pile *p)
 * \brief Fonction qui indique si la pile est pleine ou non 
 *
 * \param p la pile 
 * \return vrai si la pile est pleine
 */
bool isFull(pile *p)
{
	bool verif;
	if (p->sommet == p->longueur){
		verif = true; 
	}
	else {
		verif = false; 
	}
	return verif;
}

/**
 * \fn add(pile *p, element e)
 * \brief Fonction qui ajouter un élément dans la pile d'execution 
 *
 * \param p la pile  
 * \param e l'element à ajouter 
 */
int add(pile *p, element e)
{
	p->contenu[p->sommet] = e;
	p->sommet = p->sommet+1;
	return 0;
}

/**
 * \fn pop(pile *p)
 * \brief Fonction qui dépile un élément de la pile d'execution 
 *
 * \param p la pile 
 * \return le pointeur d'un argv
 */
char** pop(pile *p)
{
	p->sommet=p->sommet-1; 
	return p->contenu[p->sommet]; 
}
 
/**
 * \fn sizePile(pile *p)
 * \brief Fonction qui donne la taille de la pile  
 *
 * \param p la pile 
 * \return la taille de la pile
 */
int sizePile(pile *p)
{
	return p->sommet;
}


