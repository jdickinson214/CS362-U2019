#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
void estateCheck(struct gameState *state, int currentPlayer);


int baronCard(int choice1, int currentPlayer, struct gameState *state);
void redrawHand(int currentPlayer, int handPos, struct gameState *state);
int minionCard(int i, int j, int choice1, int choice2, int currentPlayer, struct gameState *state, int handPos);
int ambassadorCard(int i, int j, int choice1, int choice2, int currentPlayer, struct gameState *state, int handPos);
int tributeCard(int i, int tributeRevealedCards[], int currentPlayer, int nextPlayer, struct gameState *state, int handPos);
int mineCard(int i, int j, int choice1, int choice2, int currentPlayer, struct gameState *state, int handPos);



int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

#endif
