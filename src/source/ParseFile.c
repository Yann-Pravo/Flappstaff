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

#include <math.h>
#include "../headers/ParseFile.h"
#include "../headers/TPersonne.h"
#include "../headers/Graphe.h"

/*	
*	Construit le graphe en tant que structure (tab de personnes, matrice, tab de questions)
*	en appelant successivement les différentes fonctions de construction.
*
*	@param fichier, fichier formaté correctement (voir README)
*		contient les personnes, leurs relations, et les questions
*/
struct Graph genFromFile(FILE *fichier)
{
	int nbP, nbR, nbQ;
	struct Graph graphToReturn;	
	struct Personne *tabPers;
	struct relationship *relations;
	struct relationship *questions;
	int **matrix;
	
	fscanf(fichier, "%d\n", &nbP);
	tabPers = parseAllPersons(fichier, nbP);

	fscanf(fichier, "%d\n", &nbR);
	relations = parseAllRelations(fichier, nbR);
	matrix = createMatrix(relations, nbP, tabPers, nbR);
	free(relations);
	
	fscanf(fichier, "%d\n", &nbQ);
	questions = parseAllQuestions(fichier, nbQ);
	
	graphToReturn.tabPersonnes = tabPers;
	graphToReturn.nbPersonnes = nbP;
	graphToReturn.matrix = matrix;
	graphToReturn.questions = questions;
	graphToReturn.nbQuestions = nbQ;

	return graphToReturn;
}
	
/*
*	Extrait les personnes du fichier passé en paramètre
*	et les place dans un tableau pour être utilisé plus tard
*
*	@param fichier, fichier formaté correctement (voir README)
*		contient les personnes, leurs relations, et les questions
*	@return un tableau de personnes extraites du fichier
*/
struct Personne* parseAllPersons(FILE *fichier, int nbPersonnes)
{
	struct Personne *tabPers;
	int i;
	char *ligne = malloc(80 * sizeof(char));
	tabPers = malloc(nbPersonnes * sizeof(struct Personne));
	
	for (i = 0; i < nbPersonnes; i++)
	{
		fgets(ligne, 80, fichier);
		tabPers[i] = parsePerson(ligne);
		free(ligne);
		ligne = malloc(80*sizeof(char));		
	}
	
	free(ligne);

	return tabPers;
}

/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à la personne. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une personne.
*	
*	@param ligne, une ligne du fichier, doit contenir une personne
*	@return une personne fabriquée à partir des infos extraites
*/
struct Personne parsePerson(char *ligne)
{
	struct Personne person;	
	char nom[40];
	char id[20];
	int freq;
	
	/* On extrait les sous chaines directement ! */
	sscanf(ligne, "%[^,], %[^,], %d \n", nom, id, &freq);

	person = createPersonne(id,nom,freq);
	
	return person;
}

/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à une relation. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une relation.
*	
*	@param ligne, une ligne du fichier, doit contenir une relation
*	@return un tableau avec les ID des 2 personnes en relation
*/
struct relationship parseRelation(char *ligne)
{
	struct relationship rel;
	
	/* On extrait les sous chaines directement ! */
	sscanf(ligne, "%[^,], %[^\n]\n", rel.id1, rel.id2);
	
	return rel;
}

/*
*	Parse toutes les relations du fichier donné
*	pourvu que le curseur de celui-ci soit à la bonne place.
*
*	@param fichier, fichier contenant les relations positionné à la première ligne
*	@param nbRelation, nombre de relation à extraire. Nb extrait du fichier avant.
*	@return Tableau de relation
*/
struct relationship* parseAllRelations(FILE *fichier, int nbRelation)
{
	int i;
	struct relationship *tabRelation = malloc(nbRelation * sizeof(struct relationship));
	
	for (i = 0; i < nbRelation; i++)
	{
		char *ligne = malloc(50 * sizeof(char));
		fgets(ligne, 50, fichier);
		tabRelation[i] = parseRelation(ligne);
		free(ligne);
	}

	return tabRelation;
}

/*
*	Parse toutes les questions du fichier donné
*	pourvu que le curseur de celui-ci soit à la bonne place.
*
*	@param fichier, fichier contenant les question positionné à la première ligne
*	@param nbQuestions, nombre de questions à extraire. Nb extrait du fichier avant.
*	@return Tableau de relation (questions)
*/
struct relationship* parseAllQuestions(FILE *fichier, int nbQuestions)
{
	int i;
	struct relationship* tabQuestion = malloc(nbQuestions * sizeof(struct relationship));
	
	for (i = 0; i < nbQuestions; i++)
	{
		char *ligne = malloc(50 * sizeof(char));
		fgets(ligne, 50, fichier);
		tabQuestion[i] = parseQuestion(ligne);
		free(ligne);
	}
	
	return tabQuestion;		
}
/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à une question. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une question.
*	
*	@param ligne, une ligne du fichier, doit contenir une question
*	@return un tableau avec les ID des 2 personnes
*/
struct relationship parseQuestion(char *ligne)
{
	struct relationship quest;
	
	/* On extrait les sous chaines directement ! */
	sscanf(ligne, "%[^ ] -> %s", quest.id1, quest.id2);
	
	return quest;
}