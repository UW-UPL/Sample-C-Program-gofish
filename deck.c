#include <stdlib.h>

#include "deck.h"

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

  //connect the list
  for(i = 0; i < CARDS_PER_DECK - 1; i++){
    cards[i].next = (cards + i);
  }

  //shuffle the deck! (by swapping cards randomly)
  for(int i = 0; i < NUM_SWAPS; i++) {
    //we swap two randomly chosen cards - that's what rand does.
    swap_cards(new_deck.cards, (rand()%CARDS_PER_DECK), (rand()%CARDS_PER_DECK));
  }

  return new_deck;
}

void deal_cards(deck* deck, players* players, int players_num) {
  int i, j;
  for(i = 0; i < CARDS_PER_PLAYER; i++) {
    for(j = 0; j < players_num; j++) {
      //move a card from the deck to the player's hand, resizing things appropriately.
    }
  }
}

void free_deck(deck to_free) {
  free(to_free.cards);
}

void move_card(deck* from, deck* to, int card_index) {

}
