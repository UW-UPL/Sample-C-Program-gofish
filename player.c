#include "player.h"

int generate_players(int argc, char **argv, player *players) {
  
  int i;
  int players_num = argc-1;
  
  //allocate the memory for the players
  players = malloc(players_num * sizeof(player));
  
  if(players == NULL) {
    printf("Error allocating memory for players!\nQuitting...\n");
    exit(EXIT_FAILURE);
  }

  //set up the player's name!
  for(i = 0; i < players_num; i++) {
    players[i].name_len = strlen(argv[i+1]);
    players[i].name = malloc(players[i].name_len);
    players[i].name = strcpy(players[i].name, argv[i+1], players[i].name_len);
  }

  return players_num;
}

void free_player(player free_me) { 
  //free my name
  free(free_me.name);
}


void deal_cards(deck* deck, players* players, int players_num) {
  int i, j;
  for(i = 0; i < CARDS_PER_PLAYER; i++) {
    for(j = 0; j < players_num; j++) {
      move_card(deck, player[j]->deck, 0); //move from top of deck
      deck->cards_num--;
      player[j]->my_deck->cards_num++;
    }
  }
}