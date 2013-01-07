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
void writeFacebookFile(char* token, char* facebookID, char* fileName, int nbFriends, int nbQuest, int fake);

void facebookGen(char *mail, char *pass, char *id, char *fileName, int nbFriends, int nbQuest, int fake);