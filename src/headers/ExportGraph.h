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
*	Donne le nombre de CFC d'un tableau de CFC donné
*	
*	@param g, Graphe contenant toutes les infos
*	@param SCC, tableau de composantes fortement connexes
*	@return nombre de composantes trouvé
*/
int nbSCC(struct Graph g, int **SCC);

/*
*	Ecrit les CFC données dans le fichier passé en paramètre.
*	Le curseur du fichier doit être placé au bon endroit !
*
*	@param g, Graphe contenant toutes les infos
*	@param SCC, le tableau de composantes fortement connexes
*	@param fichier, fichier ouvert en écriture où on va écrire les CFC
*	@param importantPersons, SCC normal si 0, SCC avec personnes importante si 1
*/
void exportSCC(struct Graph g, int **SCC, FILE * fichier, int importantPersons);

/*
*	Ecrit les réponses aux questions données en paramètre dans le fichier
*	donné. Le curseur du fichier doit être positionné au bon endroit.
*
*	@param g, graphe contenant les infos
*	@param answers, liste des points du graphe formant le chemin le plus court
*		entre 2 points donnés.
*	@param fichier, fichier où seront écrites les réponses
*/
void exportQuestionsAnswers(struct Graph g, int **answers, FILE * fichier);

/*
*	Exporte le graphe donné dans un fichier avec le nom transmis
*	en paramètre. Si il existe déjà, il sera écrasé.
*
*	@param g, graphe contenant les infos à exporter
*	@param fileName, nom du fichier où on écrira
*/
void exportGraph(struct Graph g, char *fileName, int important);