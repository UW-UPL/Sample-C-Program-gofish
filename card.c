#include "card.h"

void swap_cards(card* cards, int a, int b) {
  card temp;
  memcpy(&temp, (cards + a), sizeof(card));
  memcpy((cards + a), (cards + b), sizeof(card));
  memcpy((cards + b), &temp, sizeof(card)); 
}
