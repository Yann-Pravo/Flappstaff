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
#include <curl/curl.h>
#include <regex.h>

/*	
	Le but de cette librairie est notamment de combler le trou dans
	l'API de Facebook qui ne permet pas de récupérer les amis des amis.
	Vu qu'on ne souhaite pas non plus que n'importe-qui puisse récuperer
	les amis de n'importe-qui on impose le fait d'ouvrir la connexion
	avec son compte et on cherchera les amis d'amis à partir des amis
	trouvés sur ce compte.
*/

/*
*	Permet de gérer une liste d'amis et d'avoir le nombre d'amis au total
*/
struct facebookFriends
{
	struct Personne* friends;
	int nbFriends;
};

/*
*	Permet de gérer une liste de relation et son nombre d'élément
*/
struct facebookRelationships
{
	struct relationship* relationships;
	int nbRel;
};

/*
*	Retourne une fréquence aléatoire pour la connexion au réseau
*	Test : testRandomFrequency
*
*	@return un nombre entre 720 et 1
*/
int getRandomFrequency();

/*
*	Trouve 2 ID au hasard pour chercher une mise en relation
*	
*	@param friends, la liste d'amis + son nombre
*	@return question de structure relationship (2 ID)
*/
struct relationship getRandomRelationship(struct facebookFriends friends);

/*
*	Permet de regarder si un ID est déjà présent dans la liste d'amis
*	pour éviter les doublons lors de l'insertion d'une nouvelle liste
*	Test : testAlreadyFriend
*
*	@param friends, les amis + leur nombre
*	@param fbID, ID facebook
*	@return bool, 1 si il y est, 0 sinon
*/
int isAlreadyFriend(struct facebookFriends friends, char fbID[20]);

/*
*	Crée une personne à partir de son ID Facebook
*	Permet par exemple de retrouver le nom de quelqu'un
*	Test : testGetFbPerson	
*
*	@param, id, ID facebook d'une personne
*	@return une personne avec nom, id et fréquence
*/
struct Personne getFacebookPerson(char id[20]);

/*
*	TODO: Optimiser la parcelisation des requêtes si possible
*
*	Calcul les correspondances necessaires entre les amis pour générer
*	une requête qu'on envoi sur Facebook pour savoir si 2 personnes
*	sont effectivement amis ou non. On écrit le résultat dans le fichier
*	donné en paramètre.
*	Est appelée par getFacebookRelationships.
*	Test : testWriteFbRel
*
*	@param fbfriends, les amis et leur nombre
*	@param token, token d'identification Facebook
*	@param fichier, un fichier ouvert et où on peut écrire
*	@param start, id du tableau sur lequel on commence
*/
void writeFacebookRelationships(struct facebookFriends fbFriends, char *token, FILE *fichier, int start);

/*
*	Cherche les amis pour un ID donné et les place dans un tableau
*	de structure Personne.
*
*	@param id, ID facebook de la personne dont on veut les amis
*	@param token, token corresspondant à l'ID pour avoir l'auth
*	@return les amis récupérés + leur nombre
*/
struct facebookFriends getFacebookFriends(char id[20], char* token);

/*
*	Extrait les amis d'un fichier HTML facebook donné
*	et les ajoute aux amis existants si ils ne le sont pas déjà
*	Test : testFriendsOFriends
*
*	@param fbFriends, amis existants + nombre
*	@param friendFile, string contenant la page HTML complète
*	@return amis existant + ceux récupérés et leur nombre
*/
struct facebookFriends extractFriendsFromHTML(struct facebookFriends fbFriends, char* friendFile);

/*
*	A partir d'une liste d'amis, récupère les amis de ceux-ci et les place
*	dans la liste d'amis en tant que Personne.
*	Test : testFriendsOFriends
*	
*	@param fbFriends, liste d'amis + nombre
*	@param nbMax, nombre approx d'amis souhaité
*	@return facebookFriends, liste d'amis complémentée par les amis d'amis
*/
struct facebookFriends getFriendsOfFriends(struct facebookFriends fbFriends, int nbMax);

/*
*	TODO : Utiliser les threads pour effectuer les requêtes en parallèles
*	Récupère les relations entre des amis donnés à partir de Facebook
*	et retourne un tableau de relation.
*	Test: testFacebookRelationships
*
*	@param fbFriends, amis + nb
*	@param token, token d'auth Facebook
*	@return tableau de relation (2 ID)
*/
struct facebookRelationships getFacebookRelationships(struct facebookFriends fbFriends, char *token);

/*
*	Permet de générer des questions et des relations de manière aléatoire
*	
*	@param fbFriends, amis + nb
*	@param nb, nombre de relations voulues
*	@return tableau de relations (2 ID)
*/
struct facebookRelationships getFakeRelationships(struct facebookFriends fbFriends, int nb);

/*
*	Logge une personne avec ses identifiants Facebook
*	Il faudra qu'elle ait autorisé l'accès à Flapstaff sur Facebook
*	Calcul le token necessaire aux opérations de bases
*	Test : testFacebookConnect	
*
*	@param mail, mail utilisé pour se connecter à Facebook
*	@param pass, password utilisé pour Facebook
*	@return token d'auth Facebook
*/
char* facebookConnect(char *mail, char *pass);