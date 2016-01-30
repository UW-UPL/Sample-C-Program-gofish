/**
 * Example card? game for UPL's C tutorial.
 * Needs to use loops, functions, arrays (dynamically allocated), structs, pointers.
 * Copyright Nik Ingrassia and Riccardo Mutschlechner, 2015
 */

/* Use <header.h> to include a system header */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Use "header.h" to include a local/custom header */
#include "card.h"
#include "player.h"
#include "deck.h"

/** 
 * main() is where the program starts. It can be in any file, but there
 * must only be one main and it must return an integer value.
 */
int main(int argc, char** argv) 
{
  /**
   * Time to make go fish!
   *
   * In case you don't remember/haven't played Go Fish,
   * here are the rules. Each player starts with X cards,
   * and on their turn are able to ask a fellow player for
   * a given number card. If the player has it, they give it
   * to the player. Otherwise, the player draws a card from
   * the deck. A player may discard any pair in their hand.
   * The goal of the game is to discard all of your cards!
   */

  //declare variables here! 

  //keep track of our players!
  player* players;
  int players_num;

  //we have a "deck" of cards.
  deck game_deck;

  //status for game won/lost!
  int game_over = 0;

  //actual game starts here.

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
      game_over = run_turn(current_player, players, players_num, &game_deck);
    }
  }
  
  printf("Game is finished woo, player %d won\n", game_over);

  //should take down all of the malloc'd things here just to
  //show how it should be done.
  int i;
  for(i = 0; i < players_num; i++) {
    free_player(players[i]);
  }
  free(players);
  
  exit(EXIT_SUCCESS);
}

int run_turn(int current_player, player *players, int players_num, deck *game_deck) {

  int other_player;
  int rank;
  int ch; //used for waiting for input.
  int taken;
  card* card;

  taken = 0;

  //print current player name.
  printf("%s's turn.\n", players[current_player].name);

  //flush input buffer, wait for input, then display the player's cards.
  while ((ch=getchar()) != EOF && ch != '\n');
  getchar();

  /*display cards*/
  //get first card
  card = players[current_player].my_deck.cards;
  while(card->next != NULL) {
    printf("Suit: %d Rank: %d\n", card->suit, card->rank);
  }

  //get input from player - other player and desired rank.
  printf("Who do you want to take a card from?\n");

  //print list of other players
  int i;
  for(i = 0; i < players_num; i++) {
    if(i != current_player) {
      printf("%s - %i\n", players[i].name, i);
    }
  }

  //actually get player number
  scanf("%d", other_player);

  //get desired rank
  printf("What card rank do you want?");
  scanf("%d", rank);

  //check other player's deck
  card = players[other_player].my_deck.cards; //get the list of cards.
  while(card->next != NULL) {
    if(card->next->rank == rank) {  //take card from player if successful.
      take_card(card, players[current_player].my_deck.cards);
      taken = 1;
    }
  }

  //otherwise draw card from deck (if not empty)
  if(!taken) take_card(game_deck->cards, players[current_player].my_deck.cards);

  //check for matches! (implement later)

  //if(win_condition) return current_player;

  return 0;

}
