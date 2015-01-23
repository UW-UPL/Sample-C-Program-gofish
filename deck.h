#ifndef __DECK_H
#define __DECK_H 1

#include "card.h"
#include "player.h"

#define CARDS_PER_DECK 52

typedef struct deck {
  card* cards;
  int cards_num;
};

deck generate_game_deck();

void deal_cards(deck* deck, players* players, int players_num);

#endif
