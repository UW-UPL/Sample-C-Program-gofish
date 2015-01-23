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

  //we have a "deck" of cards.
  deck game_deck;

  //status for game won/lost!
  int game_over = 0;

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
  deal_cards(&game_deck, players, players_num);

  //game loop goes here!
  while(!game_over) {
    int current_player;
    for(current_player = 0; current_player < players_num; current_player++) {
      run_turn(current_player); //this is a function
    }
  }
  
  exit(EXIT_SUCCESS);
}

void run_turn(int current_player) {
  //get input from player - other player and desired rank.

  //check other player's deck

  //take card from player if successful. 
}
