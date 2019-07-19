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




	int numPlayers = 4;


    int i, j;
    int seed = 1000;


	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);
	// initialize a game state and player cards


	printf("----------------- Testing : %s ----------------\n", TESTCARD);




	// ----------- TEST 1: choice1 = 1 discard estate card and gain 4 coins--------------
	printf("TEST 1: checking card worth and winner calculation \n");

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


	for (j = 1; j < numPlayers; j++) {
		for (i = 0; i < 5; i++){
			drawCard(j, &G);
		}
	}

	for (j = 0; j < numPlayers; j++){

		if (j == 0){
			G.hand[j][0] = estate; 
		}
		else if (j == 1){
			G.hand[j][0] = duchy;
		}
		else if (j == 2){
			G.hand[j][0] = province;
		}
		else if (j == 3){
			G.hand[j][0] = curse;
		}
	}




	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	int players[numPlayers];
	int score[numPlayers];

	getWinners(players, &testG);
	int testGwinner = -1;
	int winner = 0;
	int tieFlag = 0; // 0 = no tie, 1 = 1 tie, 2 = 2 ties etc

	score[0] = -10;
	score[1] = -10;
	score[2] = -10;
	score[3] = -10;
	//-10 mean uninitialized


	for (i = 0; i < numPlayers; i++){
		
		score[i] = scoreFor(i, &testG);
		
		if (score[i] > score[winner]){
			winner = i;
			tieFlag = 0; //resets since there's a new highscore
		}
		
		if (i != winner && score[i] == score[winner]){
			tieFlag++;
			winner = i;
		}

		if (players[i] == 1){
			testGwinner = i;
		}
	}




	printf("Scores:\n");
	printf("P1 = %d\n", score[0]);
	printf("P2 = %d\n", score[1]);
	printf("P3 = %d\n", score[2]);
	printf("P4 = %d\n", score[3]);

	printf("Winning Player = %d, expected = %d\n", testGwinner, winner);
	assertTrue(testGwinner == winner, count, passed);

	printf("# of ties = %d, expected = 0\n", tieFlag);
	assertTrue(tieFlag == 0, count, passed);



	printf("Test 1 Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += *count;
	passedTotal += *passed;

	// ----------- TEST 2: --------------
	printf("TEST 2: checking if a tie occurs \n");
	pass = 0;
	counter = 0;




	for (j = 0; j < numPlayers; j++){

		if (j == 0){
			G.hand[j][0] = estate; 
		}
		else if (j == 1){
			G.hand[j][0] = estate;
		}
		else if (j == 2){
			G.hand[j][0] = estate;
		}
		else if (j == 3){
			G.hand[j][0] = curse;
		}
	}


	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	for (i = 0; i < numPlayers; i++) {
		players[i] = 0;
	}

	getWinners(players, &testG);
	testGwinner = -1;
	winner = 0;
	int winner1 = 0;
	int winner2 = 0;
	tieFlag = 0; // 0 = no tie, 1 = 1 tie, 2 = 2 ties etc

	score[0] = -10;
	score[1] = -10;
	score[2] = -10;
	score[3] = -10;
	//-10 mean uninitialized


	for (i = 0; i < numPlayers; i++){
		
		score[i] = scoreFor(i, &testG);
		
		if (score[i] > score[winner]){
			winner = i;
			tieFlag = 0; //resets since there's a new highscore
		}
		
		if (i != winner && score[i] == score[winner]){
			tieFlag++;
			if (winner1 == 0){
				winner1 = i;
			}
			else {
				winner2 = i;
			}
		}

		if (players[i] == 1){
			testGwinner = i;
		}
	}




	printf("Scores:\n");
	printf("P1 = %d\n", score[0]);
	printf("P2 = %d\n", score[1]);
	printf("P3 = %d\n", score[2]);
	printf("P4 = %d\n", score[3]);

	printf("Winning Player = %d, expected = %d and %d and %d \n", testGwinner, winner, winner1, winner2);
	assertTrue(testGwinner == winner, count, passed);

	printf("# of ties calculated in test = %d, expected = 2\n", tieFlag);
	//no test for this, this is mainly testing the unit test and not for the actual code.


	printf("Testing Results: Passed %d tests out of %d\n\n", *passed, *count);

	countTotal += counter;
	passedTotal += pass;


	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<", TESTCARD);

	printf("\n >>>>> cardtest4 passed %d out of %d <<<<<\n\n", passedTotal, countTotal);

	return 0;
}