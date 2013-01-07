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
#include "testFacebookLib.h"
#include "testParseFile.h"
#include "testPersonne.h"
#include "testGraphe.h"
#include "testExportGraph.h"
#include "../src/headers/FacebookLib.h"
#include "../src/headers/ParseFile.h"
#include "../src/headers/Graphe.h"
#include "../src/headers/TPersonne.h"
#include "../src/headers/ExportGraph.h"

TEST_SUITES
{
	TEST_SUITE_ADD(FacebookLib),
	TEST_SUITE_ADD(ParseFile),
	TEST_SUITE_ADD(Personne),
	TEST_SUITE_ADD(Graphe),
	TEST_SUITE_ADD(ExportGraph),
	TEST_SUITES_CLOSURE
};

int main (int argc, char *argv[])
{
	CU_SET_OUT_PREFIX("results/");
	CU_RUN(argc, argv);
	
	return 0;
}