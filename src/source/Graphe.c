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
#include "../headers/Graphe.h"

/*
*	Créé une matrice vide de la taille passé en paramètre
*	Toutes les cases sont initialisées à 0
*
*	@param sizeMatrix, taille de la matrice
*	@param une matrice d'entier initialisé à 0 de la taille donnée
*/
int** initMatrix(int sizeMatrix)
{
	int** matrixEmpty;
	matrixEmpty = (int**) malloc(sizeMatrix * sizeof (int*));
	
	int i;
	for (i = 0; i < sizeMatrix; i++)
	{
		matrixEmpty[i] = (int*) malloc(sizeMatrix * sizeof (int));
	}
	
	int j;
	for (i = 0; i < sizeMatrix; i++)
	{
		for (j = 0; j < sizeMatrix; j++)
		{
				matrixEmpty[i][j] = 0;
		}
	}
	
	return matrixEmpty;
}

/*
*	Créé une matrice remplie avec les informations passées en paramètre
*
*	@param relation, liste des relations à placer dans la matrice
*	@param nbPersonnes, nombre de personne (taille) de la matrice
*	@param tab, liste des personnes correspondant à la matrice
*	@param nbRelation, nombre de relation contenues dans relation
*	@return, une matrice d'entier complétée avec les infos en param
*/
int** createMatrix(struct relationship *relation, int nbPersonnes, struct Personne *tab, int nbRelation)
{
	int **matrix;
	int i, j, k;
	
	matrix = initMatrix(nbPersonnes);
	
	for(i = 0; i < nbRelation; i++)
	{
		j = position(relation[i].id1, tab);
		k = position(relation[i].id2, tab);
		addRelation(j, k, matrix);
	}
	
	return matrix;
}

/*
*	Affiche la matrice passée en paramètre
*
*	@param matrix, la matrice à afficher
*	@param size, taille de la matrice à afficher	
*/
void printMatrix(int **matrix, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf(" %d ", matrix[i][j]);
		}
		printf("\n");
	}
}

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
void addRelation(int i, int j, int **matrix)
{
	matrix[i][j] = 1;
}

/*
*	Calcul les composantes fortement connexes sur la matrice donnée
*	a.k.a compCon (pour les intimes). Fred, si tu me lis.
*
*	@param matrix, matrice de boolean materialisant les relations entre personnes
*	@param size, taille de la matrice donnée
*	@return	une liste de composantes connexes (suite de positions dans le graphe).
*		Chaque composante et la fin de la liste sont terminées par -1.
*/
int** getStronglyConnectedComponents(int **matrix, int size)
{
	int *etat = malloc(size * sizeof(int));
	int *tag;
	int *tagDual;
	int **compos = malloc(sizeof(int*));
	
	int i, j;
	int k = 0; // gere le nombre de lignes de la matrice compos
	int l = 0; // gere le nombre de colonnes d'une ligne de la matrice compos
	
	for (i = 0; i < size; i++)
	{
		etat[i] = 0;
	}
 	
	for (i = 0; i < size; i++)
	{
		if (etat[i] == 0)
		{
			compos = realloc(compos, (k + 1) * sizeof(int*));
			compos[k] = malloc(sizeof(int));
			compos[k][l] = i;
			tag = depthTraversal(matrix, i, size, 1, -1);
			tagDual = depthTraversal(matrix, i, size, 0, -1);
			etat[i] = 1;
			
			for (j = 0; j < size; j++)
			{
				if (tag[j] == 1 && tagDual[j] == 1 && etat[j] == 0)
				{
					l++;					
					compos[k] = realloc(compos[k], (l + 1) * sizeof(int));
					compos[k][l] = j;
					
					etat[j] = 1;
				}
			}

			l++;					
			compos[k] = realloc(compos[k], (l + 1) * sizeof(int));
			compos[k][l] = -1;
			
			l = 0;

			k++;
		}
	}

	compos = realloc(compos, (k + 1) * sizeof(int *));
	compos[k] = malloc(sizeof(int));
	compos[k][l] = -1;

	free(tag);
	free(tagDual);
	free(etat);

	return compos;
}

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
void depthTraversalRec(int **matrix, int peak, int size, int *tag, int order, int ignored)
{
	int i;
	tag[peak] = 1;
		
	for (i = 0; i < size; i++)
	{
		if ((ignored != -1 && i != ignored) || ignored == -1)
		{
			if (order == 1)
			{
				if (matrix[peak][i] == 1 && tag[i] == 0)
				{
					depthTraversalRec(matrix, i, size, tag, order, ignored);

				}
			}
			else
			{
				if (matrix[i][peak] == 1 && tag[i] == 0)
				{

					 depthTraversalRec(matrix, i, size, tag, order, ignored);

				}
			}
		}
	}
}

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
int* depthTraversal(int **matrix, int peak, int size, int order, int ignored)
{
	int i;
	int *tag = malloc(size * sizeof(int));
	
	for (i = 0; i < size; i++)
	{
		tag[i] = 0;
	}
	
	depthTraversalRec(matrix, peak, size, tag, order, ignored);
	
	return tag;
}

/*
*	Retourne l'indice d'un tableau (initialisé à -1) 
*	où se trouve le plus petit élément
*
*	@param tab, un tableau d'entier initialisé à -1 
*		    (on ne tient pas compte de cette valeur
*	@param size, la taille du tableau
*	@return, l'indice du tableau où est le minimum
*/
int getMinimum(int *tab, int size)
{
	int i = 0;
	int min;
	
	while (tab[i] == -1 || tab[i] == -2)
	{
		i++;
	}
	
	min = i;
	
	for (i = min + 1; i < size; i++)
	{
		if (tab[i] != -1 && tab[i] != -2 && tab[i] < tab[min])
		{
			min = i;
		}
	}

	return min;
}

/*
*	Retourne un tableau contenant le chemin le plus court
*	pour chaque question du graphe donné
*	Les chemins sont donnés sous forme de suite d'indices du graphe
*
*	@param g, Graphe contenant les questions
*	@return un tableau de liste de int
*/
int** getQuestionsAnswers(struct Graph g)
{
	int **questionsAnswers = malloc((g.nbQuestions + 1) * sizeof(int*));
	int i;
	
	for(i = 0; i < g.nbQuestions; ++i)
	{	
		questionsAnswers[i] = getShortestRoute(g, position(g.questions[i].id1, g.tabPersonnes), 
					     		  position(g.questions[i].id2, g.tabPersonnes));
	}
	
	return questionsAnswers;
}

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
int* getShortestRoute(struct Graph g, int peakA, int peakB)
{	
	int *route = malloc(1 * sizeof(int));
	int *routePeakA = depthTraversal(g.matrix, peakA, g.nbPersonnes, 1, -1);
	
	if (routePeakA[peakB] == 0)
	{
		/* Le point n'est pas dans le parcours, il n'y a pas de chemin */
		route[0] = -2;
	}
	else
	{
		int i, n1, n2;
		int size = g.nbPersonnes;	
		int *explored = malloc(size * sizeof(int));
		int *father = malloc(size * sizeof (int));
		int *notSeen = malloc(size * sizeof(int));
		int j = 0;

		for (i = 0; i < size; i++)
		{
			explored[i] = 20000; /* Dummy number */
			father[i] = 0;
		}

		explored[peakA] = 0;

		for (i = 0; i < size; i++)
		{
			notSeen[i] = explored[i];
		}

		while (stillNotSeen(notSeen, size) == 0)
		{
			n1 = getMinimum(notSeen, size) ;
			notSeen[n1] = -1;

			for (n2 = 0; n2 < size; n2++)
			{
				if (g.matrix[n1][n2] == 1)
				{

					if (explored[n2] > explored[n1] + g.tabPersonnes[n1].freq + g.tabPersonnes[n2].freq)
		 			{
						explored[n2] = explored[n1] + g.tabPersonnes[n1].freq 
									    + g.tabPersonnes[n2].freq;
						father[n2] = n1;
					}
				}
			}

			for (i = 0; i < size; i++)
			{
				if (notSeen[i] != -1)
				{
					notSeen[i] = explored[i];
				}
			}
		}

		i = peakB;

		while (i != peakA)
		{
			route = realloc(route, (j + 1) * sizeof(int));
			route[j] = i;
			i = father[i];
			j++;
		}

		route = realloc(route, (j + 2) * sizeof(int));
		route[j] = peakA;
		route[j + 1] = -1;

		free(explored);
		free(father);
		free(notSeen);	
	}
	
	free(routePeakA);
	
	return route;
}

/*
*	Trouve si il reste des sommets encore inexplorés
*	
*	@param notSeen, tableau des points non explorés
*	@param size, taille de notSeen
*	@return 0 si il reste des points, 1 sinon
*/
int stillNotSeen(int *notSeen, int size)
{
	int result = 1;
	int i;
	
	for(i = 0; i < size && result != 0; i++)
	{
		if (notSeen[i] != -1 )
		{
			result = 0;
		}
	}
	
	return result;
}

/*
*	Retourne une "matrice" contenant pour chaque composante connexe
*	les indices des personnes importantes correspondantes, -1 s'il n'y en a pas
*
*	@ param components, la "matrice" des composantes connexes
*	@ param g, la structure de graphe
*	@ return tabImportant, la matrice des personnes importantes
*/
int** getAllImportantPersons(int **components, struct Graph g)
{
	int **tabImportant = malloc(sizeof(int*));
	int i = 0;
	int k = 0;	
	int j = 0;
	
	while (components[i][0] != -1)
	{
		while (components[i][j] != -1)
		{
			j++;
		}

		tabImportant[k] = malloc((j + 1) * sizeof(int));
		j = 0;		
		tabImportant[k] = getImportantPerson(components[i], g);
		k++;
		tabImportant = realloc(tabImportant, (k + 1) * sizeof(int*));
		i++;
	}
	
	tabImportant[k] = malloc(sizeof(int));
	tabImportant[k][0] = -2; 
	
	return tabImportant;
}

/*
*	Retourne les indices des importantes d'une composante connexe
*	-1 s'il n'y en a pas
*
* 	@ param compo, une composante connexe
*	@ param g, la structure de graphe
*	@ return result, le tableau contenant les indices des personnes importantes
*/
int* getImportantPerson(int *compo, struct Graph g)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int l, n;
	int *compo2, *compo3, *tag, *tagDual;
	int *result = malloc(sizeof(int));
	result[m] = -1;

	if (compo[1] == -1)
	{
		result[m] = compo[0];
		result = realloc(result, 2 * sizeof(int));
		result[1] = -1;
	}
	else
	{
		while (compo[i] != -1)
		{
			compo2 = malloc(sizeof(int));
		
			while (compo[j] != -1)
			{
				if (i != j)
				{
					compo2[k] = compo[j];
					k++;				
					compo2 = realloc(compo2, (k + 1) * sizeof(int));
				}
				
				j++;
			}
			 
			j = 0;
			compo2[k] = -1;
			/* On a une sous composante sans l'élément courant */

			n = compo2[0];
			compo3 = malloc(sizeof(int));
			tag = depthTraversal(g.matrix, n, g.nbPersonnes, 1, compo[i]);
			tagDual = depthTraversal(g.matrix, n, g.nbPersonnes, 0, compo[i]);
			l = 0;

			for (j = 0; j < g.nbPersonnes; j++)
			{
				if (tag[j] == 1 && tagDual[j] == 1)
				{
					compo3[l] = j;
					l++;
					compo3 = realloc(compo3, (l + 1) * sizeof(int));
				}
			}
		
			compo3[l] = -1;

			j = 0;
			k = 0;

			if (componentEqual(compo2, compo3) == 0)
			{
				result[m] = compo[i];
				m++;
				result = realloc(result, (m + 1) * sizeof(int));
			}
			
			i++;
			
			free(compo2);
			free(compo3);
			free(tag);
			free(tagDual);
		}
	
		result[m] = -1;
	}
	
	return result;	
}

/*
*	Dit si deux composantes connexes sont égales
*
* 	@ param compo1, une composante connexe
*	@ param compo2, une composante connexe
*	@ return result, 1 si elles sont egales, 0 sinon
*/
int componentEqual(int *compo1,int *compo2)		
{
	int result = 1;
	int i = 0;
	
	while (compo1[i] != -1 && result == 1)
	{
		if (compo1[i] != compo2[i])
		{
			result = 0;
		}
		
		i++;
	}
	
	if (compo1[i] == -1 && compo2[i] != -1)
	{
		result = 0;
	}
	
	return result;
}
