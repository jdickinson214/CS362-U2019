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
    printf("----------Crashed------------");
    (*crash)++;
  }
}


int checkTribute(int tributeRevealedCards[], int p, int np, struct gameState *post, int handPos, int *passPtr, int *countPtr, int *crash) {
 

  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int r, i, j;
  printf ("tribute PRE : p %d np %d totalP %d HC %3d DeC %3d DiC %3d act %3d $$ %3d buy %3d handPos %3d\n",
  p, np, pre.numPlayers, pre.handCount[np], pre.deckCount[np], pre.discardCount[np], pre.numActions, pre.coins, pre.numBuys, handPos);
    
  r = executeTribute(p, np, post);

  printf ("tribute POST: p %d np %d totalP %d HC %3d DeC %3d DiC %3d act %3d $$ %3d buy %3d handPos %3d\n",
  p, np, post->numPlayers, post->handCount[np], post->deckCount[np], post->discardCount[np], post->numActions, post->coins, post->numBuys, handPos);


  if (pre.discardCount[np] + pre.deckCount[np] <= 1){

    if (pre.deckCount[np] == 1){
      tributeRevealedCards[0] = pre.deck[np][pre.deckCount[np] - 1];
      pre.deckCount[np]--;
    }
    else if (pre.discardCount[np] == 1){
      tributeRevealedCards[0] = pre.discard[np][pre.discardCount[np] - 1];
      pre.discardCount[np]--;
    }
    else {
      printf("no cards\n");
    }
  }
  else {
    if (pre.deckCount[np] == 0){
      for (i = 0; i < pre.discardCount[np]; i++){
        pre.deck[np][i] = pre.discard[np][i];
        pre.deckCount[np]++;
        pre.discard[np][i] = -1;
        pre.discardCount[np]--;
      }
      shuffle(np, &pre);
    }
    tributeRevealedCards[0] = pre.deck[np][pre.deckCount[np] - 1];
    pre.deck[np][pre.deckCount[np] - 1] = -1;
    pre.deckCount[np]--;
    tributeRevealedCards[1] = pre.deck[np][pre.deckCount[np] - 1];
    pre.deck[np][pre.deckCount[np] - 1] = -1;
    pre.deckCount[np]--;
  }
      
  if (tributeRevealedCards[0] == tributeRevealedCards[1]) 
  {
      pre.playedCards[pre.playedCardCount] = tributeRevealedCards[1]; 
      pre.playedCardCount++;
      tributeRevealedCards[1] = -1;
  }
  
  for (i = 0; i < 2; i++)
  {
      if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold) //Treasure cards
      {
          pre.coins += 2;
      }
      else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall) //Victory Card Found
      {
          drawCard(p, &pre);
          drawCard(p, &pre);
      }
      else //Action Card
      {
          pre.numActions += 2;
      }
  }

  
//  printf ("tribute PRE : p %d np %d totalP %d HC %3d DeC %3d DiC %3d act %3d $$ %3d buy %3d handPos %3d\n",
//  p, np, pre.numPlayers, pre.handCount[np], pre.deckCount[np], pre.discardCount[np], pre.numActions, pre.coins, pre.numBuys, handPos);
 

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

  printf ("Testing Tribute.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }

    
    G.numPlayers = floor(Random() * 3) + 2; //can only be 2-4 players
    p = floor(Random() * G.numPlayers);
    int np; //next player
    if (p == G.numPlayers - 1){
      np = 0;
    }
    else {
      np = p+1;
    }

    int tributeRevealedCards[2] = {-1, -1};


    G.coins = floor(Random() * 100);
    G.numActions = floor(Random() * 100);
    G.numBuys = floor(Random() * 100);
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
          G.hand[p][handPos] = tribute;
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


    checkTribute(tributeRevealedCards, p, np, &G, handPos, passPtr, countPtr, &crash);

  }

  printf ("ALL TESTS OK\n");


  printf("\n >>>>> SUCCESS: Testing complete <<<<<");
  printf("\n >>>>> tributeCard passed %d out of %d <<<<<\n", pass, count);
  printf(" >>>>> tributeCard crashed %d out of %d <<<<<\n\n", crash, count);

  exit(0);

  return 0;
}
