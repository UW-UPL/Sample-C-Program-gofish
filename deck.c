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


void move_card(deck *from, deck *to, int card_index) {
  card *card_parent;
  card *card;
  
  //get card parent
  card_parent = from->cards;
  for(int i = 0; i < card_index-1; i++) {
    card_parent = card_parent->next;
  }

  //remove from from deck
  card = card_parent->next;
  card_parent->next = card->next; //this skips over our card in the old list

  //add to to deck (at top)!
  card->next = to->cards;
  to->cards = card;

  //change card counts for "to" and "from"
  to->cards_num++;
  from->cards_num--;
}
