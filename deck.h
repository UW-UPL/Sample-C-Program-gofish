#ifndef __DECK_H
#define __DECK_H 1

#include "card.h"
#include "player.h"

#define CARDS_PER_DECK 52

typedef struct _deck {
  card* cards;
  int cards_num;
} deck;

deck generate_game_deck();

void move_card(deck* from, deck* to, int card_index);
#endif
