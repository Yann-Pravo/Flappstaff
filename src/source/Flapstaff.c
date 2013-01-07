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
#include <unistd.h>
#include "../headers/Graphe.h"
#include "../headers/ParseFile.h"
#include "../headers/TPersonne.h"
#include "../headers/FacebookLib.h"
#include "../headers/ExportGraph.h"

/*
*	Ecrit un fichier formaté pour le programme à partir d'un ID facebook
*	et du token d'authentification correspondant
*
*	@param token, token d'auth Facebook
*	@param facebookID, ID facebook de la personne dont on a l'auth
*	@param fileName, nom du fichier dans lequel on veut écrire le résultat
*	@param nbFriends, nombre approx d'amis qu'on souhaite avoir
*	@param nbQuest, nombre de question que l'on souhaite avoir
*	@param fake, nombre de relations à génerer entre les personnes.
*	       Si fake = 0, elles proviendront de Facebook.
*/
void writeFacebookFile(char* token, char* facebookID, char* fileName, int nbFriends, int nbQuest, int fake)
{	
	int i = 0;
	FILE * fichier = fopen(fileName, "w+");
	char line[100];

	struct facebookFriends fbFriends;
	struct facebookRelationships fbRel;
	struct facebookRelationships fbQuestions;
	
	printf("Récuperation de vos amis\n");
	fbFriends = getFacebookFriends(facebookID, token);
	printf("%d amis récupérés !\n", fbFriends.nbFriends);
	
	if (fbFriends.nbFriends < nbFriends)
	{
		printf("Récuperation des amis de vos amis\n");
		fbFriends = getFriendsOfFriends(fbFriends, nbFriends);
		printf("%d amis au total ont été récupéré\n", fbFriends.nbFriends);
	}
	
	if (fake != 0)
	{
		printf("Génération aléatoire de relations\n");
		fbRel = getFakeRelationships(fbFriends, fake);
		printf("%d relations générées\n", fbRel.nbRel);
	}
	else
	{
		printf("Récuperation des relations entre toutes les personnes\n");
		fbRel = getFacebookRelationships(fbFriends, token);
		printf("Il existes %d relations entre toutes les personnes\n", fbRel.nbRel);
	}
	
	printf("Génération aléatoire de questions\n");
	fbQuestions = getFakeRelationships(fbFriends, nbQuest);
	printf("%d questions générées\n", fbQuestions.nbRel);
	
	printf("Début de l'écriture du fichier\n");
	
	sprintf(line, "%d\n", fbFriends.nbFriends);
	fputs(line, fichier);

	for(i = 0; i < fbFriends.nbFriends; ++i)
	{
		sprintf(line, "%s, %s, %d\n", fbFriends.friends[i].nom, 
					      fbFriends.friends[i].id, 
					      fbFriends.friends[i].freq);
		fputs(line, fichier);
	}
	
	if (fake != 0)
	{
		/* On Multiplie par 2 parce-que les relations de 
		Facebook sont dans les 2 sens */
		sprintf(line, "%d\n", fbRel.nbRel);
		fputs(line, fichier);

		for(i = 0; i < fbRel.nbRel; ++i)
		{
			sprintf(line, "%s, %s\n", fbRel.relationships[i].id1, 
						  fbRel.relationships[i].id2);
			fputs(line, fichier);
		}
	}
	else
	{
		/* On Multiplie par 2 parce-que les relations de 
		Facebook sont dans les 2 sens */
		sprintf(line, "%d\n", fbRel.nbRel * 2);
		fputs(line, fichier);

		for(i = 0; i < fbRel.nbRel; ++i)
		{
			sprintf(line, "%s, %s\n", fbRel.relationships[i].id1, 
						  fbRel.relationships[i].id2);
			fputs(line, fichier);

			/* On l'écrit dans l'autre sens ! */
			sprintf(line, "%s, %s\n", fbRel.relationships[i].id2, 
						  fbRel.relationships[i].id1);
			fputs(line, fichier);
		}
	}

	sprintf(line, "%d\n", fbQuestions.nbRel);
	fputs(line, fichier);

	for(i = 0; i < fbQuestions.nbRel; ++i)
	{
		sprintf(line, "%s -> %s\n", fbQuestions.relationships[i].id1, 
					    fbQuestions.relationships[i].id2);
		fputs(line, fichier);
	}
	
	printf("Fin de l'écriture du fichier\n");
	
	free(fbQuestions.relationships);
	free(fbRel.relationships);
	free(fbFriends.friends);
	
	remove("cookies_auth.txt");

	fclose(fichier);
}

void facebookGen(char *mail, char *pass, char *id, char *fileName, int nbFriends, int nbQuest, int fake)
{
	char *token = facebookConnect(mail, pass);
	
	srand(clock());
	
	if (strcmp(token, "http://www.facebook.com/login.php") != 0)
	{
		writeFacebookFile(token, id, fileName, nbFriends, nbQuest, fake);
		printf("Done\n");
	}
	else
	{
		printf("FAIL\n");
	}
	
	/* Le free(token) aurait été necessaire, mais semble causer une 
	   segfault sous Linux */
}

int main (int argc, char *argv[])
{
	if(argc == 8 || argc == 7);
	if(argc == 7 || argc == 6)
	{
		char *pass = getpass("Mot de passe Facebook : ");
		if (argc == 6)
		{
			facebookGen(argv[1], pass, argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), 0);
		}
		else
		{
			facebookGen(argv[1], pass, argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
		}
	}
	else if(argc == 4 || argc == 3)
	{
		int i;
		FILE *fichier = fopen(argv[1], "r");
		
		if (fichier == NULL)
		{
			printf("Le fichier source n'existe pas !\n");
		}
		else
		{
			struct Graph myGraph = genFromFile(fichier);

			if (argc == 3)
			{
				exportGraph(myGraph, argv[2], 0);
			}
			else
			{
				exportGraph(myGraph, argv[2], atoi(argv[3]));
			}

			free(myGraph.tabPersonnes);
			free(myGraph.questions);

			for(i = 0; i < myGraph.nbPersonnes; ++i)
			{
				free(myGraph.matrix[i]);
			}

			free(myGraph.matrix);
			
			fclose(fichier);
			
			printf("Done\n");
		}
	}
	else
	{
		printf("Usage:\n%s email idFacebook fichierDestination nbAmis nbQuestions [fakeRelations]\n\
%s fichierSource fichierDest [importantPersons]\n\
Utilisez la première commande pour générer un fichier à partir de Facebook. L'ID doit coresspondre au compte donné (email + pass).\n\
Utilisez la seconde commande pour obtenir les informations relatives au fichier source dans le fichier de destination.\n\
Choisissez de mettre importantPersons à 1 si vous souhaitez qu'elles soient trouvées.\n", argv[0], argv[0]);
	}
		
	return 0;
}
