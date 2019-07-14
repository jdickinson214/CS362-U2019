/*
 * tribute card unit test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Tribute"


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


enum CARD cardTypeIterator(int x){
	if (x == 0){
		return village;
	}
	else if (x == 1){
		return copper;
	}
	else{
		return estate;
	}
}



int main() {
	int counter = 0;
	int pass = 0;
	int *count = &counter;
	int *passed = &pass;
	int countTotal = 0;
	int passedTotal = 0;

    int xtraAct = 0;
    int xtraCoins = 0;
    int newCards = 0;

    int i, j;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
	int nextPlayer = 1;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


	// ----------- TEST 1--------------
	printf("TEST: iterate through possible combinations of top two cards in nextPlayer's pile \n");



	// copy the game state to a test case
	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			if (i == 0){
				xtraAct = 2;
			}
			if (i == 1) {
				xtraCoins = 2;
			}
			if (i == 2) {
				newCards = 2;
			}
			if (j == 0 && i != 0){
				xtraAct = 2;
			}
			if (j == 1 && i != 1) {
				xtraCoins = 2;
			}
			if (j == 2 && i != 2) {
				newCards = 2;
			}

			memcpy(&testG, &G, sizeof(struct gameState));
			testG.deck[nextPlayer][testG.deckCount[nextPlayer]-1] = cardTypeIterator(i);
			testG.deck[nextPlayer][testG.deckCount[nextPlayer]-2] = cardTypeIterator(j);

			cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

			printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);
			assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards, count, passed);

			printf("action count = %d, expected = %d\n", testG.numActions, G.numActions + xtraAct);
			assertTrue(testG.numActions == G.numActions + xtraAct, count, passed);

			printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
			assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

		}
	}


	printf("Test Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> unittest 3 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}

