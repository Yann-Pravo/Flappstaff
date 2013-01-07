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

#include "cu/cu.h"

TEST(testExportSCC);
TEST(testExportSCCImportant);
TEST(testNbSCC);
TEST(testExportAnswers);
TEST(testExportGraph);
TEST(testExportGraphImportant);

TEST_SUITE(ExportGraph)
{
	TEST_ADD(testExportSCC),
	TEST_ADD(testExportSCCImportant),
	TEST_ADD(testNbSCC),
	TEST_ADD(testExportAnswers),
	TEST_ADD(testExportGraph),
	TEST_ADD(testExportGraphImportant),
	TEST_SUITE_CLOSURE
};