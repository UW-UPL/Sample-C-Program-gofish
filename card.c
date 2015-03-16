#include "card.h"

//swap list nodes
//returns 0 on succes, 1 on failure
int swap_cards(card* cards, int a, int b) {
  card* a_parent, b_parent, temp;
  int i;
  
  a_parent = cards;
  b_parent = cards;

  //get parents
  //make sure we actually have a card for all the positions!
  for(i = 0; i < a-1; i++) {
    a_parent = a_parent->next; //advance a_parent until it's at the parent of a
    if(a_parent->next == NULL) return 1;
  }
  for(i = 0; i < b-1; i++) {
    b_parent = b_parent->next;
    if(b_parent->next == NULL) return 1;
  }

  //swap happens now!
  //swap the next card for a and b.
  temp = a_parent->next->next;
  a_parent->next->next = b_parent->next->next;
  b_parent->next->next = temp;
  //swap a and b's parents
  temp = a_parent->next;
  a_parent->next = b_parent->next;
  b_parent->next = temp;

  return 0
}

int take_card(card* from, card* to) {
  card* from_parent, to_parent, temp;
  int i;
  
  from_parent = from;
  to_parent = to;

  //take happens now
  //remove from the from list
  temp = from_parent->next;
  from_parent->next = from_parent->next->next;
  from_parent->next = temp;

  //add to to list!
  temp->next = from_parent->next;
  from_parent->next = temp;
  

}
