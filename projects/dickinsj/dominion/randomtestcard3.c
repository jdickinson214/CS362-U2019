#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define DEBUG 0
#define NOISY_TEST 1



//testing tribute


void assertTrue(int expression, int *pass, int *count) {
  if (expression == 1) {
    (*count)++;
    (*pass)++;
    printf("PASSED\n");
  }
  else{
    (*count)++;
    printf("******FAILED******\n");
  }
}


void assertCrash(int expression, int *crash){
  if (expression != 1) {
//    printf("----------Crashed------------");
    (*crash)++;
  }
}


int checkMinion(int choice1, int choice2, int p, struct gameState *post, int handPos, int *passPtr, int *countPtr, int *crash) {
  //+1 buy, can discard an estate card for +4 gold, or gain an estate card.

  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r, i, j;
  printf ("tribute PRE : choice1 %d choice2 %d p %d totalP %d HC %3d DeC %3d DiC %3d act %3d $$ %3d handPos %3d\n",
  choice1, choice2, p, pre.numPlayers, pre.handCount[p], pre.deckCount[p], pre.discardCount[p], pre.numActions, pre.coins, handPos);
    
  r = tributeCard(i, j, choice1, choice2, p, post, handPos);

  printf ("tribute POST: choice1 %d choice2 %d p %d totalP %d HC %3d DeC %3d DiC %3d act %3d $$ %3d handPos %3d ",
  choice1, choice2, p, post->numPlayers, post->handCount[p], post->deckCount[p], post->discardCount[p], post->numActions, post->coins, handPos);

  int foundEstateFlag = 0;

  discardCard(handPos, p, &pre, 0);

  if (choice1 > 0) {
    pre.coins += 2;
  }
  else if (choice2 > 0) {
    int cards = pre.handCount[p];     //get handCount as a set value
    for (j = 0; j < cards; j++){      
      discardCard(0, p, &pre, 0);     //discard whole hand

    }
    for (j = 0; j < 4; j++){
      drawCard(p, &pre);              //redraw 4 new cards
    }

    for (i = 0; i < pre.numPlayers; i++){
      if (pre.handCount[i] > 4 && i != p){//if player has more than 4 cards, or it's the current player
        int cards = pre.handCount[i];     //get handCount as a set value
        for (j = 0; j < cards; j++){      
          discardCard(0, i, &pre, 0);     //discard whole hand
  
        }
        for (j = 0; j < 4; j++){
          drawCard(i, &pre);              //redraw 4 new cards
        }
      }
    }
  }

  
  pre.numActions++;

  assertCrash((r == 0), crash);

  assertTrue((memcmp(&pre, post, sizeof(struct gameState)) == 0), passPtr, countPtr);
  return 0;
}



int main () {

  int i, n, r, p, deckCount, discardCount, handCount, choice1, choice2, handPos;

  int count = 0;
  int pass = 0;
  int *countPtr = &count;
  int *passPtr = &pass;
  int crash = 0;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, tribute, village, minion, great_hall};

  int randomCard[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer,
      council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall,
      minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager,
      sea_hag, treasure_map};


  struct gameState G;

  printf ("Testing Minion.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }

    choice1 = floor(Random() * 2);  
    if (choice1 == 0){
      choice2 = 1;
    }
    else{
      choice2 = 0;
    }
    

    G.numPlayers = floor(Random() * 3) + 2; //can only be 2-4 players
    p = floor(Random() * G.numPlayers);
    G.coins = floor(Random() * 100);
    G.numActions = floor(Random() * 100);
    G.playedCardCount = floor(Random() * MAX_HAND); 


    for (int k = 0; k < G.numPlayers; k++){

      G.handCount[k] = floor(Random() * MAX_HAND);

      if (k == p){
        handPos = floor(Random() * G.handCount[k]);
      }

      G.deckCount[k] = floor(Random() * MAX_DECK);

      G.discardCount[k] = floor(Random() * MAX_DECK);

      for (i = 0; i < G.handCount[k]; i++){
        if (i == handPos && k == p){
          G.hand[p][handPos] = %d;
        } else{
          int x = floor(Random() * 27);
          G.hand[k][i] = randomCard[x];
        }
      }
      for (i = 0; i < G.deckCount[k]; i++){
        int x = floor(Random() * 27);
        G.deck[k][i] = randomCard[x];
      }
      for (i = 0; i < G.discardCount[k]; i++){
        int x = floor(Random() * 27);
        G.discard[k][i] = randomCard[x];
      }
    }


    checkMinion(choice1, choice2, p, &G, handPos, passPtr, countPtr, &crash);

  }

  printf ("ALL TESTS OK\n");


  printf("\n >>>>> SUCCESS: Testing complete <<<<<");
  printf("\n >>>>> %dCard passed %d out of %d <<<<<\n", pass, count);
  printf(" >>>>> %dCard crashed %d out of %d <<<<<\n\n", crash, count);

  exit(0);

  return 0;
}
