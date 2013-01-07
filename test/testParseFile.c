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
#include "../src/headers/ParseFile.h"
#include "../src/headers/TPersonne.h"
#include "../src/headers/Graphe.h"
#include "cu/cu.h"

TEST(testParsePerson)
{
	struct Personne pers;
	char *line = malloc(30 * sizeof(char));
	strcpy(line, "Douglas Adams, 42, 24");
	
	pers = parsePerson(line);
	
	assertEqualsM(
	((strcmp(pers.nom, "Douglas Adams") == 0) &&
	(strcmp(pers.id, "42") == 0) &&
	(pers.freq == 24)), 1,
	"La personne n'a pas été extraite correctement");
	
	free(line);
}

TEST(testParseRelation)
{
	struct relationship rel;
	char *line = malloc(10 * sizeof(char));
	strcpy(line, "42, 24");
	
	rel = parseRelation(line);
	
	assertEqualsM(
	((strcmp(rel.id1, "42") == 0) &&
	(strcmp(rel.id2, "24") == 0)), 1,
	"La relation n'a pas été extraite correctement");
	
	free(line);
}

TEST(testParseQuestion)
{
	struct relationship rel;
	char *line = malloc(1024 * sizeof(char));
	strcpy(line, "42 -> 24\n");
	
	rel = parseQuestion(line);
	
	assertEqualsM(
	((strcmp(rel.id1, "42") == 0) &&
	(strcmp(rel.id2, "24") == 0)), 1,
	"La question n'a pas été extraite correctement");
	
	free(line);
}

TEST(testParseAllPersons)
{
	struct Personne *friends;
	FILE * fichier = tmpfile();
	int i = 0;
	
	fputs("Douglas Adams, 42, 24\n", fichier);
	fputs("Charles Bukowski, 23, 54\n", fichier);
	fputs("Chuck Palahniuk, 35, 98", fichier);
	
	friends = parseAllPersons(fichier, 3);
	
	while ((friends[i].nom != "") && (friends[i].id != "") && (i < 3))
	{
		i++;
	}
	
	assertEqualsM(i, 3, "Mauvais nombre d'amis");
	
	fclose(fichier);
	free(friends);
}

TEST(testParseAllRelations)
{
	struct relationship *rel;
	FILE * fichier = fopen("./testFiles/SmallTest.txt", "r");
	
	/* On positionne le fichier aux relations */
	fseek(fichier, 119, SEEK_SET);
	
	rel = parseAllRelations(fichier, 3);

	assertEqualsM(strcmp(rel[0].id1, "589") == 0 
		      && strcmp(rel[0].id2, "86") == 0 
		      && strcmp(rel[1].id1, "478") == 0 
		      && strcmp(rel[1].id2, "86") == 0 
		      && strcmp(rel[2].id1, "86") == 0 
		      && strcmp(rel[2].id2, "42") == 0, 1,
		      "Relations mal extraites");
	
	fclose(fichier);
	free(rel);
}

TEST(testParseAllQuestions)
{
	struct relationship *rel;
	FILE * fichier = fopen("./testFiles/SmallTest.txt", "r");
	
	/* On positionne le fichier aux relations */
	fseek(fichier, 169, SEEK_SET);
	
	rel = parseAllQuestions(fichier, 1);
	
	assertEqualsM(strcmp(rel[0].id1, "86") == 0 
		      && strcmp(rel[0].id2, "498") == 0, 1,
		      "Questions mal extraites");
	
	fclose(fichier);
	free(rel);	
}

TEST(testGenFromFile)
{
	FILE * meh = fopen("./testFiles/SmallTest.txt", "r");
	struct Graph myGraph = genFromFile(meh);
	
	printMatrix(myGraph.matrix, 5);
	
	assertEqualsM(myGraph.nbPersonnes, 5, "Mauvais nombre de personnes");
	assertEqualsM(myGraph.nbQuestions, 2, "Mauvais nombre de questions");
	assertEqualsM(myGraph.matrix[0][2] == 1 && myGraph.matrix[1][2] == 1 
		      && myGraph.matrix[3][2] == 1 && myGraph.matrix[2][3] == 1
		      && myGraph.matrix[0][4] == 1 && myGraph.matrix[4][0] == 1, 1,
	   	      "Les relations ne sont pas enregistrées correctement");
	assertEqualsM(strcmp(myGraph.questions[0].id1, "86") == 0 
		      && strcmp(myGraph.questions[0].id2, "498") == 0, 1,
		      "Les questions ne sont pas bien formées");
	fclose(meh);
}