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
#include "../src/headers/TPersonne.h"
#include "../src/headers/ParseFile.h"
#include "cu/cu.h"

void freeMatrix(int **matrix, int size)
{
	int i;
	
	for(i = 0; i < size; ++i)
	{
		free(matrix[i]);
	}
	
	free(matrix);
}

TEST(testAddRelation)
{
	int **matrix = initMatrix(5);
	addRelation(0, 1, matrix);
	
	assertEqualsM(matrix[0][1], 1, "L'ajout de relation ne fonctionne pas.");
	
	freeMatrix(matrix, 5);
}

TEST(testInitMatrix)
{
	int **matrix = initMatrix(5);
	int i, j;
	int res = 0;
	
	for(i = 0; i < 5; ++i)
	{
		for(j = 0; j < 5; ++j)
		{
			res += matrix[i][j];
		}
	}
	
	assertEqualsM(res, 0, "La matrice s'est mal initialisée");
}

TEST(testCreateMatrix)
{
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	int **matrix;
	struct relationship *rels;
	struct Personne *pers;
	
	/* On positionne le fichier aux relations */
	fseek(fichier, 119, SEEK_SET);
	rels = parseAllRelations(fichier, 6);
	
	/* On positionne le fichier aux Personnes */
	fseek(fichier, 3, SEEK_SET);
	pers = parseAllPersons(fichier, 5);
	
	matrix = createMatrix(rels, 5, pers, 6);
	
	assertEqualsM(matrix[0][2] == 1 && matrix[1][2] == 1 
		      && matrix[3][2] == 1 && matrix[2][3] == 1
		      && matrix[0][4] == 1 && matrix[4][0] == 1, 1,
	   	      "Les relations ne sont pas enregistrées correctement");
	
	freeMatrix(matrix, 5);
	free(rels);
	free(pers);
	fclose(fichier);
}

TEST(testSCCs)
{
	int **SCCs;
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	int i = 0;
	int j = 0;
	
	SCCs = getStronglyConnectedComponents(myGraph.matrix, myGraph.nbPersonnes);
	
	for (i = 0; i < myGraph.nbPersonnes && SCCs[i][j] != -1; ++i)
	{
		if (i == 0)
		{
			assertEqualsM(SCCs[i][0] == 0 && SCCs[i][1] == 4, 1,
			"Composante fausse");
		}
		else if (i == 1)
		{
			assertEqualsM(SCCs[i][0] == 1, 1,
			"Composante fausse");
		}
		else if(i == 2)
		{
			assertEqualsM(SCCs[i][0] == 2 && SCCs[i][1] == 3, 1,
			"Composante fausse");
		}
		else
		{
			assertTrue(0);
		}
	}
	
	assertEqualsM(i, 3, "Mauvais nombre de composantes");
}

TEST(testQuestionsAnswers)
{
	FILE *fichier = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	int **answers = getQuestionsAnswers(myGraph);
	
	int i = 0;
	int j = 0;
	
	for (i = 0; i < myGraph.nbQuestions && answers[i][j] != -1; i++)
	{		
		if (i == 0)
		{
			assertEqualsM(answers[i][j], -2, 
				      "Le chemin ne devrait pas exister");
		}
		else if (i == 1)
		{
			while (answers[i][j] != -1)
			{
				j++;
			}
			
			assertEqualsM(j, 2, "Le chemin ne devrait comporter que 2 points");
			j = 0;
		}
		else
		{
			assertTrue(0);
		}
	}
}

TEST(testImportantPerson)
{
	int **SCCs, **importantPersons;
	FILE *fichier = fopen("./testFiles/SmallTestv2.txt", "r");
	struct Graph myGraph = genFromFile(fichier);
	int i = 0;
	int j = 0;
	
	SCCs = getStronglyConnectedComponents(myGraph.matrix, myGraph.nbPersonnes);
	
	importantPersons = getAllImportantPersons(SCCs, myGraph);
	
	for (i = 0; i < myGraph.nbPersonnes && importantPersons[i][j] != -2; i++)
	{
		if (i == 0)
		{
			assertEqualsM(importantPersons[i][0] == 0 
				      && importantPersons[i][1] == 4
				      && importantPersons[i][2] == 5, 1,
				      "Les personnes importantes sont fausses");
		}
		else if (i == 1)
		{
			assertEqualsM(importantPersons[i][0] == 1, 1,
				      "Les personnes importantes sont fausses");
		}
		else if (i == 2)
		{
			assertEqualsM(importantPersons[i][0] == -1, 1,
				      "Les personnes importantes sont fausses");
		}
		else if (i == 3)
		{
			assertEqualsM(importantPersons[i][0] == 6, 1,
				      "Les personnes importantes sont fausses");
		}
		else
		{
			assertTrue(0);
		}
	}
}