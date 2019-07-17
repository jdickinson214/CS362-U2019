/*
 * initializeGame test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "initializeGame"


void assertTrue(int expression, int *count, int *passed) {
	if (expression == 1) {
		(*count)++;
		(*passed)++;
		printf("Passed Test\n");
	}
	else{
		(*count)++;
		printf("Failed Test\n");
	}
}

int curseCount (int numPlayers){
	if (numPlayers == 2){
		return 10;
	}
	else if (numPlayers == 3){
		return 20;
	}
	else {
		return 30;
	}
}

int victoryCount (int numPlayers) {
	if (numPlayers == 2){
		return 8;
	}
	else {
		return 12;
	}
	
}





int main() {
	int countTotal = 0;
	int passedTotal = 0;
	int counter = 0;
	int pass = 0;
	int *count = &counter;
	int *passed = &pass;

	int handCards = 5;




    int i, j, numCoppers, numEstates;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;

    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1: choice1 = 1 discard estate card and gain 4 coins--------------
	printf("TEST 1: number of players, checks 2, 3, and 4 player options and supply decks\n");

	// copy the game state to a test case


	int numPlayers;

	for(numPlayers = 2; numPlayers < 5; numPlayers++){

		initializeGame(numPlayers, k, seed, &G);
		memcpy(&testG, &G, sizeof(struct gameState));
		

		printf("# of players = %d, expected = %d\n", testG.numPlayers, numPlayers);
		assertTrue(testG.numPlayers == numPlayers, count, passed);

		printf("# of curse cards = %d, expected = %d\n", testG.supplyCount[curse], curseCount(numPlayers));
		assertTrue(testG.supplyCount[curse] == curseCount(numPlayers), count, passed);

		printf("# of victory cards \n");
		printf("estate cards = %d, expected = %d\n", testG.supplyCount[estate], victoryCount(numPlayers));
		assertTrue(testG.supplyCount[estate] == victoryCount(numPlayers), count, passed);
		printf("duchy cards = %d, expected = %d\n", testG.supplyCount[duchy], victoryCount(numPlayers));
		assertTrue(testG.supplyCount[duchy] == victoryCount(numPlayers), count, passed);
		printf("province cards = %d, expected = %d\n", testG.supplyCount[province], victoryCount(numPlayers));
		assertTrue(testG.supplyCount[province] == victoryCount(numPlayers), count, passed);
		
		numCoppers = 60-(7*numPlayers);
		
		printf("copper cards = %d, expected = %d\n", testG.supplyCount[copper], numCoppers);
		assertTrue(testG.supplyCount[province] == curseCount(numPlayers), count, passed);
	
		numCoppers = 0;
		numEstates = 0;

		for (i = 0; i < numPlayers; i++){
			for (j = 0; j < 10; j++){
				if (testG.deck[i][j] == copper){
					numCoppers++;
				}
				else if (testG.deck[i][j] == estate){
					numEstates++;
				}
				else{
					printf("Incorrect starting card in players %d's hand position %d\n", i, j);
					assertTrue(testG.deck[i][j] == copper || testG.deck[i][j] == estate, count, passed);
				}

				if (j == 9){
					printf("Player %d's estates = %d, expected = 3\n", i, numEstates);
					assertTrue(numEstates == 3, count, passed);

					printf("Player %d's coppers = %d, expected = 7\n", i, numEstates);
					assertTrue(numCoppers == 7, count, passed);
				}
			}
		}



	printf("Test 1 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;




	// ----------- TEST 2: choice1 = 1 but player does not have an estate --------------
	printf("TEST 2: first players turn initialized correctly\n");

	counter = 0;
	pass = 0;

	int numAct = 1;
	int numBuy = 1;
	int player1 = 0;


	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	
	printf("Whose turn is it? [0 = p1] player = %d, expected = %d\n", testG.whoseTurn, player1);
	assertTrue(testG.whoseTurn == player1, count, passed);

	printf("# of cards in hand = %d, expected = %d\n", testG.handCount[0], handCards);
	assertTrue(testG.numPlayers == handCards, count, passed);

	printf("# of actions = %d, expected = %d\n", testG.numActions, numAct);
	assertTrue(testG.numActions == numAct, count, passed);

	printf("# of buys = %d, expected = %d\n", testG.numBuys, numBuy);
	assertTrue(testG.numBuys == numBuy, count, passed);



	printf("Test 2 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest1 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}