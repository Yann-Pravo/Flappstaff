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

TEST(testParsePerson);
TEST(testParseRelation);
TEST(testParseQuestion);
TEST(testParseAllPersons);
TEST(testParseAllRelations);
TEST(testParseAllQuestions);
TEST(testGenFromFile);

TEST_SUITE(ParseFile)
{
	TEST_ADD(testParsePerson),
	TEST_ADD(testParseRelation),
	TEST_ADD(testParseQuestion),
	TEST_ADD(testParseAllPersons),
	TEST_ADD(testParseAllRelations),
	TEST_ADD(testParseAllQuestions),
	TEST_ADD(testGenFromFile),
	TEST_SUITE_CLOSURE
};