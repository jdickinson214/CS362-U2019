/*
 * endTurn test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "endTurn"


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




int main() {
	int countTotal = 0;
	int passedTotal = 0;
	int counter = 0;
	int pass = 0;
	int *count = &counter;
	int *passed = &pass;

	int i = 0;

	int handCount = 5;

	int numPlayers = 4;

	int xtraCoins = 0;

	int numAct = 1;

	int numBuy = 1;

    int seed = 1000;

    int thisPlayer = 0;
	int nextPlayer = thisPlayer+1;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);
	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1:--------------
	printf("TEST: checking whose turn, their hand count, coins, actions, buys\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));




	for (thisPlayer = 0; thisPlayer < numPlayers; thisPlayer++){
		endTurn(&testG);

		if (thisPlayer < numPlayers-1){
			nextPlayer = thisPlayer + 1;
		}
		else{
			nextPlayer = 0;
		}

		xtraCoins = 0;

		for (i = 0; i < testG.handCount[nextPlayer]; i++){
			if (testG.hand[nextPlayer][i] == copper){
				xtraCoins++;
			}
		}

		printf("Player %d's Turn = %d, expected = %d\n", nextPlayer, testG.whoseTurn, nextPlayer);
		assertTrue(testG.whoseTurn == nextPlayer, count, passed);

		printf("Player %d's HandCount = %d, expected = %d\n", nextPlayer, testG.handCount[testG.whoseTurn], handCount);
		assertTrue(testG.handCount[testG.whoseTurn] == handCount, count, passed);

		printf("Player %d's coins = %d, expected = %d\n", nextPlayer, testG.coins, xtraCoins);
		assertTrue(testG.coins == xtraCoins, count, passed);

		printf("# of actions = %d, expected = %d\n", testG.numActions, numAct);
		assertTrue(testG.numActions == numAct, count, passed);

		printf("# of buys = %d, expected = %d\n", testG.numBuys, numBuy);
		assertTrue(testG.numBuys == numBuy, count, passed);

		endTurn(&G);

	}





	printf("Testing Results: Passed %d tests out of %d\n\n", *passed, *count);
	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest3 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}
