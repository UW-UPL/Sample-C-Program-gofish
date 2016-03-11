/**
 * This game is provided as an example of good use of the C programming
 * language for beginners. We assume you are familiar with simple
 * programming constructs like for loops and pointers. Complicated
 * functions are well documented. This game was originally written
 * by Riccardo Mutschlechner <riccardo@cs.wisc.edu> and Nik Ingrassia,
 * but was rewritten from scratch.
 *
 * Author: John Detter <jdetter@wisc.edu>
 * 
 */

#include <string.h> /* Needed for memset */
#include <stdio.h> /* Needed for printf */
#include <unistd.h> /* Needed for read */
#include <stdlib.h> /* Needed for srand and rand (random)*/
#include <time.h> /* Needed for time (setting up random) */

#include "gofish.h"

/**
 * The player for the game 
 */
struct player game_players[PLAYERS_MAX];

/**
 * How many players are actually playing in the game? 
 */
int player_count;

/**
 * The main deck for the game
 */
struct deck game_deck;


/**
 * The names of the card ranks
 */
const char* rank_names[] =
{
        NULL, NULL, /* No R0 or R1 */
        "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
        "Nine", "Ten", "Jack", "Queen", "King", "Ace"
};

/**
 * Names for some computer players
 */
const char* computer_names[] =
{
	"alice", "bob", "carl", "dana",
	"evan", "frank", "gretta", "harry",
	"ian", "jeanne", "kate"
};

int main(int argc, char** argv)
{
	/* Setup the random seed for this game */
	srand(time(NULL));

	/* The input we will take from the user will go in here */
#define USER_INPUT_MAX 64
	char user_input[USER_INPUT_MAX];
	memset(user_input, 0, USER_INPUT_MAX); /* Zero the buffer */

	/**
	 * Ask the user how many players they wish to play with.
	 */
	player_count = -1; /* The amount of computer players */
	do
	{
		/* Prompt for player count */
		printf("You must play against at least one computer player (max 11).\n");
		printf("How many computers do you want to play with? ");
		fflush(stdout); /* Flush output */
		if(read(1, user_input, USER_INPUT_MAX - 1) < 0)
		{
			/* An error occurred in reading */
			return -1;
		}

		/* Did we get a valid number? */
		player_count = atoi(user_input);
		if(player_count <= 0 || player_count >= PLAYERS_MAX)
		{
			sleep(DIALOG_SPEED); /* Sleep for 2 seconds */
			printf("You entered an invalid player count. Exiting...\n");
			return -1; 
		}
	} while(player_count <= 0);

	/* The user's name*/
	char user_name[USER_INPUT_MAX];
	memset(user_name, 0, USER_INPUT_MAX); /* Zero the buffer */
	do
	{
		/* Prompt for player's name */
		printf("What should we call you? ");
		fflush(stdout); /* Flush output */
		if(read(1, user_name, USER_INPUT_MAX - 1) < 0)
		{
			/* An error occurred getting the user's input */
			return -1;
		}

		/* The last character is probably a new line, trim it. */
		if(user_name[strlen(user_name) - 1] == '\n')
		{
			user_name[strlen(user_name) - 1] = 0;
		}
	
		if(strlen(user_name) < 2)
		{
			printf("Your name must be at least 2 characters.\n");
		}
	} while (strlen(user_name) < 2);

	printf("Setting up game...");

	/* Zero the players */
	memset(game_players, 0, sizeof(struct player) * PLAYERS_MAX);
	/* Add the one human player */
	player_setup(game_players + 0, user_name, 1);
	player_count++; /* increment player count for human player */

	/* Add all other players */
	int player_num;
	for(player_num = 1;player_num < player_count;player_num++)
	{
		/* Add the player */
		player_setup(game_players + player_num, 
			computer_names[player_num - 1], 0);
	}

	printf("done.\nInitilizing deck...");
	memset(&game_deck, 0, sizeof(struct deck));

	/* Add all of the cards to the deck */
	deck_populate(&game_deck);

	/* Shuffle the deck */
	deck_shuffle(&game_deck);

	printf("done.\nGiving out cards...");

	/* Distribute cards to players */
	int deal_cards = 5;
	if(player_count == 2)
	{
		/* If there are only 2 players, 7 cards are used */
		deal_cards = 7;
	}

	for(player_num = 0;player_num < player_count;player_num++)
	{
		struct player* p = game_players + player_num;
		int card_count;
		for(card_count = 0;card_count < deal_cards;card_count++)
		{
			/* Draw a card */
			struct card* c = deck_draw(&game_deck);

			/* Give the card to the player */
			deck_put(&p->d, c);

			/* Free the card pointer */
			free(c);
		}
	}

	printf("done.\n");

	/* Check to see if anyone got any sets off of the start */
	int player_index;
	for(player_index = 0;player_index < player_count;player_index++)
	{
		struct player* curr_player = game_players + player_index;
		rank_t set = -1;
		do
		{
			set = deck_get_set(&curr_player->d);

			if(set > 0)
			{
				printf("%s: I have all of the %s\'s.\n",
						curr_player->name,
						rank_names[set]);
				curr_player->books++;
				sleep(DIALOG_SPEED);
			}
		} while(set != -1);
	}


	/** We're ready to play! */

	int playing = 1;
	while(playing)
	{
		rank_t r;
		struct player* p;

		/* Let each player go */
		int player_num;
		for(player_num = 0;player_num < player_count;player_num++)
		{
			struct player* curr_player = 
				game_players + player_num;

			printf("Top of player's deck: %d\n", curr_player->d.top);
			/**
			 * Does this player have any cards in their hand?
			 */
			if(curr_player->d.top <= -1)
			{
				printf("game: %s has no cards.\n",
						curr_player->name);

				/* Sleep before draw */
                                sleep(DIALOG_SPEED);

				/**
				 * Have the player draw a card
				 */
                                struct card* c = deck_draw(&game_deck);
                                if(c != NULL)
                                {
                                        /* We got a card */
                                        printf("game: %s drew a card.\n", curr_player->name);
                                        /* Add the card to the player's deck */
                                        deck_put(&curr_player->d, c);

                                        /* Did the player get the card they asked for? */
                                        if(c->r == r)
                                        {
                                                sleep(DIALOG_SPEED);
                                                /* Player goes again */
                                                player_num--;

                                                printf("game: %s drew the card they asked for.\n",
                                                                curr_player->name);
                                        }

                                        /* Free the card */
                                        free(c);
                                } else {
                                        /* We didn't get a card. */
                                        printf("game: the deck is empty.\n");
                                }

                                /* Sleep before the next player goes */
                                sleep(DIALOG_SPEED);

				continue;
			}

			/**
			 * Tell the player it is their turn
			 */
			if(curr_player->my_turn(curr_player, &r, &p) != 0)
			{
				printf("go fish: game error!\n");
				exit(1);
			}

			/**
			 * The player must actually have this card.
			 */
			struct card* has_card = 
				deck_contains(&curr_player->d, r);

			/* do a validity check */
			if(r < R2 || r > RANK_MAX || !p 
					|| p == curr_player || !has_card)
			{
				printf("go fish: player entered an "
						"invalid move.\n");
				/* Try this again */
				player_num--;
				continue;
			}

			/**
			 * Put this card back into the player's deck
			 */
			deck_put(&curr_player->d, has_card);
			free(has_card); /* Free the card */

			printf("%s: %s, do you have any %s's?\n",
					curr_player->name, p->name,
					rank_names[r]);

			/* Sleep */
			sleep(DIALOG_SPEED);

			/* How many cards did we find?*/
			int count = 0;
			struct card* taken[4];
			memset(taken, 0, sizeof(struct card*) * 4);

			/* Take the cards from the player: */
			struct card* c;
			while((c = deck_contains(&p->d, r)) != NULL)
			{
				taken[count] = c;
				count++;
			}

			/**
			 * Did we get anything? 
			 */
			if(count > 0)
			{
				printf("%s: I have %d %s%s\n", p->name, 
						count, rank_names[r], 
						count == 1 ? "" : "\'s");

				/* Hand over all of the cards to the player */
				int x;
				for(x = 0;x < count;x++)
				{
					/* Hand over a card to the player */
					deck_put(&curr_player->d, taken[x]);

					/* Free the card */
					free(taken[x]);
				}

				/* Sleep before the player goes again */
				sleep(DIALOG_SPEED);

				/** 
				 * Do a quick check to see if the player 
				 * acquired a book 
				 */
				rank_t set = -1;
				do
				{
					set = deck_get_set(&curr_player->d);

					if(set > 0)
					{
						printf("%s: I have all of the %s\'s.\n",
								curr_player->name, 
								rank_names[set]);
						curr_player->books++;
						sleep(DIALOG_SPEED);
					}
				} while(set != -1);

				/* This player goes again */
				player_num--;
				continue;
			} else {
				/* didn't get anything */
				printf("%s: Go fish.\n", p->name);
				/* Sleep before draw */
				sleep(DIALOG_SPEED);

				struct card* c = deck_draw(&game_deck);
				if(c != NULL)
				{
					/* We got a card */
					printf("game: %s drew a card.\n", curr_player->name);
					/* Add the card to the player's deck */
					deck_put(&curr_player->d, c);

					/* Did the player get the card they asked for? */
					if(c->r == r)
					{
						sleep(DIALOG_SPEED);
						/* Player goes again */
						player_num--;

						printf("game: %s drew the card they asked for.\n",
								curr_player->name);
					}

					/* Free the card */
					free(c);
				} else {
					/* We didn't get a card. */
					printf("game: the deck is empty.\n");
				}

				/* Sleep before the next player goes */
				sleep(DIALOG_SPEED);

				/** 
				 * Do a quick check to see if the player 
				 * acquired a book 
				 */
				rank_t set = -1;
				do
				{
					set = deck_get_set(&curr_player->d);

					if(set > 0)
					{
						printf("%s: I have all of the %s\'s.\n",
								curr_player->name, 
								rank_names[set]);
						curr_player->books++;
						sleep(DIALOG_SPEED);
					}
				} while(set != -1);
			}
		}

		playing = 0;
		/* Check to see if the game is over */
		int x;
		for(x = 0;x < player_count;x++)
		{
			/* Are there cards in this player's hand? */
			if(game_players[x].d.top >= 0)
			{
				/* Found a card in someone's hand. */
				playing = 1;
			}
		}
	}

	/** Who has the most books? */
	int max_books = -1;
	int winner_count = 0;
	struct player* winners[player_count];

	int x;
	for(x = 0;x < player_count;x++)
	{
		if(game_players[x].books > max_books)
		{
			/* New record */
			max_books = game_players[x].books;
			memset(winners, 0, sizeof(struct player*) * 
					player_count);
			winners[0] = game_players + x;
			winner_count = 1;
		} else if(game_players[x].books == max_books)
		{
			/* Append a winner */
			winners[winner_count] = game_players + x;
			winner_count++;
		}
	}

	/* Print out some new lines */
	printf("\n\n");

	const char* win_string = "won";
	/* Is there a single winner? */
	if(winner_count >= 1)
	{
		/* More than one winner */
		printf("game: There was a tie between %d players.\n", 
				winner_count);
		win_string = "tied";
	}

	for(x = 0;x < winner_count;x++)
	{
		printf("%s has %s the game!", winners[x]->name, win_string);
	}

	return 0;
}
