/*
 * Minion card unit test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Minion"


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
    int xtraAct = 1;
    int xtraCoins = 2;
    int newCards = 0;
    int shuffledCards = 0;

    int i;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);




	// ----------- TEST 1--------------
	printf("TEST 1: choice1 = 1 gains 2 coins\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

	discarded = 1;
	newCards = 0;
	xtraCoins = 2;
	printf("Action count = %d, expected = %d\n", testG.numActions, G.numActions + xtraAct);
	assertTrue(testG.numActions == G.numActions + xtraAct, count, passed);
	
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);
	
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards, count, passed);
	
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

	printf("Test 1 Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += *count;
	passedTotal += *passed;



	// ----------- TEST 2 --------------
	printf("TEST 2: choice1 = 0; choice2 = 1 player discards hand and redraws 4\n");
	printf("other players with more that 4 cards discard their hand and redraw 4\n");
	
	counter = 0;
	pass = 0;


	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	choice2 = 1;
	cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

	discarded = 5; //discards entire hand
	newCards = 4;
	xtraCoins = 0;
	printf("Action count = %d, expected = %d\n", testG.numActions, G.numActions + xtraAct);
	assertTrue(testG.numActions == G.numActions + xtraAct, count, passed);
	
	printf("Current Player's hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);
	
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards, count, passed);
	
	printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	assertTrue(testG.coins == G.coins + xtraCoins, count, passed);

	for (i = 0; i < numPlayers; i++){
		if (i != thisPlayer){
			if (G.handCount[i] > 4){
				printf("Player %d's hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i] + newCards - discarded);
				assertTrue(testG.handCount[i] == G.handCount[i] + newCards - discarded, count, passed);
			}
			else{
				printf("Player %d's hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i]);
				assertTrue(testG.handCount[i] == G.handCount[i], count, passed);
			}
		}
	}

	printf("Test 2 Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += *count;
	passedTotal += *passed;

	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> unittest 2 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);


	return 0;
}

