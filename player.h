#ifndef __PLAYER_H
#define __PLAYER_H 1

#include "deck.h"

#define CARDS_PER_PLAYER 5

typedef struct _player {
  char* name;
  int name_len;
  deck my_deck;
} player;

int generate_players(int argc, char** argv, player* players);

#endif
