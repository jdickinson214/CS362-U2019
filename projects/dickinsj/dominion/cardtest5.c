/*
 * drawCard test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "drawCard"


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


	int numPlayers = 4;


    int i;

    int seed = 1000;

    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);
	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1:--------------
	printf("TEST 1: drawCard when deck is full, and game initialized \n");


	//to ensure that each card is unique
	for (i = 0; i < G.deckCount[thisPlayer]; i++){
		G.deck[thisPlayer][i] = k[i];
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	drawCard(thisPlayer, &testG);

	printf("deckCount = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
	assertTrue( testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - 1, count, passed);


	printf("handCount = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	assertTrue( testG.handCount[thisPlayer] == G.handCount[thisPlayer] + 1, count, passed);

	printf("card in hand = %d, card on top of deck before draw = %d\n", testG.hand[thisPlayer][testG.handCount[thisPlayer]-1], G.deck[thisPlayer][G.deckCount[thisPlayer] - 1]);
	assertTrue( testG.hand[thisPlayer][testG.handCount[thisPlayer]-1] == G.deck[thisPlayer][G.deckCount[thisPlayer] - 1], count, passed);


	printf("Test 1 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;

	// ----------- TEST 2: --------------
	printf("TEST 2: drawCard when deck is empty \n");
	pass = 0;
	counter = 0;



	while (G.deckCount[thisPlayer] >= 1){
		G.discard[thisPlayer][G.discardCount[thisPlayer]] = G.deck[thisPlayer][G.deckCount[thisPlayer]-1];
		G.deckCount[thisPlayer]--;
		G.discardCount[thisPlayer]++;
	}


	memcpy(&testG, &G, sizeof(struct gameState));

	drawCard(thisPlayer, &testG);

	printf("deckCount = %d, expected = %d\n", testG.deckCount[thisPlayer], G.discardCount[thisPlayer]-1);
	assertTrue( testG.deckCount[thisPlayer] == G.discardCount[thisPlayer]-1, count, passed);


	printf("handCount = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	assertTrue( testG.handCount[thisPlayer] == G.handCount[thisPlayer] + 1, count, passed);


	printf("Test 2 Results: Passed %d tests out of %d\n\n", *passed, *count);
	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest5 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}