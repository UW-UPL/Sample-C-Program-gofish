#ifndef __PLAYER_H
#define __PLAYER_H 1

#include "deck.h"

typedef struct player {
  char* name;
  int name_len;
  deck my_deck;
}

int generate_players(int argc, char** argv, player* players);

#endif
