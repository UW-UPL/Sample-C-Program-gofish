#include "card.h"

//this is a good example of pointer math!
//remember that pointer math depends on the size of the pointer's type!
void swap_cards(card* cards, int a, int b) {
  card temp;
  memcpy(&temp, (cards + a), sizeof(card));
  memcpy((cards + a), (cards + b), sizeof(card));
  memcpy((cards + b), &temp, sizeof(card)); 
}
