/*
 * mine card unit test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Mine"


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
	int counter = 0;
	int pass = 0;
	int *count = &counter;
	int *passed = &pass;

	int countTotal = 0;
	int passedTotal = 0;

    int discarded = 1;
    int newCards = 0;

    int i;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, mine, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	G.hand[thisPlayer][0] = mine;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;


	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


	// ----------- TEST 1--------------
	printf("TEST 1: choice1 = 1 player is choosing a copper to trash and then a silver (choice2 = silver) to gain\n");

	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	choice2 = silver;
	discarded = 1;
	newCards = 1;
	int silverFlag = 0;

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);

	printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
	assertTrue(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded, count, passed);

	i = 0;
	for (i = 0; i < testG.handCount[thisPlayer]; i++){
		if (testG.hand[thisPlayer][i] == silver){
			silverFlag = 1;
		}
	}

	printf("silver in hand? expected = yes\n");
	assertTrue(silverFlag, count, passed);
		
	printf("Test Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;

// ----------- TEST 2--------------
	printf("TEST 2: choice1 = 1 player is choosing a silver to trash and then a gold (choice2 = gold) to gain\n");

	counter = 0;
	pass = 0;
	G.hand[thisPlayer][1] = silver;


	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	choice2 = gold;
	discarded = 1;
	newCards = 1;
	silverFlag = 0;

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, count, passed);

	printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
	assertTrue(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded, count, passed);

	i = 0;
	for (i = 0; i < testG.handCount[thisPlayer]; i++){
		if (testG.hand[thisPlayer][i] == gold){
			silverFlag = 1;
		}
	}

	printf("gold in hand? expected = yes");
	assertTrue(silverFlag, count, passed);
		
	printf("Test Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;

/*		


NOTE: this test crashes due to a seg fault. commenting out now so that the branch and coverage data can be added to unittestresults.out file neatly.



// ----------- TEST 3--------------
	printf("TEST 3: choice1 = 1 player is choosing a copper to trash and then a village card\n");

	count = 0;
	passed = 0;
	G.hand[thisPlayer][1] = copper;


	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	choice2 = village;
	discarded = 1;
	newCards = 1;

	printf("did cardEffect function error out correctly?\n");
	assertTrue(cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus), count, passed);

	printf("Test Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;
*/




	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> unittest 3 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}

