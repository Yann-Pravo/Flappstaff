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

struct facebookFriends getFakeFriends();

TEST(testFacebookConnect);
TEST(testRandomFrequency);
TEST(testNotAlreadyFriend);
TEST(testAlreadyFriend);
TEST(testRandomQuestion);
TEST(testGetFbPerson);
TEST(testFacebookQuestion);
TEST(testFacebookRelationships);
TEST(testWriteFbRel);
TEST(testFriendOfFriends);

TEST_SUITE(FacebookLib)
{
	TEST_ADD(testFacebookConnect),
	TEST_ADD(testRandomFrequency),
	TEST_ADD(testNotAlreadyFriend),
	TEST_ADD(testAlreadyFriend),
	TEST_ADD(testRandomQuestion),
	TEST_ADD(testGetFbPerson),
	TEST_ADD(testFacebookQuestion),
	TEST_ADD(testFacebookRelationships),
	TEST_ADD(testWriteFbRel),
	TEST_ADD(testFriendOfFriends),
	TEST_SUITE_CLOSURE
};