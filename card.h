#ifndef __CARD_H
#define __CARD_H 1

#define NUM_SUITS 4
#define NUM_RANKS 13

typedef struct _card {
  int suit;
  int rank;
  struct _card* next;
}card;

int swap_cards(card* cards, int a, int b);
int take_card(card* from, card* to);

#endif
