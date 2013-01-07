/*
 _____ _                 _         __  __ 
|  ___| | __ _ _ __  ___| |_ __ _ / _|/ _|
| |_  | |/ _` | '_ \/ __| __/ _` | |_| |_ 
|  _| | | (_| | |_) \__ \ || (_| |  _|  _|
|_|   |_|\__,_| .__/|___/\__\__,_|_| |_|  
              |_|                         
Flapstaff is a Light And Powerful Social Text Analyser For Facebook

Coders:
benjamin.crespo.ballester(at)gmail(dot)com
yann.pravo(at)gmail(dot)com
mathieu.triay(at)gmail(dot)com

Repo: http://bitbucket.org/nagy/flapstaff/
*/

#include "../headers/TPersonne.h"
#include <string.h>

/*
*	Création d'une personne à partir des paramètres donnés.
*
*	@param idP, ID de la personne (ex: ID facebook)
*	@param nomP, Nom complet (nom + prénom) de la personne
*	@param freqP, fréquence de connexion de la personne
*	@return une Personne de structure Personne avec les params donnés
*/
struct Personne createPersonne(char* idP, char *nomP, int freqP)
{
	struct Personne p;
	strcpy(p.id, idP);
	strcpy(p.nom, nomP);
	p.freq = freqP;
	
	return p;
}

/*
*	CASOU
*
*	Affiche les détails de la personne donnée en paramètre
*	
*	@param person, personne de struct Personne

void displayPersonne(struct Personne person)
{
	printf("Nom : %s, ID : %s, Freq : %d\n", person.nom, person.id, person.freq);
}
*/

/*
*	Retourne la position d'un ID donné dans un tableau
*	Attention, cette fonction en doit être utilisée que lorsqu'on
*	est certain que l'id se trouve dans le tableau !
*	
*	@param id, id dont on veut la position
*	@param tab, tableau de personne concerné
*	@return la position dans le tableau (entre 0 et taille tab)
*/
int position(char *id, struct Personne *tab)
{
	int i = 0;
	
	while (strcmp(tab[i].id, id) != 0)
	{
		i++;
	}

	return i;
}	
