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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	
*	Construit le graphe en tant que structure (tab de personnes, matrice, tab de questions)
*	en appelant successivement les différentes fonctions de construction.
*
*	@param fichier, fichier formaté correctement (voir README)
*		contient les personnes, leurs relations, et les questions
*/
struct Graph genFromFile(FILE *fichier);

/*
*	Extrait les personnes du fichier passé en paramètre
*	et les place dans un tableau pour être utilisé plus tard
*
*	@param fichier, fichier formaté correctement (voir README)
*		contient les personnes, leurs relations, et les questions
*	@param nbPersonnes, le nombre de personne déterminé par la fonction genFromFile
*	@return un tableau de personnes extraites du fichier
*/
struct Personne* parseAllPersons(FILE *fichier, int nbPersonnes);

/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à la personne. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une personne.
*	
*	@param ligne, une ligne du fichier, doit contenir une personne
*	@return une personne fabriquée à partir des infos extraites
*/
struct Personne parsePerson(char *ligne);

/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à une relation. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une relation.
*	
*	@param ligne, une ligne du fichier, doit contenir une relation
*	@return un tableau avec les ID des 2 personnes en relation
*/
struct relationship parseRelation(char *ligne);

/*
*	Parse toutes les relations du fichier donné
*	pourvu que le curseur de celui-ci soit à la bonne place.
*
*	@param fichier, fichier contenant les relations positionné à la première ligne
*	@param nbRelation, nombre de relation à extraire. Nb extrait du fichier avant.
*	@return Tableau de relation
*/
struct relationship* parseAllRelations(FILE *fichier, int nbRelation);

/*
*	Parse une ligne du fichier et extrait les informations relatives
*	à une question. Cette fonction ne doit être utilisé que lorsqu'on
*	est sûr d'avoir affaire à une ligne contenant une question.
*	
*	@param ligne, une ligne du fichier, doit contenir une question
*	@return un tableau avec les ID des 2 personnes
*/
struct relationship parseQuestion(char *ligne);

/*
*	Parse toutes les questions du fichier donné
*	pourvu que le curseur de celui-ci soit à la bonne place.
*
*	@param fichier, fichier contenant les question positionné à la première ligne
*	@param nbQuestions, nombre de questions à extraire. Nb extrait du fichier avant.
*	@return Tableau de relation (questions)
*/
struct relationship* parseAllQuestions(FILE *fichier, int nbQuestions);