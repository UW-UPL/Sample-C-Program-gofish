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

#include <string.h> /* Needed for memmove, strlen, strcmp */
#include <stdio.h> /* Needed for printf */
#include <unistd.h> /* Needed for read */
#include <stdlib.h> /* Needed for rand */

#include "gofish.h"

/**
 * Setup the given player with the given values. The name will
 * be copied into the player struct. The player will be setup
 * with the proper playing functions based on whether human
 * is 1 (TRUE) or 0 (FALSE).
 */
void player_setup(struct player* p, const char* name, int human)
{
	/* Copy in the name */
	strncpy(p->name, name, NAME_MAX);

	/* Are we setting up a human or a computer? */
	if(human == 1)
	{
		/* Use the human player turn method for humans */
		p->my_turn = human_player_turn;
	} else {
		/* Use the computer player's turn method */
		p->my_turn = computer_player_turn;
	}

	p->books = 0;
}

/**
 * It is a human's turn. This function is complicated so it has
 * been heavily commented.
 *
 * Parameters:
 *   p: The player who is going now.
 *   guess_rank: The rank that is guessed.
 *   guess_player: The player who we are asking.
 *
 *
 * The double pointer for player may seem complicated. Basically,
 * the game is asking us for a pointer to a player to ask. Therefore,
 * The game has provided us with a pointer to a location where we can
 * store the value of the player we will ask:
 *
 *                  +---------+
 *                  | Game    |
 *                  |         |
 *            +---->| rank:4  |
 *            | +-->| player: |--+
 *            | |   +---------+  |
 *            | |                |
 * +--------+ | |                |   +----------+
 * | HPT    | | |                +-->|Player    |
 * |        | | |                    |          |
 * | rank   |-+ |                    |name: bill|
 * | player |---+                    |cards: ...|
 * +--------+                        +----------+
 *
 *
 * You can see that the value player in Game points to the player
 * bill. We are given a pointer to a pointer to a player in 
 * human_player_turn. We will fill this pointer with a pointer
 * to the player bill. This is how it ends up being a double
 * pointer.
 *
 */
int human_player_turn(struct player* p, 
	rank_t* guess_rank, 
	struct player** guess_player)
{
	rank_t human_r = -1; /* The rank we are asking for */ 
	struct player* human_p = NULL; /* The player we are asking*/

	/* Print a couple of new lines */
	printf("\n\n");

	/* Your turn prompt */
	printf("%s, it is your turn\n", p->name);

	/* Let the player know what is in their hand */
	printf("Here is your hand:\n\t");

	/* Count how many of each card do we have */
	char cards_count[RANK_MAX + 1];
	memset(cards_count, 0, RANK_MAX + 1);

	int x;
	for(x = 0;x <= p->d.top;x++)
	{
		/* Get the card's rank */
		rank_t r = p->d.cards[x].r;

		/* Increment the card count for this card */
		cards_count[r]++;
	}

	/* Did we print any counts? 0 = FALSE, 1 = TRUE */
	int printed = 0;

	/* Print the counts: */
	for(x = R2;x <= RANK_MAX;x++)
	{
		/* do we have any of these cards? */
		if(cards_count[x] == 0)
		{
			continue;
		}

		/* Print a comma */
		if(printed != 0)
		{
			printf(", ");
		}

		printf("%d %s%s", cards_count[x], rank_names[x],
			cards_count[x] == 1 ? "" : "\'s");

		/* We printed something */
		printed = 1;
	}
	printf("\n\n");


	/* Where we will put the human's input */
#define INPUT_BUFF_MAX 64
	char input_buff[INPUT_BUFF_MAX];

	/**
	 * Get the card rank that the human is asking for 
	 */
	do
	{
		human_r = -1;
		human_p = NULL;

		printf("Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A\n");

		/* Prompt for a rank */
		printf("What rank are you looking for?   ");
		fflush(stdout);

		/* Clear the buffer we're reading into */
		memset(input_buff, 0, INPUT_BUFF_MAX);
		/* read the user's input */
		int sz = read(1, input_buff, INPUT_BUFF_MAX - 1);
		
		/* Did we encounter an I/O error? */	
		if(sz <= 0)
		{
			/* The user has quit */
			return -1;
		}

		/* What did we get? Parse the input. */
		switch(input_buff[0])
		{
			case '2': case '3': case '4': case '5': 
			case '6': case '7': case '8': case '9':
				/* We got a numerical value */
				human_r = input_buff[0] - '0';
				break;
			case '1':
				/* We probably got 10, let's check */
				if(input_buff[1] == '0')
				{
					human_r = R10;
				} else {
					human_r = -1;
				}
				break;
			case 'J':
				/* Got a Jack */
				human_r = RJ;
				break;
			case 'Q':
				/* Got a queen */
				human_r = RQ;
				break;
			case 'K':
				/* Got a king */
				human_r = RK;
				break;
			case 'A':
				/* Got an Ace. */
				human_r = RA;
				break;
			default:
				/* Got something invalid */
				human_r = -1;
				break;
		}

		/**
 		 * Print out opponents
		 */
		printf("\nValid players:");
		for(x = 0;x < PLAYERS_MAX;x++)
		{
			/* Is this player valid? */
			if(strlen(game_players[x].name) == 0)
				break;

			/* Is this the current player? */
			if(game_players + x == p)
				continue;

			/* Print commas */
			if(x != 0)
			{
				printf(",");
			}

			/* Print this player's name */
			printf(" %s", game_players[x].name);
		}
		printf("\n");

		/* Now lets ask for a player. */
		printf("Which player do you want to ask? ");
		fflush(stdout); /* Flush the output */

		/* Clear the buffer we're reading into */
		memset(input_buff, 0, INPUT_BUFF_MAX);
		/* Read the player's input */
		sz = read(1, input_buff, INPUT_BUFF_MAX - 1);

		/* The last character is usually a new line, trim it. */
		if(input_buff[strlen(input_buff) - 1] == '\n')
			input_buff[strlen(input_buff) - 1] = 0;

		/* Did we encounter an I/O error? */
		if(sz <= 0)
		{
			/* A system error has occurred. */
			return -1;
		}

		/* Find the player with that name */
		for(x = 0;x < PLAYERS_MAX;x++)
		{
			/* Is this player valid? */
			if(strlen(game_players[x].name) == 0)
				break;

			/* Do we have a match on the name? */
			if(!strcmp(game_players[x].name, input_buff))
			{
				human_p = game_players + x;
				break;
			}
		}

		/* Check to see if everything we got from the user is valid! */
	} while(
			/* Is the given rank valid? */
			(human_r < R2 || human_r > RANK_MAX) ||
			/* Is the given player valid? */
			(human_p == NULL) ||
			/* Do we have the card we're asking for? */
			(!cards_count[human_r])
	       );

	/**
	 * Let the game know who we want to ask and which
	 * rank we want to ask for.
	 */
	*guess_rank = human_r;
	*guess_player = human_p;

	/* Print a newline for the dialog */
	printf("\n");

	/* Return success */
	return 0;
}

/**
 * Logic function for doing a computer player's turn. The computer
 * players will basically do everything as random as possible. They
 * will ask random players for cards that they have in their hand.
 * Returns 0 on success, -1 if a move could not be generated.
 */
int computer_player_turn(struct player* p,
        rank_t* guess_rank,
        struct player** guess_player)
{
	rank_t ret_rank; /* The rank we will return */
	struct player* ret_player; /* The player we will return */

	int tries; /* How many times have we tried generating a move? */
	for(tries = 0;tries < 100;tries++)
	{
		/* Guess a random rank */
		ret_rank = R2 + (rand() % (RANK_MAX + 1 - R2));

		/* Do we have this rank? */
		struct card* c = deck_contains(&p->d, ret_rank);

		if(!c)
		{
			/**
			 * We don't even have this rank, let's make a better
			 * guess that will be able to get us cards we want.
			 */
			continue;
		}

		/* Add this card back to our deck */
		deck_put(&p->d, c);
		
		ret_player = NULL;
		do
		{
			/* Now, pick a random player */
			int player_num = rand() % player_count;
			ret_player = game_players + player_num;
		} while (ret_player == p);

		/* We have found a valid move */
		break;
	}

	/* Return the random player */
	*guess_player = ret_player;
	/* Return the random rank */
	*guess_rank = ret_rank;

	/* Return success */
	return 0;
}

