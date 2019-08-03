#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define DEBUG 0
#define NOISY_TEST 1



//testing Baron


void assertTrue(int expression, int *pass, int *count) {
  if (expression == 1) {
    (*count)++;
    (*pass)++;
    printf("PASSED\n");
  }
  else{
    (*count)++;
    printf("**********FAILED***********\n");
  }
}


void assertCrash(int expression, int *crash){
  if (expression != 1) {
    printf("----------Crashed------------");
    (*crash)++;
  }
}


int checkBaron(int choice1, int p, struct gameState *post, int *passPtr, int *countPtr, int *crash) {
  //+1 buy, can discard an estate card for +4 gold, or gain an estate card.

  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;
  printf ("baron PRE: choice1 %d p %d HC %d DeC %d DiC %d Buy %d $$ %d EstateSupply %d\n",
  choice1, p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p], pre.numBuys, pre.coins, supplyCount(estate, &pre));
    
  r = executeBaron(choice1, post, p);

  printf ("baron POST: choice1 %d p %d HC %d DeC %d DiC %d Buy %d $$ %d EstateSupply %d\n",
  choice1, p, post->handCount[p], post->deckCount[p], post->discardCount[p], post->numBuys, post->coins,  supplyCount(estate, post));

  int foundEstateFlag = 0;

  int i;

  if (choice1 > 0) {
    for (i = 0; i < pre.handCount[p]; i++){
      if (pre.hand[p][i] == estate){
        pre.discard[p][pre.discardCount[p]] = pre.hand[p][i];
        pre.discardCount[p]++;
        for (int j = i; j < pre.handCount[p]; j++){
          pre.hand[p][j] = pre.hand[p][j+1];
        }
        pre.hand[p][pre.handCount[p]] = -1;
        pre.handCount[p]--;
        pre.coins += 4;
        foundEstateFlag = 1;
        i = pre.handCount[p];
      }
    }
    if (foundEstateFlag == 0){
      gainCard(estate, &pre, 0, p);
    }
  }
  else if (supplyCount(estate, &pre) > 0){
    gainCard(estate, &pre, 0, p);
  }

  pre.numBuys++;

  assertCrash((r == 0), crash);

  assertTrue((memcmp(&pre, post, sizeof(struct gameState)) == 0), passPtr, countPtr);
  return 0;
}



int main () {

  int i, n, r, p, deckCount, discardCount, handCount, choice1;

  int count = 0;
  int pass = 0;
  int *countPtr = &count;
  int *passPtr = &pass;
  int crash = 0;

  int k[10] = {adventurer, council_room, feast, gardens, mine,
	       remodel, smithy, village, baron, great_hall};

  struct gameState G;

  printf ("Testing Baron.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }

    choice1 = floor(Random() * 2);
    p = floor(Random() * 4);
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);
    G.handCount[p] = floor(Random() * MAX_HAND);
    G.supplyCount[estate] = floor(Random() * 12);
    G.coins = floor(Random() * 100);
    G.numBuys = floor(Random() * 100);

    //coinflip is just a random 0 or 1
    int coinFlip = floor(Random() * 2);
  //  printf("coinflip = %d (1 = estate is in hand, 0 = estate not in hand)\n", coinFlip);
    //if the coinflip is true, then place an estate card in the player's hand somewhere
    int estateInHand = 0;

    //if coinflip is 1, check if estate in hand, if not, place in hand.
    if (coinFlip == 1){ 
      for (i = 0; i < G.handCount[p]; i++){
        if (G.hand[p][i] == estate){
          estateInHand = 1;
          i = G.handCount[p];
        }
      }
      if (estateInHand == 0){
        int handPosition = floor(Random() * G.handCount[p]);
        G.hand[p][handPosition] = estate;
      }
    }
    
    checkBaron(choice1, p, &G, passPtr, countPtr, &crash);
  }

  printf ("ALL TESTS OK\n");


  printf("\n >>>>> SUCCESS: Testing complete <<<<<");
  printf("\n >>>>> executeBaron passed %d out of %d <<<<<\n", pass, count);
  printf(" >>>>> executeBaron crashed %d out of %d <<<<<\n\n", crash, count);

  exit(0);

  return 0;
}
