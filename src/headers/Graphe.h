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
*	Structure contenant le tableau de personnes, 
*	la matrice en elle meme et le tableau de questions
*/
struct Graph
{
	struct Personne *tabPersonnes;
	int nbPersonnes;
	int **matrix;
	struct relationship *questions;
	int nbQuestions;
};

/*
*	Créé une matrice vide de la taille passé en paramètre
*	Toutes les cases sont initialisées à 0
*
*	@param sizeMatrix, taille de la matrice
*	@param une matrice d'entier initialisé à 0 de la taille donnée
*/
int** initMatrix(int sizeMatrix);

/*
*	Créé une matrice remplie avec les informations passées en paramètre
*
*	@param relation, liste des relations à placer dans la matrice
*	@param nbPersonnes, nombre de personne (taille) de la matrice
*	@param tab, liste des personnes correspondant à la matrice
*	@param nbRelation, nombre de relation contenues dans relation
*	@return, une matrice d'entier complétée avec les infos en param
*/
int** createMatrix(struct relationship *relation, int nbPersonnes, struct Personne *tab, int nbRelation);

/*
*	Affiche la matrice passée en paramètre
*
*	@param matrix, la matrice à afficher
*	@param size, taille de la matrice à afficher	
*/
void printMatrix(int **matrix, int size);

/*
*	Ajoute une relation entre 2 ID donnés dans la matrice donnée
*	Une relation effective vaut 1, une non existante vaut 0
*	i et j doivent exister dans matrix !!!
*
*	@param i, place de id1 dans le tableau
*	@param j, place de id2 dans le tableau
*	@param matrix, matrice à remplir, elle est modifiée directement
*		=> Attention aux effets de bord !
*/
void addRelation(int i, int j, int **matrix);

/*
*	Calcul les composantes fortement connexes sur la matrice donnée
*
*	@param matrix, matrice de boolean materialisant les relations entre personnes
*	@param size, taille de la matrice donnée
*	@return	une liste de composantes connexes (suite de positions dans le graphe).
*		Chaque composante et la fin de la liste sont terminées par -1.
*/
int** getStronglyConnectedComponents(int **matrix, int size);

/*
*	Partie du parcours en prodondeur
*	Est utilisée par depthTraversal.
*
*	@param matrix, matrice de boolean materialisant les relations
*	@param peak, point de départ du parcours
*	@param size, taille de la matrice
*	@param tag, liste des points taggés
*	@param order, 1 pour le sens normal, et 0 pour le dual
*	@param ignored, si il est différent de -1, il indique un point à 
*	       ignorer lors du parcours, sinon ce sera un parcours complet
*/
void depthTraversalRec(int **matrix, int peak, int size, int *tag, int order, int ignored);

/*
*	Parcours en profondeur de la matrice à partir d'un point donné.
*	
*	@param matrix, matrice de boolean materialisant les relations
*	@param peak, point de départ du parcours en profondeur
*	@param size, taille de la matrice
*	@param order, 1 pour le sens normal, et 0 pour le dual
*	@param ignored, si il est différent de -1, il indique un point à 
*	       ignorer lors du parcours, sinon ce sera un parcours complet
*	@return liste des sommets du parcours
*/
int* depthTraversal(int **matrix, int peak, int size, int order, int ignored);

/*
*	Retourne l'indice d'un tableau (initialisé à -1) 
*	où se trouve le plus petit élément
*
*	@param tab, un tableau d'entier initialisé à -1 
*		    (on ne tient pas compte de cette valeur
*	@param size, la taille du tableau
*	@return, l'indice du tableau où est le minimum
*/
int getMinimum(int *tab, int size);

/*
*	Calcul le chemin le plus court entre 2 points donné
*	dans le Graphe donné. Algo de Dijkstra.
*	
*	@param g, graphe contenant toutes les infos necessaires
*	@param peakA, point de départ, position dans le graphe (pas ID)
*	@param peakB, point d'arrivée, position dans le graphe (pas ID)
*	@return tableau avec la liste des positions dans le graphe, 
*		dans l'odre inverse. Ex: peakB..peakN..peakN-1..PeakA
*		On retourne -2 directement si pas de chemin
*/
int* getShortestRoute(struct Graph g, int peakA, int peakB);

/*
*	Retourne un tableau contenant le chemin le plus court
*	pour chaque question du graphe donné
*	Les chemins sont donnés sous forme de suite d'indices du graphe
*
*	@param g, Graphe contenant les questions
*	@return un tableau de liste de int
*/
int** getQuestionsAnswers(struct Graph g);

/*
*	Trouve si il reste des sommets encore inexplorés
*	
*	@param notSeen, tableau des points non explorés
*	@param size, taille de notSeen
*	@return 0 si il reste des points, 1 sinon
*/
int stillNotSeen(int *notSeen,int size);

/*
*	Retourne une matrice contenant pour chaque composante connexe
*	les indices des personnes importantes correspondantes, -1 s'il n'y en a pas
*
*	@ param components, la "matrice" des composantes connexes
*	@ param g, la structure de graphe
*	@ return tabImportant, la matrice des personnes importantes
*/
int** getAllImportantPersons(int **components, struct Graph g);

/*
*	Retourne les indices des importantes d'une composante connexe
*	-1 s'il n'y en a pas
*
* 	@ param compo, une composante connexe
*	@ param g, la structure de graphe
*	@ return result, le tableau contenant les indices des personnes importantes
*/
int* getImportantPerson(int *compo, struct Graph g);

/*
*	Dit si deux composantes connexes sont égales
*
* 	@ param compo1, une composante connexe
*	@ param compo2, une composante connexe
*	@ return result, 1 si elles sont egales, 0 sinon
*/
int componentEqual(int *compo1,int *compo2);
