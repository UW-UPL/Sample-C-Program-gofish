#include <string.h> /* Needed for memmove, strlen, strcmp */
#include <stdio.h> /* Needed for printf */
#include <unistd.h> /* Needed for read */

#include "gofish.h"

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
		
	}
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




	/* Welcome, turn prompt */
	printf("%s, it is your turn\n", p->name);

	/* Let the player know what is in their hand */
	printf("Here is your hand:\n\t");

	/* Count how many of each card do we have */
	char cards_count[RANK_MAX];
	memset(cards_count, 0, RANK_MAX);

	int x;
	for(x = 0;x <= p->d->top;x++)
	{
		/* Get the card's rank */
		rank_t r = p->d->cards[x].r;

		/* Increment the card count for this card */
		cards_count[r]++;
	}

	/* Did we print any counts? 0 = FALSE, 1 = TRUE */
	int printed = 0;

	/* Print the counts: */
	for(x = R2;x < RANK_MAX;x++)
	{
		/* do we have any of these cards? */
		if(cards_count[x] == 0)
		{
			continue;
		}

		/* Print a comma */
		if(printed != 0)
		{
			printf(",");
		}

		printf("%d %s%s ", cards_count[x], rank_names[x],
			cards_count[x] == 1 ? "" : "\'s");

		/* We printed something */
		printed = 1;
	}
	printf("\n");


	printf("Valid ranks: 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A\n");
	printf("Valid players:");
	for(x = 0;x < PLAYERS_MAX;x++)
	{
		/* Is this player valid? */
		if(strlen(game_players[x].name) == 0)
			break;

		/* Print commas */
		if(x != 0)
		{
			printf(",");
		}

		/* Print this player's name */
		printf(" %s", game_players[x].name);
	}
	printf("\n");

	/* Where we will put the human's input */
#define INPUT_BUFF_MAX 64
	char input_buff[INPUT_BUFF_MAX];

	do
	{
		human_r = -1;
		human_p = NULL;

		/* Prompt for a rank */
		printf("What rank are you looking for?   ");

		/* read the user's input */
		int sz = read(1, input_buff, INPUT_BUFF_MAX - 1);
		
		/* Make sure string is null terminated (safe programming) */
		input_buff[INPUT_BUFF_MAX - 1] = 0;
	
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

		/* Now lets ask for a player. */
		printf("Which player do you want to ask? ");
		sz = read(1, input_buff, INPUT_BUFF_MAX - 1);

		/* Make sure string is null terminated (safe programming) */
		input_buff[INPUT_BUFF_MAX - 1] = 0;

		/* Did we encounter an I/O error? */
                if(sz <= 0)
                {
                        /* The user has quit */
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
			(human_r >= R2 && human_r <= RANK_MAX) &&
			/* Is the given player valid? */
			(human_p != NULL)
	       );

	/**
	 * Let the game know who we want to ask and which
	 * rank we want to ask for.
	 */
	*guess_rank = human_r;
	*guess_player = human_p;

	/* Return success */
	return 0;
}
