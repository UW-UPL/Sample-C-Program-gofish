//Example card? game for UPL's C tutorial.
//Needs to use loops, functions, arrays, structs, pointers.
//Written by Nik Ingrassia 2015
//also makefile?

//Includes with <> are for system libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Includes with "" are for local files
#include "card.h"
#include "player.h"
#include "deck.h"

//Defines are magic!
//A define is replaced by its value before compiliation happens!
//Defines are useful to set constants that don't use memory!
#define CARDS_PER_PLAYER 5
#define CARDS_PER_DECK 52
#define NUM_SUITS 4
#define NUM_RANKS 13

int main(int argc, char** argv) {

  //Time to make go fish!
  /*
  In case you don't remember/haven't played Go Fish,
  here are the rules. Each player starts with X cards,
  and on their turn are able to ask a fellow player for
  a given number card. If the player has it, they give it
  to the player. Otherwise, the player draws a card from
  the deck. A player may discard any pair in their hand.
  The goal of the game is to discard all of your cards!
  */

  //declare variables here! 

  //keep track of our players!
  player* players;
  int players_num;

  deck game_deck;

  //Structure:

  //seed the RNG
  srand((unsigned)time(NULL));

  //Read in arguments and generate players.
  players_num = generate_players(argc, argv, players);
  if(players_num < 3) {
    printf("Not enough players!\nQuitting...");
    exit(EXIT_FAILURE);
  }

  //Generate the deck.
  game_deck = generate_game_deck();

  //Deal cards to each player.
  exit(EXIT_SUCCESS);
}

//This is a function!
//move me to deck.c
deck generate_game_deck() {
  deck new_deck;
  
  //allocate memory for the deck
  new_deck.cards_num = CARDS_PER_DECK;
  new_deck.cards = malloc(CARDS_PER_DECK * sizeof(card));

  //generate cards!
  int i,j;
  for(i = 0; i < NUM_SUITS; i++) {
    for(j = 0; j < NUM_RANKS; j++) {
      cards[(i*NUM_RANKS) + j].suit = i;
      cards[(i*NUM_RANKS) + j].rank = j;
    }
  }

  //shuffle the deck! (by swapping cards randomly)
  for(int i = 0; i < NUM_SWAPS; i++) {
    swap_cards(new_deck.cards, (rand()%CARDS_PER_DECK), (rand()%CARDS_PER_DECK));
  }

  return new_deck;
}

//move to card.c or deck.c?
void swap_cards(card* cards, int a, int b) {
  card temp;
  memcpy(&temp, (cards + a), sizeof(card));
  memcpy((cards + a), (cards + b), sizeof(card));
  memcpy((cards + b), &temp, sizeof(card)); 
}

void deal_cards(deck* deck, players* players, int players_num) {
  int i, j;
  for(i = 0; i < CARDS_PER_PLAYER; i++) {
    for(j = 0; j < players_num; j++) {
      //move a card from the deck to the player's hand, resizing things appropriately.
    }
  }
}

//Also a function!
//move me to player.c
int generate_players(int argc, char** argv, player* players) {
  
  int i;
  int players_num = argc-1;
  
  //allocate the memory for the players
  players = malloc(players_num * sizeof(player));
  
  if(players == NULL) {
    printf("Error allocating memory for players!\nQuitting...\n");
    exit(EXIT_FAILURE);
  }

  //set up the player's name!
  for(i = 0; i < players_num; i++) {
    players[i].name_len = strlen(argv[i+1]);
    players[i].name = malloc(players[i].name_len);  
  }

  return players_num;
}
