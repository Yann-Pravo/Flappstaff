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
#include "../src/headers/Graphe.h"
#include "../src/headers/ParseFile.h"
#include "../src/headers/ExportGraph.h"
#include "cu/cu.h"

TEST(testNbSCC)
{
	int **SCCs;
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	SCCs = getStronglyConnectedComponents(myGraph.matrix, myGraph.nbPersonnes);
	
	assertEqualsM(nbSCC(myGraph, SCCs), 3, "Le nombre de SCC n'est pas correct");
}

TEST(testExportSCC)
{	
	char *line = malloc(50 * sizeof(char));
	int **SCCs;
	int cmp = 0;
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	SCCs = getStronglyConnectedComponents(myGraph.matrix, myGraph.nbPersonnes);
	
	FILE *meh = tmpfile();
	
	exportSCC(myGraph, SCCs, meh, 0);
	
	rewind(meh);
	
	fgets(line, 50, meh);
	cmp += strcmp(line, "589, 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "478\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "86, 42\n");
	
	free(line);
	
	assertEqualsM(cmp, 0, "Mauvais export de SCC");
}

TEST(testExportSCCImportant)
{	
	char *line = malloc(50 * sizeof(char));
	int **SCCs;
	int cmp = 0;
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	SCCs = getStronglyConnectedComponents(myGraph.matrix, myGraph.nbPersonnes);
	
	FILE *meh = tmpfile();
	
	exportSCC(myGraph, SCCs, meh, 1);
	
	rewind(meh);
	
	fgets(line, 50, meh);
	cmp += strcmp(line, "589, 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "*478\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "86, 42\n");
	
	free(line);
	
	assertEqualsM(cmp, 0, "Mauvais export de SCC");
}

TEST(testExportAnswers)
{	
	char *line = malloc(50 * sizeof(char));
	int **answers;
	int cmp = 0;
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	answers = getQuestionsAnswers(myGraph);
	
	FILE *meh = tmpfile();
	
	exportQuestionsAnswers(myGraph, answers, meh);
	
	rewind(meh);
	
	fgets(line, 50, meh);
	cmp += strcmp(line, "Il n'y a pas de chemin entre 86 et 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "24: 86, 42\n");
	
	free(line);
	
	assertEqualsM(cmp, 0, "Mauvais export de réponses");
}

TEST(testExportGraph)
{
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	exportGraph(myGraph, "./.meh", 0);
	
	FILE *meh = fopen("./.meh", "r");
	int cmp = 0;
	char *line = malloc(50 * sizeof(char));
	
	fgets(line, 50, meh);
	cmp += strcmp(line, "3\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "589, 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "478\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "86, 42\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "Il n'y a pas de chemin entre 86 et 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "24: 86, 42\n");
	
	assertEqualsM(cmp, 0, "Mauvais Exportation");
	
	free(line);
	fclose(meh);
	remove("./.meh");
}

TEST(testExportGraphImportant)
{
	FILE *fichier = fopen("./testFiles/SmallTestv2.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	
	exportGraph(myGraph, "./.meh", 1);
	
	FILE *meh = fopen("./.meh", "r");
	int cmp = 0;
	char *line = malloc(50 * sizeof(char));
	
	fgets(line, 50, meh);
	cmp += strcmp(line, "4\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "*589, *498, *585, 469\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "*478\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "86, 42\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "*345\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "Il n'y a pas de chemin entre 86 et 498\n");
	fgets(line, 50, meh);
	cmp += strcmp(line, "24: 86, 42\n");
	
	assertEqualsM(cmp, 0, "Mauvais Exportation");
	
	free(line);
	fclose(meh);
	remove("./.meh");
}