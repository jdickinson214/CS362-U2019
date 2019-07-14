/*
 * Baron card unit test
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Baron"


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

    int discarded = 1;
    int xtraBuys = 1;
    int xtraCoins = 0;
    int newCards = 0;
    int shuffledCards = 0;

    int i;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);




	// ----------- TEST 1: choice1 = 1 discard estate card and gain 4 coins--------------
	printf("TEST 1: choice1 = 1 discard estate and gain 4 coins\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	testG.hand[thisPlayer][1] = estate;
	cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

	discarded = 2;
	newCards = 0;
	xtraCoins = 4;
	printf("buy count = %d, expected = %d\n", testG.numBuys, G.numBuys + xtraBuys);
	assertTrue(testG.numBuys == G.numBuys + xtraBuys, count, passed);
	
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);
	
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards, count, passed);
	
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

	printf("Test 1 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;




	// ----------- TEST 2: choice1 = 1 but player does not have an estate --------------
	printf("TEST 2: choice1 = 1 but player does not have an estate\n");

	counter = 0;
	pass = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	i = 0;
	//set hand to all coppers, to ensure no estate cards are drawn
	while (i < testG.handCount[thisPlayer]){
		testG.hand[thisPlayer][i] = copper;
		i++;
	}
	cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

	int numEstates = testG.supplyCount[estate];
	discarded = 1;
	int estateCardGained = 1;
	xtraCoins = 0;

	printf("buy count = %d, expected = %d\n", testG.numBuys, G.numBuys + xtraBuys);
	assertTrue(testG.numBuys == G.numBuys + xtraBuys, count, passed);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);	

	printf("estate supply count = %d, expected = %d\n", numEstates, G.supplyCount[estate] - estateCardGained);
	assertTrue(numEstates == G.supplyCount[estate] - estateCardGained, count, passed);
		
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

	printf("Test 2 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;




	// ----------- TEST 3: choice1 = 0, player chooses not to discard estate, gains estate --------------

	printf("TEST 3: choice1 = 0, player chooses not to discard estate, gains estate\n");

	counter = 0;
	pass = 0;


	choice1 = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

	numEstates = testG.supplyCount[estate];
	discarded = 1;
	estateCardGained = 1;
	xtraCoins = 0;

	printf("buy count = %d, expected = %d\n", testG.numBuys, G.numBuys + xtraBuys);
	assertTrue(testG.numBuys == G.numBuys + xtraBuys, count, passed);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);	

	printf("estate supply count = %d, expected = %d\n", numEstates, G.supplyCount[estate] - estateCardGained);
	assertTrue( numEstates == G.supplyCount[estate] - estateCardGained, count, passed);
		
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

	printf("Test 2 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;



	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> unittest 1 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}




/*
	// cycle through each eligible combination of 2 cards to trash
	for (i=1; i<G.handCount[thisPlayer]; i++) {
		for (j=i+1; j<G.handCount[thisPlayer]; j++) {

			G.hand[thisPlayer][0] = steward;
			G.hand[thisPlayer][1] = copper;
			G.hand[thisPlayer][2] = duchy;
			G.hand[thisPlayer][3] = estate;
			G.hand[thisPlayer][4] = feast;

			// copy the game state to a test case
			memcpy(&testG, &G, sizeof(struct gameState));

			printf("starting cards: ");
			for (m=0; m<testG.handCount[thisPlayer]; m++) {
				printf("(%d)", testG.hand[thisPlayer][m]);
			}
			printf("; ");

			choice2 = j;
			choice3 = i;
			remove1 = testG.hand[thisPlayer][i];
			remove2 = testG.hand[thisPlayer][j];
			cardEffect(steward, choice1, choice2, choice3, &testG, handpos, &bonus);

			printf("removed: (%d)(%d); ", remove1, remove2);
			printf("ending cards: ");

			// tests that the removed cards are no longer in the player's hand
			for (m=0; m<testG.handCount[thisPlayer]; m++) {
				printf("(%d)", testG.hand[thisPlayer][m]);
				assert(testG.hand[thisPlayer][m] != remove1);
				assert(testG.hand[thisPlayer][m] != remove2);
			}
			printf(", expected: ");
			for (m=1; m<G.handCount[thisPlayer]; m++) {
				if (G.hand[thisPlayer][m] != G.hand[thisPlayer][i] && G.hand[thisPlayer][m] != G.hand[thisPlayer][j]) {
					printf("(%d)", G.hand[thisPlayer][m]);
				}
			}
			printf("\n");

			// tests for the appropriate number of remaining cards
			newCards = 0;
			xtraCoins = 0;
			discarded = 3;
			if (i==1 && j==2) {
				printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
				printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
			}
			assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
			assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
		}

	}*/