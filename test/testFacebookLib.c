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
#include "../src/headers/FacebookLib.h"
#include "../src/headers/TPersonne.h"
#include "cu/cu.h"

struct facebookFriends getFakeFriends()
{
	struct facebookFriends fb;
	fb.friends = malloc(5 * sizeof(struct Personne));
	fb.nbFriends = 5;
	
	strcpy(fb.friends[0].id, "576234550");
	strcpy(fb.friends[1].id, "1286221719");
	strcpy(fb.friends[2].id, "1318575395");
	strcpy(fb.friends[3].id, "275489");
	strcpy(fb.friends[4].id, "42");
	
	strcpy(fb.friends[0].nom, "Yann Pravo");
	strcpy(fb.friends[1].nom, "Ben Crespo");
	strcpy(fb.friends[2].nom, "Mathieu Triay");
	strcpy(fb.friends[3].nom, "Charles Bukowski");
	strcpy(fb.friends[4].nom, "Douglas Adams");
	
	fb.friends[0].freq = 4;
	fb.friends[1].freq = 6;
	fb.friends[2].freq = 2;
	fb.friends[3].freq = 80;
	fb.friends[4].freq = 42;
	
	return fb;
}

char *token;

/* Si ce test échoue, il faut considerer les autres comme invalides */
TEST(testFacebookConnect)
{
	token = malloc(200 * sizeof(char));
	token = facebookConnect("projetcc@yahoo.fr", "projetig3");
	
	assertEqualsM(strcmp(token, "http://www.facebook.com/login.php") != 0, 1,
		      "Le token n'a pas pu être récupéré, tous les tests sont invalides");
}

TEST(testRandomFrequency)
{
	int freq = getRandomFrequency();
	assertEqualsM(freq >= 1 && freq <= 720, 1, 
		      "Fréquence générée incorrecte");
}

TEST(testNotAlreadyFriend)
{
	struct facebookFriends fb = getFakeFriends();
	char id[20] = "54";
	
	assertEqualsM(isAlreadyFriend(fb, id), 0, 
		      "L'ami ne devrait pas être trouvé");
		
	free(fb.friends);
}

TEST(testAlreadyFriend)
{
	struct facebookFriends fb = getFakeFriends();
	char id[20] = "42";
	
	assertEqualsM(isAlreadyFriend(fb, id), 1, 
		      "L'ami devrait être trouvé");
		
	free(fb.friends);	
}

TEST(testRandomQuestion)
{
	struct facebookFriends fb = getFakeFriends();
	struct relationship rel = getRandomRelationship(fb);
	
	assertEqualsM(isAlreadyFriend(fb, rel.id1) && isAlreadyFriend(fb, rel.id2), 1, 
		      "L'un des ID est faux !");
		
	free(fb.friends);
}

TEST(testGetFbPerson)
{
	char id[20] = "1318575395";
	struct Personne pers = getFacebookPerson(id);
	
	assertEqualsM(pers.nom != NULL && pers.id != NULL, 1, 
		      "La personne n'est pas créé correctement");
	assertEqualsM(pers.freq > 1 && pers.freq < 720, 1,
		      "La fréquence n'est pas correcte");
}

TEST(testFacebookQuestion)
{
	struct facebookRelationships quest = getFakeRelationships(getFakeFriends(), 3);
	
	assertEqualsM(quest.nbRel == 3, 1,
		      "La génération de question est fausse");
}

TEST(testWriteFbRel)
{
	FILE * fichier = tmpfile();
	struct facebookFriends fb = getFakeFriends();
		
	int fileSize, nbRel;
	char *relationshipsList, *relToken;
	
	writeFacebookRelationships(fb, token, fichier, 0);
	
	fileSize = ftell(fichier);
	rewind(fichier);

	relationshipsList = malloc(fileSize * sizeof(char));
	fgets(relationshipsList, fileSize, fichier);
	fclose(fichier);
	
	relToken = strtok(relationshipsList, "}");
	nbRel = 0;
	
	while (relToken != NULL)
	{
		nbRel++;
		relToken = strtok(NULL, "}");
	}
	
	assertEqualsM(nbRel, 10,
		      "Le nombre de relation obtenu est faux !");
	free(relationshipsList);
	free(relToken);
	free(fb.friends);
}

/* Ce test est basé sur le fait que Ben, Yann et Mathieu sont toujours amis :D */
TEST(testFacebookRelationships)
{
	struct facebookFriends fb = getFakeFriends();
	struct facebookRelationships rel;
	
	rel = getFacebookRelationships(fb, token);
	
	assertEqualsM(rel.nbRel, 3,
		      "Le nombre de relation est incorrect !");
	free(fb.friends);
}

TEST(testFriendOfFriends)
{
	struct facebookFriends fb = getFakeFriends();
	fb = getFriendsOfFriends(fb, 10);
	
	assertEqualsM(fb.nbFriends > 5, 1,
		      "Les amis n'ont pas été récupérés correctement ? Est-on bien connecté ?");
	free(fb.friends);
	remove("cookies_auth.txt");
}
