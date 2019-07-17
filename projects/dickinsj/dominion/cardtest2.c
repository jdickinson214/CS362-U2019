/*
 * shuffle test
 *
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "shuffle"


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

	int handCards = 5;


	int numPlayers = 2;


    int i, j, numCoppers, numEstates;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;

    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);
	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1: choice1 = 1 discard estate card and gain 4 coins--------------
	printf("TEST Checking if sorted and if all cards are accounted for.\n");

	

	//to ensure that each card is unique
	int deckCount = G.deckCount[thisPlayer];
	for (i = 0; i < deckCount; i++){
		G.deck[thisPlayer][i] = k[i];
	}
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	

	//perform shuffle
	shuffle(thisPlayer, testG);


	//first check if count is the same
	printf("did deckCount change? %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
	assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], count, passed);


	if (deckCount == testG.deckCount[thisPlayer]){	//will not perform if deckCount fails.

		int afterDeck[deckCount];

		for (i = 0; i < deckCount; i++){
			afterDeck[j] = testG.deck[thisPlayer][i];
		}

		int matching = 0;
		int inOrder = 0;

		for (i = 0; i < deckCount; i++){
			for (j = 0; j < deckCount; j++){
				if (testG.deck[thisPlayer][i] == G.deck[thisPlayer][j]){
				
					matching++; //if a common card is found
				
					if (i == j){
						inOrder++;	//if the common card is at the same position originally
					}				//if this == deckCount, then nothing has been shuffled
				}
			}
		}

		printf("Checking if all cards before match after shuffle\n");
		assertTrue(matching == deckCount, count, passed);

		printf("Making sure the cards are in a different order\n");
		assertTrue(inOrder != deckCount, count, passed);

	}
	else{

		printf("Checking if all cards before match after shuffle\n");
		assertTrue(-1, count, passed);

		printf("Making sure the cards are in a different order\n");
		assertTrue(-1, count, passed);

	}


	printf("Test Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest2 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}