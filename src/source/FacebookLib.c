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

#include "../headers/FacebookLib.h"
#include "../headers/TPersonne.h"

/*
*	Retourne une fréquence aléatoire pour la connexion au réseau
*
*	@return un nombre entre 720 et 1
*/
int getRandomFrequency()
{	
	return (rand() % 720) + 1;
}

/*
*	Trouve 2 ID au hasard pour chercher une mise en relation
*	
*	@param friends, la liste d'amis + son nombre
*	@return question de structure relationship (2 ID)
*/
struct relationship getRandomRelationship(struct facebookFriends friends)
{
	struct relationship question;

	strcpy(question.id1, friends.friends[(rand() % friends.nbFriends)].id);	
	strcpy(question.id2, friends.friends[(rand() % friends.nbFriends)].id);
	
	return question;
}

/*
*	Permet de regarder si un ID est déjà présent dans la liste d'amis
*	pour éviter les doublons lors de l'insertion d'une nouvelle liste
*	Test: testAlreadyFriend
*
*	@param friends, les amis + leur nombre
*	@param fbID, ID facebook
*	@return bool, 1 si il y est, 0 sinon
*/
int isAlreadyFriend(struct facebookFriends friends, char fbID[20])
{
	int i = 0;
	int found = 0;
	
	while (i < friends.nbFriends && !found)
	{
		found = (strcmp(friends.friends[i].id, fbID) == 0);
		i++;
	}
	
	return found;
}

/*
*	Crée une personne à partir de son ID Facebook
*	Permet par exemple de retrouver le nom de quelqu'un
*	
*	@param, id, ID facebook d'une personne
*	@return une personne avec nom, id et fréquence
*/
struct Personne getFacebookPerson(char id[20])
{
	CURL *curl;
	CURLcode res;
	char* nom = malloc(50*sizeof(char));
	char urlQuery[50];
	FILE * fichier = tmpfile();
	struct Personne pers;
	
	sprintf(urlQuery, "http://graph.facebook.com/%s", id);

  	curl = curl_easy_init();

    	curl_easy_setopt(curl, CURLOPT_URL, urlQuery);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
	
    	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
		
	rewind(fichier);

	/* On lit le fichier qu'on vient d'écrire pour extraire le status */
	fscanf(fichier, "%*[^:]:%*[^:]: \"%[^\"]\"", nom);
	
	fclose(fichier);
	
	pers = createPersonne(id, nom, getRandomFrequency());
	free(nom);
	
	return pers;
}

/*
*	TODO: Optimiser la parcelisation des requêtes si possible (threads)
*
*	Calcul les correspondances necessaires entre les amis pour générer
*	une requête qu'on envoi sur Facebook pour savoir si 2 personnes
*	sont effectivement amis ou non. On écrit le résultat dans le fichier
*	donné en paramètre.
*	Est appelée par getFacebookRelationships
*
*	@param fbfriends, les amis et leur nombre
*	@param token, token d'identification Facebook
*	@param fichier, un fichier ouvert et où on peut écrire
*	@param start, id du tableau sur lequel on commence
*/
void writeFacebookRelationships(struct facebookFriends fbFriends, char *token, FILE * fichier, int start)
{
	CURL* curl;
	CURLcode res;
	char *postfields;
		
	int i, j;
	int nPos1 = 0;
	int nPos2 = 0;
	int nbRel = 0;

	/* Calcul du nombre de relation :
		Le nombre de relations total :
			fbFriends.nbFriends * (fbFriends.nbFriends - 1))/2
		Le nombre de relations pour M amis sur les N amis totaux :
			N-1 + N-1 +...+ N-M = M(2N - M - 1)/2
	
	   Si il y a plus de 250 000 relations, Facebook ne renvoit rien,
	   on limite donc le risque en découpant en morceaux. On ajoute 
	   le nombre d'amis aux 250 000 pour éviter le cas où on dépasse les 
	   250 000 (249 999 au dernier tour de boucle, on repart mais on 
	   dépassera si on ne prévoit pas une marge).
	*/

	char *ids2 = (char*) calloc(16 * (250000 + fbFriends.nbFriends), sizeof(char));
	char *ids1 = (char*) calloc(16 * (250000 + fbFriends.nbFriends), sizeof(char));
	
	printf("Préparation de la requête\n");
	
	/* Génération des ids et des correspondances */
	for(i = start; i < fbFriends.nbFriends && nbRel < 250000; i++)
	{
		for(j = i + 1; j < fbFriends.nbFriends; j++)
		{	
			nbRel++;
			nPos2 += sprintf(ids2 + nPos2, "%s,", fbFriends.friends[j].id);
			nPos1 += sprintf(ids1 + nPos1, "%s,", fbFriends.friends[i].id);
		}
	}
	
	postfields = malloc((200 + (((250000 + fbFriends.nbFriends) * 2) * 16)) * sizeof(char));
	
	printf("Fin de la préparation de la requête\n");
	
	sprintf(postfields, "&access_token=%s&uids1=%s&uids2=%s", token, ids1, ids2);
	
	free(ids1);
	free(ids2);
	
	printf("Envoi de la requête à Facebook\n");
	/* Envoi de la requête à Facebook _VERY_SLOW_ */
  	curl = curl_easy_init();
    	curl_easy_setopt(curl, CURLOPT_URL, "https://api.facebook.com/method/friends.areFriends?format=json");
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
    	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	free(postfields);
	
	if (i < fbFriends.nbFriends)
	{
		writeFacebookRelationships(fbFriends, token, fichier, i);
	}
}

/*
*	Cherche les amis pour un ID donné et les place dans un tableau
*	de structure Personne.
*
*	@param id, ID facebook de la personne dont on veut les amis
*	@param token, token corresspondant à l'ID pour avoir l'auth
*	@return les amis récupérés + leur nombre
*/
struct facebookFriends getFacebookFriends(char id[20], char* token)
{
	CURL *curl;
	CURLcode res;
	
	struct Personne pers;
	char *name = malloc(40 * sizeof(char));
	char *idPers = malloc(20 * sizeof(char));
	
	struct facebookFriends fbFriends;
	fbFriends.friends = malloc(sizeof(struct Personne));
	fbFriends.nbFriends = 0;

	char urlQuery[200];
	FILE * fichier = tmpfile();
	long fileSize;
	char *friendList, *friendToken;
	
	sprintf(urlQuery, "https://graph.facebook.com/%s/friends/?access_token=%s", id, token);

  	curl = curl_easy_init();

    	curl_easy_setopt(curl, CURLOPT_URL, urlQuery);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
	
    	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	/* On obtient la taille du fichier ! */
	fileSize = ftell(fichier);
	rewind(fichier);
	
	/* On stocke la ligne (!!) dans une variable string */
	friendList = malloc(fileSize * sizeof(char));
	fgets(friendList, fileSize, fichier);
	fclose(fichier);
	
	friendList[0] = '['; /* Remplace le premier { gênant pour extraire */
	friendToken = strtok(friendList, "}");
	
	while (friendToken != NULL)
	{
		if (sscanf(friendToken, "%*[^{]{\"name\":\"%[^\"]\",\"id\":\"%[^\"]", name, idPers) != -1)
		{
			pers = createPersonne(idPers, name, getRandomFrequency());

			fbFriends.friends[fbFriends.nbFriends] = pers;
			fbFriends.nbFriends += 1;

			fbFriends.friends = (struct Personne*) realloc(fbFriends.friends, 
				            (fbFriends.nbFriends + 1) * sizeof(struct Personne));
		}
		
		friendToken = strtok(NULL, "}");
	}
	
	/* On ajoute la personne originale */
	fbFriends.friends[fbFriends.nbFriends] = getFacebookPerson(id);
	fbFriends.nbFriends += 1;

	free(name);
	free(idPers);
	free(friendList);
	free(friendToken);

	return fbFriends;
}

/*
*	Extrait les amis d'un fichier HTML facebook donné
*	et les ajoute aux amis existants si ils ne le sont pas déjà
*	Test: testFriendOfFriends
*
*	@param fbFriends, amis existants + nombre
*	@param friendFile, string contenant la page HTML complète
*	@return amis existant + ceux récupérés et leur nombre
*/
struct facebookFriends extractFriendsFromHTML(struct facebookFriends fbFriends, char *friendFile)
{
	char idFriend[20], nameFriend[40];
	struct Personne pers;
	struct facebookFriends newFbFriends = fbFriends;
	
	regex_t rId;
	regex_t rName;
	regmatch_t matches[2];	
		
	int start = 0;			
	int result = 0;
	
	/* On prépare les regex pour utiliser l'ID et le nom */
	regcomp(&rId, "<div class=.\"ffriend clearfix.\" id=.\"f([0123456789]+).\"", REG_EXTENDED);
	regcomp(&rName, "class=.\"fname.\">([a-zA-Z \\0-9]+)<", REG_EXTENDED);

	while (result == 0)
	{
		/* On regarde si on voit un ID à partir du point précédent */
		result = regexec(&rId, friendFile + start, 2, matches, 0);

		if (result == 0)
		{
			/* On récupère la taille pour couper les bits en trop */
			int len = matches[1].rm_eo - matches[1].rm_so;

			/* On copie l'ID dans la variable et on coupe à la longueur */
			strncpy(idFriend, friendFile + start + matches[1].rm_so, len);
			idFriend[len] = 0;
			
			/* On avance dans le fichier jusqu'a la fin de l'ID */
			start += matches[1].rm_eo;
			
			/* Si l'ID n'est pas déjà présent, 
			on va chercher le nom et on ajoute */
			if (!isAlreadyFriend(newFbFriends, idFriend))
			{
				result = regexec(&rName, friendFile + start, 2, matches, 0);

				/* Si la personne a un nom trop long, on l'oubli... */
				if (result == 0 && (matches[1].rm_eo - matches[1].rm_so) < 40)
				{
					/* Même processus que l'ID */
					
					int len = matches[1].rm_eo - matches[1].rm_so;
					
					strncpy(nameFriend, friendFile + start + matches[1].rm_so, len);
					nameFriend[len] = 0;
					
					start += matches[1].rm_eo;
					
					/* On créée la personne et on l'ajoute ! */
					pers = createPersonne(idFriend, nameFriend, getRandomFrequency());
					newFbFriends.friends = (struct Personne*) realloc(newFbFriends.friends, 
								(newFbFriends.nbFriends + 1) * sizeof(struct Personne));
					newFbFriends.friends[newFbFriends.nbFriends] = pers;
					newFbFriends.nbFriends += 1;
				}	
			}
		}
	}
	
	regfree(&rId);
	regfree(&rName);
	
	return newFbFriends;
}

/*
*	Logge une personne avec ses identifiants Facebook
*	IL faudra qu'elle ait autorisé l'accès à Flapstaff sur Facebook
*	Calcul le token necessaire aux opérations de bases
*
*	@param mail, mail utilisé pour se connecter à Facebook
*	@param pass, password utilisé pour Facebook
*	@return token d'auth Facebook
*/
char* facebookConnect(char *mail, char *pass)
{
	CURL *curl;			
	CURLcode res;
	FILE * fichier = tmpfile();
	
	/* On se créée un faux header - de qualité - pour éviter de se faire ban */
	struct curl_slist *chunk = NULL;
	chunk = curl_slist_append(chunk, "'Accept: text/html, application/xhtml+xml, application/xml");
	chunk = curl_slist_append(chunk, "Accept-Charset: utf-8;q=1.0,*;q=0.1");
	chunk = curl_slist_append(chunk, "Accept-Language: en-gb, en");
	chunk = curl_slist_append(chunk, "Keep-Alive: 300");
	chunk = curl_slist_append(chunk, "Connection: keep-alive");
	chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-GB; rv:1.9.0.6) Gecko/2009020911 Ubuntu/8.10 (intrepid) Firefox/3");

	char *newURL = malloc(200 * sizeof(char));
	char *token = malloc(200 * sizeof(char));
	char *postfields = malloc(200 * sizeof(char));
	
	sprintf(postfields, "email=%s&pass=%s&persistent=1", mail, pass);
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_URL, "https://login.facebook.com/login.php?login_attempt=1");
	res = curl_easy_perform(curl);

	curl_easy_setopt(curl, CURLOPT_URL, "https://login.facebook.com/login.php?login_attempt=1");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies_auth.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies_auth.txt");
	res = curl_easy_perform(curl);
	
	curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/oauth/authorize?client_id=106303829413149&redirect_uri=http://www.facebook.com/connect/login_success.html&type=user_agent&display=popup");
	res = curl_easy_perform(curl);
	res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &newURL);
	
	fclose(fichier);
	curl_slist_free_all(chunk);
	curl_easy_cleanup(curl);
	free(postfields);
	
	sscanf(newURL, "%*[^#]#access_token=%s", token);
	
	return token;
}

/*
*	A partir d'une liste d'amis, récupère les amis de ceux-ci et les place
*	dans la liste d'amis en tant que Personne.
*	Necessite qu'une connexion à Facebook ait été établie avec 
*	facebookConnect().
*	Test : testFriendsOfFriends	
*
*	@param fbFriends, liste d'amis + nombre
*	@param nbMax, nombre approx d'amis souhaité
*	@return facebookFriends, liste d'amis complémentée par les amis d'amis
*/
struct facebookFriends getFriendsOfFriends(struct facebookFriends fbFriends, int nbMax)
{	
	CURL *curl;			
	CURLcode res;
	
	struct curl_slist *chunk = NULL;
	chunk = curl_slist_append(chunk, "'Accept: text/html, application/xhtml+xml, application/xml");
	chunk = curl_slist_append(chunk, "Accept-Charset: utf-8;q=1.0,*;q=0.1");
	chunk = curl_slist_append(chunk, "Accept-Language: en-gb, en");
	chunk = curl_slist_append(chunk, "Keep-Alive: 300");
	chunk = curl_slist_append(chunk, "Connection: keep-alive");
	chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-GB; rv:1.9.0.6) Gecko/2009020911 Ubuntu/8.10 (intrepid) Firefox/3");
	
	struct facebookFriends newFbFriends = fbFriends;
	FILE * fichier;
	char urlQuery[200];
	int i, fileSize;
	char *friendFile;
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies_auth.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies_auth.txt");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
	
	for(i = 0; i < fbFriends.nbFriends && newFbFriends.nbFriends < nbMax; i++)
	{
		sprintf(urlQuery, "http://www.facebook.com/friends/?id=%s", newFbFriends.friends[i].id);
		fichier = tmpfile();

		/* On récupère le fichier HTML et on le stocke dans un string */
		curl_easy_setopt(curl, CURLOPT_URL, urlQuery);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fichier);
	    	res = curl_easy_perform(curl);
	
		fileSize = ftell(fichier);
		rewind(fichier);
		
		friendFile = malloc(fileSize * sizeof(char));
		fread(friendFile, 1, fileSize, fichier);
		fclose(fichier);
		
		/* Si la page commence par un script, 
		la personne a protégée ses amis du public */
		if (strncmp("<script>", friendFile, 8) != 0)
		{
			newFbFriends = extractFriendsFromHTML(newFbFriends, friendFile);
		}	
	
		free(friendFile);
	}

	curl_slist_free_all(chunk);
	curl_easy_cleanup(curl);

	return newFbFriends;
}

/*
*	Récupère les relations entre des amis donnés à partir de Facebook
*	et retourne un tableau de relation.
*	Test : testFacebookRelationships
*
*	@param fbFriends, amis + nb
*	@param token, token d'auth Facebook
*	@return tableau de relation (2 ID)
*/
struct facebookRelationships getFacebookRelationships(struct facebookFriends fbFriends, char* token)
{	
	struct facebookRelationships fbRel;
	fbRel.relationships = malloc(sizeof(struct relationship));
	fbRel.nbRel = 0;
	
	long fileSize;
	char *relationshipsList, *relToken;
	char *id1 = malloc(20 * sizeof(char));
	char *id2 = malloc(20 * sizeof(char));
	char *status = malloc(5 * sizeof(char));
	FILE * fichier;
	
	if ((fbFriends.nbFriends * (fbFriends.nbFriends - 1))/2 > 250000)
	{
		printf("Plusieurs requêtes seront necessaires\n");
	}
	
	/* On envoi la requête de relation à Facebook 
	et on écrit le résultat dans un fichier */
	fichier = tmpfile();
	writeFacebookRelationships(fbFriends, token, fichier, 0);
	
	fileSize = ftell(fichier);
	rewind(fichier);

	relationshipsList = malloc(fileSize * sizeof(char));
	fgets(relationshipsList, fileSize, fichier);
	fclose(fichier);
	
	/* On sépare sur } pour découper les différentes relations */
	relToken = strtok(relationshipsList, "}");
	
	while (relToken != NULL)
	{
		if (sscanf(relToken, "%*[^{]{\"uid1\":%[^,],\"uid2\":%[^,],\"are_friends\":%s", id1, id2, status) != -1)
		{	
			if (strcmp(status, "true") == 0)
			{			
				strcpy(fbRel.relationships[fbRel.nbRel].id1, id1);
				strcpy(fbRel.relationships[fbRel.nbRel].id2, id2);
				fbRel.nbRel += 1;
									
				fbRel.relationships = 
				(struct relationship*) realloc(fbRel.relationships, 
				(fbRel.nbRel + 1) * (sizeof(struct relationship)));
			}
		}
		
		relToken = strtok(NULL, "}");
	}
	
	free(id1);
	free(id2);
	free(status);
	free(relationshipsList);
	free(relToken);
	
	return fbRel;
}

/*
*	Permet de générer des questions et des relations de manière aléatoire
*	
*	@param fbFriends, amis + nb
*	@param nb, nombre de relations voulues
*	@return tableau de relations (2 ID)
*/
struct facebookRelationships getFakeRelationships(struct facebookFriends fbFriends, int nb)
{
	int i;
	struct facebookRelationships fbRel;
	fbRel.relationships = malloc(nb * sizeof(struct relationship));
	fbRel.nbRel = nb;
	
	for(i = 0; i < nb; ++i)
	{
		fbRel.relationships[i] = getRandomRelationship(fbFriends);
	}
	
	return fbRel;
}