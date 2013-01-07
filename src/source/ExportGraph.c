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

#include "../headers/Graphe.h"
#include "../headers/TPersonne.h"

/*
*	Donne le nombre de CFC d'un tableau de CFC donné
*	
*	@param g, Graphe contenant toutes les infos
*	@param SCC, tableau de composantes fortement connexes
*	@return nombre de composantes trouvé
*/
int nbSCC(struct Graph g, int **SCC)
{
	int i = 0;
	int j = 0;
	
	while (i < g.nbPersonnes && SCC[i][j] != -1)
	{
		i++;
	}
	
	return i;
}

/*
*	Ecrit les CFC données dans le fichier passé en paramètre.
*	Le curseur du fichier doit être placé au bon endroit !
*
*	@param g, Graphe contenant toutes les infos
*	@param SCC, le tableau de composantes fortement connexes
*	@param fichier, fichier ouvert en écriture où on va écrire les CFC
*	@param importantPersons, SCC normal si 0, SCC avec personnes importante si 1
*/
void exportSCC(struct Graph g, int **SCC, FILE * fichier, int importantPersons)
{
	int i = 0;
	int j = 0;
	int **vip;
	int k = 0;
	
	if (importantPersons == 1)
	{
		vip = getAllImportantPersons(SCC, g);
	}
	
	for (i = 0; i < g.nbPersonnes && SCC[i][j] != -1; ++i)
	{
		for (j = 0; j < g.nbPersonnes && SCC[i][j] != -1; ++j)
		{
			if (SCC[i][j + 1] != -1)
			{
				if (importantPersons && vip[i][k] == SCC[i][j])
				{
					fprintf(fichier, "*%s, ", g.tabPersonnes[SCC[i][j]].id);
					k++;
				}
				else
				{
					fprintf(fichier, "%s, ", g.tabPersonnes[SCC[i][j]].id);
				}
			}
			else
			{
				if (importantPersons && vip[i][k] == SCC[i][j])
				{
					fprintf(fichier, "*%s\n", g.tabPersonnes[SCC[i][j]].id);
					k++;
				}
				else
				{
					fprintf(fichier, "%s\n", g.tabPersonnes[SCC[i][j]].id);
				}
			}
		}
		
		k = 0;
		j = 0;
	}
	
	if (importantPersons == 1)
	{
		i = 0;
		
		while (vip[i][0] != -2)
		{
			free(vip[i]);
			i++;
		}
		
		free(vip);
	}
}

/*
*	Ecrit les réponses aux questions données en paramètre dans le fichier
*	donné. Le curseur du fichier doit être positionné au bon endroit.
*
*	@param g, graphe contenant les infos
*	@param answers, liste des points du graphe formant le chemin le plus court
*		entre 2 points donnés.
*	@param fichier, fichier ouvert où seront écrites les réponses
*/
void exportQuestionsAnswers(struct Graph g, int **answers, FILE * fichier)
{
	int i, j, freq;
	j = 0;
	freq = 0;
	
	for (i = 0; i < g.nbQuestions && answers[i][j] != -1; i++)
	{		
		if (answers[i][0] != -2)
		{
			for (j = 0; j < g.nbPersonnes && answers[i][j + 1] != -1; j++)
			{
				freq += g.tabPersonnes[answers[i][j]].freq;
			}

			fprintf(fichier, "%d: ", freq);

			for (j = j; j >= 0; --j)
			{
				if (j == 0)
				{
					fprintf(fichier, "%s\n", g.tabPersonnes[answers[i][j]].id);
				}
				else
				{
					fprintf(fichier, "%s, ", g.tabPersonnes[answers[i][j]].id);	
				}
			}

			j = 0;
			freq = 0;
		}
		else
		{
			/* le chemin n'existe pas */
			fprintf(fichier, "Il n'y a pas de chemin entre %s et %s\n", 
				g.questions[i].id1, g.questions[i].id2);
		}	
	}
}

/*
*	Exporte le graphe donné dans un fichier avec le nom transmis
*	en paramètre. Si il existe déjà, il sera écrasé.
*
*	@param g, graphe contenant les infos à exporter
*	@param fileName, nom du fichier où on écrira
*/
void exportGraph(struct Graph g, char *fileName, int important)
{
	int **answers, **SCCs;
	FILE * fichier = fopen(fileName, "w+");
	int i, nbSCCs;
	
	answers = getQuestionsAnswers(g);
	SCCs = getStronglyConnectedComponents(g.matrix, g.nbPersonnes);
	nbSCCs = nbSCC(g, SCCs);
	
	fprintf(fichier, "%d\n", nbSCCs);
	exportSCC(g, SCCs, fichier, important);
	exportQuestionsAnswers(g, answers, fichier);
	
	/* Le +1 est necessaire car on a une case allouée supplémentaire */
	for(i = 0; i < nbSCCs + 1; ++i)
	{
		free(SCCs[i]);
	}
	
	free(SCCs);
	
	for (i = 0; i < g.nbQuestions && answers[i][0] != -1; i++)
	{
		free(answers[i]);
	}
	
	free(answers);
	
	fclose(fichier);
}