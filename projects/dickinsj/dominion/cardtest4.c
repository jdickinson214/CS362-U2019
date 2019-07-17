/*
 * getWinners test
 *
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "getWinners"


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

	int numPlayers = 4;

	int startCoins = 0;

	int numAct = 1;

	int numBuy = 1;


    int i, j, numCoppers, numEstates;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;

    int thisPlayer = 0;
	int nextPlayer = thisPlayer+1;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);
	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1: choice1 = 1 discard estate card and gain 4 coins--------------
	printf("TEST: checking card worth and winner calculation \n");


	//set up players with only coppers
	for (j = 0; j < numPlayers; j++){

		for (i = 0; i < G.deckCount[j]; i++){

			G.deck[j][i] = copper;
		}
		for (i = 0; i < G.handCount[j]; i++){

			G.hand[j][i] = copper;		
		}
		for (i = 0; i < G.discardCount[j]; i++){

			G.discard[j][i] = copper;
		}
	}

	for (j = 0; j < numPlayers; j++){

		if (j == 0){
			G.deck[j][1] = estate; 
		}
		else if (j == 1){
			G.deck[j][1] = duchy;
		}
		else if (j == 2){
			G.deck[j][1] = providence;
		}
		else if (j == 3){
			G.deck[j][1] = curse;
		}
	}


	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	int players[numPlayers];

	getWinners(players, testG);

	int winner;
	int tieFlag = 0; // 0 = no tie, 1 = 1 tie, 2 = 2 ties etc

	for (i = 0; i < numPlayer; i++){
		if (players[i] == players[winner]){
			tieFlag++;
			winner = i;
		}
		if (players[i] > player[winner]){
			winner = i;
		}
	}

	printf("Highest Score = %d, expected = 0\n", winner);
	assertTrue(winner == 2, count, passed);

	printf("# of ties = %d, expected = 0\n", tieFlag);
	assertTrue(tieFlag == 0, count, passed);


	printf("Testing Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest4 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}