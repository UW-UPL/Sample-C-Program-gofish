#ifndef __CARD_H
#define __CARD_H 1

#define NUM_SUITS 4
#define NUM_RANKS 13

typedef struct _card {
  int suit;
  int rank;
  struct _card* next;
}card;

void swap_cards(card* cards, int a, int b);

#endif
