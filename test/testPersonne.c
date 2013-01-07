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
#include "../src/headers/TPersonne.h"
#include "cu/cu.h"

TEST(testCreatePerson)
{
	struct Personne pers;
	
	pers = createPersonne("42", "Douglas Adams", 24);
	
	assertEqualsM((strcmp(pers.nom, "Douglas Adams") == 0) && 
		      (strcmp(pers.id, "42") == 0) &&
		      (pers.freq == 24), 1, "Personne mal créée");
}

TEST(testPosition)
{
	struct Personne *friends = malloc(3 * sizeof(struct Personne));
	
	friends[0] = createPersonne("467", "Charles Bukowski", 95);
	friends[1] = createPersonne("42", "Douglas Adams", 24);
	friends[2] = createPersonne("4789", "Chuck Palahniuk", 456);
	
	assertEqualsM(position("42", friends), 1, "Position fausse");
	
	free(friends);
}