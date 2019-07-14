/*
 * Ambassador card unit test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Ambassador"


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

    int i, j;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	G.hand[thisPlayer][0] = ambassador;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = duchy;
	G.hand[thisPlayer][3] = estate;
	G.hand[thisPlayer][4] = silver;
	
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


	// ----------- TEST 1--------------
	printf("TEST 1: choice1 = Discard 1 card and all other players get that card \n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));


	for (j = 1; j < 5; j++){ //checks each choice in player's hand except first ambassador card

		choice1 = j;
		cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

		discarded = 2;
		newCards = 1; //only for other players

		printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);
		assertTrue(testG.playedCardCount == G.playedCardCount + discarded, count, passed);

		printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
		assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] - discarded, count, passed);
		
		for (i = 0; i < testG.numPlayers; i++){
			
			if (i == 0) {
				printf("Current Player deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
				assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], count, passed);
			}
			else{
				printf("Player %d's deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i] + newCards);
				assertTrue(testG.deckCount[i] == G.deckCount[i] + newCards, count, passed);
			}

		}

	}

	printf("Test 1 Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;



	// ----------- TEST 2 --------------
	printf("TEST 2: choice1 = Discard 2 of same card and all other players get 1 of that card\n");
		
	counter = 0;
	pass = 0;
	G.hand[thisPlayer][4] = copper; //need two copper for it to work properly


	choice1 = 1; //will discard copper
	printf("Chose copper card in position 1 for discard, must choose another copper card\n");
	for (j = 0; j < testG.handCount[thisPlayer]; j++){
		memcpy(&testG, &G, sizeof(struct gameState)); //reset gamestate
		choice2 = j; //will iterate through all choices to see if game errors out.
		if (testG.hand[thisPlayer][j] == copper){
			printf("Choosing hand position = %d, expected function return = %d\n", j, -1);
			assertTrue(cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus) == -1, count, passed);
		}
		else{
			printf("Choosing hand position = %d, expected function return = %d\n", j, 1);
			assertTrue(cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus) == 1, count, passed);
		}
	}

	memcpy(&testG, &G, sizeof(struct gameState)); //reset gamestate
	choice2 = 4;
	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);


	discarded = 3;
	newCards = 1;

	printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);
	assertTrue(testG.playedCardCount == G.playedCardCount + discarded, count, passed);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
	assertTrue(testG.handCount[thisPlayer] == G.handCount[thisPlayer] - discarded, count, passed);
	
	for (i = 0; i < testG.numPlayers; i++){
		
		if (i == 0) {
			printf("Current Player deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
			assertTrue(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], count, passed);
		}
		else{
			printf("Player %d's deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i] + newCards);
			assertTrue(testG.deckCount[i] == G.deckCount[i] + newCards, count, passed);
		}
	}

	printf("Test 2 Results: Passed %d tests out of %d\n\n", pass, counter);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> unittest 3 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}

